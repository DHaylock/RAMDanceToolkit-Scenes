//
//  FlickerControl.h
//  RAMDanceToolkit
//
//  Created by David Haylock on 25/10/2017.
//

#ifndef FlickerControl_h
#define FlickerControl_h

#import "ofMain.h"
#import "ramMain.h"
#import "ofxMidi.h"

class FlickerControl : public rdtk::BaseScene
{
	public:
	
		//--------------------------------------------------------------
		FlickerControl() {
			
			name = "Flicker Control";
		
			lowpassamount = 1.0;
			iVelocity = 0;
			midiOut.listPorts();
			midiOut.openPort(0);
			
			rotationRangeLeftElbowX[0] = -180;
			rotationRangeLeftElbowX[1] = 180;
			rotationRangeLeftElbowY[0] = -180;
			rotationRangeLeftElbowY[1] = 180;
			rotationRangeLeftElbowZ[0] = -180;
			rotationRangeLeftElbowZ[1] = 180;
			
			rotationRangeRightElbowX[0] = -180;
			rotationRangeRightElbowX[1] = 180;
			rotationRangeRightElbowY[0] = -180;
			rotationRangeRightElbowY[1] = 180;
			rotationRangeRightElbowZ[0] = -180;
			rotationRangeRightElbowZ[1] = 180;
			
			rotationRangeLeftKneeX[0] = -180;
			rotationRangeLeftKneeX[1] = 180;
			rotationRangeLeftKneeY[0] = -180;
			rotationRangeLeftKneeY[1] = 180;
			rotationRangeLeftKneeZ[0] = -180;
			rotationRangeLeftKneeZ[1] = 180;
			
			rotationRangeRightKneeX[0] = -180;
			rotationRangeRightKneeX[1] = 180;
			rotationRangeRightKneeY[0] = -180;
			rotationRangeRightKneeY[1] = 180;
			rotationRangeRightKneeZ[0] = -180;
			rotationRangeRightKneeZ[1] = 180;
			
			constantTrigger = false;
			
			for(int i = 0; i < 4; i++) { activeOrientation[i] = 0; currentTrackId[i] = 0; }
		}
	
		//--------------------------------------------------------------
		~FlickerControl()
		{
			midiOut.closePort();
		}
	
		//--------------------------------------------------------------
		string getName() const
		{
			return name;
		}
	
		//--------------------------------------------------------------
		void drawImGui()
		{
			ImGui::Checkbox("Constant Trigger", &constantTrigger);
			ImGui::SliderFloat("Low Pass", &lowpassamount,0.0,1.0);
			ImGui::SliderInt("Velocity", &iVelocity,0,127);
			
			ImGui::PushID("1");
			ImGui::Text("Left Elbow");
			ImGui::RadioButton("X",&activeOrientation[0], 0); ImGui::SameLine();
			ImGui::RadioButton("Y",&activeOrientation[0], 1); ImGui::SameLine();
			ImGui::RadioButton("Z",&activeOrientation[0], 2);
			ImGui::DragIntRange2("Rotation Range X", &rotationRangeLeftElbowX[0], &rotationRangeLeftElbowX[1]);
			ImGui::DragIntRange2("Rotation Range Y", &rotationRangeLeftElbowY[0], &rotationRangeLeftElbowY[1]);
			ImGui::DragIntRange2("Rotation Range Z", &rotationRangeLeftElbowZ[0], &rotationRangeLeftElbowZ[1]);
			ImGui::PopID();
			
			ImGui::Separator();
			
			ImGui::PushID("2");
			ImGui::Text("Right Elbow");
			ImGui::RadioButton("X",&activeOrientation[1], 0); ImGui::SameLine();
			ImGui::RadioButton("Y",&activeOrientation[1], 1); ImGui::SameLine();
			ImGui::RadioButton("Z",&activeOrientation[1], 2);
			ImGui::DragIntRange2("Rotation Range X", &rotationRangeRightElbowX[0], &rotationRangeRightElbowX[1]);
			ImGui::DragIntRange2("Rotation Range Y", &rotationRangeRightElbowY[0], &rotationRangeRightElbowY[1]);
			ImGui::DragIntRange2("Rotation Range Z", &rotationRangeRightElbowZ[0], &rotationRangeRightElbowZ[1]);
			ImGui::PopID();
			
			ImGui::Separator();
			
			ImGui::PushID("3");
			ImGui::Text("Left Knee");
			ImGui::RadioButton("X",&activeOrientation[2], 0); ImGui::SameLine();
			ImGui::RadioButton("Y",&activeOrientation[2], 1); ImGui::SameLine();
			ImGui::RadioButton("Z",&activeOrientation[2], 2);
			ImGui::DragIntRange2("Rotation Range X", &rotationRangeLeftKneeX[0], &rotationRangeLeftKneeX[1]);
			ImGui::DragIntRange2("Rotation Range Y", &rotationRangeLeftKneeY[0], &rotationRangeLeftKneeY[1]);
			ImGui::DragIntRange2("Rotation Range Z", &rotationRangeLeftKneeZ[0], &rotationRangeLeftKneeZ[1]);
			ImGui::PopID();
			
			ImGui::Separator();
			
			ImGui::PushID("4");
			ImGui::Text("Right Knee");
			ImGui::RadioButton("X",&activeOrientation[3], 0); ImGui::SameLine();
			ImGui::RadioButton("Y",&activeOrientation[3], 1); ImGui::SameLine();
			ImGui::RadioButton("Z",&activeOrientation[3], 2);
			ImGui::DragIntRange2("Rotation Range X", &rotationRangeRightKneeX[0], &rotationRangeRightKneeX[1]);
			ImGui::DragIntRange2("Rotation Range Y", &rotationRangeRightKneeY[0], &rotationRangeRightKneeY[1]);
			ImGui::DragIntRange2("Rotation Range Z", &rotationRangeRightKneeZ[0], &rotationRangeRightKneeZ[1]);
			ImGui::PopID();
		}
	
