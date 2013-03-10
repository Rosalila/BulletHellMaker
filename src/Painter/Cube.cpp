#include "Painter/Cube.h"

void Cube::logic()
{
    if (this->fuel > 0)
    {
        for (int i = 0; i < NUM_PARTICLES; i++)
        {
            this->particles[i].position[0] += this->particles[i].speed[0] * 0.2;
            this->particles[i].position[1] += this->particles[i].speed[1] * 0.2;
            this->particles[i].position[2] += this->particles[i].speed[2] * 0.2;

            this->particles[i].color[0] -= 1.0 / 500.0;
            if (this->particles[i].color[0] < 0.0)
            {
                this->particles[i].color[0] = 0.0;
            }

            this->particles[i].color[1] -= 1.0 / 100.0;
            if (this->particles[i].color[1] < 0.0)
            {
                this->particles[i].color[1] = 0.0;
            }

            this->particles[i].color[2] -= 1.0 / 50.0;
            if (this->particles[i].color[2] < 0.0)
            {
                this->particles[i].color[2] = 0.0;
            }
        }

        for (int i = 0; i < NUM_DEBRIS; i++)
        {
            this->debris[i].position[0] += this->debris[i].speed[0] * 0.1;
            this->debris[i].position[1] += this->debris[i].speed[1] * 0.1;
            this->debris[i].position[2] += this->debris[i].speed[2] * 0.1;

            this->debris[i].orientation[0] += this->debris[i].orientationSpeed[0] * 10;
            this->debris[i].orientation[1] += this->debris[i].orientationSpeed[1] * 10;
            this->debris[i].orientation[2] += this->debris[i].orientationSpeed[2] * 10;
        }

        --this->fuel;
    }

    this->angle += 0.3;  /* Always continue to rotate the camera */
}

void Cube::newExplosion (void)
{
  int    i;

  for (i = 0; i < NUM_PARTICLES; i++)
    {
      particles[i].position[0] = 0.0;
      particles[i].position[1] = 0.0;
      particles[i].position[2] = 0.0;

      particles[i].color[0] = 1.0;
      particles[i].color[1] = 1.0;
      particles[i].color[2] = 0.5;

      newSpeed (particles[i].speed);
    }

  for (i = 0; i < NUM_DEBRIS; i++)
    {
      debris[i].position[0] = 0.0;
      debris[i].position[1] = 0.0;
      debris[i].position[2] = 0.0;

      debris[i].orientation[0] = 0.0;
      debris[i].orientation[1] = 0.0;
      debris[i].orientation[2] = 0.0;

      debris[i].color[0] = 0.7;
      debris[i].color[1] = 0.7;
      debris[i].color[2] = 0.7;

      debris[i].scale[0] = (20.0 *
			    ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
      debris[i].scale[1] = (20.0 *
			    ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
      debris[i].scale[2] = (20.0 *
			    ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;

      newSpeed (debris[i].speed);
      newSpeed (debris[i].orientationSpeed);
    }

  fuel = 100;
}

void Cube::newSpeed (float dest[3])
{
  float    x;
  float    y;
  float    z;
  float    len;

  x = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
  y = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
  z = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;

  /*
   * Normalizing the speed vectors gives a "fireball" effect
   *
   */

  if (wantNormalize)
    {
      len = sqrt (x * x + y * y + z * z);

      if (len)
	{
	  x = x / len;
	  y = y / len;
	  z = z / len;
	}
    }

  dest[0] = x;
  dest[1] = y;
  dest[2] = z;
}
