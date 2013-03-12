#include "Painter/Painter.h"

Painter::Painter()
{
    //XML Initializations
    char *archivo=new char[255];
    strcpy(archivo,"config.xml");
    TiXmlDocument doc_t( archivo );
    doc_t.LoadFile();
    TiXmlDocument *doc;
    doc=&doc_t;

    TiXmlNode *config_file=doc->FirstChild("ConfigFile");

    TiXmlElement *resolution_element=config_file->FirstChild("Resolution")->ToElement();
    screen_resized_width=atoi(resolution_element->Attribute("x"));
    screen_resized_height=atoi(resolution_element->Attribute("y"));

    TiXmlElement *fullscreen_element=config_file->FirstChild("Fullscreen")->ToElement();
    fullscreen=strcmp(fullscreen_element->Attribute("enabled"),"yes")==0;

    TiXmlElement *font_element=config_file->FirstChild("Font")->ToElement();
    int font_size=atoi(font_element->Attribute("size"));
    int font_red=atoi(font_element->Attribute("red"));
    int font_green=atoi(font_element->Attribute("green"));
    int font_blue=atoi(font_element->Attribute("blue"));

    //Internal initializations
    screen=NULL;
    joystick_1 = NULL;
    joystick_2 = NULL;

    screen_width = 1280;
    screen_height = 800;

    screen_bpp = 32;
    camera_x=camera_y=0;

    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING | SDL_INIT_AUDIO | SDL_INIT_VIDEO ) == -1 )
    {
        writeLogLine(SDL_GetError());
        return;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        writeLogLine(SDL_GetError());
        return;
    }

    font = NULL;
    textColor.r = font_red;
    textColor.g = font_green;
    textColor.b = font_blue;

    font = TTF_OpenFont( "misc/font.ttf", font_size );

    if(font==NULL)
    {
        writeLogLine("Could not init font. Place it on /misc/font.ttf .");
    }

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ); // *new*

    //Set up the screen
    if(!fullscreen)
        screen = SDL_SetVideoMode( screen_resized_width, screen_resized_height, screen_bpp, SDL_OPENGL );
    else
        screen = SDL_SetVideoMode( screen_resized_width, screen_resized_height, screen_bpp, SDL_OPENGL | SDL_FULLSCREEN );

    //Set the openGL state?
    glEnable( GL_TEXTURE_2D );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    glViewport( 0, 0, screen_resized_width, screen_resized_height );

    glClear( GL_COLOR_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho(0.0f, screen_width, screen_height, 0.0f, -1.0f, 1.0f);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Fps cap
    frames_per_seccond = 60;
    frame = 0;
    fps=new Timer();
    update=new Timer();
    fps->start();
    update->start();

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        writeLogLine("Error: Could not initialize SDL screen.");
        return;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Rosalila fighter engine", NULL );

    //Init joysticks
    if( SDL_NumJoysticks() == 1 )
    {
        writeLogLine("1 joystick was found.");
        joystick_1 = SDL_JoystickOpen( 0 );
        if(joystick_1 != NULL)
            writeLogLine("Joystick for player 1 initialized succesfully.");
        else
            writeLogLine("Error initializing joystick for player 1.");
    }
    if( SDL_NumJoysticks() == 2 )
    {
        writeLogLine("2 joysticks were found.");
        joystick_1 = SDL_JoystickOpen( 0 );
        if(joystick_1 != NULL)
            writeLogLine("Joystick for player 1 initialized succesfully.");
        else
            writeLogLine("Error initializing joystick for player 1.");
        joystick_2 = SDL_JoystickOpen( 1 );
        if(joystick_2 != NULL)
            writeLogLine("Joystick for player 2 initialized succesfully.");
        else
            writeLogLine("Error initializing joystick for player 2.");
    }

    //If everything initialized fine
    writeLogLine("Success! SDL initialized.");
    return;
}

void Painter::resetScreen()
{
    //Set up the screen
    if(!fullscreen)
        screen = SDL_SetVideoMode( screen_resized_width, screen_resized_height, screen_bpp, SDL_OPENGL );
    else
        screen = SDL_SetVideoMode( screen_resized_width, screen_resized_height, screen_bpp, SDL_OPENGL | SDL_FULLSCREEN );
}

Painter::~Painter()
{
    //Free the surface
    SDL_FreeSurface( screen );

    //Quit SDL
    SDL_Quit();
}


