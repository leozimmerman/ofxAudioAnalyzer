#pragma once

#include "ofMain.h"
#include "ofxAudioAnalyzerUnit.h"

class ofxAudioAnalyzer{
 
 public:
    
    void setup(int sampleRate, int bufferSize, int channels);
    
    void reset(int sampleRate, int bufferSize, int channels);
    
    void analyze(const ofSoundBuffer & inBuffer);
    
    void exit();
    
    vector<ofxAudioAnalyzerUnit*>& getChannelAnalyzersPtrs(){return channelAnalyzerUnits;}
    
    ofSoundBuffer soundBuffer;
    

 private:
    
    int _samplerate;
    int _buffersize;
    int _channels;
    
    vector<ofxAudioAnalyzerUnit*> channelAnalyzerUnits;
    
    
};

