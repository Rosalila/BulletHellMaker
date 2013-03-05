#include "Utility/Utility.h"

bool writeLogLine(std::string text)
{
    ofstream log_file("log", ios::app);
    log_file<<text<<"\n";
    log_file.close();

    return true;
}

bool clearLog()
{
    ofstream log_file;
    log_file.open ("log");
    log_file<<"";
    log_file.close();

    return true;
}

std::string toString(int number)
{
    if (number == 0)
        return "0";
    std::string temp="";
    std::string returnvalue="";
    while (number>0)
    {
        temp+=number%10+48;
        number/=10;
    }
    for (int i=0;i<temp.length();i++)
        returnvalue+=temp[temp.length()-i-1];
    return returnvalue;
}


bool pointIsInRect(int point_x,int point_y,
              int rect_x,int rect_y,int rect_width,int rect_height)
{
    if(point_x>rect_x&& point_x<rect_x+rect_width
    && point_y>rect_y && point_y<rect_y+rect_height)
        return true;
    return false;
}


//bool hitboxCollision(int a_x,int a_y,int a_width,int a_height,float a_angle,
//              int b_x,int b_y,int b_width,int b_height,float b_angle)
//{
//////
////painter->drawRectangle(a_x,a_y,4,4,0,0,255,255,255,true);
////painter->drawRectangle(a_x+a_width,a_y,4,4,0,0,255,255,255,true);
////painter->drawRectangle(a_x,a_y+a_height,4,4,0,0,255,255,255,true);
////painter->drawRectangle(a_x+a_width,a_y+a_height,4,4,0,0,255,255,255,true);
//////
//
//    //Move the universe (the point b)
//    float s = sin(a_angle*PI/180);
//    float c = cos(a_angle*PI/180);
//
//    // translate point back to origin:
//    b_x -= a_x;
//    b_y -= a_y;
//
//    // rotate point
//    float xnew = b_x * c - b_y * s;
//    float ynew = b_x * s + b_y * c;
//
//    // translate point back:
//    b_x = xnew + a_x;
//    b_y = ynew + a_y;
//
//    //Rotate to move the other points
//    b_angle-=a_angle;
//
//    if(pointIsInRect(b_x,b_y,
//                    a_x,a_y,a_width,a_height) )
//        return true;
//
//////
////painter->drawRectangle(b_x,b_y,4,4,0,0,255,0,255,true);
//////
//
//
//    int change_x_1 = cos (b_angle*PI/180) * b_width;
//    int change_y_1 = sin (b_angle*PI/180) * b_width;
//
//    if(pointIsInRect(b_x+change_x_1,b_y-change_y_1,
//                    a_x,a_y,a_width,a_height) )
//        return true;
//
//
//////
////painter->drawRectangle(b_x+change_x_1,b_y-change_y_1,4,4,0,0,255,0,255,true);
//////
//
//
//    int change_x_2 = sin (b_angle*PI/180) * b_height;
//    int change_y_2 = cos (b_angle*PI/180) * b_height;
//
//    if(pointIsInRect(b_x+change_x_2,b_y+change_y_2,
//                    a_x,a_y,a_width,a_height) )
//        return true;
//
//////
////painter->drawRectangle(b_x+change_x_2,b_y+change_y_2,4,4,0,0,255,0,255,true);
//////
//
//
//    if(pointIsInRect(b_x+change_x_1+change_x_2,b_y-change_y_1+change_y_2,
//                    a_x,a_y,a_width,a_height) )
//        return true;
//
//////
////painter->drawRectangle(b_x+change_x_1+change_x_2,b_y-change_y_1+change_y_2,4,4,0,0,255,0,255,true);
//////
//
//    return false;
//}

