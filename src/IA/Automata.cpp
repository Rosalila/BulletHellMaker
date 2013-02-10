#include "IA/Automata.h"

Automata::Automata(char* archivo,char* archivo_default)
{
    estado_actual="S";
    estados["S"]=new Estado(vector<Transicion*>());
    cargarDesdeXML(archivo,archivo_default);
    transicion_a_recompenzar=NULL;
    wait=0;
}

Estado* Automata::getEstadoInicial()
{
    return estados["S"];
}

std::string Automata::getNextInput(std::map<std::string,std::string>*strings,
                               std::map<std::string,std::string>*strings_contrario,
                               std::map<std::string,int>*enteros,
                               std::map<std::string,int>*enteros_contrario
                               )
{
    if(wait>0)
    {
        wait--;
        //transicion_a_recompenzar=NULL;
        return "5";
    }else
    {
        Estado* e=estados[estado_actual];
        Transicion* t=e->getNextTransicion(strings,strings_contrario,enteros,enteros_contrario);
        if(t->recompensable)
        {
            transicion_a_recompenzar=t;
        }
        estado_actual=t->getDestino();
        wait=t->getWait();
        return t->getInput();
    }
}

void Automata::cargarDesdeXML(char* archivo,char* archivo_default)
{
    TiXmlDocument doc_t(archivo);
    bool loadOkay = doc_t.LoadFile();
    if(!loadOkay)
    {
        doc_t=TiXmlDocument(archivo_default);
        loadOkay=doc_t.LoadFile();
        cout.flush();
    }
    TiXmlDocument *doc;
    doc=&doc_t;

    for(TiXmlNode* nodo=doc->FirstChild("State");
            nodo!=NULL;
            nodo=nodo->NextSibling("State"))
    {
        std::vector<Transicion*> transiciones;
        for(TiXmlElement *e_action=nodo->FirstChild("Action")->ToElement();
                e_action!=NULL;
                e_action=e_action->NextSiblingElement("Action"))
        {
            Transicion *transicion;
            if(e_action->Attribute("rewardable")!=NULL)
                transicion=new Transicion(e_action->Attribute("input"),e_action->Attribute("destination"),strcmp(e_action->Attribute("rewardable"),"yes")==0);
            else
                transicion=new Transicion(e_action->Attribute("input"),e_action->Attribute("destination"),false);
            if(e_action->Attribute("reward")!=NULL)
                transicion->setRecompensa(atoi(e_action->Attribute("reward")));
            if(e_action->Attribute("wait")!=NULL)
                transicion->setWait(atoi(e_action->Attribute("wait")));




            if(e_action->FirstChild("condition")!=NULL)
            {
                for(TiXmlElement *elemento_condicion=e_action->FirstChild("condition")->ToElement();
                        elemento_condicion!=NULL;
                        elemento_condicion=elemento_condicion->NextSiblingElement("condition"))
                {
                    std::string exp_i(elemento_condicion->Attribute("left_exp"));
                    std::string op(elemento_condicion->Attribute("relational_op"));
                    std::string exp_d(elemento_condicion->Attribute("right_exp"));
                    std::string str_contrario(elemento_condicion->Attribute("to_opponent"));
                    bool contrario=(str_contrario=="yes");
                    transicion->agregarCondicion(Condicion2(exp_i,op,exp_d,contrario));
                }
            }

            if(e_action->FirstChild("condition_integer")!=NULL)
            {
                for(TiXmlElement *elemento_condicion=e_action->FirstChild("condition_integer")->ToElement();
                        elemento_condicion!=NULL;
                        elemento_condicion=elemento_condicion->NextSiblingElement("condition_integer"))
                {
                    std::string exp_i(elemento_condicion->Attribute("left_exp"));
                    std::string op(elemento_condicion->Attribute("relational_op"));
                    int exp_d=atoi(elemento_condicion->Attribute("right_exp"));
                    std::string str_contrario(elemento_condicion->Attribute("to_opponent"));
                    bool contrario=(str_contrario=="yes");
                    transicion->agregarCondicion(Condicion2(exp_i,op,exp_d,contrario));
                }
            }
            transiciones.push_back(transicion);
        }
        estados[std::string(nodo->ToElement()->Attribute("name"))]=new Estado(transiciones);
    }
}
