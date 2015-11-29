#include "STGMenu.h"
Menu::Menu(RosalilaGraphics* painter,Receiver* receiver,Sound* sonido,char* archivo)
{
    this->painter=painter;
    this->receiver=receiver;
    this->sonido=sonido;
    this->exit_signal=false;
    this->save_inputs_signal=false;
    this->char_select=NULL;
    this->selectables_container=NULL;

    loading_screen=painter->getTexture(assets_directory+"misc/loading_screen.png");

    string config_directory = assets_directory+"config.xml";
    TiXmlDocument doc_t((char*)config_directory.c_str());
    doc_t.LoadFile();
    TiXmlDocument *doc;
    doc=&doc_t;

    TiXmlNode *config_file=doc->FirstChild("ConfigFile");

    std::vector<std::string> chars,stages;
    for(TiXmlNode* node_chars=config_file->FirstChild("Chars");
            node_chars!=NULL;
            node_chars=node_chars->NextSibling("Chars"))
    {
        for(TiXmlNode* node_chars2=node_chars->FirstChild("char");
                node_chars2!=NULL;
                node_chars2=node_chars2->NextSibling("char"))
        {
            chars.push_back(std::string(node_chars2->ToElement()->Attribute("name")));
        }
    }

    for(TiXmlNode* node_chars=config_file->FirstChild("Stages");
            node_chars!=NULL;
            node_chars=node_chars->NextSibling("Stages"))
    {
        for(TiXmlNode* node_chars2=node_chars->FirstChild("stage");
                node_chars2!=NULL;
                node_chars2=node_chars2->NextSibling("stage"))
        {
            stages.push_back(std::string(node_chars2->ToElement()->Attribute("name")));
        }
    }

    cargarDesdeXml(archivo,chars,stages);
}

void Menu::iniciarJuego(std::string character_name,std::string stage_name)
{
    this->printLoadingScreen();
    writeLogLine("Initializing game.");
    Stage*stage=new Stage(painter,sonido,receiver);
    stage->loadFromXML(stage_name);
    Player*player=new Player(sonido,painter,receiver,character_name);
    Enemy*enemy=new Enemy(sonido,painter,receiver,stage_name,player);
    STG*stg=new STG(sonido,painter,receiver,player,enemy,stage);
    delete stg;
    this->playMusic();
    char_select->clearLocks();
}

