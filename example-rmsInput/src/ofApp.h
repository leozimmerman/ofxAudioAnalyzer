#pragma once

#include "ofMain.h"
#include "ofxAudioAnalyzer.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();
    
        void audioIn(ofSoundBuffer &inBuffer);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        ofSoundStream soundStream;
        ofxAudioAnalyzer audioAnalyzer;
    
        float rms_l, rms_r;
        float smooth;
    
		
};
