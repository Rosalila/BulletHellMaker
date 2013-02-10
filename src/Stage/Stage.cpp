#include "Stage/Stage.h"

Stage::Stage(Painter* painter,Sonido* sonido,Receiver*receiver)
{
    this->painter=painter;
    this->sonido=sonido;
    this->receiver=receiver;
    enemies=new std::map<std::string,Character*>();
    this->iterator=0;
}

void Stage::drawLayer(Layer* layer)
{
    //Animation speed
    if(layer->time_elapsed>layer->frame_duration)
    {
        layer->current_frame++;
        layer->time_elapsed=0;
    }

    //Loop animation
    layer->time_elapsed++;
    if(layer->current_frame>=layer->textures.size())
        layer->current_frame=0;

    //Get current image
    Image* texture=layer->textures[layer->current_frame];

    //Paint
    int size_x=layer->textures_size_x[layer->current_frame];
    int size_y=layer->textures_size_y[layer->current_frame];

    int dimension_x=texture->getWidth();
    int dimension_y=texture->getHeight();

    int pos_x=layer->alignment_x;
    int pos_y=painter->screen_height-size_y-layer->alignment_y;

    for(int i=0;i<painter->screen_width/(size_x+layer->separation_x)+2;i++)
    {
        painter->draw2DImage
        (   texture,
            size_x,size_y,
            pos_x+i*(size_x+layer->separation_x),pos_y,
            1.0,
            0.0,
            false,
            layer->depth_effect_x,
            layer->depth_effect_y,
            Color(255,255,255,255),
            false);
    }

    if(layer->depth_effect_x>0)
    {
        if(painter->camera_x/layer->depth_effect_x>size_x+layer->separation_x+layer->alignment_x)
        {
            layer->alignment_x+=size_x+layer->separation_x;
//            layer->alignment_y+=50;
        }
    }else if(layer->depth_effect_x<0)
    {
        if(painter->camera_x-layer->depth_effect_x>size_x+layer->alignment_x)
        {
            layer->alignment_x+=size_x;
//            layer->alignment_y+=50;
        }
    }

}

void Stage::dibujarBack()
{
    for(int i=0;i<back.size();i++)
    {
        Layer* layer=back[i];
        drawLayer(layer);
    }
}

void Stage::dibujarFront()
{
    for(int i=0;i<front.size();i++)
    {
        Layer* layer=front[i];
        drawLayer(layer);
    }

    for (std::list<Dialogue*>::iterator dialogue = active_dialogues.begin(); dialogue != active_dialogues.end(); dialogue++)
        ((Dialogue*)*dialogue)->render();
}

void Stage::loadDialogues(std::string file)
{
    writeLogLine("Loading dialogues from XML.");

    char *archivo=new char[255];
    strcpy(archivo,"stages/");
    strcat(archivo,file.c_str());
    strcat(archivo,"/dialogues.xml");
    TiXmlDocument doc_t( archivo );
    doc_t.LoadFile();
    TiXmlDocument *doc;
    doc=&doc_t;

    TiXmlNode *dialogues_file=doc->FirstChild("DialoguesFile");

    for(TiXmlNode *dialogue_node=dialogues_file->FirstChild("dialogue");
            dialogue_node!=NULL;
            dialogue_node=dialogue_node->NextSibling("dialogue"))
    {
        int frame=atoi(dialogue_node->ToElement()->Attribute("frame"));
        std::string text=dialogue_node->ToElement()->Attribute("text");

        dialogues[frame]=new Dialogue(painter,sonido,receiver,text);
    }
}