void Menu::loopMenu()
{
    bool tecla_arriba=false;
    llenarRosalilaInputssBotones();
    inputa=new RosalilaInputs();
    inputb=new RosalilaInputs();
    inputa->loadFromXML(1,receiver);
    inputb->loadFromXML(2,receiver);
    bool tecla_arriba_p1=true;
    bool tecla_arriba_p2=true;
    //inicio
	for (;;)
	{
        dibujarMenu();
        //Move Elements
        for(int i=0;i<(int)elementos.size();i++)
        {
            Elemento*e=elementos[i];
            if(e->getTipo()=="Imagen")//Is MenuImagen
            {
                MenuImagen* mi=(MenuImagen*)e;
                if(mi->fade_in_current!=-1)
                {
                    mi->fade_in_current+=mi->fade_in_speed;
                    if(mi->fade_in_current>255)
                        mi->fade_in_current=255;
                }
            }
            if(e->current_displacement_x<e->stop_displacement_x_at)
            {
                e->x+=e->displacement_x;
                e->current_displacement_x++;
            }
            if(e->current_displacement_y<e->stop_displacement_y_at)
            {
                e->y+=e->displacement_y;
                e->current_displacement_y++;
            }
        }
        if(selectables_container!=NULL)
        {
            tecla_arriba=false;
            if(receiver->isKeyPressed(SDLK_ESCAPE) || receiver->isKeyPressed(SDLK_x) || receiver->isJoyPressed(5,0))
            {
                sonido->playSound(std::string("Menu.back"));
                exit_signal=true;
                break;
            }
            else if(receiver->isKeyPressed(SDL_SCANCODE_DOWN)
                    || receiver->isJoyPressed(-2,0))
            {
                sonido->playSound(std::string("Menu.move"));
                ((MenuContenedor*)selectables_container)->avanzar();

                if(((MenuContenedor*)selectables_container)->getElementoSeleccionado()->getTipo()=="Lista")
                {
                    MenuLista* ml=((MenuLista*)((MenuContenedor*)selectables_container)->getElementoSeleccionado());

                    if(!char_select->listoPA())
                    {
                        sonido->playSound(std::string("Menu.move_char"));
                        char_select->select_p1_y++;
                        if(char_select->select_p1_y>=char_select->size_y)
                            char_select->select_p1_y=0;
                    }
                    else
                    {
                    }
                }
            }
            else if(receiver->isKeyPressed(SDL_SCANCODE_UP)
                    || receiver->isJoyPressed(-8,0))
            {
                sonido->playSound(std::string("Menu.move"));
                ((MenuContenedor*)selectables_container)->retroceder();

                if(((MenuContenedor*)selectables_container)->getElementoSeleccionado()->getTipo()=="Lista")
                {
                    MenuLista* ml=((MenuLista*)((MenuContenedor*)selectables_container)->getElementoSeleccionado());

                    if(!char_select->listoPA())
                    {
                        sonido->playSound(std::string("Menu.move_char"));
                        char_select->select_p1_y--;
                        if(char_select->select_p1_y<0)
                            char_select->select_p1_y=char_select->size_y-1;
                    }
                    else
                    {
                    }
                }
            }
            else if(receiver->isKeyPressed(SDL_SCANCODE_RIGHT)
                    || receiver->isJoyPressed(-6,0))
            {
                if(((MenuContenedor*)selectables_container)->getElementoSeleccionado()->getTipo()=="Lista")
                {
                    if(!char_select->listoPA())
                    {
                        sonido->playSound(std::string("Menu.move_char"));
                        char_select->select_p1_x++;
                        if(char_select->select_p1_x>=char_select->size_x)
                            char_select->select_p1_x=0;
                    }
                    else
                    {
                        MenuLista* ml=((MenuLista*)((MenuContenedor*)selectables_container)->getElementoSeleccionado());
                        ml->avanzar();
                    }
                }
            }
            else if(receiver->isKeyPressed(SDL_SCANCODE_LEFT)
                    || receiver->isJoyPressed(-4,0))
            {
                if(((MenuContenedor*)selectables_container)->getElementoSeleccionado()->getTipo()=="Lista")
                {
                    if(!char_select->listoPA())
                    {
                        sonido->playSound(std::string("Menu.move_char"));
                        char_select->select_p1_x--;
                        if(char_select->select_p1_x<0)
                            char_select->select_p1_x=char_select->size_x-1;
                    }
                    else
                    {
                        MenuLista* ml=((MenuLista*)((MenuContenedor*)selectables_container)->getElementoSeleccionado());
                        ml->retroceder();
                    }
                }
            }else if(receiver->isKeyPressed(SDLK_RETURN) || receiver->isKeyPressed(SDLK_z) || receiver->isJoyPressed(0,0))
            {
                if(((MenuContenedor*)selectables_container)->getElementoSeleccionado()->getTipo()=="Lista")
                {
                    if(!char_select->listoPA())
                    {
                        sonido->playSound(std::string("Menu.select_char"));
                        char_select->lockPA(0);
                    }
                    else
                    {
                        sonido->playSound(std::string("Menu.select"));
                        iniciarJuego(char_select->getLockedNamesPA()[0],getStage());
                    }
                }

                if(((MenuContenedor*)selectables_container)->getElementoSeleccionado()->getTipo()=="Boton")
                {
                    MenuBoton*mb=((MenuBoton*)((MenuContenedor*)selectables_container)->getElementoSeleccionado());
                    if(mb->getAccion()=="Start")
                    {
//                        iniciarJuego();
                    }
                    if(mb->getAccion()=="Continue")
                    {
                        break;
                    }
                    if(mb->getAccion()=="Exit")
                    {
                        exit_signal=true;
                        break;
                    }
                    if(mb->getAccion()=="load")
                    {
                        string menu_directory = assets_directory+mb->load_menu;
                        Menu *temp=new Menu(painter,receiver,sonido,(char*)menu_directory.c_str());
                        temp->loopMenu();
                    }
                    if(mb->getAccion().substr(0,6)=="Player")
                    {
                        int player;
                        std::string mapeo="";
                        std::string accion=mb->getAccion();
                        if(accion.substr(0,7)=="Player1")
                            player=1;
                        else
                            player=2;
                        RosalilaInputs* temp=new RosalilaInputs();
                        RosalilaInputs* temp2=new RosalilaInputs();
                        if(player==1)
                        {
                            temp->loadFromXML(1,receiver);
                            temp2->loadFromXML(2,receiver);
                        }else
                        {
                            temp->loadFromXML(2,receiver);
                            temp2->loadFromXML(1,receiver);
                        }
                        if(accion=="Player1.KeyConfig:up"||accion=="Player2.KeyConfig:up")mapeo="8";
                        if(accion=="Player1.KeyConfig:down"||accion=="Player2.KeyConfig:down")mapeo="2";
                        if(accion=="Player1.KeyConfig:left"||accion=="Player2.KeyConfig:left")mapeo="4";
                        if(accion=="Player1.KeyConfig:right"||accion=="Player2.KeyConfig:right")mapeo="6";
                        if(accion=="Player1.KeyConfig:a"||accion=="Player2.KeyConfig:a")mapeo="a";
                        if(accion=="Player1.KeyConfig:b"||accion=="Player2.KeyConfig:b")mapeo="b";
                        if(accion=="Player1.KeyConfig:c"||accion=="Player2.KeyConfig:c")mapeo="c";
                        if(accion=="Player1.KeyConfig:d"||accion=="Player2.KeyConfig:d")mapeo="d";
                        if(accion=="Player1.KeyConfig:e"||accion=="Player2.KeyConfig:e")mapeo="e";
                        if(accion=="Player1.KeyConfig:f"||accion=="Player2.KeyConfig:f")mapeo="f";

                        mb->input_config="";
                        std::string str_input=getRosalilaInputsPressed();
                        //key
                        if((char)str_input[0]!='j')
                        {
                            int pos=-1,posc=-1;
                            for(int j=0;j<(int)temp->botones.size();j++)
                                if(temp->botones[j].getMapeo()==mapeo && !temp->botones[j].usaJoystick())
                                    pos=j;

                            for(int j=0;j<(int)temp->cruz.size();j++)
                                if(temp->cruz[j].getMapeo()==mapeo && !temp->cruz[j].usaJoystick())
                                    posc=j;

                            Boton b(receiver,toKeyCode(str_input),mapeo);
                            if(pos!=-1)
                                temp->botones[pos]=b;
                            else if (posc!=-1)
                                temp->cruz[posc]=b;
                            else
                                temp->botones.push_back(b);
                        }else//joy
                        {
                            int pos=-1,posc=-1;
                            for(int j=0;j<(int)temp->botones.size();j++)
                                if(temp->botones[j].getMapeo()==mapeo && temp->botones[j].usaJoystick())
                                    pos=j;

                            for(int j=0;j<(int)temp->cruz.size();j++)
                                if(temp->cruz[j].getMapeo()==mapeo && temp->cruz[j].usaJoystick())
                                    posc=j;

                            int int_input=((int)(str_input[3]))-48;
                            if((char)str_input[3]=='u')
                                int_input=-8;
                            if((char)str_input[3]=='d')
                                int_input=-2;
                            if((char)str_input[3]=='l')
                                int_input=-4;
                            if((char)str_input[3]=='r')
                                int_input=-6;
                            Boton b(receiver,int_input,((int)str_input[1])-48,mapeo);
                            if(pos!=-1)
                            {
                                temp->botones[pos]=b;
                            }else if(posc!=-1)
                            {
                                temp->cruz[posc]=b;
                            }else
                            {
                                temp->botones.push_back(b);
                            }
                        }

                        escribirRosalilaInputssXML(temp,temp2);
                        //mb->input_config=b.keyToString();
                        llenarRosalilaInputssBotones();
                    }
                }
            }else
            {
                tecla_arriba=true;
            }
        }
	}
}

