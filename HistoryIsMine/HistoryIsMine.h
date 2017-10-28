//
//  History.h
//  RAMDanceToolkit
//
//  Created by David Haylock on 10/02/2016.
//
//

#ifndef HistoryIsMine_h
#define HistoryIsMine_h

#include "ofxXmlSettings.h"
#include "ramNodeLine.h"

class HistoryParticle {
	HistoryParticle() {}
	~HistoryParticle() {}
	HistoryParticle(ofPolyline line) {
		this->line = line;
	}
	
	void draw()
	{
		z--;
		ofPushMatrix();
		ofTranslate(0,0,z);
		line.draw();
		ofPopMatrix();
	}
	int z = 0;
	ofPolyline line;
};

class HistoryIsMine : public rdtk::BaseScene {
public:
	
	HistoryIsMine() {}
	~HistoryIsMine() {}
	string getName() const { return "HistoryIsMine"; }
	
	deque <ofPolyline> bodyHistory;
	bool enabled = true;
	float length = 250.0;
	float captureFrames = 5.0;
	float polySmoothing = 3.0;
	
	void setupControlPanel()
	{
		rdtk::GetGUI().addToggle("Enabled", &enabled);
		rdtk::GetGUI().addSlider("Length", 1.0, 1000.0, &length);
		rdtk::GetGUI().addSlider("Poly Smoothing", 1.0, 10, &polySmoothing);
		rdtk::GetGUI().addSlider("Capture Rate", 1.0, 60, &captureFrames);
	}
	
	void drawImGui()
	{
		ImGui::Checkbox("Enabled", &enabled);
		ImGui::DragFloat("Length", &length, 1.0, 1000.0);
		ImGui::DragFloat("Poly Smoothing", &polySmoothing, 1.0, 10);
		ImGui::DragFloat("Capture Rate", &captureFrames, 1.0, 60);
	}
	
	void update() {
		
	}
	
	void draw() {
		rdtk::BeginCamera();
		for (int i = 0; i < bodyHistory.size(); i++) {
			ofSetColor(255,100);
			bodyHistory[i].setClosed(true);
			bodyHistory[i].draw();
		}
		rdtk::EndCamera();
	}
	
	void drawActor(const ramActor &actor) {
		
		
		if (bodyHistory.size() > length) {
			bodyHistory.pop_front();
		}
		
//		cout << actor.getNode(ramActor::JOINT_HEAD).getOrientationEuler() << endl;
		
		
		ofPolyline temp;
		temp.addVertex(actor.getNode(ramActor::JOINT_HEAD).getGlobalPosition());
		temp.addVertex(actor.getNode(ramActor::JOINT_RIGHT_SHOULDER).getGlobalPosition());
		temp.addVertex(actor.getNode(ramActor::JOINT_RIGHT_ELBOW).getGlobalPosition());
		temp.addVertex(actor.getNode(ramActor::JOINT_RIGHT_HAND).getGlobalPosition());
		temp.addVertex(actor.getNode(ramActor::JOINT_RIGHT_HIP).getGlobalPosition());
		temp.addVertex(actor.getNode(ramActor::JOINT_RIGHT_KNEE).getGlobalPosition());
		temp.addVertex(actor.getNode(ramActor::JOINT_RIGHT_ANKLE).getGlobalPosition());
		temp.addVertex(actor.getNode(ramActor::JOINT_LEFT_ANKLE).getGlobalPosition());
		temp.addVertex(actor.getNode(ramActor::JOINT_LEFT_KNEE).getGlobalPosition());
		temp.addVertex(actor.getNode(ramActor::JOINT_LEFT_HIP).getGlobalPosition());
		temp.addVertex(actor.getNode(ramActor::JOINT_LEFT_HAND).getGlobalPosition());
		temp.addVertex(actor.getNode(ramActor::JOINT_LEFT_ELBOW).getGlobalPosition());
		temp.addVertex(actor.getNode(ramActor::JOINT_LEFT_SHOULDER).getGlobalPosition());
		
		temp.close();
		
		
		if (ofGetFrameNum() % (int)(captureFrames) == 0) {
			bodyHistory.push_back(temp.getSmoothed(polySmoothing));
		}
	}
	
};
#endif /* HistoryIsMine_h */

