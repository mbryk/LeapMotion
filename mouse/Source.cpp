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
  double intersect_x = cursor.x;
  double intersect_y = cursor.y;
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
	  int pointables_count = frame.pointables().count(); 
	  if(intersect.isValid()){
		  if(pointables_count < 3){
			  intersect_x = intersect[0]*screen.widthPixels();
			  int height = screen.heightPixels();
			  intersect_y = height - intersect[1]*height;
			  if(pointables_count > 1){
				  int diff_x = (intersect_x - cursor.x);
				  int diff_y = (intersect_y - cursor.y);
				  
				  intersect_x = cursor.x + ((diff_x >> 31)*2 +1);
				  intersect_y = cursor.y + ((diff_y >> 31)*2 +1);
			  }
			  SetCursorPos(intersect_x,intersect_y);
		  }
		  
		  float z_pos = pointable.tipPosition()[2];
		  float z_vel = pointable.tipVelocity()[2];
		  if(!SampleListener::mouseDown){
			  if((z_pos < 0) && (z_vel<-100)){
				  mouse_event(MOUSEEVENTF_LEFTDOWN, intersect_x, intersect_y, 0, 0);
				  SampleListener::mouseDown = true;
			  }
		  }
		  if(SampleListener::mouseDown){
			  if((z_pos)>0 || (z_vel>150)){
				  mouse_event(MOUSEEVENTF_LEFTUP, intersect_x, intersect_y, 0, 0);
				  SampleListener::mouseDown = false;
			  }
		  }
	  }
  }
}

int main() {
	// Create a sample listener and controller
	SampleListener listener;
	Controller controller;

	// Have the sample listener receive events from the controller
	controller.addListener(listener);

	// Keep this process running until Enter is pressed
	std::cout << "Press Enter to quit..." << std::endl;
	std::cin.get();

	// Remove the sample listener when done
	controller.removeListener(listener);
	
	return 0;
}