void Menu::dibujarMenu()
{
    for(int i=0;i<(int)elementos.size();i++)
    {
        if(char_select==NULL)
        {
            elementos[i]->dibujar();
        }else if(elementos[i]->getTipo()!="Contenedor" || char_select->listoPA())
        {
            elementos[i]->dibujar();
        }
    }

    receiver->updateInputs();
    painter->updateScreen();
}

void Menu::cargarDesdeXml(char* archivo,vector<std::string> chars,vector<std::string> stages)
{
    music_path=assets_directory+"menu/audio/music.ogg";

    //cargarConfig();

    TiXmlDocument doc_t( archivo );
    doc_t.LoadFile();
    TiXmlDocument *doc;
    doc=&doc_t;

    sonido->addSound("Menu.select",assets_directory+"menu/audio/select.ogg");
    sonido->addSound("Menu.select_char",assets_directory+"menu/audio/select_char.ogg");
    sonido->addSound("Menu.move",assets_directory+"menu/audio/move.ogg");
    sonido->addSound("Menu.move_char",assets_directory+"menu/audio/move_char.ogg");
    sonido->addSound("Menu.back",assets_directory+"menu/audio/back.ogg");

    TiXmlNode* elemento=doc->FirstChild("svg");
    TiXmlNode* g_node=elemento->FirstChild("g");

    std::vector<Elemento*>elementos_contenedor;
    for(TiXmlNode* image_node=g_node->FirstChild("image");
            image_node!=NULL;
            image_node=image_node->NextSibling("image"))
    {
        TiXmlElement* e=image_node->ToElement();

        if(e->Attribute("type")==NULL)
        {
            std::string path(e->Attribute("xlink:href"));

            std::string dir("menu/");
            path=dir+path;
            int x=0;
            int y=0;
            int width=0;
            int height=0;
            int displacement_x=0;
            int displacement_y=0;
            int stop_displacement_x_at=0;
            int stop_displacement_y_at=0;
            int fade_in_initial=-1;
            int fade_in_speed=0;

            if(e->Attribute("x")!=NULL)
                x=atoi(e->Attribute("x"));
            if(e->Attribute("y")!=NULL)
                y=atoi(e->Attribute("y"));
            if(e->Attribute("width")!=NULL)
                width=atoi(e->Attribute("width"));
            if(e->Attribute("height")!=NULL)
                height=atoi(e->Attribute("height"));
            if(e->Attribute("displacement_x")!=NULL)
                displacement_x=atoi(e->Attribute("displacement_x"));
            if(e->Attribute("displacement_y")!=NULL)
                displacement_y=atoi(e->Attribute("displacement_y"));
            if(e->Attribute("stop_displacement_x_at")!=NULL)
                stop_displacement_x_at=atoi(e->Attribute("stop_displacement_x_at"));
            if(e->Attribute("stop_displacement_y_at")!=NULL)
                stop_displacement_y_at=atoi(e->Attribute("stop_displacement_y_at"));
            if(e->Attribute("fade_in_initial")!=NULL)
                fade_in_initial=atoi(e->Attribute("fade_in_initial"));
            if(e->Attribute("fade_in_speed")!=NULL)
                fade_in_speed=atoi(e->Attribute("fade_in_speed"));
            Image* image=painter->getTexture(assets_directory+path);

            elementos.push_back((Elemento*)new MenuImagen(painter,x,y,displacement_x,displacement_y,stop_displacement_x_at,stop_displacement_y_at,fade_in_initial,fade_in_speed,
                                                          width,height,image,""
                                                          ));
        }else if(strcmp(e->Attribute("type"),"button")==0)
        {
            std::string action="";
            if(e->Attribute("action")!=NULL)
               action=e->Attribute("action");

            char* menu_load=new char[255];
            strcpy(menu_load,"");
            if(action=="load")
            {
                strcpy(menu_load,"menu/");
                strcat(menu_load,e->Attribute("load_menu"));
            }

            int x=0;
            int y=0;
            int width=0;
            int height=0;

            std::string path=e->Attribute("xlink:href");

            if(e->Attribute("x")!=NULL)
                x=atoi(e->Attribute("x"));

            if(e->Attribute("y")!=NULL)
                y=atoi(e->Attribute("y"));

            if(e->Attribute("width")!=NULL)
                width=atoi(e->Attribute("width"));

            if(e->Attribute("height")!=NULL)
                height=atoi(e->Attribute("height"));

            int text_x=x;
            if(e->Attribute("text_x")!=NULL)
                text_x=atoi(e->Attribute("text_x"));

            int text_y=y;
            if(e->Attribute("text_y")!=NULL)
                text_x=atoi(e->Attribute("text_y"));

            std::string text="";
            if(e->Attribute("text")!=NULL)
                text=e->Attribute("text");

            std::string path_selected=path;
            if(e->Attribute("image_selected")!=NULL)
                path_selected=e->Attribute("image_selected");

            int text_x_selected=text_x;
            if(e->Attribute("text_x_selected")!=NULL)
                text_x_selected=atoi(e->Attribute("text_x_selected"));

            int text_y_selected=text_y;
            if(e->Attribute("text_y_selected")!=NULL)
                text_y_selected=atoi(e->Attribute("text_y_selected"));

            std::string text_selected=text;
            if(e->Attribute("text_selected")!=NULL)
                text_selected=e->Attribute("text_selected");


            elementos_contenedor.push_back((Elemento*)new MenuBoton(painter,
                                                                    x,y,
                                                                    width,height,
                                                                    painter->getTexture(assets_directory+std::string("menu/")+path),
                                                                    text_x,text_y,text,
                                                                    painter->getTexture(assets_directory+std::string("menu/")+path_selected),
                                                                    text_x_selected,text_y_selected,text_selected,
                                                                    action,menu_load
                                                                    ));

        }
//        else if(strcmp(e->Attribute("type"),"bar")==0)
//        {
//            int accion=-1;
//            int default_value=0;
//            if(e->Attribute("default_value")!=NULL)
//                atoi(e->Attribute("default_value"));
//            if(e->Attribute("action")!=NULL)
//            {
//                if(strcmp(e->Attribute("action"),"AI level")==0)
//                {
//                    accion=0;
//                    default_value=ai_level;
//                }
//                accion=atoi(e->Attribute("action"));
//            }
//
//            int x=atoi(e->Attribute("x"));
//            int y=atoi(e->Attribute("y"));
//            int width=atoi(e->Attribute("width"));
//            int height=atoi(e->Attribute("height"));
//
//            std::string path(e->Attribute("xlink:href"));
//
//            std::string background=path;
//            if(e->Attribute("background")!=NULL)
//                background=e->Attribute("background");
//
//            int bar_x=x;
//            if(e->Attribute("bar_x")!=NULL)
//                bar_x=atoi(e->Attribute("bar_x"));
//
//            int bar_y=y;
//            if(e->Attribute("bar_y")!=NULL)
//                bar_y=atoi(e->Attribute("bar_y"));
//
//            std::string background_selected=background;
//            if(e->Attribute("background_selected")!=NULL)
//                background_selected=e->Attribute("background_selected");
//
//            int bar_x_selected=bar_x;
//            if(e->Attribute("bar_x_selected")!=NULL)
//                bar_x_selected=atoi(e->Attribute("bar_x_selected"));
//
//            int bar_y_selected=bar_y;
//            if(e->Attribute("bar_y_selected")!=NULL)
//                bar_y_selected=atoi(e->Attribute("bar_y_selected"));
//
//            std::string image_selected=path;
//            if(e->Attribute("image_selected")!=NULL)
//                image_selected=e->Attribute("image_selected");
//
//            int max=9999;
//            if(e->Attribute("max")!=NULL)
//                max=atoi(e->Attribute("max"));
//
//            elementos_contenedor.push_back((Elemento*)new MenuBarra(painter,
//                                                                    x,y,
//                                                                    width,height,
//                                                                    painter->getTexture(std::string("menu/")+background),
//                                                                    bar_x,bar_y,
//                                                                    painter->getTexture(std::string("menu/")+path),
//                                                                    painter->getTexture(std::string("menu/")+background_selected),
//                                                                    bar_x_selected,bar_y_selected,
//                                                                    painter->getTexture(std::string("menu/")+image_selected),
//                                                                    max,default_value,accion
//                                                                    )
//                                           );
//        }
    }

    if(g_node->FirstChild("rect")!=NULL)
    for(TiXmlNode* rect_node=g_node->FirstChild("rect");
            rect_node!=NULL;
            rect_node=rect_node->NextSibling("rect"))
    {
        TiXmlElement* e=rect_node->ToElement();
        if(e->Attribute("type")!=NULL)
        {
            if(strcmp(e->Attribute("type"),"char_select")==0)
            {
                int x=0;
                if(e->Attribute("x")!=NULL)
                    x=atoi(e->Attribute("x"));

                int y=0;
                if(e->Attribute("y")!=NULL)
                    y=atoi(e->Attribute("y"));

                int size_x=3;
                if(e->Attribute("size_x")!=NULL)
                    size_x=atoi(e->Attribute("size_x"));

                int size_y=3;
                if(e->Attribute("size_y")!=NULL)
                    size_y=atoi(e->Attribute("size_y"));

                int box_size_x=100;
                if(e->Attribute("box_size_x")!=NULL)
                    box_size_x=atoi(e->Attribute("box_size_x"));

                int box_size_y=100;
                if(e->Attribute("box_size_y")!=NULL)
                    box_size_y=atoi(e->Attribute("box_size_y"));

                int box_separation_x=0;
                if(e->Attribute("box_separation_x")!=NULL)
                    box_separation_x=atoi(e->Attribute("box_separation_x"));

                int box_separation_y=0;
                if(e->Attribute("box_separation_y")!=NULL)
                    box_separation_y=atoi(e->Attribute("box_separation_y"));

                int chars_player1=1;
                if(e->Attribute("chars_player1")!=NULL)
                    chars_player1=atoi(e->Attribute("chars_player1"));

                int chars_player2=1;
                if(e->Attribute("chars_player2")!=NULL)
                    chars_player2=atoi(e->Attribute("chars_player2"));

                int preview_player1_x=0;
                if(e->Attribute("preview_player1_x")!=NULL)
                    preview_player1_x=atoi(e->Attribute("preview_player1_x"));

                int preview_player1_y=0;
                if(e->Attribute("preview_player1_y")!=NULL)
                    preview_player1_y=atoi(e->Attribute("preview_player1_y"));

                int preview_player2_x=0;
                if(e->Attribute("preview_player2_x")!=NULL)
                    preview_player2_x=atoi(e->Attribute("preview_player2_x"));

                int preview_player2_y=0;
                if(e->Attribute("preview_player2_y")!=NULL)
                    preview_player2_y=atoi(e->Attribute("preview_player2_y"));

                int player1_cursor_x=0;
                if(e->Attribute("player1_cursor_x")!=NULL)
                    player1_cursor_x=atoi(e->Attribute("player1_cursor_x"));

                int player1_cursor_y=0;
                if(e->Attribute("player1_cursor_y")!=NULL)
                    player1_cursor_y=atoi(e->Attribute("player1_cursor_y"));

                int player2_cursor_x=0;
                if(e->Attribute("player2_cursor_x")!=NULL)
                    player2_cursor_x=atoi(e->Attribute("player2_cursor_x"));

                int player2_cursor_y=0;
                if(e->Attribute("player2_cursor_y")!=NULL)
                    player2_cursor_y=atoi(e->Attribute("player2_cursor_y"));

                char_select=new MenuCharSelect(painter,x,y,
                                               size_x,size_y,
                                               box_size_x,box_size_y,
                                               box_separation_x,box_separation_y,
                                               chars_player1,chars_player2,
                                               preview_player1_x,preview_player1_y,
                                               preview_player2_x,preview_player2_y,
                                               chars,
                                               player1_cursor_x,player1_cursor_y,
                                               player2_cursor_x,player2_cursor_y
                                               );
                elementos.push_back((Elemento*)char_select);

            }else if(strcmp(e->Attribute("type"),"stage_select")==0)
            {
                pos_stage=elementos_contenedor.size();
                std::vector<Elemento*>elem_lista;
                for(int i=0;i<(int)stages.size();i++)
                {
                    int displacement_x=0;
                    int displacement_y=0;
                    int stop_displacement_x_at=0;
                    int stop_displacement_y_at=0;
                    int fade_in_initial=-1;
                    int fade_in_speed=0;
//                    if(e->Attribute("displacement_x")!=NULL)
//                        displacement_x=atoi(e->Attribute("displacement_x"));
//                    if(e->Attribute("displacement_y")!=NULL)
//                        displacement_y=atoi(e->Attribute("displacement_y"));
//                    if(e->Attribute("stop_displacement_x_at")!=NULL)
//                        stop_displacement_x_at=atoi(e->Attribute("stop_displacement_x_at"));
//                    if(e->Attribute("stop_displacement_y_at")!=NULL)
//                        stop_displacement_y_at=atoi(e->Attribute("stop_displacement_y_at"));
//                    if(e->Attribute("fade_in_initial")!=NULL)
//                        fade_in_initial=atoi(e->Attribute("fade_in_initial"));
//                    if(e->Attribute("fade_in_speed")!=NULL)
//                        fade_in_speed=atoi(e->Attribute("fade_in_speed"));

                    Image*image=painter->getTexture(assets_directory+std::string("stages/")+stages[i]+std::string("/images/preview.png"));


                    elem_lista.push_back((Elemento*)new MenuImagen(painter,
                                                                   atoi(e->Attribute("x")),atoi(e->Attribute("y")),
                                                                   displacement_x,displacement_y,stop_displacement_x_at,stop_displacement_y_at,fade_in_initial,fade_in_speed,
                                                                   atoi(e->Attribute("width")),atoi(e->Attribute("height")),
                                                                   image,stages[i]
                                                                   ));
                }

                int x=0;
                if(e->Attribute("x")!=NULL)
                    x=atoi(e->Attribute("x"));

                int y=0;
                if(e->Attribute("y"))
                    y=atoi(e->Attribute("y"));

                int arrow_left_x=x;
                if(e->Attribute("arrow_left_x"))
                    atoi(e->Attribute("arrow_left_x"));

                int arrow_left_y=y;
                if(e->Attribute("arrow_left_y"))
                    atoi(e->Attribute("arrow_left_y"));

                Image*path_left=NULL;
                if(e->Attribute("path_left")!=NULL)
                    path_left=painter->getTexture(std::string("menu/")+std::string(e->Attribute("path_left")));

                int arrow_right_x=x;
                if(e->Attribute("arrow_right_x"))
                    atoi(e->Attribute("arrow_right_x"));

                int arrow_right_y=y;
                if(e->Attribute("arrow_right_y"))
                    atoi(e->Attribute("arrow_right_y"));

                Image*path_right=NULL;
                if(e->Attribute("path_right")!=NULL)
                    path_right=painter->getTexture(assets_directory+std::string("menu/")+std::string(e->Attribute("path_right")));

                int arrow_left_x_selected=x;
                if(e->Attribute("arrow_left_x_selected"))
                    atoi(e->Attribute("arrow_left_x_selected"));

                int arrow_left_y_selected=y;
                if(e->Attribute("arrow_left_y_selected"))
                    atoi(e->Attribute("arrow_left_y_selected"));

                Image*path_left_selected=path_left;
                if(e->Attribute("path_left_selected")!=NULL)
                    path_left_selected=painter->getTexture(assets_directory+std::string("menu/")+std::string(e->Attribute("path_left_selected")));

                int arrow_right_x_selected=x;
                if(e->Attribute("arrow_right_x_selected"))
                    atoi(e->Attribute("arrow_right_x_selected"));

                int arrow_right_y_selected=y;
                if(e->Attribute("arrow_right_y_selected"))
                    atoi(e->Attribute("arrow_right_y_selected"));

                Image*path_right_selected=path_right;
                if(e->Attribute("path_right_selected")!=NULL)
                    path_right_selected=painter->getTexture(assets_directory+std::string("menu/")+std::string(e->Attribute("path_right_selected")));

                elementos_contenedor.push_back((Elemento*)new MenuLista(painter,x,y,
                                                                        arrow_left_x,arrow_left_y,
                                                                        path_left,
                                                                        arrow_right_x,arrow_right_y,
                                                                        path_right,
                                                                        arrow_left_x_selected,arrow_left_y_selected,
                                                                        path_left_selected,
                                                                        arrow_right_x_selected,arrow_right_y_selected,
                                                                        path_right_selected,
                                                                        elem_lista,
                                                                        "start"
                                                                        )
                                               );

            }
        }
    }


    for(TiXmlNode* text_node=g_node->FirstChild("text");
            text_node!=NULL;
            text_node=text_node->NextSibling("text"))
    {
        TiXmlElement* e=text_node->ToElement();

        int x=0;
        if(atoi(e->Attribute("x")))
        {
            x=atoi(e->Attribute("x"));
        }

        int y=0;
        if(atoi(e->Attribute("y")))
        {
            y=atoi(e->Attribute("y"));
        }

        std::string tspan="";
        if(text_node->FirstChild("tspan")!=NULL)
        {
            TiXmlElement* text_span_elem=text_node->FirstChild("tspan")->ToElement();
            if(text_span_elem->GetText()!=NULL)
                tspan=text_span_elem->GetText();
        }

        elementos.push_back((Elemento*)new MenuTexto(painter,x,y,
                                                     tspan
                                                     ));
    }

    selectables_container=new MenuContenedor(painter,elementos_contenedor);
    elementos.push_back((Elemento*)selectables_container);
}