		//--------------------------------------------------------------
		void drawActor(const rdtk::NodeArray &actor) {
			
			const rdtk::NodeArray actor1 = lowPassFilter.update(actor);
			
			rdtk::DrawBasicActor(actor1);
		
			ofDrawBitmapString("Left Elbow: " + ofToString(actor.getNode(rdtk::Actor::JOINT_LEFT_ELBOW).getOrientationEuler()) ,10,300);
			ofDrawBitmapString("Right Elbow: "+ ofToString(actor.getNode(rdtk::Actor::JOINT_RIGHT_ELBOW).getOrientationEuler()) ,10,315);
			ofDrawBitmapString("Left Knee: "+ ofToString(actor.getNode(rdtk::Actor::JOINT_LEFT_KNEE).getOrientationEuler()) ,10,330);
			ofDrawBitmapString("Right Knee: "+ ofToString(actor.getNode(rdtk::Actor::JOINT_RIGHT_ELBOW).getOrientationEuler()) ,10,345);
			
			int leftE = 0;
			switch(activeOrientation[0])
			{
				case 0:
				{
					leftE = ofMap(actor.getNode(rdtk::Actor::JOINT_LEFT_ELBOW).getOrientationEuler().x,
								  rotationRangeLeftElbowX[0],rotationRangeLeftElbowX[1],
								  0,9);
				}
				break;
				case 1:
				{
					leftE = ofMap(actor.getNode(rdtk::Actor::JOINT_LEFT_ELBOW).getOrientationEuler().y,
								  rotationRangeLeftElbowY[0],rotationRangeLeftElbowY[1],
								  0,9);
				}
				break;
				case 2:
				{
					leftE = ofMap(actor.getNode(rdtk::Actor::JOINT_LEFT_ELBOW).getOrientationEuler().z,
								  rotationRangeLeftElbowZ[0],rotationRangeLeftElbowZ[1],
								  0,9);
				}
				break;
				default: break;
			}
			
			int rightE = 0;
			switch(activeOrientation[1])
			{
				case 0:
				{
					rightE = ofMap(actor.getNode(rdtk::Actor::JOINT_RIGHT_ELBOW).getOrientationEuler().x,
								  rotationRangeRightElbowX[0],rotationRangeRightElbowX[1],
								  10,19);
				}
				break;
				case 1:
				{
					rightE = ofMap(actor.getNode(rdtk::Actor::JOINT_RIGHT_ELBOW).getOrientationEuler().y,
								   rotationRangeRightElbowY[0],rotationRangeRightElbowY[1],
								   10,19);
				}
				break;
				case 2:
				{
					rightE = ofMap(actor.getNode(rdtk::Actor::JOINT_RIGHT_ELBOW).getOrientationEuler().z,
								   rotationRangeRightElbowZ[0],rotationRangeRightElbowZ[1],
								   10,19);
				}
				break;
				default:
					break;
			}
			
			int leftK = 0;
			switch(activeOrientation[2])
			{
				case 0:
				{
					leftK = ofMap(actor.getNode(rdtk::Actor::JOINT_LEFT_KNEE).getOrientationEuler().x,
								 rotationRangeRightElbowX[0],rotationRangeRightElbowX[1],
								 20,29);
				}
				break;
				case 1:
				{
					leftK = ofMap(actor.getNode(rdtk::Actor::JOINT_LEFT_KNEE).getOrientationEuler().y,
								  rotationRangeRightElbowY[0],rotationRangeRightElbowY[1],
								  20,29);
				}
					break;
				case 2:
				{
					leftK = ofMap(actor.getNode(rdtk::Actor::JOINT_LEFT_KNEE).getOrientationEuler().z,
								  rotationRangeRightElbowZ[0],rotationRangeRightElbowZ[1],
								  20,29);
				}
					break;
				default: break;
			}
			
			int rightK = 0;
			switch(activeOrientation[3])
			{
				case 0:
				{
					leftK = ofMap(actor.getNode(rdtk::Actor::JOINT_RIGHT_KNEE).getOrientationEuler().x,
								  rotationRangeRightElbowX[0],rotationRangeRightElbowX[1],
								  30,39);
				}
					break;
				case 1:
				{
					leftK = ofMap(actor.getNode(rdtk::Actor::JOINT_RIGHT_KNEE).getOrientationEuler().y,
								  rotationRangeRightElbowY[0],rotationRangeRightElbowY[1],
								  30,39);
				}
					break;
				case 2:
				{
					leftK = ofMap(actor.getNode(rdtk::Actor::JOINT_RIGHT_KNEE).getOrientationEuler().z,
								  rotationRangeRightElbowZ[0],rotationRangeRightElbowZ[1],
								  30,39);
				}
					break;
				default: break;
			}
			
			currentTrackId[0] = ofClamp(leftE,0,9);
			currentTrackId[1] = ofClamp(rightE,10,19);
			currentTrackId[2] = ofClamp(leftK,20,29);
			currentTrackId[3] = ofClamp(rightK,30,39);
		
			if(constantTrigger) {
				if(currentTrackId[0] != previousTrackId[0]) {
					midiOut.sendNoteOn(1,currentTrackId[0],10);
					previousTrackId[0] = currentTrackId[0];
				}
				
				if(currentTrackId[1] != previousTrackId[1]) {
					midiOut.sendNoteOn(2,currentTrackId[1],10);
					previousTrackId[1] = currentTrackId[1];
				}
				
				if(currentTrackId[2] != previousTrackId[2]) {
					midiOut.sendNoteOn(3,currentTrackId[2],10);
					previousTrackId[2] = currentTrackId[2];
				}
				
				if(currentTrackId[3] != previousTrackId[3]) {
					midiOut.sendNoteOn(4,currentTrackId[3],10);
					previousTrackId[3] = currentTrackId[3];
				}
				
				usleep(5);
				midiOut.sendNoteOn(1,currentTrackId[0],0);
				midiOut.sendNoteOn(2,currentTrackId[1],0);
				midiOut.sendNoteOn(3,currentTrackId[2],0);
				midiOut.sendNoteOn(4,currentTrackId[3],0);
				
				float back = ofMap(actor.getNode(rdtk::Actor::JOINT_CHEST).getOrientationEuler().x,-180,180,10,127);
				back = ofClamp(back,10,127);
				midiOut.sendControlChange(2,10,back);
			}
		}
	
