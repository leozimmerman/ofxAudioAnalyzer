#pragma once

#include "ofMain.h"
#include "ofxAudioAnalyzer.h"

class ofxAAMain{
public:
    //void setup(ofSoundBuffer & soundBuffer);
    void setup(int sr, int bs, int ch);
    void analyze(const ofSoundBuffer & inBuffer);
    
    void exit();
    
    vector<ofxAudioAnalyzer*>& getChannelAnalyzersPtrs(){return channelAnalyzers;}
    
    ofSoundBuffer soundBuffer;
    
    
private:
    
    int _samplerate;
    int _buffersize;
    int _channels;
    
    vector<ofxAudioAnalyzer*> channelAnalyzers;
    vector<ofSoundBuffer> channelBuffers;
    
    
};