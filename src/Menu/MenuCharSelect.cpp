#include "Menu/MenuCharSelect.h"

MenuCharSelect::MenuCharSelect(Painter*painter,int x, int y, int width, int height,
                   int size_x,int size_y,
                   int size_cuadro_x,int size_cuadro_y,
                   int separacion_x,int separacion_y,
                   int max_locked_chars_pa,int max_locked_chars_pb,
                   int preview_pa_x,int preview_pa_y,
                   int preview_pb_x,int preview_pb_y,
                   std::vector<std::string>names,
                   int select_p1_x,int select_p1_y,
                   int select_p2_x,int select_p2_y)
{
    this->x=x;
    this->y=y;
    this->height=height;
    this->width=width;
    this->visible=visible;
    this->painter=painter;

    this->size_x=size_x;
    this->size_y=size_y;
    this->size_cuadro_x=size_cuadro_x;
    this->size_cuadro_y=size_cuadro_y;

    this->separacion_x=separacion_x;
    this->separacion_y=separacion_y;

    this->max_locked_chars_pa=max_locked_chars_pa;
    this->max_locked_chars_pb=max_locked_chars_pb;

    this->preview_pa_x=preview_pa_x;
    this->preview_pa_y=preview_pa_y;
    this->preview_pb_x=preview_pb_x;
    this->preview_pb_y=preview_pb_y;

    for(int i=0;i<(int)names.size();i++)
    {
        portraits.push_back(painter->getTexture(std::string("chars/")+names[i]+std::string("/portrait.png")));
        previews.push_back(painter->getTexture(std::string("chars/")+names[i]+std::string("/preview.png")));
    }
    this->names=names;

    this->select_p1_x=select_p1_x;
    this->select_p1_y=select_p1_y;
    this->select_p2_x=select_p2_x;
    this->select_p2_y=select_p2_y;

    selected_char_p1=painter->getTexture("menu/selected_char_p1.png");
    selected_char_p2=painter->getTexture("menu/selected_char_p2.png");
    locked_char_p1=painter->getTexture("menu/locked_char_p1.png");
    locked_char_p2=painter->getTexture("menu/locked_char_p2.png");
    no_portrait=painter->getTexture("menu/no_portrait.png");
}

void MenuCharSelect::lockPA(int num_paleta)
{
    //get name
    int name_pos=select_p1_x+select_p1_y*size_x;

    if(name_pos>=(int)names.size())//random
    {
        //reget name
        name_pos=rand()%5;
    }
    //si existe
    if(name_pos>=names.size())
        return;
    std::string name=names[name_pos];
    Image*preview=previews[name_pos];
    //la paleta es seleccionable?
    for(int i=0;i<(int)locks_pb.size();i++)
        if(locks_pb[i].name==name && locks_pb[i].num_paleta==num_paleta)
            num_paleta++;
    if(num_paleta>=8)
        num_paleta=0;

    //inicio lock
    if((int)locks_pa.size()<max_locked_chars_pa)
    {
        locks_pa.push_back(LockedChar(select_p1_x,select_p1_y,name,num_paleta,preview));
    }
}

void MenuCharSelect::lockPB(int num_paleta)
{
    //get name
    int name_pos=select_p2_x+select_p2_y*size_x;

    if(name_pos>=(int)names.size())//random
    {
        //reget name
        name_pos=rand()%5;
    }
    //si existe
    if(name_pos>=names.size())
        return;
    std::string name=names[name_pos];
    Image*preview=previews[name_pos];
    //la paleta es seleccionable?
    for(int i=0;i<(int)locks_pb.size();i++)
        if(locks_pb[i].name==name && locks_pb[i].num_paleta==num_paleta)
            num_paleta++;
    if(num_paleta>=8)
        num_paleta=0;

    if((int)locks_pb.size()<max_locked_chars_pb)
    {
        locks_pb.push_back(LockedChar(select_p2_x,select_p2_y,name,num_paleta,preview));
    }
}

std::vector<std::string> MenuCharSelect::getLockedNamesPA()
{
    std::vector<std::string>res;
    for(int i=0;i<(int)locks_pa.size();i++)
        res.push_back(locks_pa[i].name);
    return res;
}

std::vector<std::string> MenuCharSelect::getLockedNamesPB()
{
    std::vector<std::string>res;
    for(int i=0;i<(int)locks_pb.size();i++)
        res.push_back(locks_pb[i].name);
    return res;
}

std::vector<int> MenuCharSelect::getLockedPalettesPA()
{
    std::vector<int>res;
    for(int i=0;i<(int)locks_pa.size();i++)
        res.push_back(locks_pa[i].num_paleta);
    return res;
}

std::vector<int> MenuCharSelect::getLockedPalettesPB()
{
    std::vector<int>res;
    for(int i=0;i<(int)locks_pb.size();i++)
        res.push_back(locks_pb[i].num_paleta);
    return res;
}

std::vector<Image*> MenuCharSelect::getLockedPreviewsPA()
{
    std::vector<Image*>res;
    for(int i=0;i<(int)locks_pa.size();i++)
        res.push_back(locks_pa[i].preview);
    return res;
}

