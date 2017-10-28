// 
// FourPoints.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "ramMain.h"

#include "SphereMesh.h"
#include "ramGeometry.h"

class FourPoints : public rdtk::BaseScene
{
public:
	
	ofxUIToggleMatrix *m4p1, *m4p2, *m4p3, *m4p4;
	
	int currentSelected[4] = {0,0,0,0};
	bool showFourPointTwist, showFourPointSphere;
	bool pickExtents, pickCore, pickKneesElbows;
	float twistResolution, twistExtensionWidth, twistExtensionHeight, pointSize;
	
	void selectFourPoints(int i0, int i1, int i2, int i3) 
	{
		currentSelected[0] = i0;
		currentSelected[1] = i1;
		currentSelected[2] = i2;
		currentSelected[3] = i3;
		
		m4p1->setAllToggles(false);
		m4p2->setAllToggles(false);
		m4p3->setAllToggles(false);
		m4p4->setAllToggles(false);
		m4p1->getToggles()[i0]->setValue(true);
		m4p2->getToggles()[i1]->setValue(true);
		m4p3->getToggles()[i2]->setValue(true);
		m4p4->getToggles()[i3]->setValue(true);
	}
	
	void setupControlPanel()
	{
		
#ifdef RAM_GUI_SYSTEM_OFXUI
		
		ofxUICanvas* panel = rdtk::GetGUI().getCurrentUIContext();
		
		showFourPointSphere = false;
		showFourPointTwist = true;
		twistResolution = 20;
		twistExtensionWidth = 1;
		twistExtensionHeight = 1;
		pointSize = 3;
		
		pickExtents = false;
		pickCore = false;
		pickKneesElbows = false;
		
		float dim = 20;
		float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
		float length = 320 - xInit;
		panel->addLabelToggle("Show four point sphere", &showFourPointSphere, length, dim);
		panel->addLabelToggle("Show four point twist", &showFourPointTwist, length, dim);
		panel->addSlider("Twist resolution", 2, 200, &twistResolution, length, dim);
		panel->addSlider("Twist extension width", 1, 40, &twistExtensionWidth, length, dim);
		panel->addSlider("Twist extension height", 1, 8, &twistExtensionHeight, length, dim);
		panel->addLabelButton("Pick extents", &pickExtents);
		panel->addLabelButton("Pick core", &pickCore);
		panel->addLabelButton("Pick knees and elbows", &pickKneesElbows);
		
		int matrixSize = 10;
		m4p1 = (ofxUIToggleMatrix*) panel->addWidgetDown(new ofxUIToggleMatrix(matrixSize, matrixSize, 5, 5, "4p.1"));
		m4p2 = (ofxUIToggleMatrix*) panel->addWidgetRight(new ofxUIToggleMatrix(matrixSize, matrixSize, 5, 5, "4p.2"));
		m4p3 = (ofxUIToggleMatrix*) panel->addWidgetRight(new ofxUIToggleMatrix(matrixSize, matrixSize, 5, 5, "4p.3"));
		m4p4 = (ofxUIToggleMatrix*) panel->addWidgetRight(new ofxUIToggleMatrix(matrixSize, matrixSize, 5, 5, "4p.4"));
		m4p1->setAllowMultiple(false);
		m4p2->setAllowMultiple(false);
		m4p3->setAllowMultiple(false);
		m4p4->setAllowMultiple(false);
	
#endif
		
	}
	
	void drawImGui()
	{
		ImGui::Checkbox("Show four point sphere", &showFourPointSphere);
		ImGui::Checkbox("Show four point twist", &showFourPointTwist);
		ImGui::DragFloat("Twist resolution", &twistResolution, 0.1f, 2, 200);
		ImGui::DragFloat("Twist extension width",  &twistExtensionWidth, 0.01f, 1, 40);
		ImGui::DragFloat("Twist extension height", &twistExtensionHeight, 0.01f, 1, 8);
		
		if (ImGui::Button("Pick Extents"))
			selectFourPoints(rdtk::Actor::JOINT_LEFT_HAND,
							 rdtk::Actor::JOINT_RIGHT_HAND,
							 rdtk::Actor::JOINT_LEFT_TOE,
							 rdtk::Actor::JOINT_RIGHT_TOE);
		
		if (ImGui::Button("Pick core"))
			selectFourPoints(rdtk::Actor::JOINT_LEFT_SHOULDER,
							 rdtk::Actor::JOINT_RIGHT_SHOULDER,
							 rdtk::Actor::JOINT_LEFT_HIP,
							 rdtk::Actor::JOINT_RIGHT_HIP);
		
		if (ImGui::Button("Pick knees and elbows"))
			selectFourPoints(rdtk::Actor::JOINT_LEFT_ELBOW,
							 rdtk::Actor::JOINT_RIGHT_ELBOW,
							 rdtk::Actor::JOINT_LEFT_KNEE,
							 rdtk::Actor::JOINT_RIGHT_KNEE);
		
		bool updated = false;
		for (int j = 0;j < rdtk::Actor::NUM_JOINTS;j++)
		{
			for (int i = 0;i < 4;i++)
			{
				if (i > 0) ImGui::SameLine();
				ImGui::PushID((rdtk::Actor::getJointName(j)+ofToString(i)).c_str());
				if (ImGui::RadioButton(i == 3 ? rdtk::Actor::getJointName(j).c_str() : "",
									   &currentSelected[i], j)) updated = true;
				ImGui::PopID();
			}
		}
		
		if (updated) selectFourPoints(currentSelected[0],
									  currentSelected[1],
									  currentSelected[2],
									  currentSelected[3]);
	}
	
