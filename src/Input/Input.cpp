#include "Input/Input.h"
#include <sstream>

void Input::actualizarBuffer(std::map<std::string,std::string>*strings,
                             std::map<std::string,std::string>*strings_contrario,
                             std::map<std::string,int>*enteros,
                             std::map<std::string,int>*enteros_contrario
                             )
{
    std::string resultado="";
    if(!inteligencia_artificial)
    {
        for(int i=0;i<(int)cruz.size();i++)
            if(cruz[i].estaPresionado())
                resultado+=cruz[i].getMapeo();
        if(resultado=="24" || resultado=="42")
            resultado="1";
        if(resultado=="26" || resultado=="62")
            resultado="3";
        if(resultado=="84" || resultado=="48")
            resultado="7";
        if(resultado=="68" || resultado=="86")
            resultado="9";
        if(tecla_arriba)
        {
            tecla_arriba=false;
            for(int i=0;i<(int)botones.size();i++)
                if(botones[i].estaPresionado())
                    resultado+=botones[i].getMapeo();
        }
        bool flag=false;
        for(int i=0;i<(int)botones.size();i++)
            if(botones[i].estaPresionado())
                flag=true;
        if(!flag)
            tecla_arriba=true;
    }
    else
    {
        resultado=ia->getInput(strings,strings_contrario,enteros,enteros_contrario);
    }
    if(resultado=="")
        resultado="5";

    if(std::string(strings->operator[]("orientation"))==std::string("i"))
    {
        for(int i=0;i<(int)resultado.size();i++)
        {
            if(resultado[i]=='4')resultado[i]='6';
            else if(resultado[i]=='6')resultado[i]='4';
            else if(resultado[i]=='3')resultado[i]='1';
            else if(resultado[i]=='1')resultado[i]='3';
            else if(resultado[i]=='7')resultado[i]='9';
            else if(resultado[i]=='9')resultado[i]='7';
        }
    }
    buffer_inputs.insert(buffer_inputs.begin(),resultado);
    buffer_inputs.pop_back();
}

void Input::actualizarBuffer()
{
    std::string resultado="";

    for(int i=0;i<(int)cruz.size();i++)
        if(cruz[i].estaPresionado())
            resultado+=cruz[i].getMapeo();

    if(resultado=="24" || resultado=="42")
        resultado="1";
    if(resultado=="26" || resultado=="62")
        resultado="3";
    if(resultado=="84" || resultado=="48")
        resultado="7";
    if(resultado=="68" || resultado=="86")
        resultado="9";
    if(tecla_arriba)
    {
        tecla_arriba=false;
        for(int i=0;i<(int)botones.size();i++)
            if(botones[i].estaPresionado())
                resultado+=botones[i].getMapeo();
    }
    bool flag=false;
    for(int i=0;i<(int)botones.size();i++)
        if(botones[i].estaPresionado())
            flag=true;
    if(!flag)
        tecla_arriba=true;
    if(resultado=="")
        resultado="5";

    buffer_inputs.insert(buffer_inputs.begin(),resultado);
    buffer_inputs.pop_back();
}

vector<std::string> Input::getBufferInputs()
{
    return buffer_inputs;
}

//std::string stringw_to_stdstring(irr::core::stringw sw)
//{
//    std::stringstream ss;
//        ss << sw.c_str();
//        return ss.str();
//}


