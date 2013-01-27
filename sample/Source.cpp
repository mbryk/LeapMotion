/******************************************************************************\
* Copyright (C) 2012-2013 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

#include <iostream>
#include "Leap.h"
#define LEAP_WAIT_TIME 500000
#define LEAP_DEBOUNCE_TIME 800000
using namespace Leap;


class SampleListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
	//int64_t timestamp;
	int64_t lastMove;
};

void SampleListener::onInit(const Controller& controller) {
  std::cout << "Initialized" << std::endl;
  SampleListener::lastMove = 0;
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
  // Get the most recent frame and report some basic information
  const Frame frame = controller.frame();
  /*std::cout << "Frame id: " << frame.id()
            << ", timestamp: " << frame.timestamp()
            << ", hands: " << frame.hands().count()
            << ", fingers: " << frame.fingers().count()
            << ", tools: " << frame.tools().count() << std::endl;

  if (!frame.hands().empty()) {
    // Get the first hand
    const Hand hand = frame.hands()[0];

    // Check if the hand has any fingers
    const FingerList fingers = hand.fingers();
    if (!fingers.empty()) {
      // Calculate the hand's average finger tip position
      Vector avgPos;
      for (int i = 0; i < fingers.count(); ++i) {
        avgPos += fingers[i].tipPosition();
      }
      avgPos /= (float)fingers.count();
      std::cout << "Hand has " << fingers.count()
                << " fingers, average finger tip position" << avgPos << std::endl;
    }

    // Get the hand's sphere radius and palm position
    std::cout << "Hand sphere radius: " << hand.sphereRadius()
              << " mm, palm position: " << hand.palmPosition() << std::endl;

    // Get the hand's normal vector and direction
    const Vector normal = hand.palmNormal();
    const Vector direction = hand.direction();

    // Calculate the hand's pitch, roll, and yaw angles
    std::cout << "Hand pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
              << "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
              << "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl << std::endl;
  }
  */

  /*int64_t diff = frame.timestamp() - SampleListener::timestamp;
  if(diff> LEAP_WAIT_TIME) {
	  std::cout << diff << std::endl;
	  SampleListener::timestamp = frame.timestamp();
  }*/
  //positive jerk over 1000 means you're going to the right
  if (!frame.pointables().empty()) {
	  const Pointable pointable = frame.pointables()[0];
	  if(frame.timestamp() - SampleListener::lastMove > LEAP_DEBOUNCE_TIME){
		  
		  const Vector velocity = pointable.tipVelocity();
		  
		  float velocity_x = velocity[0];
		  float velocity_y = velocity[1];
		  if(velocity_x > 1000){
			  std::cout<< "Right"<<std::endl;
			  SampleListener::lastMove = frame.timestamp();
		  } else if (velocity_x < -1000){
			  std::cout<< "Left"<<std::endl;
			  SampleListener::lastMove = frame.timestamp();
		  } else if (velocity_y < -1000){
			  std::cout<< "Down"<<std::endl;
			  SampleListener::lastMove = frame.timestamp();
		  } else if (velocity_y > 1000){
			  std::cout<< "Up"<<std::endl;
			  SampleListener::lastMove = frame.timestamp();
		  } else {


		  }

		  /*const Vector velocity = hand.palmVelocity();
		  float velocity_x = velocity[0];
		  if(velocity_x > 4000){
			  std::cout<< "Right"<<std::endl;
			  SampleListener::lastMove = frame.timestamp();
		  } else if (velocity_x < -4000){
			  std::cout<< "Left"<<std::endl;
			  SampleListener::lastMove = frame.timestamp();
		  }
		  if(std::abs(velocity_x)>2000)
			  std::cout<<velocity_x<<std::endl;
			  */
	  }
	  const Vector direction = pointable.direction();
	  std::cout << "X: "<<direction[0]<< "Y: "<<direction[1]<< "Z: "<<direction[2]<<std::endl;
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
  std::cin.get();
  return 0;
}
