#pragma once

#include "ofMain.h"
#include "ofxAudioAnalyzer.h"

#define DB_MIN -6
#define DB_MAX 0

#define MAX_FREQUENCY 3000.0

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();
    
        void audioOut(ofSoundBuffer &outBuffer);

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
    
        mutex audioMutex;
        ofSoundBuffer lastBuffer;
    
        int sampleRate;
        int bufferSize;
    
        float 	pan;
        float 	volume;
        
        bool 	bNoise;
    
        //------------------- for the simple sine wave synthesis
        float 	targetFrequency;
        float 	phase;
        float 	phaseAdder;
        float 	phaseAdderTarget;
    
        //------------------- analysis results
        float rms_l, rms_r;
        float pitchFreq_l, pitchFreq_r;
        vector<float> spectrum_l, spectrum_r;
    
    
};