std::string Menu::getStage()
{
    //!
    MenuContenedor *mc=((MenuContenedor*)selectables_container);
    MenuLista *ml=(MenuLista*)mc->elementos[pos_stage];
    MenuImagen *mt=(MenuImagen*)ml->getElementoActual();
    std::string str=mt->value;

    return str;
}

bool Menu::getExitSignal()
{
    bool temp=exit_signal;
    exit_signal=false;
    return temp;
}

bool Menu::getSaveRosalilaInputssSignal()
{
    bool temp=save_inputs_signal;
    save_inputs_signal=false;
    return temp;
}

void Menu::escribirRosalilaInputssXML(RosalilaInputs* ia,RosalilaInputs* ib)
{
    TiXmlDocument *doc=new TiXmlDocument();
    ib->getXML(ia->getXML(doc));
    doc->SaveFile( "misc/inputs.xml" );
}

std::string Menu::getRosalilaInputsPressed()
{
    while(true)
    {

        dibujarMenu();
//        if(receiver->isKeyDown(irr::KEY_KEY_Q))
//            return "Q";
//        if(receiver->isKeyDown(SDLK_w))
//            return "W";
//        if(receiver->isKeyDown(irr::KEY_KEY_E))
//            return "E";
//        if(receiver->isKeyDown(irr::KEY_KEY_R))
//            return "R";
//        if(receiver->isKeyDown(irr::KEY_KEY_T))
//            return "T";
//        if(receiver->isKeyDown(irr::KEY_KEY_Y))
//            return "Y";
//        if(receiver->isKeyDown(irr::KEY_KEY_U))
//            return "U";
//        if(receiver->isKeyDown(irr::KEY_KEY_I))
//            return "I";
//        if(receiver->isKeyDown(irr::KEY_KEY_O))
//            return "O";
//        if(receiver->isKeyDown(irr::KEY_KEY_P))
//            return "P";
//        if(receiver->isKeyDown(irr::KEY_KEY_A))
//            return "A";
//        if(receiver->isKeyDown(irr::KEY_KEY_S))
//            return "S";
//        if(receiver->isKeyDown(irr::KEY_KEY_D))
//            return "D";
//        if(receiver->isKeyDown(irr::KEY_KEY_F))
//            return "F";
//        if(receiver->isKeyDown(irr::KEY_KEY_G))
//            return "G";
//        if(receiver->isKeyDown(irr::KEY_KEY_H))
//            return "H";
//        if(receiver->isKeyDown(irr::KEY_KEY_J))
//            return "J";
//        if(receiver->isKeyDown(irr::KEY_KEY_K))
//            return "K";
//        if(receiver->isKeyDown(irr::KEY_KEY_L))
//            return "L";
//        if(receiver->isKeyDown(irr::KEY_KEY_Z))
//            return "Z";
//        if(receiver->isKeyDown(irr::KEY_KEY_X))
//            return "X";
//        if(receiver->isKeyDown(irr::KEY_KEY_C))
//            return "C";
//        if(receiver->isKeyDown(irr::KEY_KEY_V))
//            return "V";
//        if(receiver->isKeyDown(irr::KEY_KEY_B))
//            return "B";
//        if(receiver->isKeyDown(irr::KEY_KEY_N))
//            return "N";
//        if(receiver->isKeyDown(irr::KEY_KEY_M))
//            return "M";

        if(receiver->isJoyDown(0,0))
            return "j0-0";
        if(receiver->isJoyDown(1,0))
            return "j0-1";
        if(receiver->isJoyDown(2,0))
            return "j0-2";
        if(receiver->isJoyDown(3,0))
            return "j0-3";
        if(receiver->isJoyDown(4,0))
            return "j0-4";
        if(receiver->isJoyDown(5,0))
            return "j0-5";
        if(receiver->isJoyDown(6,0))
            return "j0-6";
        if(receiver->isJoyDown(7,0))
            return "j0-7";
        if(receiver->isJoyDown(8,0))
            return "j0-8";
        if(receiver->isJoyDown(9,0))
            return "j0-9";
        if(receiver->isJoyDown(10,0))
            return "j0-10";
        if(receiver->isJoyDown(-2,0))
            return "j0-d";
        if(receiver->isJoyDown(-4,0))
            return "j0-l";
        if(receiver->isJoyDown(-6,0))
            return "j0-r";
        if(receiver->isJoyDown(-8,0))
            return "j0-u";

        if(receiver->isJoyDown(0,1))
            return "j1-0";
        if(receiver->isJoyDown(1,1))
            return "j1-1";
        if(receiver->isJoyDown(2,1))
            return "j1-2";
        if(receiver->isJoyDown(3,1))
            return "j1-3";
        if(receiver->isJoyDown(4,1))
            return "j1-4";
        if(receiver->isJoyDown(5,1))
            return "j1-5";
        if(receiver->isJoyDown(6,1))
            return "j1-6";
        if(receiver->isJoyDown(7,1))
            return "j1-7";
        if(receiver->isJoyDown(8,1))
            return "j1-8";
        if(receiver->isJoyDown(9,1))
            return "j1-9";
        if(receiver->isJoyDown(10,1))
            return "j1-10";
        if(receiver->isJoyDown(-2,1))
            return "j1-d";
        if(receiver->isJoyDown(-4,1))
            return "j1-l";
        if(receiver->isJoyDown(-6,1))
            return "j1-r";
        if(receiver->isJoyDown(-8,1))
            return "j1-u";
    }
}