Image* Painter::getTexture(std::string filename)
{
    SDL_Surface *surface;
    GLenum texture_format;
    GLint  nOfColors;
    GLuint texture;

    if ( (surface = IMG_Load(filename.c_str())) ) {

            // get the number of channels in the SDL surface
            nOfColors = surface->format->BytesPerPixel;
            if (nOfColors == 4)     // contains an alpha channel
            {
                    if (surface->format->Rmask == 0x000000ff)
                            texture_format = GL_RGBA;
                    //else
                            //texture_format = GL_BGRA;
            } else if (nOfColors == 3)     // no alpha channel
            {
                    if (surface->format->Rmask == 0x000000ff)
                            texture_format = GL_RGB;
                    //else
                            //texture_format = GL_BGR;
            } else {
                writeLogLine("Warning: "+ filename+ " is not truecolor. This will probably break.");
                    // this error should not go unhandled
            }

        // Have OpenGL generate a texture object handle for us
        glGenTextures( 1, &texture );

        // Bind the texture object
        glBindTexture( GL_TEXTURE_2D, texture );

        // Set the texture's stretching properties
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        // Edit the texture object's image data using the information SDL_Surface gives us
        glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                          texture_format, GL_UNSIGNED_BYTE, surface->pixels );
    }
    else {
        std::string sdl_error=SDL_GetError();
        writeLogLine("SDL could not load "+filename+": "+sdl_error);
        SDL_Quit();
        return NULL;
    }


    Image*image=new Image();
    image->setTexture(texture);
    image->setWidth(surface->w);
    image->setHeight(surface->h);

    // Free the SDL_Surface only if it was successfully created
    if ( surface ) {
        SDL_FreeSurface( surface );
    }

    writeLogLine(filename+" loaded");

    return image;
}

void Painter::draw2DImage	(
             Image* texture,
             int size_x,int size_y,
             int position_x,int position_y,
             float scale,
             float rotation,
             bool flipHorizontally,
             int depth_effect_x,
             int depth_effect_y,
             Color color_effects,
             bool camera_align)
{
    glEnable( GL_TEXTURE_2D );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho(0.0f, screen_width, screen_height, 0.0f, -1.0f, 1.0f);
    glMatrixMode( GL_MODELVIEW );

    glDisable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glDisable (GL_DEPTH_TEST);



    //Camera and depth effect
    if(depth_effect_x>0)
    {
        position_x-=camera_x/depth_effect_x;
    }else if(depth_effect_x<0)
    {
        position_x-=camera_x*-depth_effect_x;
    }else if(camera_align)
    {
        position_x-=camera_x;
    }

    if(depth_effect_y>0)
    {
        position_y+=camera_y/depth_effect_y;
    }else if(depth_effect_y<0)
    {
        position_y+=camera_y*-depth_effect_y;
    }else if(camera_align)
    {
        position_y+=camera_y;
    }

    GLfloat x1=0.f+position_x;
    GLfloat y1=0.f+position_y;
    GLfloat x2=0.f+position_x+(float)size_x*scale;
    GLfloat y2=0.f+position_y+(float)size_y*scale;

    //Flip
    if(flipHorizontally)
    {
        GLfloat temp=x1;
        x1=x2;
        x2=temp;
    }

    //OpenGL draw
    //Save the current matrix.
    glPushMatrix();
    //Change the current matrix.
    float translate_x=(x2-x1)/2+position_x;
    float translate_y=(y2-y1)/2+position_y;
    glTranslatef(translate_x,translate_y, 1.0);
    glRotatef(-rotation, 0, 0, 1.0);


    glBindTexture( GL_TEXTURE_2D, texture->getTexture() );
    glColor4ub(color_effects.getRed(), color_effects.getGreen(), color_effects.getBlue(),color_effects.getAlpha());
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glBegin( GL_QUADS );
        //Bottom-left vertex (corner)
        glTexCoord2i( 0, 0 );
        glVertex3f( x1-translate_x, y1-translate_y, 0.0f );

        //Bottom-right vertex (corner)
        glTexCoord2i( 1, 0 );
        glVertex3f( x2-translate_x, y1-translate_y, 0.f );

        //Top-right vertex (corner)
        glTexCoord2i( 1, 1 );
        glVertex3f( x2-translate_x, y2-translate_y, 0.f );

        //Top-left vertex (corner)
        glTexCoord2i( 0, 1 );
        glVertex3f( x1-translate_x, y2-translate_y, 0.f );

    glEnd();


    //Reset the current matrix to the one that was saved.
    glPopMatrix();
}