void Input::cargarDesdeXML(int jugador,Receiver* receiver)
{
    this->jugador=jugador;
    this->receiver=receiver;
    this->inteligencia_artificial=false;
    TiXmlDocument doc_t((char*)"misc/inputs.xml");
    doc_t.LoadFile();
    TiXmlDocument *doc;
    doc=&doc_t;

    std::vector<Boton> botones;
    for(TiXmlNode* input=doc->FirstChild("Input");
            input!=NULL;
            input=input->NextSibling("Input"))
    {
        if(jugador==atoi(input->ToElement()->Attribute("player")))
        {
            //Key
            if(strcmp("keyboard",input->ToElement()->Attribute("type"))==0)
            {
                for(TiXmlNode* boton=input->FirstChild("button");
                        boton!=NULL;
                        boton=boton->NextSibling("button"))
                {
                    int key=0;
                    if(boton->ToElement()->Attribute("input")[0]=='Q' || boton->ToElement()->Attribute("input")[0]=='q')
                        key=SDLK_q;
                    if(boton->ToElement()->Attribute("input")[0]=='W' || boton->ToElement()->Attribute("input")[0]=='w')
                        key=SDLK_w;
                    if(boton->ToElement()->Attribute("input")[0]=='E' || boton->ToElement()->Attribute("input")[0]=='e')
                        key=SDLK_e;
                    if(boton->ToElement()->Attribute("input")[0]=='R' || boton->ToElement()->Attribute("input")[0]=='r')
                        key=SDLK_r;
                    if(boton->ToElement()->Attribute("input")[0]=='T' || boton->ToElement()->Attribute("input")[0]=='t')
                        key=SDLK_t;
                    if(boton->ToElement()->Attribute("input")[0]=='Y' || boton->ToElement()->Attribute("input")[0]=='y')
                        key=SDLK_y;
                    if(boton->ToElement()->Attribute("input")[0]=='U' || boton->ToElement()->Attribute("input")[0]=='u')
                        key=SDLK_u;
                    if(boton->ToElement()->Attribute("input")[0]=='I' || boton->ToElement()->Attribute("input")[0]=='i')
                        key=SDLK_i;
                    if(boton->ToElement()->Attribute("input")[0]=='O' || boton->ToElement()->Attribute("input")[0]=='o')
                        key=SDLK_o;
                    if(boton->ToElement()->Attribute("input")[0]=='P' || boton->ToElement()->Attribute("input")[0]=='p')
                        key=SDLK_p;
                    if(boton->ToElement()->Attribute("input")[0]=='A' || boton->ToElement()->Attribute("input")[0]=='a')
                        key=SDLK_a;
                    if(boton->ToElement()->Attribute("input")[0]=='S' || boton->ToElement()->Attribute("input")[0]=='s')
                        key=SDLK_s;
                    if(boton->ToElement()->Attribute("input")[0]=='D' || boton->ToElement()->Attribute("input")[0]=='d')
                        key=SDLK_d;
                    if(boton->ToElement()->Attribute("input")[0]=='F' || boton->ToElement()->Attribute("input")[0]=='f')
                        key=SDLK_f;
                    if(boton->ToElement()->Attribute("input")[0]=='G' || boton->ToElement()->Attribute("input")[0]=='g')
                        key=SDLK_g;
                    if(boton->ToElement()->Attribute("input")[0]=='H' || boton->ToElement()->Attribute("input")[0]=='h')
                        key=SDLK_h;
                    if(boton->ToElement()->Attribute("input")[0]=='J' || boton->ToElement()->Attribute("input")[0]=='j')
                        key=SDLK_j;
                    if(boton->ToElement()->Attribute("input")[0]=='K' || boton->ToElement()->Attribute("input")[0]=='k')
                        key=SDLK_k;
                    if(boton->ToElement()->Attribute("input")[0]=='L' || boton->ToElement()->Attribute("input")[0]=='l')
                        key=SDLK_l;
                    if(boton->ToElement()->Attribute("input")[0]=='Z' || boton->ToElement()->Attribute("input")[0]=='z')
                        key=SDLK_z;
                    if(boton->ToElement()->Attribute("input")[0]=='X' || boton->ToElement()->Attribute("input")[0]=='x')
                        key=SDLK_x;
                    if(boton->ToElement()->Attribute("input")[0]=='C' || boton->ToElement()->Attribute("input")[0]=='c')
                        key=SDLK_c;
                    if(boton->ToElement()->Attribute("input")[0]=='V' || boton->ToElement()->Attribute("input")[0]=='v')
                        key=SDLK_v;
                    if(boton->ToElement()->Attribute("input")[0]=='B' || boton->ToElement()->Attribute("input")[0]=='b')
                        key=SDLK_b;
                    if(boton->ToElement()->Attribute("input")[0]=='N' || boton->ToElement()->Attribute("input")[0]=='n')
                        key=SDLK_n;
                    if(boton->ToElement()->Attribute("input")[0]=='M' || boton->ToElement()->Attribute("input")[0]=='m')
                        key=SDLK_m;

                    botones.push_back(Boton(receiver,key,std::string(boton->ToElement()->Attribute("map"))));
                }
            }
            //Joy
            if(strcmp("joystick",input->ToElement()->Attribute("type"))==0)
            {
                for(TiXmlNode* boton=input->FirstChild("button");
                        boton!=NULL;
                        boton=boton->NextSibling("button"))
                {
                    int int_boton;
                    if(strcmp(boton->ToElement()->Attribute("input"),"up")==0)
                        int_boton=-8;
                    else if(strcmp(boton->ToElement()->Attribute("input"),"down")==0)
                        int_boton=-2;
                    else if(strcmp(boton->ToElement()->Attribute("input"),"left")==0)
                        int_boton=-4;
                    else if(strcmp(boton->ToElement()->Attribute("input"),"right")==0)
                        int_boton=-6;
                    else
                        int_boton=boton->ToElement()->Attribute("input")[0]-48;
                    botones.push_back(Boton(receiver,int_boton,input->ToElement()->Attribute("joystick_number")[0]-48,boton->ToElement()->Attribute("map")));
                }
            }
        }
    }

    tecla_arriba=true;
    for(int i=0;i<20;i++)
        buffer_inputs.push_back("5");
    for(int i=0;i<(int)botones.size();i++)
    {
        if(botones[i].getMapeo()=="2" || botones[i].getMapeo()=="4" || botones[i].getMapeo()=="6" || botones[i].getMapeo()=="8")
        {
            this->cruz.push_back(botones[i]);
        }
        else
        {
            this->botones.push_back(botones[i]);
        }
    }
}

