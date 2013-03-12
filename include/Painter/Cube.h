#include "SDL/SDL_image.h"

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <stdio.h>
#include <math.h>

#define NUM_PARTICLES    2          /* Number of particles  */
#define NUM_DEBRIS       4            /* Number of debris     */

/* GLUT menu entries */

#define PAUSE              0
#define NORMALIZE_SPEED    1
#define QUIT               2

class Cube
{
public:
    /* A particle */

    struct particleData
    {
      float   position[3];
      float   speed[3];
      float   color[3];
    };
    typedef struct particleData    particleData;


    /* A piece of debris */

    struct debrisData
    {
      float   position[3];
      float   speed[3];
      float   orientation[3];        /* Rotation angles around x, y, and z axes */
      float   orientationSpeed[3];
      float   color[3];
      float   scale[3];
    };
    typedef struct debrisData    debrisData;


    /* Globals */

    particleData     particles[NUM_PARTICLES];
    debrisData       debris[NUM_DEBRIS];
    int fuel;                /* "fuel" of the explosion */

    float angle;             /* camera rotation angle */

    int wantNormalize;   /* Speed vector normalization flag */
    int wantPause;       /* Pause flag */


    /* Light sources and material */

    GLfloat  light0Amb[4];
    GLfloat  light0Dif[4];
    GLfloat  light0Spec[4];
    GLfloat  light0Pos[4];

    GLfloat  light1Amb[4];
    GLfloat  light1Dif[4];
    GLfloat  light1Spec[4];
    GLfloat  light1Pos[4];

    GLfloat  materialAmb[4];
    GLfloat  materialDif[4];
    GLfloat  materialSpec[4];
    GLfloat  materialShininess;

    Cube()
    {
        fuel = 0;                /* "fuel" of the explosion */

        angle = 0.0;             /* camera rotation angle */

        wantNormalize = 0;   /* Speed vector normalization flag */
        wantPause = 0;       /* Pause flag */


        /* Light sources and material */
//
        light0Amb[0]=1.0;
        light0Amb[1]=0.6;
        light0Amb[2]=0.8;
        light0Amb[3]=0.5;

        light0Dif[0]=1.0;
        light0Dif[1]=0.6;
        light0Dif[2]=0.8;
        light0Dif[3]=0.5;

        light0Spec[0]=1.0;
        light0Spec[1]=0.6;
        light0Spec[2]=0.8;
        light0Spec[3]=0.5;

        light0Pos[0]=1.0;
        light0Pos[1]=0.6;
        light0Pos[2]=0.8;
        light0Pos[3]=0.5;

        light1Amb[0]=1.0;
        light1Amb[1]=1.0;
        light1Amb[2]=0.0;
        light1Amb[3]=0.0;

        light1Dif[0]=1.0;
        light1Dif[1]=1.0;
        light1Dif[2]=1.0;
        light1Dif[3]=0.0;

        light1Spec[0]=1.0;
        light1Spec[1]=1.0;
        light1Spec[2]=1.0;
        light1Spec[3]=0.0;

        light1Pos[0]=0.0;
        light1Pos[1]=5.0;
        light1Pos[2]=5.0;
        light1Pos[3]=0.0;

        materialAmb[0]=232.0/255.0;
        materialAmb[1]=54.0/255.0;
        materialAmb[2]=222.0/255.0;
        materialAmb[3]=125.0/255.0;

        materialDif[0]=0.0;
        materialDif[1]=0.0;
        materialDif[2]=0.0;
        materialDif[3]=0.5;

        materialSpec[0]=0.0;
        materialSpec[1]=0.0;
        materialSpec[2]=0.0;
        materialSpec[3]=0.5;

        materialShininess = 20.0;
    }

//    GLfloat  light0Amb[4] =  { 1.0, 0.6, 0.2, 1.0 };
//    GLfloat  light0Dif[4] =  { 1.0, 0.6, 0.2, 1.0 };
//    GLfloat  light0Spec[4] = { 0.0, 0.0, 0.0, 1.0 };
//    GLfloat  light0Pos[4] =  { 0.0, 0.0, 0.0, 1.0 };
//
//    GLfloat  light1Amb[4] =  { 0.0, 0.0, 0.0, 1.0 };
//    GLfloat  light1Dif[4] =  { 1.0, 1.0, 1.0, 1.0 };
//    GLfloat  light1Spec[4] = { 1.0, 1.0, 1.0, 1.0 };
//    GLfloat  light1Pos[4] =  { 0.0, 5.0, 5.0, 0.0 };
//
//    GLfloat  materialAmb[4] = { 0.25, 0.22, 0.26, 1.0 };
//    GLfloat  materialDif[4] = { 0.63, 0.57, 0.60, 1.0 };
//    GLfloat  materialSpec[4] = { 0.99, 0.91, 0.81, 1.0 };
//    GLfloat  materialShininess = 27.8;

    void logic();
    void newExplosion (void);
    void newSpeed (float dest[3]);
};