		void draw()
		{
			ofDrawBitmapString("Left Elbow Active Orientation: " + ofToString(activeOrientation[0]) ,10,100);
			ofDrawBitmapString("Right Elbow Active Orientation: "+ ofToString(activeOrientation[1]) ,10,115);
			ofDrawBitmapString("Left Knee Active Orientation: "+ ofToString(activeOrientation[2]) ,10,130);
			ofDrawBitmapString("Right Knee Active Orientation: "+ ofToString(activeOrientation[3]) ,10,145);
			
			ofDrawBitmapString("Left Elbow Track ID: " + ofToString(currentTrackId[0]) ,10,200);
			ofDrawBitmapString("Right Elbow Track ID: "+ ofToString(currentTrackId[1]) ,10,215);
			ofDrawBitmapString("Left Knee Track ID: "+ ofToString(currentTrackId[2]) ,10,230);
			ofDrawBitmapString("Right Knee Track ID: "+ ofToString(currentTrackId[3]) ,10,245);
		}
	
	ofxMidiOut midiOut;
	
	float startAngle;
	float endAngle;
	float speedWhichRotated;
	
	int iVelocity;
	
	int activeOrientation[4];
	int currentTrackId[4];
	int previousTrackId[4];

	int rotationRangeLeftElbowX[2];
	int rotationRangeLeftElbowY[2];
	int rotationRangeLeftElbowZ[2];

	int rotationRangeRightElbowX[2];
	int rotationRangeRightElbowY[2];
	int rotationRangeRightElbowZ[2];
	
	int rotationRangeLeftKneeX[2];
	int rotationRangeLeftKneeY[2];
	int rotationRangeLeftKneeZ[2];
	
	int rotationRangeRightKneeX[2];
	int rotationRangeRightKneeY[2];
	int rotationRangeRightKneeZ[2];
	
	rdtk::LowPassFilter lowPassFilter;
	float lowpassamount;
	bool constantTrigger;
	
	string name;
	
};

#endif /* FlickerControl_h */