int Menu::toKeyCode(std::string str)
{
    return -1;
//    if(str==std::string("Q"))
//        return irr::KEY_KEY_Q;
//    if(str==std::string("W"))
//        return SDLK_w;
//    if(str==std::string("E"))
//        return irr::KEY_KEY_E;
//    if(str==std::string("R"))
//        return irr::KEY_KEY_R;
//    if(str==std::string("T"))
//        return irr::KEY_KEY_T;
//    if(str==std::string("Y"))
//        return irr::KEY_KEY_Y;
//    if(str==std::string("U"))
//        return irr::KEY_KEY_U;
//    if(str==std::string("I"))
//        return irr::KEY_KEY_I;
//    if(str==std::string("O"))
//        return irr::KEY_KEY_O;
//    if(str==std::string("P"))
//        return irr::KEY_KEY_P;
//    if(str==std::string("A"))
//        return irr::KEY_KEY_A;
//    if(str==std::string("S"))
//        return irr::KEY_KEY_S;
//    if(str==std::string("D"))
//        return irr::KEY_KEY_D;
//    if(str==std::string("F"))
//        return irr::KEY_KEY_F;
//    if(str==std::string("G"))
//        return irr::KEY_KEY_G;
//    if(str==std::string("H"))
//        return irr::KEY_KEY_H;
//    if(str==std::string("J"))
//        return irr::KEY_KEY_J;
//    if(str==std::string("K"))
//        return irr::KEY_KEY_K;
//    if(str==std::string("L"))
//        return irr::KEY_KEY_L;
//    if(str==std::string("Z"))
//        return irr::KEY_KEY_Z;
//    if(str==std::string("X"))
//        return irr::KEY_KEY_X;
//    if(str==std::string("C"))
//        return irr::KEY_KEY_C;
//    if(str==std::string("V"))
//        return irr::KEY_KEY_V;
//    if(str==std::string("B"))
//        return irr::KEY_KEY_B;
//    if(str==std::string("N"))
//        return irr::KEY_KEY_N;
//    if(str==std::string("M"))
//        return irr::KEY_KEY_M;
//    return irr::KEY_ESCAPE;
}