void Input::cargarIAXML(int jugador,char* archivo,char* archivo_default)
{
    this->jugador=jugador;
    this->inteligencia_artificial=true;
    tecla_arriba=true;
    for(int i=0;i<20;i++)
        buffer_inputs.push_back("5");
    ia=new IA(archivo,archivo_default);

    tecla_arriba=true;
    for(int i=0;i<20;i++)
        buffer_inputs.push_back("5");
    for(int i=0;i<(int)botones.size();i++)
    {
        if(botones[i].getMapeo()=="2" || botones[i].getMapeo()=="4" || botones[i].getMapeo()=="6" || botones[i].getMapeo()=="8")
        {
            this->cruz.push_back(botones[i]);
        }
        else
        {
            this->botones.push_back(botones[i]);
        }
    }
}
TiXmlDocument* Input::getXML(TiXmlDocument *doc)
{
 	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
	doc->LinkEndChild( decl );

	TiXmlElement * root = new TiXmlElement( "Input" );
	doc->LinkEndChild( root );
	root->SetDoubleAttribute("player",jugador);
	root->SetAttribute("type", "keyboard");

	for(int i=0;i<(int)botones.size();i++)
	{
	    if(botones[i].usaJoystick())
            continue;
        TiXmlElement * cxn = new TiXmlElement( "button" );
        root->LinkEndChild( cxn );
        cxn->SetAttribute("input", (char*)botones[i].getInput().c_str());
        cxn->SetAttribute("map", (char*)botones[i].getMapeo().c_str());
	}
	for(int i=0;i<(int)cruz.size();i++)
	{
	    if(cruz[i].usaJoystick())
            continue;
        TiXmlElement * cxn = new TiXmlElement( "button" );
        root->LinkEndChild( cxn );
        if(cruz[i].getInput()[0]=='d')
            cxn->SetAttribute("input", "down");
        else if(cruz[i].getInput()[0]=='l')
            cxn->SetAttribute("input", "left");
        else if(cruz[i].getInput()[0]=='r')
            cxn->SetAttribute("input", "right");
        else if(cruz[i].getInput()[0]=='u')
            cxn->SetAttribute("input", "up");
        else
            cxn->SetAttribute("input", (char*)cruz[i].getInput().c_str());
        cxn->SetAttribute("map", (char*)cruz[i].getMapeo().c_str());
	}


    //Joystick
	root = new TiXmlElement( "Input" );
	doc->LinkEndChild( root );
	root->SetDoubleAttribute("player",jugador);
	root->SetAttribute("type", "joystick");

	for(int i=0;i<(int)botones.size();i++)
	{
	    if(!botones[i].usaJoystick())
            continue;
        root->SetDoubleAttribute("joystick_number",botones[i].getNumJoystick());
        TiXmlElement * cxn = new TiXmlElement( "button" );
        root->LinkEndChild( cxn );
        cxn->SetAttribute("input", (char*)botones[i].getInput().c_str());
        cxn->SetAttribute("map", (char*)botones[i].getMapeo().c_str());
	}
	for(int i=0;i<(int)cruz.size();i++)
	{
	    if(!cruz[i].usaJoystick())
            continue;
        TiXmlElement * cxn = new TiXmlElement( "button" );
        root->LinkEndChild( cxn );
        if(cruz[i].getInput()[0]=='d')
            cxn->SetAttribute("input", "down");
        else if(cruz[i].getInput()[0]=='l')
            cxn->SetAttribute("input", "left");
        else if(cruz[i].getInput()[0]=='r')
            cxn->SetAttribute("input", "right");
        else if(cruz[i].getInput()[0]=='u')
            cxn->SetAttribute("input", "up");
        else
            cxn->SetAttribute("input", (char*)cruz[i].getInput().c_str());
        cxn->SetAttribute("map", (char*)cruz[i].getMapeo().c_str());
	}

	return doc;
	//doc->SaveFile( "appsettings.xml" );
}
