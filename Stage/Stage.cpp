#include "Stage.h"

Stage::Stage(RosalilaGraphics* painter,Sound* sonido,Receiver*receiver)
{
    this->painter=painter;
    this->sonido=sonido;
    this->receiver=receiver;
    this->iterator=0;
    //this->dialogue_bg=painter->getTexture("misc/dialogue_bg.png");

    //slow extra control
    this->iterate_slowdown_flag=false;
    this->current_slowdown_iteration=0;
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
    if(getIterateSlowdownFlag())
        layer->time_elapsed++;

    if(layer->current_frame>=(int)layer->textures.size())
        layer->current_frame=0;

    //Get current image
    Image* texture=layer->textures[layer->current_frame];

    //Paint
    int size_x=layer->textures_size_x[layer->current_frame];
    int size_y=layer->textures_size_y[layer->current_frame];

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
            0,0,
            false);
    }

    if(layer->depth_effect_x>0)
    {
        if(painter->camera_x/layer->depth_effect_x>size_x+layer->separation_x+layer->alignment_x)
        {
            layer->alignment_x+=size_x+layer->separation_x;
        }
    }else if(layer->depth_effect_x<0)
    {
        if(painter->camera_x*-layer->depth_effect_x>size_x+layer->separation_x+layer->alignment_x)
        {
            layer->alignment_x+=size_x+layer->separation_x;
        }
    }

}

void Stage::dibujarBack()
{
    for(int i=0;i<(int)back.size();i++)
    {
        Layer* layer=back[i];
        drawLayer(layer);
    }
}

void Stage::dibujarFront()
{
    for(int i=0;i<(int)front.size();i++)
    {
        Layer* layer=front[i];
        drawLayer(layer);
    }

    for (std::list<Dialogue*>::iterator dialogue = active_dialogues.begin(); dialogue != active_dialogues.end(); dialogue++)
    {
        painter->draw2DImage
        (   dialogue_bg,
            dialogue_bg->getWidth(),dialogue_bg->getHeight(),
            dialogue_x,dialogue_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            0,0,
            false);
        ((Dialogue*)*dialogue)->render(this->dialogue_x+this->dialogue_padding_x,this->dialogue_y+this->dialogue_padding_y);
    }
}

void Stage::loadDialogues(std::string file)
{
    writeLogLine("Loading dialogues from XML.");

    string dialogues_path = assets_directory+"stages/"+file+"/dialogues.xml";
    TiXmlDocument doc_t((char*)dialogues_path.c_str());
    doc_t.LoadFile();
    TiXmlDocument *doc;
    doc=&doc_t;

    TiXmlNode *dialogues_file=doc->FirstChild("DialoguesFile");

    if(dialogues_file==NULL)
        return;

    for(TiXmlNode *dialogue_node=dialogues_file->FirstChild("dialogue");
            dialogue_node!=NULL;
            dialogue_node=dialogue_node->NextSibling("dialogue"))
    {
        int frame=atoi(dialogue_node->ToElement()->Attribute("frame"));
        std::string text=dialogue_node->ToElement()->Attribute("text");
        std::string path=dialogue_node->ToElement()->Attribute("path");

        dialogues[frame]=new Dialogue(painter,sonido,receiver,text,painter->getTexture(assets_directory+"stages/"+file+"/"+path));
    }
}