void Menu::llenarRosalilaInputssBotones()
{
    if(selectables_container==NULL)
        return;
    for(int i=0;i<(int)selectables_container->elementos.size();i++)
    {
        if(((MenuContenedor*)selectables_container)->elementos[i]->getTipo()=="Contenedor")
        {
            MenuBoton*mb=(MenuBoton*)selectables_container->elementos[i];
            if(mb->getAccion().substr(0,6)=="Player")
            {
                int player;
                std::string mapeo="";
                std::string accion=mb->getAccion();
                if(accion.substr(0,7)=="Player1")
                    player=1;
                else
                    player=2;
                RosalilaInputs* temp=new RosalilaInputs();
                RosalilaInputs* temp2=new RosalilaInputs();
                if(player==1)
                {
                    temp->loadFromXML(1,receiver);
                    temp2->loadFromXML(2,receiver);
                }else
                {
                    temp->loadFromXML(2,receiver);
                    temp2->loadFromXML(1,receiver);
                }
                if(accion=="Player1.KeyConfig:up"||accion=="Player2.KeyConfig:up")mapeo="8";
                if(accion=="Player1.KeyConfig:down"||accion=="Player2.KeyConfig:down")mapeo="2";
                if(accion=="Player1.KeyConfig:left"||accion=="Player2.KeyConfig:left")mapeo="4";
                if(accion=="Player1.KeyConfig:right"||accion=="Player2.KeyConfig:right")mapeo="6";
                if(accion=="Player1.KeyConfig:a"||accion=="Player2.KeyConfig:a")mapeo="a";
                if(accion=="Player1.KeyConfig:b"||accion=="Player2.KeyConfig:b")mapeo="b";
                if(accion=="Player1.KeyConfig:c"||accion=="Player2.KeyConfig:c")mapeo="c";
                if(accion=="Player1.KeyConfig:d"||accion=="Player2.KeyConfig:d")mapeo="d";
                if(accion=="Player1.KeyConfig:e"||accion=="Player2.KeyConfig:e")mapeo="e";
                if(accion=="Player1.KeyConfig:f"||accion=="Player2.KeyConfig:f")mapeo="f";

                //key
                mb->input_config="";
                int pos=-1,posc=-1;
                for(int j=0;j<(int)temp->botones.size();j++)
                    if(temp->botones[j].getMapeo()==mapeo && !temp->botones[j].usaJoystick())
                        pos=j;

                for(int j=0;j<(int)temp->cruz.size();j++)
                    if(temp->cruz[j].getMapeo()==mapeo && !temp->cruz[j].usaJoystick())
                        posc=j;

                if(pos!=-1)
                    mb->input_config=temp->botones[pos].keyToString();

                if(posc!=-1)
                    mb->input_config=temp->cruz[posc].keyToString();

                //joy
                pos=-1;
                for(int j=0;j<(int)temp->botones.size();j++)
                    if(temp->botones[j].getMapeo()==mapeo && temp->botones[j].usaJoystick())
                        pos=j;
                posc=-1;
                for(int j=0;j<(int)temp->cruz.size();j++)
                    if(temp->cruz[j].getMapeo()==mapeo && temp->cruz[j].usaJoystick())
                        posc=j;

                if(pos!=-1)
                {
                    mb->input_config+=" j";
                    mb->input_config+=toString((int)temp->botones[pos].getNumJoystick());
                    mb->input_config+=toString((int)temp->botones[pos].joystick);
                }

                if(posc!=-1)
                {
                    mb->input_config+=" j";
                    mb->input_config+=toString((int)temp->cruz[posc].getNumJoystick());
                    mb->input_config+="-";
                    if(temp->cruz[posc].joystick==-8)
                        mb->input_config+=std::string("up");
                    else if(temp->cruz[posc].joystick==-2)
                        mb->input_config+=std::string("down");
                    else if(temp->cruz[posc].joystick==-4)
                        mb->input_config+=std::string("left");
                    else if(temp->cruz[posc].joystick==-6)
                        mb->input_config+=std::string("right");
                    else
                        mb->input_config+=toString((int)temp->cruz[posc].joystick);
                }
            }
        }
    }
}

void Menu::printLoadingScreen()
{
    painter->draw2DImage
    (   loading_screen,
        loading_screen->getWidth(),loading_screen->getHeight(),
        0,0,
        1.0,
        0.0,
        false,
        0,0,
        Color(255,255,255,255),
        0,0,
        false,
        FlatShadow());

    painter->updateScreen();
}

void Menu::playMusic()
{
    sonido->playMusic(this->music_path);
}
