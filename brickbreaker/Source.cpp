//Copyright by NOONE BITCHES FREE RIGHTS TO ALL

#include "SDL/include/SDL.h"	
#include "SDL/include/SDL_opengl.h"	
#include <iostream>
#pragma comment (lib, "SDL/lib/SDL.lib")


bool checkCollision(float Ax, float Ay, float Aw, float Ah, float Bx, float By, float Bw, float Bh)
{
	if ( Ay+Ah < By ) return false; 
  else if ( Ay > By+Bh ) return false; 
  else if ( Ax+Aw < Bx ) return false; 
  else if ( Ax > Bx+Bw ) return false; 
 
  return true; 
}

struct Brick
{
	float x;
	float y;
	float width;
	float height;
	bool alive;
};

#ifndef LINUX
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)	// Linux users should quit this line
#else
int main()
#endif
// int main ()  // Linux users should use this line instead the previous one
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	SDL_WM_SetCaption( "Brick Destroyer!", NULL );

	SDL_SetVideoMode(600,400,32, SDL_OPENGL );

	glClearColor(1,1,1,1); //Red, Green, Blue, Alpha

	glViewport(0,0,600,400);

	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	std::cout << "OpenGL IS RUNNING\n";
	std::cout << "Main Loop Started\n";

	bool isRunning = true;

	SDL_Event event;

	float myX = 300;
	float myY = 370;
	float width = 80;
	float height = 20;

	float ballX = 50;
	float ballY = 350;
	float ballWH = 20;

	float velX = 5;
	float velY = -5;

	const static int BRICKS = 45;
	Brick bricks[BRICKS];
	for ( int n = 0, x = 4, y = 10; n < BRICKS; n++, x+=66 )
	{
		if ( x > 560)
		{
			x = 4;
			y += 25;
		}
		bricks[n].x = x;
		bricks[n].y = y;
		bricks[n].width = 60;
		bricks[n].height = 20;
		bricks[n].alive = true;
	}


	

	bool left = false,right = false;
	while ( isRunning )
	{
		while ( SDL_PollEvent(&event) )
		{
			if ( event.type == SDL_QUIT )
			{
				isRunning = false;
			}

			if ( event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE )
			{
				isRunning = false;
			}
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_r )
			{
				glClearColor(1,0,0,1);
			}
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_a)
				{
					left = true;
				}
				else if (event.key.keysym.sym == SDLK_d)
				{
					right = true;
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				if (event.key.keysym.sym == SDLK_a)
				{
					left = false; 
				}
				else if (event.key.keysym.sym == SDLK_d)
				{
					right = false;
				}
			}
		}

		if (left == true)
        {
            myX -= 9;
        }
        if (right == true)
        {
            myX += 9;
        }
 
        if (myX < 0)
        {
            myX = 0;
        }
        if (myX+width > 600)
        {
            myX = 600-width;
        }

        ballX += velX;

        for ( int n = 0; n < BRICKS; n++ )
        {
        	if  ( bricks[n].alive == true )
        	{
        		if (checkCollision(ballX,ballY,ballWH,ballWH,bricks[n].x,bricks[n].y,bricks[n].width,bricks[n].height) == true)
        		{
        			velX = -velX;
        			bricks[n].alive = false;
        			break;
        		}
        	}	
        }

        ballY += velY;

        for ( int n = 0; n < BRICKS; n++ )
        {
        	if  ( bricks[n].alive == true )
        	{
        		if (checkCollision(ballX,ballY,ballWH,ballWH,bricks[n].x,bricks[n].y,bricks[n].width,bricks[n].height) == true)
        		{
        			velY = -velY;
        			bricks[n].alive = false;
        			break;
        		}
        	}	
        }


        if (ballX < 0)
        {
        	velX = -velX;
        }

        else if (ballX+ballWH>600)
        {
        	velX = -velX;
        }

        if (checkCollision(ballX,ballY,ballWH,ballWH,myX,myY,width,height) == true)
         {
         	velX = -velX;
         }

        if (ballY < 0)
        {
        	velY = -velY;
        }
        else if (ballY+ballWH > 400)
        {
        	isRunning = false;
        }

         if (checkCollision(ballX,ballY,ballWH,ballWH,myX,myY,width,height) == true)
         {
         	velY = -velY;
         }

		glClear(GL_COLOR_BUFFER_BIT);

		glPushMatrix();

		glOrtho(0,600,400,0,-1,1);
		glColor4ub(0,0,0,255);

		glBegin(GL_QUADS);
		glVertex2f(myX,myY);
		glVertex2f(myX+width,myY);
		glVertex2f(myX+width,myY+height);
		glVertex2f(myX,myY+height);
		glEnd();


		glColor4ub(0,0,0,255);
		glBegin(GL_QUADS);
		glVertex2f(ballX,ballY);
		glVertex2f(ballX+ballWH,ballY);
		glVertex2f(ballX+ballWH,ballY+ballWH);
		glVertex2f(ballX,ballY+ballWH);
		glEnd();

		glColor4ub(0,0,255,255);
		glBegin(GL_QUADS);
		for ( int n=0; n < BRICKS; n++ )
		{
			if ( bricks[n].alive == true )
			{
			glVertex2f(bricks[n].x,bricks[n].y);
			glVertex2f(bricks[n].x+bricks[n].width,bricks[n].y);
			glVertex2f(bricks[n].x+bricks[n].width,bricks[n].y+bricks[n].height);
			glVertex2f(bricks[n].x,bricks[n].y+bricks[n].height);
			}
		}


		glEnd();
		/*glColor4ub(156,40,253,255);

		glBegin(GL_QUADS);
		glVertex2f(5,5);
		glVertex2f(595,5);

		glColor4ub(0,255,0,255);

		glVertex2f(595,395);
		glVertex2f(5,395);
		glEnd();

		glColor4ub(0,0,0,255);
		glBegin(GL_LINES);
		
		glVertex2f(5,5);
		glVertex2f(595,395);

		glVertex2f(595,5);
		glVertex2f(5,395);

		glEnd();*/

		glPopMatrix();

		SDL_GL_SwapBuffers();
		SDL_Delay(1);
	}

	SDL_Quit();

	return 0;
}