void Stage::loadFromXML(std::string name)
{
    this->name=name;

    writeLogLine("Loading stage from XML.");

    string main_path = assets_directory+"stages/"+name+"/main.xml";
    TiXmlDocument doc_t((char*)main_path.c_str());
    doc_t.LoadFile();
    TiXmlDocument *doc;
    doc=&doc_t;

    TiXmlNode *stage_file=doc->FirstChild("StageFile");

    //Load settings
    music_path = assets_directory+"stages/"+name+"/music.ogg";

    this->dialogue_x=0;
    this->dialogue_y=0;
    this->dialogue_padding_x=0;
    this->dialogue_padding_y=0;
//    if(stage_file->FirstChild("DialoguePosition")!=NULL)
//    {
//        TiXmlNode *dialogue_pos_node=stage_file->FirstChild("DialoguePosition");
//        this->dialogue_x=atoi(dialogue_pos_node->ToElement()->Attribute("x"));
//        this->dialogue_y=atoi(dialogue_pos_node->ToElement()->Attribute("y"));
//        this->dialogue_padding_x=atoi(dialogue_pos_node->ToElement()->Attribute("padding_x"));
//        this->dialogue_padding_y=atoi(dialogue_pos_node->ToElement()->Attribute("padding_y"));
//    }

    TiXmlNode *nodo_bounds=stage_file->FirstChild("Bounds");
    this->bound_x1=0;
    this->bound_y1=0;
    this->bound_x2=painter->screen_width;
    this->bound_y2=painter->screen_height;

    if(nodo_bounds->ToElement()->Attribute("x1")!=NULL)
        this->bound_x1=atoi(nodo_bounds->ToElement()->Attribute("x1"));

    if(nodo_bounds->ToElement()->Attribute("y1")!=NULL)
        this->bound_y1=atoi(nodo_bounds->ToElement()->Attribute("y1"));

    if(nodo_bounds->ToElement()->Attribute("x2")!=NULL)
        this->bound_x2=atoi(nodo_bounds->ToElement()->Attribute("x2"));

    if(nodo_bounds->ToElement()->Attribute("y2")!=NULL)
        this->bound_y2=atoi(nodo_bounds->ToElement()->Attribute("y2"));


    TiXmlNode *nodo_misc=stage_file->FirstChild("Misc");

    this->velocity=0;
    if(nodo_misc->ToElement()->Attribute("velocity")!=NULL)
        this->velocity=atoi(nodo_misc->ToElement()->Attribute("velocity"));

    writeLogLine("Loading stage's BackLayers.");

    //Load back layer
    for(TiXmlNode *nodo_back=stage_file->FirstChild("BackLayer");
            nodo_back!=NULL;
            nodo_back=nodo_back->NextSibling("BackLayer"))
    {
        int frame_duration=0;
        if(nodo_back->ToElement()->Attribute("frame_duration")!=NULL)
            frame_duration=atoi(nodo_back->ToElement()->Attribute("frame_duration"));

        int depth_effect_x=0;
        if(nodo_back->ToElement()->Attribute("depth_effect_x")!=NULL)
            depth_effect_x=atoi(nodo_back->ToElement()->Attribute("depth_effect_x"));

        int depth_effect_y=0;
        if(nodo_back->ToElement()->Attribute("depth_effect_y")!=NULL)
            depth_effect_y=atoi(nodo_back->ToElement()->Attribute("depth_effect_y"));

        int align_x=0;
        if(nodo_back->ToElement()->Attribute("align_x")!=NULL)
            align_x=atoi(nodo_back->ToElement()->Attribute("align_x"));

        int align_y=0;
        if(nodo_back->ToElement()->Attribute("align_y")!=NULL)
            align_y=atoi(nodo_back->ToElement()->Attribute("align_y"));

        int separation_x=0;
        if(nodo_back->ToElement()->Attribute("separation_x")!=NULL)
            separation_x=atoi(nodo_back->ToElement()->Attribute("separation_x"));

        std::vector <Image*> textures;
        std::vector <int> textures_size_x;
        std::vector <int> textures_size_y;

        for(TiXmlNode* layer=nodo_back->FirstChild("frame");
                layer!=NULL;
                layer=layer->NextSibling("frame"))
        {
            char *image=new char[255];
            strcpy(image,"stages/");
            strcat(image,name.c_str());
            strcat(image,"/images/");
            strcat(image,layer->ToElement()->Attribute("image_path"));

            Image *image_temp=painter->getTexture(assets_directory+image);

            int size_x=image_temp->getWidth();
            int size_y=image_temp->getHeight();

            if(layer->ToElement()->Attribute("width")!=NULL)
                size_x=atoi(layer->ToElement()->Attribute("width"));
            if(layer->ToElement()->Attribute("height")!=NULL)
                size_y=atoi(layer->ToElement()->Attribute("height"));

            textures.push_back(image_temp);
            textures_size_x.push_back(size_x);
            textures_size_y.push_back(size_y);
        }

        back.push_back(new Layer(textures,textures_size_x,textures_size_y,frame_duration,depth_effect_x,depth_effect_y,align_x,align_y,separation_x));
    }

    writeLogLine("Loading stage's FrontLayers.");

    //Load front layer
    for(TiXmlNode *nodo_back=stage_file->FirstChild("FrontLayer");
            nodo_back!=NULL;
            nodo_back=nodo_back->NextSibling("FrontLayer"))
    {
        int frame_duration=0;
        if(nodo_back->ToElement()->Attribute("frame_duration")!=NULL)
            frame_duration=atoi(nodo_back->ToElement()->Attribute("frame_duration"));

        int depth_effect_x=0;
        if(nodo_back->ToElement()->Attribute("depth_effect_x")!=NULL)
            depth_effect_x=atoi(nodo_back->ToElement()->Attribute("depth_effect_x"));

        int depth_effect_y=0;
        if(nodo_back->ToElement()->Attribute("depth_effect_y")!=NULL)
            depth_effect_y=atoi(nodo_back->ToElement()->Attribute("depth_effect_y"));

        int align_x=0;
        if(nodo_back->ToElement()->Attribute("align_x")!=NULL)
            align_x=atoi(nodo_back->ToElement()->Attribute("align_x"));

        int align_y=0;
        if(nodo_back->ToElement()->Attribute("align_y")!=NULL)
            align_y=atoi(nodo_back->ToElement()->Attribute("align_y"));

        int separation_x=0;
        if(nodo_back->ToElement()->Attribute("separation_x")!=NULL)
            separation_x=atoi(nodo_back->ToElement()->Attribute("separation_x"));

        std::vector <Image*> textures;
        std::vector <int> textures_size_x;
        std::vector <int> textures_size_y;

        for(TiXmlNode* layer=nodo_back->FirstChild("frame");
                layer!=NULL;
                layer=layer->NextSibling("frame"))
        {
            string image_path = assets_directory+"stages/"+name+"/images/"+layer->ToElement()->Attribute("image_path");

            Image *image_temp=painter->getTexture(image_path);

            int size_x=image_temp->getWidth();
            int size_y=image_temp->getHeight();

            if(layer->ToElement()->Attribute("width")!=NULL)
                size_x=atoi(layer->ToElement()->Attribute("width"));
            if(layer->ToElement()->Attribute("height")!=NULL)
                size_y=atoi(layer->ToElement()->Attribute("height"));

            textures.push_back(image_temp);
            textures_size_x.push_back(size_x);
            textures_size_y.push_back(size_y);
        }

        front.push_back(new Layer(textures,textures_size_x,textures_size_y,frame_duration,depth_effect_x,depth_effect_y,align_x,align_y,separation_x
                                  ));
    }
    writeLogLine("Stage loaded succesfully from XML.");

    loadDialogues(name);
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
string Stage::getName()
{
    return name;
}
void Stage::setVelocity(int velocity)
{
    this->velocity=velocity;
}

void Stage::render()
{
//    for (std::list<Character*>::iterator character = active_enemies->begin(); character != active_enemies->end(); character++)
//        ((Character*)*character)->render();
}

void Stage::logic()
{
    if(getIterateSlowdownFlag())
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

void Stage::playMusic()
{
    sonido->playMusic(this->music_path);
}
