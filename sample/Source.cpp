/******************************************************************************\
* Copyright (C) 2012-2013 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

#include <iostream>
#include <Windows.h>
#include "Leap.h"
#define LEAP_WAIT_TIME 500000
#define LEAP_DEBOUNCE_TIME 800000
using namespace Leap;


class SampleScreen : public Screen {
public:
	int mouse_x;
	int mouse_y;

};
class SampleListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
	Screen screen;
	int32_t mousePointer;
	bool mouseDown;
};


void SampleListener::onInit(const Controller& controller) {
  std::cout << "Initialized" << std::endl;
  ScreenList screens = controller.calibratedScreens();
  SampleListener::screen = screens[0];
  SampleListener::mouseDown = false;
}

void SampleListener::onConnect(const Controller& controller) {
  std::cout << "Connected" << std::endl;
}

void SampleListener::onDisconnect(const Controller& controller) {
  std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
  std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
  const Frame frame = controller.frame();
  POINT cursor;
  GetCursorPos(&cursor);
  int intersect_x = cursor.x, intersect_y = cursor.y;
  if (!frame.pointables().empty()) {
	  Pointable pointable;
	  if(!SampleListener::mousePointer){//check if Listener has an associated pointable
		  pointable = frame.pointables()[0];
		  SampleListener::mousePointer = pointable.id();
	  } else{
		  pointable = frame.pointable(SampleListener::mousePointer);
		  if(!pointable.isValid()){
			  pointable = frame.pointables()[0];
			  SampleListener::mousePointer = pointable.id();
		  }
	  }
	  Screen screen = SampleListener::screen;
	  Vector intersect = screen.intersect(pointable, true);
	  //if there is a second finger, pause the mouse
	  bool second = (frame.pointables()[1].isValid() && screen.intersect(frame.pointables()[1],false).isValid()); 
	  if(intersect.isValid() && !second){
		  int intersect_x = intersect[0]*screen.widthPixels();
		  int height = screen.heightPixels();
		  int intersect_y = height - intersect[1]*height;
		  SetCursorPos(intersect_x,intersect_y);
	  }
	  /*if(frame.pointables()[1].isValid() && screen.intersect(frame.pointables()[1],false).isValid()){
			  mouse_event(MOUSEEVENTF_LEFTDOWN, intersect_x, intersect_y, 0, 0);
			  mouse_event(MOUSEEVENTF_LEFTUP, intersect_x, intersect_y, 0, 0)
	  }*/
	  float z_pos = pointable.tipPosition()[2];
	  float z_vel = pointable.tipVelocity()[2];
	  if(!SampleListener::mouseDown){
		  if((z_pos < 0) && (z_vel<-100)){
			  mouse_event(MOUSEEVENTF_LEFTDOWN, intersect_x, intersect_y, 0, 0);
			  SampleListener::mouseDown = true;
		  }
	  }
	  if(SampleListener::mouseDown){
		  std::cout<<"Z Vel: "<<z_vel<<std::endl;
		  if((z_pos)>0 || (z_vel>150)){
			  mouse_event(MOUSEEVENTF_LEFTUP, intersect_x, intersect_y, 0, 0);
			  SampleListener::mouseDown = false;
		  }
	  }
  }
}

int main() {
	/*
	POINT cursor;
   
    while(!GetAsyncKeyState(VK_ESCAPE))
    {
        GetCursorPos(&cursor);
        
        if(GetAsyncKeyState(39))
        {
            if(cursor.x<1600)
            cursor.x++;
            SetCursorPos(cursor.x,cursor.y);
        }
        if(GetAsyncKeyState(37))
        {
            if(cursor.x>0)
            cursor.x--;
            SetCursorPos(cursor.x,cursor.y);
        }
        if(GetAsyncKeyState(40))
        {
            if(cursor.y<900)
            cursor.y++;
            SetCursorPos(cursor.x,cursor.y);
        }
        if(GetAsyncKeyState(38))
        {
            if(cursor.y > 0)
            cursor.y--;
            SetCursorPos(cursor.x,cursor.y);
        }
        
        Sleep(1);
    }
    return 0;*/
  
	// Create a sample listener and controller
  SampleListener listener;
  Controller controller;

  // Have the sample listener receive events from the controller
  controller.addListener(listener);

  // Keep this process running until Enter is pressed
  std::cout << "Press Enter to pause..." << std::endl;
  std::cin.get();

  // Remove the sample listener when done
  controller.removeListener(listener);
  std::cin.get();
  return 0;
}
