#pragma once

#include "ofMain.h"
#include "ofxAudioAnalyzer.h"
#include "ofSoundPlayerExtended.h"
#include "ofxGui.h"
#include "ofxOsc.h"

class Trigger {
public:
    float value = 0;
    int counter = 0;
    float lastInput, totalPower;
    bool notTriggered = true;
    
    float update(float input, float sensitivity) {
        
        if (input >= lastInput) {
            totalPower += (input - lastInput);
            
            
            if ((totalPower >= (1 - sensitivity))&&(notTriggered)) {
                
                counter += 1;
                
                value = input;
                notTriggered = false;
                
            } else {
                value = 0;
            }
            
        } else {
            notTriggered = true;
            totalPower = 0;
            value = 0;
        }
        
        lastInput = input;
        
        return value;
    }
};

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
    void sendMessage(string address, float value);

    void audioIn(ofSoundBuffer & buffer);

    ofxAudioAnalyzer audioAnalyzer;
    ofSoundPlayerExtended player;

    int sampleRate;
    int bufferSize;

    ofSoundBuffer playerBuffer;

    float rms;
    float power;
    float strongPeak;
    float strongPeakNorm;
    float strongDecay;
    float strongDecayNorm;


    ofSoundStream sound;
    ofSoundStreamSettings settings;

    vector<float> spectrum;
    vector<float> melBands;

    vector<float> tristimulus;

    bool isOnset;
    bool bAudioInput;

    ofxPanel gui;
    ofParameter<float> smoothAttack;
    ofParameter<float> smoothRelease;
    ofParameter<float> melbandASensi;
    ofParameter<float> melbandBSensi;
    
    float melbandA, melbandB;
    float melbandTrigA, melbandTrigB;
    
    Trigger triggerA, triggerB;
    
    
    ofxOscReceiver receiver;
    ofxOscSender sender;
    
    bool bMessages;
    
};
