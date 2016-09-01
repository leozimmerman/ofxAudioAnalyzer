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
    
        float rms;
        float power;
        float pitchFreq;
        float pitchConf;
        float pitchSalience;
        float inharmonicity;
        float hfc;
        float hfcNorm;
        float specComp;
        float specCompNorm;
        float centroid;
        float centroidNorm;
        float dissonance;
   
        vector<float> spectrum;
        vector<float> melBands;
        vector<float> mfcc;
        vector<float> hpcp;
        vector<float> multiPitches;
    
        vector<SalienceFunctionPeak> saliencePeaks;
    
        bool isOnset;
    
    
        ofxPanel gui;
        ofxFloatSlider rms_smth;
        ofxFloatSlider power_smth;
        ofxFloatSlider pitchFreq_smth;
        ofxFloatSlider pitchConf_smth;
        ofxFloatSlider melodySalience_smth;
        ofxFloatSlider inharmonicity_smth;
        ofxFloatSlider hfc_smth;
        ofxFloatSlider specComp_smth;
        ofxFloatSlider centroid_smth;
    
        ofxFloatSlider dissonance_smth;
    
        ofxFloatSlider spectrum_smth;
        ofxFloatSlider melBands_smth;
        ofxFloatSlider mfcc_smth;
        ofxFloatSlider hpcp_smth;
};
