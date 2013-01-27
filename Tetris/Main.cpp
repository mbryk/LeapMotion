/*****************************************************************************************
/* Desc: Tetris tutorial
/*
/* gametuto.com - Javier López López (javilop.com)
/*
/*****************************************************************************************
/*
/* Creative Commons - Attribution 3.0 Unported
/* You are free:
/*	to Share — to copy, distribute and transmit the work
/*	to Remix — to adapt the work
/*
/* Under the following conditions:
/* Attribution. You must attribute the work in the manner specified by the author or licensor 
/* (but not in any way that suggests that they endorse you or your use of the work).
/*
/*****************************************************************************************/

#include "Game.h"
#ifndef LINUX
#include <windows.h>
#endif

IO mIO;
int mScreenHeight = mIO.GetScreenHeight();

// Pieces
Pieces mPieces;

// Board
Board mBoard (&mPieces, mScreenHeight);

// Game
Game mGame (&mBoard, &mPieces, &mIO, mScreenHeight);

#define LEAP_DEBOUNCE_TIME 600000
#define MIN_VELOCITY 1500
#include <iostream>
#include "Leap.h"
using namespace Leap;

class SampleListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
	int64_t lastMove;
};

void SampleListener::onInit(const Controller& controller) {
	SampleListener::lastMove = 0;
}

void SampleListener::onConnect(const Controller& controller) {
}

void SampleListener::onDisconnect(const Controller& controller) {
}

void SampleListener::onExit(const Controller& controller) {
}

void SampleListener::onFrame(const Controller& controller) {
  // Get the most recent frame and report some basic information
  const Frame frame = controller.frame();
  if (!frame.pointables().empty()) {
	  if(frame.timestamp() - SampleListener::lastMove > LEAP_DEBOUNCE_TIME){
		  const Pointable pointable = frame.pointables()[0];
		  const Vector velocity = pointable.tipVelocity();
		  float velocity_x = velocity[0];
		  float velocity_y = velocity[1];
		  const Vector direction = pointable.direction();
		  if(velocity_x > MIN_VELOCITY){
			  if (mBoard.IsPossibleMovement (mGame.mPosX + 1, mGame.mPosY, mGame.mPiece, mGame.mRotation))
				  mGame.mPosX++;
			  
			  SampleListener::lastMove = frame.timestamp();
		  } else if (velocity_x < -MIN_VELOCITY){
			  if (mBoard.IsPossibleMovement (mGame.mPosX - 1, mGame.mPosY, mGame.mPiece, mGame.mRotation))
					mGame.mPosX--;

			  SampleListener::lastMove = frame.timestamp();
		  } else if (velocity_y < -MIN_VELOCITY){
			  if (mBoard.IsPossibleMovement (mGame.mPosX, mGame.mPosY + 1, mGame.mPiece, mGame.mRotation))
					mGame.mPosY++;
			  SampleListener::lastMove = frame.timestamp();
		  } else if (velocity_y > MIN_VELOCITY){
			  if (mBoard.IsPossibleMovement (mGame.mPosX, mGame.mPosY, mGame.mPiece, (mGame.mRotation + 1) % 4))
					mGame.mRotation = (mGame.mRotation + 1) % 4;
			  SampleListener::lastMove = frame.timestamp();
		  } else if () //check direction
	  }
  }
}


/*
==================
Main
==================
*/
#ifndef LINUX
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)	// Linux users should quit this line
#else
int main()
#endif
// int main ()  // Linux users should use this line instead the previous one
{

	SampleListener listener;
	Controller controller;

  // Have the sample listener receive events from the controller
  
	
  
	// ----- Vars -----

	// Class for drawing staff, it uses SDL for the rendering. Change the methods of this class
	// in order to use a different renderer


	// Get the actual clock milliseconds (SDL)
	unsigned long mTime1 = SDL_GetTicks();

	// ----- Main Loop -----

	while (!mIO.IsKeyDown (SDLK_ESCAPE))
	{

		// ----- Draw -----

		mIO.ClearScreen (); 		// Clear screen
		mGame.DrawScene ();			// Draw staff
		mIO.UpdateScreen ();		// Put the graphic context in the screen

		// ----- Input -----
		controller.addListener(listener);
		// ----- Vertical movement -----

		unsigned long mTime2 = SDL_GetTicks();

		if ((mTime2 - mTime1) > WAIT_TIME)
		{
			if (mBoard.IsPossibleMovement (mGame.mPosX, mGame.mPosY + 1, mGame.mPiece, mGame.mRotation))
			{
				mGame.mPosY++;
			}
			else
			{
				mBoard.StorePiece (mGame.mPosX, mGame.mPosY, mGame.mPiece, mGame.mRotation);

				mBoard.DeletePossibleLines ();

				if (mBoard.IsGameOver())
				{
					mIO.Getkey();
					controller.removeListener(listener);
					exit(0);
				}

				mGame.CreateNewPiece();
			}

			mTime1 = SDL_GetTicks();
		}
	}
	controller.removeListener(listener);
	return 0;
}