void Painter::drawRectangle(int x,int y,int width,int height,float rotation,int red,int green,int blue,int alpha,bool camera_align)
{
    glDisable(GL_TEXTURE_2D);
    GLubyte r=red;
    GLubyte g=green;
    GLubyte b=blue;
    GLubyte a=alpha;
    glColor4ub(r,g,b,a);

    if(camera_align)
    {
        y+=camera_y;
        x-=camera_x;
    }

    //OpenGL draw
    //Save the current matrix.
    glPushMatrix();
    //Change the current matrix.
    float translate_x=x;
    float translate_y=y;
    glTranslatef(translate_x,translate_y, 1.0);
    glRotatef(-rotation, 0, 0, 1.0);
    x-=translate_x;
    y-=translate_y;

    glRecti(x, y, width+x, height+y);
    glFlush();

    //Reset the current matrix to the one that was saved.
    glPopMatrix();
}

void Painter::frameCap()
{
    frame++;

    //If a second has passed since the caption was last updated

    //If we want to cap the frame rate
    if(update->get_ticks() < 1000 / frames_per_seccond)
    {
        //Sleep the remaining frame time
        SDL_Delay( ( 1000 / frames_per_seccond ) - update->get_ticks() );
    }

    std::string caption;

    //Calculate the frames per second and create the string
    caption = "Average Frames Per Second: " + toString(frame / ( fps->get_ticks() / 1000.f ));

    //Reset the caption
    SDL_WM_SetCaption( caption.c_str(), NULL );

    //Restart the update timer
    update->start();
}

void Painter::drawText(std::string text,int position_x,int position_y)
{
    GLuint texture;

    SDL_Surface *message = NULL;
    message = TTF_RenderUTF8_Blended( font, text.c_str(), textColor );


    // Prepare the texture for the font
    GLenum textFormat;
    if(message->format->BytesPerPixel == 4)
    {
        // alpha
        if(message->format->Rmask == 0x000000ff)
            textFormat = GL_RGBA;
        else
            textFormat = GL_BGRA_EXT;
    }

    // Create the font's texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, message->format->BytesPerPixel, message->w, message->h, 0, textFormat, GL_UNSIGNED_BYTE, message->pixels);

    GLfloat x1=0.f+position_x;
    GLfloat y1=0.f+position_y;
    GLfloat x2=0.f+position_x+message->w;
    GLfloat y2=0.f+position_y+message->h;
    SDL_FreeSurface(message);


    //OpenGL draw
    glBindTexture( GL_TEXTURE_2D, texture );
    glColor3ub(255, 255, 255);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glBegin( GL_QUADS );

        //Bottom-left vertex (corner)
        glTexCoord2i( 0, 0 );
        glVertex3f( x1, y1, 0.0f );

        //Bottom-right vertex (corner)
        glTexCoord2i( 1, 0 );
        glVertex3f( x2, y1, 0.f );

        //Top-right vertex (corner)
        glTexCoord2i( 1, 1 );
        glVertex3f( x2, y2, 0.f );

        //Top-left vertex (corner)
        glTexCoord2i( 0, 1 );
        glVertex3f( x1, y2, 0.f );

    glEnd();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void Painter::draw3D(float pos_x,float pos_y)
{
    //  glViewport (0.0, 0.0, (GLfloat) screen_width, (GLfloat) screen_height);
    //glDisable(GL_BLEND);
    glEnable(GL_BLEND);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (45.0, (GLfloat) screen_width / (GLfloat) screen_height, 0.1, 100.0);
    glMatrixMode (GL_MODELVIEW);



    srand (time (NULL));

    glEnable (GL_LIGHT0);
    glEnable (GL_LIGHT1);
    glLightfv (GL_LIGHT0, GL_AMBIENT, cube.light0Amb);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, cube.light0Dif);
    glLightfv (GL_LIGHT0, GL_SPECULAR, cube.light0Spec);
    glLightfv (GL_LIGHT0, GL_POSITION, cube.light0Pos);
//    glLightfv (GL_LIGHT1, GL_AMBIENT, cube.light1Amb);
//    glLightfv (GL_LIGHT1, GL_DIFFUSE, cube.light1Dif);
//    glLightfv (GL_LIGHT1, GL_SPECULAR, cube.light1Spec);
//    glLightfv (GL_LIGHT1, GL_POSITION, cube.light1Pos);
    glLightModelf (GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, cube.materialAmb);
    glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, cube.materialDif);
    glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, cube.materialSpec);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, cube.materialShininess);
    glEnable (GL_NORMALIZE);

    cube.logic();


    //glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity ();

    /* Place the camera */
    //
    glPushMatrix ();

    pos_x+=650.0;
    pos_y+=400.0;
    glTranslatef (-((screen_width-pos_x)/2.0)/5.0, ((screen_height-pos_y)/2.0)/5.0, -100.0);
    glRotatef (cube.angle, 0.0, 1.0, 0.0);


    /* If no explosion, draw cube */

    if (cube.fuel == 0)
    {
        glEnable (GL_LIGHTING);
        glDisable (GL_LIGHT0);
        glEnable (GL_DEPTH_TEST);
        glutSolidCube (4.0);
    }

    if (cube.fuel > 0)
    {
        glPushMatrix ();

        glDisable (GL_LIGHTING);
        glDisable (GL_DEPTH_TEST);

        glBegin (GL_POINTS);

        for (int i = 0; i < NUM_PARTICLES; i++)
        {
            glColor3fv (cube.particles[i].color);
            glVertex3fv (cube.particles[i].position);
        }

        glEnd ();

        glPopMatrix ();

        glEnable (GL_LIGHTING);
        glEnable (GL_LIGHT0);
        glEnable (GL_DEPTH_TEST);

        glNormal3f (0.0, 0.0, 1.0);

        for (int i = 0; i < NUM_DEBRIS; i++)
        {
            glColor3fv (cube.debris[i].color);

            glPushMatrix ();

            glTranslatef (cube.debris[i].position[0],
            cube.debris[i].position[1],
            cube.debris[i].position[2]);

            glRotatef (cube.debris[i].orientation[0], 1.0, 0.0, 0.0);
            glRotatef (cube.debris[i].orientation[1], 0.0, 1.0, 0.0);
            glRotatef (cube.debris[i].orientation[2], 0.0, 0.0, 1.0);

//            glScalef (cube.debris[i].scale[0],
//            cube.debris[i].scale[1],
//            cube.debris[i].scale[2]);
            drawObject();

            glPopMatrix ();
        }
    }
    glPopMatrix();
}