bool hitboxCollision(int a_x,int a_y,int a_width,int a_height,float a_angle,
              int b_x,int b_y,int b_width,int b_height,float b_angle)
{
    Point pa1(a_x,
              a_y);

    Point pa2(a_x + cos (a_angle*PI/180) * a_width
             ,a_y - sin (a_angle*PI/180) * a_width);

    Point pa3(a_x + cos (a_angle*PI/180) * a_width + sin (a_angle*PI/180) * a_height,
              a_y - sin (a_angle*PI/180) * a_width + cos (a_angle*PI/180) * a_height);

    Point pa4(a_x + sin (a_angle*PI/180) * a_height,
              a_y + cos (a_angle*PI/180) * a_height);


    Point pb1(b_x,
              b_y);

    Point pb2(b_x + cos (b_angle*PI/180) * b_width
             ,b_y - sin (b_angle*PI/180) * b_width);

    Point pb3(b_x + cos (b_angle*PI/180) * b_width + sin (b_angle*PI/180) * b_height,
              b_y - sin (b_angle*PI/180) * b_width + cos (b_angle*PI/180) * b_height);

    Point pb4(b_x + sin (b_angle*PI/180) * b_height,
              b_y + cos (b_angle*PI/180) * b_height);

    Line la1(pa1,pa2);
    Line la2(pa2,pa3);
    Line la3(pa3,pa4);
    Line la4(pa4,pa1);

    Line lb1(pb1,pb2);
    Line lb2(pb2,pb3);
    Line lb3(pb3,pb4);
    Line lb4(pb4,pb1);

    if(segmentIntersection(la1,lb1))
        return true;
    if(segmentIntersection(la1,lb2))
        return true;
    if(segmentIntersection(la1,lb3))
        return true;
    if(segmentIntersection(la1,lb4))
        return true;

    if(segmentIntersection(la2,lb1))
        return true;
    if(segmentIntersection(la2,lb2))
        return true;
    if(segmentIntersection(la2,lb3))
        return true;
    if(segmentIntersection(la2,lb4))
        return true;

    if(segmentIntersection(la3,lb1))
        return true;
    if(segmentIntersection(la3,lb2))
        return true;
    if(segmentIntersection(la3,lb3))
        return true;
    if(segmentIntersection(la3,lb4))
        return true;

    if(segmentIntersection(la4,lb1))
        return true;
    if(segmentIntersection(la4,lb2))
        return true;
    if(segmentIntersection(la4,lb3))
        return true;
    if(segmentIntersection(la4,lb4))
        return true;






    vector<Point*>intersections;
    intersections.push_back(lineIntersection(la1,lb1));
    intersections.push_back(lineIntersection(la1,lb2));
    intersections.push_back(lineIntersection(la1,lb3));
    intersections.push_back(lineIntersection(la1,lb4));

    intersections.push_back(lineIntersection(la2,lb1));
    intersections.push_back(lineIntersection(la2,lb2));
    intersections.push_back(lineIntersection(la2,lb3));
    intersections.push_back(lineIntersection(la2,lb4));

    intersections.push_back(lineIntersection(la3,lb1));
    intersections.push_back(lineIntersection(la3,lb2));
    intersections.push_back(lineIntersection(la3,lb3));
    intersections.push_back(lineIntersection(la3,lb4));

    intersections.push_back(lineIntersection(la4,lb1));
    intersections.push_back(lineIntersection(la4,lb2));
    intersections.push_back(lineIntersection(la4,lb3));
    intersections.push_back(lineIntersection(la4,lb4));


    int x_min=0;int x_max=0;
    int y_max=0;int y_min=0;

    if(a_width*a_height>b_width*b_height)
    {
        x_min = pa1.x;
        x_min=min(x_min,pa2.x);
        x_min=min(x_min,pa3.x);
        x_min=min(x_min,pa4.x);
        x_max = pa1.x;
        x_max=max(x_max,pa2.x);
        x_max=max(x_max,pa3.x);
        x_max=max(x_max,pa4.x);

        y_min = pa1.y;
        y_min=min(y_min,pa2.y);
        y_min=min(y_min,pa3.y);
        y_min=min(y_min,pa4.y);
        y_max = pa1.y;
        y_max=max(y_max,pa2.y);
        y_max=max(y_max,pa3.y);
        y_max=max(y_max,pa4.y);
    }else
    {
        x_min = pb1.x;
        x_min=min(x_min,pb2.x);
        x_min=min(x_min,pb3.x);
        x_min=min(x_min,pb4.x);
        x_max = pb1.x;
        x_max=max(x_max,pb2.x);
        x_max=max(x_max,pb3.x);
        x_max=max(x_max,pb4.x);

        y_min = pb1.y;
        y_min=min(y_min,pb2.y);
        y_min=min(y_min,pb3.y);
        y_min=min(y_min,pb4.y);
        y_max = pb1.y;
        y_max=max(y_max,pb2.y);
        y_max=max(y_max,pb3.y);
        y_max=max(y_max,pb4.y);
    }

    int cont=0;

    for(int i=0;i<intersections.size();i++)
    {
        Point* point=intersections[i];
        if(point!=NULL)
        {
            if(point->x > x_min
               && point->x < x_max
               && point->y > y_min
               && point->y < y_max)
            {
                cont++;
            }
        }
    }

    vector<Point*>::iterator i;
    for ( i = intersections.begin() ; i < intersections.end(); i++ )
    {
        delete * i;
    }


    if(cont>=8)
        return true;

    return false;
}

