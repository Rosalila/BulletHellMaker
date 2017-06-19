#include "Stage.h"

Stage::Stage()
{
    this->iterator=0;
    this->iterate_slowdown_flag=false;
    this->current_slowdown_iteration=0;
    this->layer_transparency=255;
    this->is_mod = false;
    this->path = "";
}

Stage::~Stage()
{
    rosalila()->utility->writeLogLine("Deleting stage.");
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

    if(layer->current_frame>=(int)layer->layer_frames.size())
        layer->current_frame=0;

    if(getGameOver())
    {
        if(getPlayerWon())
        {
            layer_transparency=128;
        }else
        {
            layer->color.red -= 3;
            layer->color.green -= 3;
            layer->color.blue -= 3;
            if(layer->color.red < 0)
                layer->color.red = 0;
            if(layer->color.green < 0)
                layer->color.green = 0;
            if(layer->color.blue < 0)
                layer->color.blue = 0;
        }
    }

    //Paint

    LayerFrame* current_layer_frame = layer->layer_frames[layer->current_frame];

    int frame_width=current_layer_frame->width;
    int frame_heigth=current_layer_frame->height;

    vector<DrawableRectangle*>rectangles;

    for(int i=0;i<rosalila()->graphics->screen_width/(frame_width+layer->separation_x)+2;i++)
    {

        int pos_x = layer->alignment_x + i*(frame_width+layer->separation_x);
        int pos_y = rosalila()->graphics->screen_height - frame_heigth - layer->alignment_y;

        if(current_layer_frame->type == "image")
        {
            Image* image = current_layer_frame->image;

            rosalila()->graphics->draw2DImage
            (   image,
                frame_width,frame_heigth,
                pos_x,pos_y,
                1.0,
                0.0,
                false,
                layer->depth_effect_x,
                layer->depth_effect_y,
                Color(layer->color.red,layer->color.green,layer->color.blue,layer->color.alpha),
                0,0,
                false,
                FlatShadow());
        }
        if(current_layer_frame->type == "rectangle")
        {
            rectangles.push_back(new DrawableRectangle(pos_x,pos_y,frame_width,frame_heigth,0,Color(layer->color.red,layer->color.green,layer->color.blue,layer->color.alpha)));
        }
    }

    rosalila()->graphics->drawRectangles(rectangles,layer->depth_effect_x,layer->depth_effect_y,true);

    for(int i=0;i<(int)rectangles.size();i++)
    {
        delete rectangles[i];
    }

    if(layer->depth_effect_x>0)
    {
        if(rosalila()->graphics->camera_x/layer->depth_effect_x>frame_width+layer->separation_x+layer->alignment_x)
        {
            layer->alignment_x+=frame_width+layer->separation_x;
        }
    }else if(layer->depth_effect_x<0)
    {
        if(rosalila()->graphics->camera_x*-layer->depth_effect_x > frame_width+layer->separation_x+layer->alignment_x)
        {
            layer->alignment_x+=frame_width+layer->separation_x;
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
//    rosalila()->graphics->clearScreen(Color(0,0,0,0));
}

void Stage::dibujarFront()
{
    for(int i=0;i<(int)front.size();i++)
    {
        Layer* layer=front[i];
        drawLayer(layer);
    }
}

void Stage::loadFromXML(std::string name, bool is_mod)
{
    this->name=name;

    this->is_mod = is_mod;
    if(!is_mod)
    {
      path = assets_directory+"stages/";
    }else
    {
      path = assets_directory+"mods/stages/";
    }

    rosalila()->utility->writeLogLine("Loading stage from XML.");

    Node* root_node = rosalila()->parser->getNodes(path+name+"/main.xml");

    //Load settings
    music_path = path+name+"/music.ogg";

    Node* nodo_bounds = root_node->getNodeByName("Bounds");
    this->bound_x1=0;
    this->bound_y1=0;
    this->bound_x2=rosalila()->graphics->screen_width;
    this->bound_y2=rosalila()->graphics->screen_height;

    if(nodo_bounds->hasAttribute("x1"))
        this->bound_x1=atoi(nodo_bounds->attributes["x1"].c_str());

    if(nodo_bounds->hasAttribute("y1"))
        this->bound_y1=atoi(nodo_bounds->attributes["y1"].c_str());

    if(nodo_bounds->hasAttribute("x2"))
        this->bound_x2=atoi(nodo_bounds->attributes["x2"].c_str());

    if(nodo_bounds->hasAttribute("y2"))
        this->bound_y2=atoi(nodo_bounds->attributes["y2"].c_str());


    Node* nodo_misc=root_node->getNodeByName("Misc");

    this->velocity=0;
    if(nodo_misc->hasAttribute("velocity"))
        this->velocity=atoi(nodo_misc->attributes["velocity"].c_str());

    rosalila()->utility->writeLogLine("Loading stage's BackLayers.");

    vector<int>random_colors_r;
    vector<int>random_colors_g;
    vector<int>random_colors_b;

    int u=rosalila()->utility->getNonSeededRandomNumber()%3+1;
    for(int i=0;i<u;i++)
    {
        int random_number=rosalila()->utility->getNonSeededRandomNumber()%(*getColorPaletteR()).size();

        random_colors_r.push_back((*getColorPaletteR())[random_number]);
        random_colors_g.push_back((*getColorPaletteG())[random_number]);
        random_colors_b.push_back((*getColorPaletteB())[random_number]);
    }

    map<string,list<int> >randomized_appereance;
    map<string,int>max_layers;
    int current_layer=0;

    vector<Node*> backlayer_nodes = root_node->getNodesByName("BackLayer");

    for(int i=0;i<(int)backlayer_nodes.size();i++)
    {
        if(backlayer_nodes[i]->hasAttribute("randomize_appereance")
           && backlayer_nodes[i]->attributes["randomize_appereance"]=="yes")
       {
           string random_group="";
           if(backlayer_nodes[i]->hasAttribute("random_group"))
           {
               random_group=backlayer_nodes[i]->attributes["random_group"];
           }
           randomized_appereance[random_group].push_back(current_layer);
       }
       current_layer++;
    }

    max_layers["a"]=1+rosalila()->utility->getNonSeededRandomNumber()%2;
    max_layers["b"]=1+rosalila()->utility->getNonSeededRandomNumber()%3;

    for(map<string,list<int> >::iterator randomized_appereance_iterator=randomized_appereance.begin();
        randomized_appereance_iterator!=randomized_appereance.end();
        randomized_appereance_iterator++)
    {
        list<int>current_list=(*randomized_appereance_iterator).second;
        while((int)current_list.size()>max_layers[(*randomized_appereance_iterator).first])
        {
            int random_to_remove = rosalila()->utility->getNonSeededRandomNumber()%current_list.size();
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


    for(int i=0;i<(int)backlayer_nodes.size();i++)
    {
        bool included=false;

        if(!backlayer_nodes[i]->hasAttribute("randomize_appereance")
            || backlayer_nodes[i]->attributes["randomize_appereance"]!="yes")
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
        if(backlayer_nodes[i]->hasAttribute("frame_duration"))
            frame_duration=atoi(backlayer_nodes[i]->attributes["frame_duration"].c_str());

        if(backlayer_nodes[i]->hasAttribute("randomize_frame_duration"))
            frame_duration+=rosalila()->utility->getNonSeededRandomNumber()%atoi(backlayer_nodes[i]->attributes["randomize_frame_duration"].c_str());

        int depth_effect_x=0;
        if(backlayer_nodes[i]->hasAttribute("depth_effect_x"))
            depth_effect_x=atoi(backlayer_nodes[i]->attributes["depth_effect_x"].c_str());

        if(backlayer_nodes[i]->hasAttribute("randomize_depth_effect_x"))
            depth_effect_x+=rosalila()->utility->getNonSeededRandomNumber()%atoi(backlayer_nodes[i]->attributes["randomize_depth_effect_x"].c_str());

        int depth_effect_y=0;
        if(backlayer_nodes[i]->hasAttribute("depth_effect_y"))
            depth_effect_y=atoi(backlayer_nodes[i]->attributes["depth_effect_y"].c_str());

        if(backlayer_nodes[i]->hasAttribute("randomize_depth_effect_y"))
            depth_effect_y+=rosalila()->utility->getNonSeededRandomNumber()%atoi(backlayer_nodes[i]->attributes["randomize_depth_effect_y"].c_str());

        int align_x=0;
        if(backlayer_nodes[i]->hasAttribute("align_x"))
            align_x=atoi(backlayer_nodes[i]->attributes["align_x"].c_str());

        if(backlayer_nodes[i]->hasAttribute("randomize_align_x"))
            align_x+=rosalila()->utility->getNonSeededRandomNumber()%atoi(backlayer_nodes[i]->attributes["randomize_align_x"].c_str());

        int align_y=0;
        if(backlayer_nodes[i]->hasAttribute("align_y"))
            align_y=atoi(backlayer_nodes[i]->attributes["align_y"].c_str());

        if(backlayer_nodes[i]->hasAttribute("randomize_align_y"))
            align_y+=rosalila()->utility->getNonSeededRandomNumber()%atoi(backlayer_nodes[i]->attributes["randomize_align_y"].c_str());

        int separation_x=0;
        if(backlayer_nodes[i]->hasAttribute("separation_x"))
            separation_x=atoi(backlayer_nodes[i]->attributes["separation_x"].c_str());

        if(backlayer_nodes[i]->hasAttribute("randomize_separation_x"))
            separation_x+=rosalila()->utility->getNonSeededRandomNumber()%atoi(backlayer_nodes[i]->attributes["randomize_separation_x"].c_str());

        vector<Node*> frame_nodes = backlayer_nodes[i]->getNodesByName("frame");

        vector<LayerFrame*> layer_frames;

        for(int j=0;j<(int)frame_nodes.size();j++)
        {
            layer_frames.push_back(getFrameFromNode(frame_nodes[j]));
        }

        Color color(255,255,255,255);

        if(backlayer_nodes[i]->hasAttribute("randomize_color")
           && backlayer_nodes[i]->attributes["randomize_color"]=="yes")
        {
            int random_number_pos=rosalila()->utility->getNonSeededRandomNumber()%random_colors_r.size();
            color.red=random_colors_r[random_number_pos];
            color.green=random_colors_g[random_number_pos];
            color.blue=random_colors_b[random_number_pos];
        }

        if(backlayer_nodes[i]->hasAttribute("alpha"))
            color.alpha=atoi(backlayer_nodes[i]->attributes["alpha"].c_str());

        back.push_back(new Layer(layer_frames,frame_duration,depth_effect_x,depth_effect_y,align_x,align_y,separation_x,color));
    }

    rosalila()->utility->writeLogLine("Loading stage's FrontLayers.");


    vector<Node*> frontlayer_nodes = root_node->getNodesByName("FrontLayer");
    //Load front layer
    for(int i=0;i<(int)frontlayer_nodes.size();i++)
    {
        int frame_duration=0;
        if(frontlayer_nodes[i]->hasAttribute("frame_duration"))
            frame_duration=atoi(frontlayer_nodes[i]->attributes["frame_duration"].c_str());

        int depth_effect_x=0;
        if(frontlayer_nodes[i]->hasAttribute("depth_effect_x"))
            depth_effect_x=atoi(frontlayer_nodes[i]->attributes["depth_effect_x"].c_str());

        int depth_effect_y=0;
        if(frontlayer_nodes[i]->hasAttribute("depth_effect_y"))
            depth_effect_y=atoi(frontlayer_nodes[i]->attributes["depth_effect_y"].c_str());

        int align_x=0;
        if(frontlayer_nodes[i]->hasAttribute("align_x"))
            align_x=atoi(frontlayer_nodes[i]->attributes["align_x"].c_str());

        int align_y=0;
        if(frontlayer_nodes[i]->hasAttribute("align_y"))
            align_y=atoi(frontlayer_nodes[i]->attributes["align_y"].c_str());

        int separation_x=0;
        if(frontlayer_nodes[i]->hasAttribute("separation_x"))
            separation_x=atoi(frontlayer_nodes[i]->attributes["separation_x"].c_str());

        std::vector <Image*> textures;
        std::vector <int> textures_size_x;
        std::vector <int> textures_size_y;

        vector<Node*> frame_nodes = frontlayer_nodes[i]->getNodesByName("frame");

        vector<LayerFrame*> layer_frames;

        for(int j=0;j<(int)frame_nodes.size();j++)
        {
            layer_frames.push_back(getFrameFromNode(frame_nodes[j]));
        }

        Color color(255,255,255,255);

        if(backlayer_nodes[i]->hasAttribute("randomize_color")
           && backlayer_nodes[i]->attributes["randomize_color"]=="yes")
        {
            int random_number_pos=rosalila()->utility->getNonSeededRandomNumber()%random_colors_r.size();
            color.red=random_colors_r[random_number_pos];
            color.green=random_colors_g[random_number_pos];
            color.blue=random_colors_b[random_number_pos];
        }

        if(backlayer_nodes[i]->hasAttribute("alpha"))
            color.alpha=atoi(frontlayer_nodes[i]->attributes["alpha"].c_str());

        front.push_back(new Layer(layer_frames,frame_duration,depth_effect_x,depth_effect_y,align_x,align_y,separation_x,
                                  color
                                  ));
    }

    delete root_node;
    rosalila()->utility->writeLogLine("Stage loaded succesfully from XML.");
}

LayerFrame* Stage::getFrameFromNode(Node* frame_node)
{
    Image *image_temp = NULL;
    string type;
    int width = 0;
    int height = 0;

    if(frame_node->hasAttribute("type"))
        type = frame_node->attributes["type"];

    if(frame_node->hasAttribute("image_path"))
    {
        string image_path = path+name+"/images/"+frame_node->attributes["image_path"];
        image_temp=rosalila()->graphics->getTexture(image_path);

        width = image_temp->getWidth();
        height = image_temp->getHeight();
    }

    if(frame_node->hasAttribute("width"))
        width = atoi(frame_node->attributes["width"].c_str());
    if(frame_node->hasAttribute("height"))
        height = atoi(frame_node->attributes["height"].c_str());

    return new LayerFrame(image_temp,type,width,height);
}

void Stage::logic()
{
    if(getIterateSlowdownFlag())
        iterator++;
}

void Stage::playMusic()
{
    rosalila()->sound->playMusic(this->music_path,-1);
}