std::vector<Image*> MenuCharSelect::getLockedPreviewsPB()
{
    std::vector<Image*>res;
    for(int i=0;i<(int)locks_pb.size();i++)
        res.push_back(locks_pb[i].preview);
    return res;
}

bool MenuCharSelect::listo()
{
    return (int)locks_pa.size()==max_locked_chars_pa && (int)locks_pb.size()==max_locked_chars_pb;
}

bool MenuCharSelect::listoPA()
{
    return (int)locks_pa.size()==max_locked_chars_pa;
}

bool MenuCharSelect::listoPB()
{
    return (int)locks_pb.size()==max_locked_chars_pb;
}

std::string MenuCharSelect::getTipo()
{
    return "CharSelect";
}

void MenuCharSelect::dibujar()
{

    int cont=0;

    for(int j=0;j<size_y;j++)
        for(int i=0;i<size_x;i++)
        {
            //draw portraits
            if(cont<(int)portraits.size())
            {
                //draw preview pa
                if(select_p1_x+select_p1_y*size_x==cont)
                {
                    Image *image=previews[cont];
                    painter->draw2DImage
                    (   image,
                        image->getWidth(),image->getHeight(),
                        preview_pa_x,preview_pa_y,
                        1.0,
                        0.0,
                        false,
                        0,0,
                        Color(255,255,255,255),
                        false);
                }
                if(select_p2_x+select_p2_y*size_x==cont)
                {
                    Image *image=previews[cont];
                    painter->draw2DImage
                    (   image,
                        image->getWidth(),image->getHeight(),
                        preview_pb_x,preview_pb_y,
                        1.0,
                        0.0,
                        false,
                        0,0,
                        Color(255,255,255,255),
                        false);
                }
            }
            cont++;
        }
    cont=0;
    for(int j=0;j<size_y;j++)
        for(int i=0;i<size_x;i++)
        {
            //dibujar portraits
            if(cont>=(int)portraits.size())
            {
                Image *image=no_portrait;
                painter->draw2DImage
                (   image,
                    size_cuadro_x,size_cuadro_y,
                    x+i*(size_cuadro_x+separacion_x),y+j*(size_cuadro_y+separacion_y),
                    1.0,
                    0.0,
                    false,
                    0,0,
                    Color(255,255,255,255),
                    false);
            }else
            {
                Image *image=portraits[cont];
                painter->draw2DImage
                (   image,
                    size_cuadro_x,size_cuadro_y,
                    x+i*(size_cuadro_x+separacion_x),y+j*(size_cuadro_y+separacion_y),
                    1.0,
                    0.0,
                    false,
                    0,0,
                    Color(255,255,255,255),
                    false);
            }
            //dibjuar locks pa
            for(int l=0;l<(int)locks_pa.size();l++)
            {
                if(locks_pa[l].x==i && locks_pa[l].y==j)
                {
                    Image *image=locked_char_p1;
                    painter->draw2DImage
                    (   image,
                        size_cuadro_x,size_cuadro_y,
                        x+locks_pa[l].x*(size_cuadro_x+separacion_x),y+locks_pa[l].y*(size_cuadro_y+separacion_y),
                        1.0,
                        0.0,
                        false,
                        0,0,
                        Color(255,255,255,255),
                        false);
                }
            }
            //dibjuar locks pb
            for(int l=0;l<(int)locks_pb.size();l++)
            {
                if(locks_pb[l].x==i && locks_pb[l].y==j)
                {
                    Image *image=locked_char_p2;
                    painter->draw2DImage
                    (   image,
                        size_cuadro_x,size_cuadro_y,
                        x+locks_pb[l].x*(size_cuadro_x+separacion_x),y+locks_pb[l].y*(size_cuadro_y+separacion_y),
                        1.0,
                        0.0,
                        false,
                        0,0,
                        Color(255,255,255,255),
                        false);
                }
            }
            //dibjujar cursor PA
            if(select_p1_x==i&&select_p1_y==j && (int)locks_pa.size()<max_locked_chars_pa)
            {
                Image*image=selected_char_p1;
                painter->draw2DImage
                (   image,
                    size_cuadro_x,size_cuadro_y,
                    x+i*(size_cuadro_x+separacion_x),y+j*(size_cuadro_y+separacion_y),
                    1.0,
                    0.0,
                    false,
                    0,0,
                    Color(255,255,255,255),
                    false);
            }
            //dibjujar cursor PB
            if(select_p2_x==i&&select_p2_y==j && (int)locks_pb.size()<max_locked_chars_pb)
            {
                Image *image=selected_char_p2;
                painter->draw2DImage
                (   image,
                    size_cuadro_x,size_cuadro_y,
                    x+i*(size_cuadro_x+separacion_x),y+j*(size_cuadro_y+separacion_y),
                    1.0,
                    0.0,
                    false,
                    0,0,
                    Color(255,255,255,255),
                    false);
            }
            cont++;
        }
}

void MenuCharSelect::dibujar(int alineacion_x,int alineacion_y)
{
}

void MenuCharSelect::clearLocks()
{
    locks_pa.clear();
    locks_pb.clear();
}
