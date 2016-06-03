
#include "ofxAudioAnalyzer.h"

//TODO: Document all this funcs and classes. Add description

//---------------------------------------
void ofxAudioAnalyzer::setup(int sampleRate, int bufferSize, int channels){
    
    _samplerate = sampleRate;
    _buffersize = bufferSize;
    _channels = channels;
    
    if(_channels <= 0){
        ofLogWarning()<<"ofxAudioAnalyzer: channels cant be set to none. Setting 1 channel";
        _channels = 1;
    }
    
    for(int i=0; i<_channels; i++){
        ofxAudioAnalyzerUnit * aaUnit = new ofxAudioAnalyzerUnit(_samplerate, _buffersize);
        channelAnalyzerUnits.push_back(aaUnit);
    }
    
}
//---------------------------------------
void ofxAudioAnalyzer::reset(int sampleRate, int bufferSize, int channels){
    
    _samplerate = sampleRate;
    _buffersize = bufferSize;
    _channels = channels;
    
    if(_channels <= 0){
        ofLogWarning()<<"ofxAudioAnalyzer: channels cant be set to none. Setting 1 channel";
        _channels = 1;
    }
    
    for (int i=0; i<channelAnalyzerUnits.size(); i++){
        channelAnalyzerUnits[i]->exit();
    }
    channelAnalyzerUnits.clear();
    
    for(int i=0; i<_channels; i++){
        ofxAudioAnalyzerUnit * aaUnit = new ofxAudioAnalyzerUnit(_samplerate, _buffersize);
        channelAnalyzerUnits.push_back(aaUnit);
    }
    
    
}

//---------------------------------------
void ofxAudioAnalyzer::analyze(const ofSoundBuffer & inBuffer){
   
    if(inBuffer.getNumChannels() != _channels){
        ofLogError()<<"ofxAudioAnalyzer: inBuffer channels number incorrect.";
        return;
    }
    
    if(channelAnalyzerUnits.size()!= _channels){
        ofLogError()<<"ofxAudioAnalyzer: wrong number of audioAnalyzers";
        return;
    }
    
    for (int i=0; i<_channels; i++){
        ofSoundBuffer chBuff;
        inBuffer.getChannel(chBuff, i);//copy channel from inBuffer
        if(channelAnalyzerUnits[i]!=nullptr){
            channelAnalyzerUnits[i]->analyze(chBuff.getBuffer());
            //cout<<"analyzer-"<<i<<"rms"<<channelAnalyzerUnits[i]->getRms()<<endl;
        }else{
            ofLogError()<<"ofxAudioAnalyzer: channelAnalyzer NULL pointer";
        }
        
    }
    
    
    
}
//---------------------------------------
void ofxAudioAnalyzer::exit(){
    
    for(int i=0; i<channelAnalyzerUnits.size();i++){
        channelAnalyzerUnits[i]->exit();
    }
    
    
}