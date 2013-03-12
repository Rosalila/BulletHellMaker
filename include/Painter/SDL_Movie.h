#ifndef SDLMOVIE_H
#define SDLMOVIE_H

class SDL_Movie
{
private:
	// Surface for the main screen
	SDL_Surface *screen;

	// Surface for the movie
	SDL_Surface *movieSurface;

	// Holds the movie information
	SMPEG_Info movieInfo;

	// Load the movie
	SMPEG *movie;

	// The max we can scale by
	int MaxScaleX;
	int MaxScaleY;
	int MaxScale;

	// Locations on screen to draw at
	int X, Y;

	void DrawIMG(SDL_Surface *img, SDL_Surface *dst, int x, int y)
	{
		SDL_Rect dest;
		dest.x = x;
		dest.y = y;
		SDL_BlitSurface(img, NULL, dst, &dest);
	}

public:
	SDL_Movie()
	{
		MaxScaleX = MaxScaleY = MaxScale = 1;
		
		screen = 0;
		movieSurface = 0;
		movie = 0;
		X = Y = 0;
	}

	// Free our movie
	~SDL_Movie()
	{
		Stop();
		SMPEG_delete( movie );
		movie = 0;
		SDL_FreeSurface( movieSurface );
		movieSurface = 0;
	}

	void ClearScreen()
	{
		SDL_FillRect( movieSurface, 0, 0 );
	}

	// Set's the volume on a scale of 0 - 100
	void SetVolume( int vol )
	{
		SMPEG_setvolume( movie, vol );
	}

	// Scale the movie by the desired factors
	void Scale( int w, int h )
	{
		// Prevent a divide by 0
		if( w == 0 )
			w = 1;
		if( h == 0 )
			h = 1;

		SMPEG_scaleXY( movie, w, h );
		#ifdef _DEBUG
			CheckErrors();
		#endif
	}

	// Scale the movie by the desired factor
	void ScaleBy( int factor )
	{
		// Prevent a divide by 0
		if( factor == 0 )
			factor = 1;
		// Make sure we don't scale larger than the surface size
		if( factor > MaxScale )
			factor = MaxScale;

		SMPEG_scale( movie, factor );
		#ifdef _DEBUG
			CheckErrors();
		#endif
	}

	// Sets the region of the video to be shown
	void SetDisplayRegion( int x, int y, int w, int h )
	{
		SMPEG_setdisplayregion( movie, x, y, w, h );
		#ifdef _DEBUG
			CheckErrors();
		#endif
	}

	// Set the position that the movie should be drawn at on the screen
	void SetPosition( int x, int y )
	{
		X = x;
		Y = y;
	}

	// Check for any errors
	void CheckErrors()
	{
		char* error = SMPEG_error( movie );
		if( error )
			printf( "Error: %s\n", error );
	}

	// Load the movie
	void Load( const char* fileName, SDL_Surface* s, int maxscalex = 1, int maxscaley = 1 )
	{
		MaxScaleX = maxscalex;
		MaxScaleY = maxscaley;
		
		// Limit how much we can scale by
		MaxScale = (maxscalex > maxscaley ? maxscaley : maxscalex);

		// Assign the screen surface
		screen = s;

		// Load the movie and store the information about it
		movie = SMPEG_new( fileName, &movieInfo, true );

		#ifdef _DEBUG
			CheckErrors();
		#endif

		// Create a temporary surface to render the movie to
		SDL_Surface *tempSurface2 = SDL_CreateRGBSurface( SDL_SWSURFACE, movieInfo.width * MaxScaleX, movieInfo.height * MaxScaleY, 32, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask );

		// Now make a surface optimized for the main screen
		movieSurface = SDL_DisplayFormat( tempSurface2 );

		// Free the temporary surface
		SDL_FreeSurface( tempSurface2 );

		// Set the surface to draw to
		SMPEG_setdisplay( movie, movieSurface, 0, 0 );

		#ifdef _DEBUG
			CheckErrors();
		#endif

		// Set the display region
		SMPEG_setdisplayregion( movie, 0, 0, movieInfo.width, movieInfo.height );

		#ifdef _DEBUG
			CheckErrors();
		#endif
	}

	// Set the looping of hte movie
	void SetLoop( int val )
	{
		SMPEG_loop( movie, val );
		#ifdef _DEBUG
			CheckErrors();
		#endif
	}

	// Play the movie
	void Play()
	{
		SMPEG_play( movie );

		#ifdef _DEBUG
			CheckErrors();
		#endif
	}

	// Pause the movie
	void Pause()
	{
		SMPEG_pause( movie );

		#ifdef _DEBUG
			CheckErrors();
		#endif
	}

	// Stops the movie, but keeps current position
	void Stop()
	{
		SMPEG_stop( movie );

		#ifdef _DEBUG
			CheckErrors();
		#endif
	}

	// Rewind the movie back to 0:00:00
	void Rewind()
	{
		SMPEG_rewind( movie );

		#ifdef _DEBUG
			CheckErrors();
		#endif
	}

	// Seek a number of bytes into the movie
	void Seek( int bytes )
	{
		SMPEG_seek( movie, bytes );

		#ifdef _DEBUG
			CheckErrors();
		#endif
	}

	// Skip a number of seconds
	void Skip( float seconds )
	{
		SMPEG_skip( movie, seconds );

		#ifdef _DEBUG
			CheckErrors();
		#endif
	}

	// Render some frame of the movie
	void RenderFrame( int frame )
	{
		SMPEG_renderFrame( movie, frame );

		#ifdef _DEBUG
			CheckErrors();
		#endif
	}

	// Render the final frame of the movie
	void RenderFinal()
	{
		SMPEG_renderFinal( movie, movieSurface, 0, 0 );

		#ifdef _DEBUG
			CheckErrors();
		#endif
	}

	// Draw the movie surface to the main screen at x, y
	void DisplayAt( int x, int y )
	{
		DrawIMG( movieSurface, screen, x, y );
	}

	// Draw the movie surface to the main screen at x, y
	void Display()
	{
		DrawIMG( movieSurface, screen, X, Y );
	}

	// Return the current info for the movie
	SMPEG_Info GetInfo()
	{
		SMPEG_getinfo( movie, &movieInfo );
		return movieInfo;
	}

	// Get the current status of the movie, can be SMPEG_ERROR = -1, SMPEG_STOPPED, SMPEG_PLAYING
	SMPEGstatus GetStatus()
	{
		return SMPEG_status(movie);
	}
};

#endif
