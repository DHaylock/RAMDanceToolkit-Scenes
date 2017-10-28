//
//  RGBSuiteOut.h
//  RAMDanceToolkit
//
//  Created by David Haylock on 23/05/2017.
//
//

#ifndef RGBSuiteOut_h
#define RGBSuiteOut_h

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ramMain.h"

class RGBSuiteOut : public ramBaseScene
{
	public:
	
		//-------------------------------------------------------------------------
		RGBSuiteOut()
		{
			sOsc.setup("127.0.0.1",12345);
		}
	
		//-------------------------------------------------------------------------
		~RGBSuiteOut() { }
	
		//-------------------------------------------------------------------------
		string getName() const
		{
			return "RGBSuiteOut";
		}
	
		//-------------------------------------------------------------------------
		void drawActor(const ramActor &actor)
		{
			ofxOscBundle b;
			int noOfNodes = actor.getNumNode();
			for(int i = 0; i < noOfNodes; i++)
			{
				ofxOscMessage m;
				m.setAddress("/" + actor.getJointName(i));
				
				ofVec3f pos = actor.getNode(i).getPosition();
				ofVec3f accel = actor.getNode(i).getAcceleration();
				ofVec3f rot = actor.getNode(i).getOrientationEuler();
				
				m.addFloatArg(pos.x);
				m.addFloatArg(pos.y);
				m.addFloatArg(pos.z);
				
				m.addFloatArg(accel.x);
				m.addFloatArg(accel.y);
				m.addFloatArg(accel.z);
				
				m.addFloatArg(rot.x);
				m.addFloatArg(rot.y);
				m.addFloatArg(rot.z);
				
				b.addMessage(m);
			}
			
			sOsc.sendBundle(b);
		}
		//--------------------------------------------------------------
		void onActorExit(const ramActor &actor)
		{
			cout << "Removed New Actor" << endl;
		}
		//--------------------------------------------------------------
		void onActorEnter(const ramActor &actor)
		{
			cout << "Added New Actor" << endl;
		}
		ramLowPassFilter lowPass;
	
	protected:
		ofxOscSender sOsc;
};


#endif /* RGBSuiteOut_h */
