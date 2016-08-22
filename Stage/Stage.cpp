#include "Stage.h"

Stage::Stage()
{
    this->iterator=0;
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
    int pos_y=Rosalila()->Graphics->screen_height-size_y-layer->alignment_y;

    for(int i=0;i<Rosalila()->Graphics->screen_width/(size_x+layer->separation_x)+2;i++)
    {
        Rosalila()->Graphics->draw2DImage
        (   texture,
            size_x,size_y,
            pos_x+i*(size_x+layer->separation_x),pos_y,
            1.0,
            0.0,
            false,
            layer->depth_effect_x,
            layer->depth_effect_y,
            Color(layer->random_color_r,layer->random_color_g,layer->random_color_b,255),
            0,0,
            false,
            FlatShadow());
    }

    if(layer->depth_effect_x>0)
    {
        if(Rosalila()->Graphics->camera_x/layer->depth_effect_x>size_x+layer->separation_x+layer->alignment_x)
        {
            layer->alignment_x+=size_x+layer->separation_x;
        }
    }else if(layer->depth_effect_x<0)
    {
        if(Rosalila()->Graphics->camera_x*-layer->depth_effect_x>size_x+layer->separation_x+layer->alignment_x)
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

    TiXmlNode *nodo_bounds=stage_file->FirstChild("Bounds");
    this->bound_x1=0;
    this->bound_y1=0;
    this->bound_x2=Rosalila()->Graphics->screen_width;
    this->bound_y2=Rosalila()->Graphics->screen_height;

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


vector<int>random_colors_r;
vector<int>random_colors_g;
vector<int>random_colors_b;

int u=rand()%3+1;
for(int i=0;i<u;i++)
{
int random_number=rand()%19;
int random_color_r,random_color_g,random_color_b;
if(random_number==0)
{
    random_color_r=244;random_color_g=67;random_color_b=54;
}
if(random_number==1)
{
    random_color_r=102;random_color_g=58;random_color_b=182;
}
if(random_number==2)
{
    random_color_r=3;random_color_g=167;random_color_b=244;
}
if(random_number==3)
{
    random_color_r=76;random_color_g=175;random_color_b=80;
}
if(random_number==4)
{
    random_color_r=255;random_color_g=235;random_color_b=59;
}
if(random_number==5)
{
    random_color_r=255;random_color_g=87;random_color_b=34;
}
if(random_number==6)
{
    random_color_r=96;random_color_g=125;random_color_b=139;
}
if(random_number==7)
{
    random_color_r=233;random_color_g=30;random_color_b=99;
}
if(random_number==8)
{
    random_color_r=63;random_color_g=81;random_color_b=181;
}
if(random_number==9)
{
    random_color_r=0;random_color_g=188;random_color_b=212;
}
if(random_number==10)
{
    random_color_r=139;random_color_g=195;random_color_b=74;
}
if(random_number==11)
{
    random_color_r=255;random_color_g=193;random_color_b=7;
}
if(random_number==12)
{
    random_color_r=121;random_color_g=85;random_color_b=72;
}
if(random_number==13)
{
    random_color_r=156;random_color_g=39;random_color_b=176;
}
if(random_number==14)
{
    random_color_r=33;random_color_g=150;random_color_b=243;
}
if(random_number==15)
{
    random_color_r=0;random_color_g=150;random_color_b=136;
}
if(random_number==16)
{
    random_color_r=205;random_color_g=220;random_color_b=57;
}
if(random_number==17)
{
    random_color_r=255;random_color_g=152;random_color_b=0;
}
if(random_number==18)
{
    random_color_r=158;random_color_g=158;random_color_b=158;
}

random_colors_r.push_back(random_color_r);
random_colors_g.push_back(random_color_g);
random_colors_b.push_back(random_color_b);

}


map<string,list<int> >randomized_appereance;
map<string,int>max_layers;
int current_layer=0;
for(TiXmlNode *nodo_back=stage_file->FirstChild("BackLayer");
        nodo_back!=NULL;
        nodo_back=nodo_back->NextSibling("BackLayer"))
{
    if(nodo_back->ToElement()->Attribute("randomize_appereance")!=NULL
       && strcmp(nodo_back->ToElement()->Attribute("randomize_appereance"),"yes")==0)
   {
       string random_group="";
       if(nodo_back->ToElement()->Attribute("random_group")!=NULL)
       {
           random_group=nodo_back->ToElement()->Attribute("random_group");
       }
       randomized_appereance[random_group].push_back(current_layer);
   }
   current_layer++;
}
//int max_layers=rand()%3+1;
//max_layer["a"]=rand()%3+1;
//max_layer["b"]=rand()%3+1;
max_layers["a"]=1+rand()%2;
max_layers["b"]=1+rand()%3;

for(map<string,list<int> >::iterator randomized_appereance_iterator=randomized_appereance.begin();
    randomized_appereance_iterator!=randomized_appereance.end();
    randomized_appereance_iterator++)
{
    list<int>current_list=(*randomized_appereance_iterator).second;
    while((int)current_list.size()>max_layers[(*randomized_appereance_iterator).first])
    {
        int random_to_remove = rand()%current_list.size();
        list<int>::iterator remove_iterator = current_list.begin();
        for(int i=0;i<random_to_remove;i++)
        {
            remove_iterator++;
        }
        current_list.erase(remove_iterator);
    }
    randomized_appereance[(*randomized_appereance_iterator).first]=current_list;
}

current_layer=0;

    //Load back layer
    for(TiXmlNode *nodo_back=stage_file->FirstChild("BackLayer");
            nodo_back!=NULL;
            nodo_back=nodo_back->NextSibling("BackLayer"))
    {
        bool included=false;

        if(nodo_back->ToElement()->Attribute("randomize_appereance")==NULL
            || strcmp(nodo_back->ToElement()->Attribute("randomize_appereance"),"yes")!=0)
        {
            included=true;
        }else
        {

for(map<string,list<int> >::iterator randomized_appereance_iterator=randomized_appereance.begin();
    randomized_appereance_iterator!=randomized_appereance.end();
    randomized_appereance_iterator++)
{
    list<int>current_list=(*randomized_appereance_iterator).second;
    for(list<int>::iterator i=current_list.begin();
        i!=current_list.end();
        i++)
    {
        if(current_layer==(*i))
        {
            included=true;
            break;
        }
    }
}


        }
        current_layer++;
        if(!included)
        {
            continue;
        }

        int frame_duration=0;
        if(nodo_back->ToElement()->Attribute("frame_duration")!=NULL)
            frame_duration=atoi(nodo_back->ToElement()->Attribute("frame_duration"));

        if(nodo_back->ToElement()->Attribute("randomize_frame_duration")!=NULL)
            frame_duration+=rand()%atoi(nodo_back->ToElement()->Attribute("randomize_frame_duration"));

        int depth_effect_x=0;
        if(nodo_back->ToElement()->Attribute("depth_effect_x")!=NULL)
            depth_effect_x=atoi(nodo_back->ToElement()->Attribute("depth_effect_x"));

        if(nodo_back->ToElement()->Attribute("randomize_depth_effect_x")!=NULL)
            depth_effect_x+=rand()%atoi(nodo_back->ToElement()->Attribute("randomize_depth_effect_x"));

        int depth_effect_y=0;
        if(nodo_back->ToElement()->Attribute("depth_effect_y")!=NULL)
            depth_effect_y=atoi(nodo_back->ToElement()->Attribute("depth_effect_y"));

        if(nodo_back->ToElement()->Attribute("randomize_depth_effect_y")!=NULL)
            depth_effect_y+=rand()%atoi(nodo_back->ToElement()->Attribute("randomize_depth_effect_y"));

        int align_x=0;
        if(nodo_back->ToElement()->Attribute("align_x")!=NULL)
            align_x=atoi(nodo_back->ToElement()->Attribute("align_x"));

        if(nodo_back->ToElement()->Attribute("randomize_align_x")!=NULL)
            align_x+=rand()%atoi(nodo_back->ToElement()->Attribute("randomize_align_x"));

        int align_y=0;
        if(nodo_back->ToElement()->Attribute("align_y")!=NULL)
            align_y=atoi(nodo_back->ToElement()->Attribute("align_y"));

        if(nodo_back->ToElement()->Attribute("randomize_align_y")!=NULL)
            align_y+=rand()%atoi(nodo_back->ToElement()->Attribute("randomize_align_y"));

        int separation_x=0;
        if(nodo_back->ToElement()->Attribute("separation_x")!=NULL)
            separation_x=atoi(nodo_back->ToElement()->Attribute("separation_x"));

        if(nodo_back->ToElement()->Attribute("randomize_separation_x")!=NULL)
            separation_x+=rand()%atoi(nodo_back->ToElement()->Attribute("randomize_separation_x"));

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

            Image *image_temp=Rosalila()->Graphics->getTexture(assets_directory+image);

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

        int random_color_r=255;
        int random_color_g=255;
        int random_color_b=255;
        if(nodo_back->ToElement()->Attribute("randomize_color")
           && strcmp(nodo_back->ToElement()->Attribute("randomize_color"),"yes")==0)
        {
            int random_number_pos=rand()%random_colors_r.size();
            random_color_r=random_colors_r[random_number_pos];
            random_color_g=random_colors_g[random_number_pos];
            random_color_b=random_colors_b[random_number_pos];
        }

        back.push_back(new Layer(textures,textures_size_x,textures_size_y,frame_duration,depth_effect_x,depth_effect_y,align_x,align_y,separation_x,random_color_r,random_color_g,random_color_b));
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

            Image *image_temp=Rosalila()->Graphics->getTexture(image_path);

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

        int random_color_r=255;
        if(nodo_back->ToElement()->Attribute("random_color_r")!=NULL)
            random_color_r=atoi(nodo_back->ToElement()->Attribute("random_color_r"));

        int random_color_g=255;
        if(nodo_back->ToElement()->Attribute("random_color_g")!=NULL)
            random_color_g=atoi(nodo_back->ToElement()->Attribute("random_color_g"));

        int random_color_b=255;
        if(nodo_back->ToElement()->Attribute("random_color_b")!=NULL)
            random_color_b=atoi(nodo_back->ToElement()->Attribute("random_color_b"));

        front.push_back(new Layer(textures,textures_size_x,textures_size_y,frame_duration,depth_effect_x,depth_effect_y,align_x,align_y,separation_x,
                                  random_color_r,random_color_g,random_color_b
                                  ));
    }
    writeLogLine("Stage loaded succesfully from XML.");
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

void Stage::logic()
{
    if(getIterateSlowdownFlag())
        iterator++;
}

void Stage::playMusic()
{
    Rosalila()->Sound->playMusic(this->music_path,-1);
}
