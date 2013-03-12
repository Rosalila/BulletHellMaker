#include "SDL/SDL_image.h"

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Point3D
{
public:
    GLfloat coordinates[3];
    Point3D(GLfloat x,GLfloat y,GLfloat z)
    {
        this->coordinates[0]=x;
        this->coordinates[1]=y;
        this->coordinates[2]=z;
    }
};

class Face
{
public:
    int vertex1;
    int vertex2;
    int vertex3;
    Face(int vertex1, int vertex2, int vertex3)
    {
        this->vertex1=vertex1;
        this->vertex2=vertex2;
        this->vertex3=vertex3;
    }
};

class Object3D
{
public:
    GLfloat size;

    std::vector<Point3D*> vertex;
    std::vector<Face*> faces;

    Object3D();
    GLfloat* getVertex(int number);
    void draw();
};