void Stage::cargarDesdeXML(std::string path)
{
    writeLogLine("Loading stage from XML.");

    char *archivo=new char[255];
    strcpy(archivo,"stages/");
    strcat(archivo,path.c_str());
    strcat(archivo,"/main.xml");
    TiXmlDocument doc_t( archivo );
    doc_t.LoadFile();
    TiXmlDocument *doc;
    doc=&doc_t;

    TiXmlNode *stage_file=doc->FirstChild("StageFile");

    //Load settings
    char *music=new char[255];
    strcpy(music,"stages/");
    strcat(music,path.c_str());
    strcat(music,"/music.ogg");
    music_path=(std::string)music;

    TiXmlNode *nodo_ss=stage_file->FirstChild("StageSize");
    this->size=atoi(nodo_ss->ToElement()->Attribute("x"));

    TiXmlNode *nodo_floor=stage_file->FirstChild("Floor");
    this->pos_piso=atoi(nodo_floor->ToElement()->Attribute("position"));

    TiXmlNode *nodo_bounds=stage_file->FirstChild("Bounds");
    this->bound_x1=atoi(nodo_bounds->ToElement()->Attribute("x1"));
    this->bound_y1=atoi(nodo_bounds->ToElement()->Attribute("y1"));
    this->bound_x2=atoi(nodo_bounds->ToElement()->Attribute("x2"));
    this->bound_y2=atoi(nodo_bounds->ToElement()->Attribute("y2"));

    TiXmlNode *nodo_misc=stage_file->FirstChild("Misc");
    this->velocity=atoi(nodo_misc->ToElement()->Attribute("velocity"));

    writeLogLine("Loading stage's BackLayers.");

    //Load back layer
    for(TiXmlNode *nodo_back=stage_file->FirstChild("BackLayer");
            nodo_back!=NULL;
            nodo_back=nodo_back->NextSibling("BackLayer"))
    {
        int frame_duration=atoi(nodo_back->ToElement()->Attribute("frame_duration"));
        int depth_effect_x=atoi(nodo_back->ToElement()->Attribute("depth_effect_x"));
        int depth_effect_y=atoi(nodo_back->ToElement()->Attribute("depth_effect_y"));
        int alignment_x=atoi(nodo_back->ToElement()->Attribute("alignment_x"));
        int alignment_y=atoi(nodo_back->ToElement()->Attribute("alignment_y"));
        int separation_x=atoi(nodo_back->ToElement()->Attribute("separation_x"));

        std::vector <Image*> textures;
        std::vector <int> textures_size_x;
        std::vector <int> textures_size_y;

        for(TiXmlNode* layer=nodo_back->FirstChild("frame");
                layer!=NULL;
                layer=layer->NextSibling("frame"))
        {
            char *image=new char[255];
            strcpy(image,"stages/");
            strcat(image,path.c_str());
            strcat(image,"/images/");
            strcat(image,layer->ToElement()->Attribute("image_path"));
            int size_x=atoi(layer->ToElement()->Attribute("size_x"));
            int size_y=atoi(layer->ToElement()->Attribute("size_y"));

            textures.push_back(painter->getTexture(image));
            textures_size_x.push_back(size_x);
            textures_size_y.push_back(size_y);
        }

        back.push_back(new Layer(textures,textures_size_x,textures_size_y,frame_duration,depth_effect_x,depth_effect_y,alignment_x,alignment_y,separation_x));
    }

    writeLogLine("Loading stage's FrontLayers.");

    //Load front layer
    for(TiXmlNode *nodo_back=stage_file->FirstChild("FrontLayer");
            nodo_back!=NULL;
            nodo_back=nodo_back->NextSibling("FrontLayer"))
    {
        int frame_duration=atoi(nodo_back->ToElement()->Attribute("frame_duration"));
        int depth_effect_x=atoi(nodo_back->ToElement()->Attribute("depth_effect_x"));
        int depth_effect_y=atoi(nodo_back->ToElement()->Attribute("depth_effect_y"));
        int alignment_x=atoi(nodo_back->ToElement()->Attribute("alignment_x"));
        int alignment_y=atoi(nodo_back->ToElement()->Attribute("alignment_y"));
        int separation_x=atoi(nodo_back->ToElement()->Attribute("separation_x"));

        std::vector <Image*> textures;
        std::vector <int> textures_size_x;
        std::vector <int> textures_size_y;

        for(TiXmlNode* layer=nodo_back->FirstChild("frame");
                layer!=NULL;
                layer=layer->NextSibling("frame"))
        {
            char *image=new char[255];
            strcpy(image,"stages/");
            strcat(image,path.c_str());
            strcat(image,"/images/");
            strcat(image,layer->ToElement()->Attribute("image_path"));
            int size_x=atoi(layer->ToElement()->Attribute("size_x"));
            int size_y=atoi(layer->ToElement()->Attribute("size_y"));

            textures.push_back(painter->getTexture(image));
            textures_size_x.push_back(size_x);
            textures_size_y.push_back(size_y);
        }

        front.push_back(new Layer(textures,textures_size_x,textures_size_y,frame_duration,depth_effect_x,depth_effect_y,alignment_x,alignment_y,separation_x
                                  ));
    }
    writeLogLine("Stage loaded succesfully from XML.");

    std::string test="test";
    (*enemies)[test]=new Character(sonido,painter,receiver,"stages/"+path+"/Enemy/");

    loadDialogues(path);
}

Stage::~Stage()
{
    writeLogLine("Deleting stage.");
    for(;!back.empty();)
    {
        Layer*layer=back.back();
        back.pop_back();
        delete layer;
    }
    for(;!front.empty();)
    {
        Layer*layer=front.back();
        front.pop_back();
        delete layer;
    }
}

int Stage::getBoundX1()
{
    return bound_x1;
}
int Stage::getBoundY1()
{
    return bound_y1;
}
int Stage::getBoundX2()
{
    return bound_x2;
}
int Stage::getBoundY2()
{
    return bound_y2;
}
int Stage::getVelocity()
{
    return velocity;
}
void Stage::setVelocity(int velocity)
{
    this->velocity=velocity;
}

std::map<std::string,Character*>*Stage::getEnemies()
{
    return enemies;
}

void Stage::render()
{
//    for (std::list<Character*>::iterator character = active_enemies->begin(); character != active_enemies->end(); character++)
//        ((Character*)*character)->render();
}

void Stage::logic()
{
    iterator++;
    if(dialogues.find(iterator)!=dialogues.end())
    {
        active_dialogues.push_back(dialogues[iterator]);
    }

    std::list<Dialogue*>::iterator i = active_dialogues.begin();
    while (i != active_dialogues.end())
    {
        Dialogue*d=(Dialogue*)*i;
        d->logic();
        if (d->destroyFlag())
        {
            active_dialogues.erase(i++);
        }
        else
        {
            ++i;
        }
    }
}