void Painter::drawObject()
{
    Object3D object;
    object.draw();
}

void Painter::explode()
{
    cube.newExplosion();
}

void Painter::updateScreen()
{
    //Write errors to the log
    std::string error= ">>>";
    error+=SDL_GetError();
    if(error!=">>>")
        writeLogLine(error);


    //Draw
    frameCap();
    SDL_GL_SwapBuffers();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Painter::video(Painter*painter)
{
    // Surface for the movie
    SDL_Surface *movieSurface = 0;

    // Holds the movie information
    SMPEG_Info movieInfo;

    // Load the movie
    SMPEG *movie =  0;

    painter->screen = SDL_SetVideoMode(painter->screen_resized_width, painter->screen_resized_height, painter->screen_bpp,SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT);

    // Load the movie and store the information about it
    movie = SMPEG_new("demo.mpg", &movieInfo, true);

    char* error = SMPEG_error(movie);

    if( error != NULL || movie == NULL )
    {
        printf( "Error loading MPEG: %s\n", error );
        return;
    }

    // Create a temporary surface to render the movie to
    SDL_Surface *tempSurface2 = SDL_CreateRGBSurface(SDL_SWSURFACE,
    movieInfo.width, movieInfo.height, 32, painter->screen->format->Rmask,
    painter->screen->format->Gmask, painter->screen->format->Bmask, painter->screen->format->Amask);

    // Now make a surface optimized for the main screen
    movieSurface = SDL_DisplayFormat(tempSurface2);

    // Free the temporary surface
    SDL_FreeSurface(tempSurface2);

    // Set the surface to draw to
    SMPEG_setdisplay(movie, movieSurface, 0, 0);

    // Set the display region
    SMPEG_setdisplayregion(movie, 0, 0, movieInfo.width, movieInfo.height);

    // Loop forever
    SMPEG_loop(movie, -1);

    SDL_ShowCursor(SDL_DISABLE);

    int done = 0;

    SMPEG_play(movie);
    SMPEG_getinfo(movie, &movieInfo);

    while(done == 0)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                done = 1;
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    done = 1;
                }
            }
        }

        SDL_Rect dest;
        dest.x = 0;
        dest.y = 0;
        SDL_BlitSurface(movieSurface, NULL, painter->screen, &dest);

        // Flip the main screen
        SDL_Flip(painter->screen);
    }

    SDL_FreeSurface(movieSurface);
    SMPEG_stop(movie);
    SMPEG_delete(movie);
    movie = NULL;
    SDL_ShowCursor(SDL_ENABLE);
    painter->resetScreen();
}