	void setup()
	{		
		_ofSetIcoSphereResolution(3);
	}
	
	void update()
	{
		if(pickExtents)
		{			
			selectFourPoints(rdtk::Actor::JOINT_LEFT_HAND, rdtk::Actor::JOINT_RIGHT_HAND, rdtk::Actor::JOINT_LEFT_TOE, rdtk::Actor::JOINT_RIGHT_TOE);
		}
		if(pickCore)
		{
			selectFourPoints(rdtk::Actor::JOINT_LEFT_SHOULDER, rdtk::Actor::JOINT_RIGHT_SHOULDER, rdtk::Actor::JOINT_LEFT_HIP, rdtk::Actor::JOINT_RIGHT_HIP);			
		}
		if(pickKneesElbows)
		{
			selectFourPoints(rdtk::Actor::JOINT_LEFT_ELBOW, rdtk::Actor::JOINT_RIGHT_ELBOW, rdtk::Actor::JOINT_LEFT_KNEE, rdtk::Actor::JOINT_RIGHT_KNEE);
		}
	}
	
	//--------------------------------------------------------------
	void draw()
	{	
	}
	
	//--------------------------------------------------------------
	int getChoice(ofxUIToggleMatrix* matrix) {
		vector<ofxUIToggle*> toggles = matrix->getToggles();
		for(int i = 0; i < toggles.size(); i++) {
			if(toggles[i]->getValue()) {
				return i;
			}
		}
		return 0;
	}
	
	//--------------------------------------------------------------
	void drawActor(const rdtk::Actor &actor)
	{		
		// maybe this is slow...? need a better way to do point size/depth testing.
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPointSize(pointSize);
		glDisable(GL_DEPTH_TEST);
		
		ofPushStyle();
		ofNoFill();
		ofSetColor(0);
		
		ofEnableAlphaBlending();
		ofVec3f j1 = actor.getNode(getChoice(m4p1));
		ofVec3f j2 = actor.getNode(getChoice(m4p2));
		ofVec3f j3 = actor.getNode(getChoice(m4p3));
		ofVec3f j4 = actor.getNode(getChoice(m4p4));
		if(j1 != j2 && j1 != j3 && j1 != j4 && j2 != j3 && j2 != j4 && j3 != j4) {
			if(showFourPointSphere) {
				ofVec3f sphereCenter;
				float sphereRadius;
				findSphere(j1, j2, j3, j4, sphereCenter, sphereRadius);
				ofSetColor(255, 32);
				ofIcoSphere(sphereCenter, sphereRadius);
			}
			if(showFourPointTwist) {
				ofVec3f ap = (j1 + j2) / 2, bp = (j3 + j4) / 2;
				ofVec3f an = j2 - j1, bn = j4 - j3;
				float al = an.length(), bl = bn.length();
				ofQuaternion aq, bq;
				aq.makeRotate(ofVec3f(1, 0, 0), an.getNormalized());
				bq.makeRotate(ofVec3f(1, 0, 0), bn.getNormalized());
				ofSetColor(255, 192);
				for(int i = 0; i < twistResolution; i++) {
					float t = ofMap(i, 0, twistResolution - 1, 1 - twistExtensionHeight, 0 + twistExtensionHeight);
					ofPushMatrix();
					ofTranslate(ap.getInterpolated(bp, t));
					ofQuaternion q;
					q.slerp(t, aq, bq);
					ofMultMatrix(ofMatrix4x4(q));
					float radius = twistExtensionWidth * ofLerp(al, bl, t) / 2;
					ofDrawLine(-radius, 0, radius, 0);
					ofPopMatrix();
				}
			}
		}
		
		ofPopStyle();
		glPopAttrib();
	}
	
	//--------------------------------------------------------------
	void drawRigid(const rdtk::RigidBody &rigid)
	{
	}
	
	string getName() const { return "Four Points"; }
	
};
