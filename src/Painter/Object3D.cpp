#include "Painter/Object3D.h"

Object3D::Object3D()
{
    std::ifstream File("object.obj", ifstream::in);
    std::string Line;
    std::string Name;
    while(std::getline(File, Line))
    {
        if(Line == "" || Line[0] == '#')// Skip everything and continue with the next line
            continue;

        std::istringstream LineStream(Line);
        LineStream >> Name;

        if(Name == "v")
        {// Vertex
            float v[3];
            sscanf(Line.c_str(), "%*s %f %f %f", &v[0], &v[1], &v[2]);
            vertex.push_back(new Point3D((GLfloat)v[0], (GLfloat)v[1], (GLfloat)v[2]));
        }

        if(Name == "f")
        {// Vertex
            int f[3];
            sscanf(Line.c_str(), "%*s %i %i %i", &f[0], &f[1], &f[2]);
            faces.push_back(new Face(f[0]-1,f[1]-1,f[2]-1));
        }
    };
    size=2.0;

//    faces.push_back(new Face(0,1,2));
//    faces.push_back(new Face(0,2,3));
//    faces.push_back(new Face(4,0,3));
//    faces.push_back(new Face(5,4,3));
}

GLfloat* Object3D::getVertex(int number)
{
    return vertex[number]->coordinates;
}

void Object3D::draw()
{
    glBegin (GL_TRIANGLES);
        for(int i=0;i<(int)faces.size();i++)
        {
            glVertex3fv (getVertex(faces[i]->vertex1));
            glVertex3fv (getVertex(faces[i]->vertex2));
            glVertex3fv (getVertex(faces[i]->vertex3));
        }
    glEnd();
}
