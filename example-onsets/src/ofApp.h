#pragma once

#include "ofMain.h"
#include "ofxAudioAnalyzer.h"
#include "ofSoundPlayerExtended.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofxAudioAnalyzer audioAnalyzer;
        ofSoundPlayerExtended player;
    
        int sampleRate;
        int bufferSize;
    
        ofSoundBuffer soundBuffer;
    
        bool onset;
    
        ofxPanel gui;
        ofxFloatSlider alpha;
        ofxFloatSlider silenceThreshold;
        ofxToggle useTimeThreshold;
        ofxFloatSlider timeThreshold;
    
};
