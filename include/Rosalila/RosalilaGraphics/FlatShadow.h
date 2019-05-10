#ifndef FLATSHADOW_H
#define FLATSHADOW_H

#include <vector>
#include "../RosalilaUtility/RosalilaUtility.h"
#include "../RosalilaGraphics/Image.h"

class Point;
class RosalilaGraphics;

class FlatShadow
{
    public:
        Image* image;
        float shadow_lenght;
        float alpha_init;
        float alpha_end;
        float shadow_x;
        float shadow_y;
        vector<Point*>points_left;
        vector<Point*>points_right;
        vector<Point*>points_top;

        vector<Point*>inbetween_points_left;
        vector<Point*>inbetween_points_right;
        vector<Point*>inbetween_points_top;

        FlatShadow(Image* image, float shadow_lenght,float alpha_init,float alpha_end,float shadow_x,float shadow_y,
                   vector<Point*>points_left,vector<Point*>points_right,vector<Point*>points_top,
                   vector<Point*>inbetween_points_left,vector<Point*>inbetween_points_right,vector<Point*>inbetween_points_top);
        FlatShadow();
        virtual ~FlatShadow();
    protected:
    private:
};

#endif // FLATSHADOW_H