bool segmentIntersection(Line l1,Line l2)
{
    Point* p=lineIntersection(l1,l2);
    if(p!=NULL)
    {
        if((p->x<l1.p1.x && p->x<l1.p2.x)
           ||(p->x>l1.p1.x && p->x>l1.p2.x))
           {
               delete p;
               return false;
           }

        if((p->x<l2.p1.x && p->x<l2.p2.x)
           ||(p->x>l2.p1.x && p->x>l2.p2.x))
           {
               delete p;
               return false;
           }

        if((p->y<l1.p1.y && p->y<l1.p2.y)
           ||(p->y>l1.p1.y && p->y>l1.p2.y))
           {
               delete p;
               return false;
           }

        if((p->y<l2.p1.y && p->y<l2.p2.y)
           ||(p->y>l2.p1.y && p->y>l2.p2.y))
           {
               delete p;
               return false;
           }
        delete p;
        return true;
    }
    delete p;
    return false;
}

Point* lineIntersection(Line l1,Line l2) {
    Point p1=l1.p1;
    Point p2=l1.p2;
    Point p3=l2.p1;
    Point p4=l2.p2;

    // Store the values for fast access and easy
    // equations-to-code conversion
    float x1 = p1.x, x2 = p2.x, x3 = p3.x, x4 = p4.x;
    float y1 = p1.y, y2 = p2.y, y3 = p3.y, y4 = p4.y;

    float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    // If d is zero, there is no intersection
    if (d == 0) return NULL;

    // Get the x and y
    float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
    float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
    float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;

    // Check if the x and y coordinates are within both lines
//    if ( x < min(x1, x2) || x > max(x1, x2) ||
//    x < min(x3, x4) || x > max(x3, x4) ) return NULL;
//    if ( y < min(y1, y2) || y > max(y1, y2) ||
//    y < min(y3, y4) || y > max(y3, y4) ) return NULL;

    // Return the point of intersection
    Point* ret = new Point();
    ret->x = x;
    ret->y = y;
    return ret;
}

Point rotateAroundPoint(Point point,Point pivot,float angle)
{
    //Move the universe (the point b)
    float s = sin(angle*PI/180);
    float c = cos(angle*PI/180);

    // translate point back to origin:
    int res_x = point.x;
    int res_y = point.y;

    // rotate point
    float xnew = res_x * c + res_y * s;
    float ynew = -res_x * s + res_y * c;

    // translate point back:
    res_x = xnew + pivot.x;
    res_y = ynew + pivot.y;

    return Point(res_x,res_y);
}
