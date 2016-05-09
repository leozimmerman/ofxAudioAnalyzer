
#include "ofxAAMain.h"
//---------------------------------------
//void ofxAAMain::setup(ofSoundBuffer & soundBuffer){
//    
//    _samplerate = soundBuffer.getSampleRate();
//    _buffersize = soundBuffer.getNumFrames();
//    _channels = soundBuffer.getNumChannels();
//    
//    
//    
//}
//---------------------------------------
void ofxAAMain::setup(int sampleRate, int bufferSize, int channels){
    
    _samplerate = sampleRate;
    _buffersize = bufferSize;
    _channels = channels;
    
    if(_channels <= 0){
        ofLogWarning()<<"ofxAudioAnalyzer: channels cant be set to none. Setting 1 channel";
        _channels = 1;
    }
    
    //for(channels)
    
    
    for(int i=0; i<_channels; i++){
        ofxAudioAnalyzer * aa = new ofxAudioAnalyzer(_samplerate, _buffersize);
        channelAnalyzers.push_back(aa);
    }
    
    
    
}
//---------------------------------------
void ofxAAMain::reset(int sampleRate, int bufferSize, int channels){
    
    _samplerate = sampleRate;
    _buffersize = bufferSize;
    _channels = channels;
    
    if(_channels <= 0){
        ofLogWarning()<<"ofxAudioAnalyzer: channels cant be set to none. Setting 1 channel";
        _channels = 1;
    }
    
    for (int i=0; i<channelAnalyzers.size(); i++){
        channelAnalyzers[i]->exit();
    }
    channelAnalyzers.clear();
    
    for(int i=0; i<_channels; i++){
        ofxAudioAnalyzer * aa = new ofxAudioAnalyzer(_samplerate, _buffersize);
        channelAnalyzers.push_back(aa);
    }
    
    
}

//---------------------------------------
void ofxAAMain::analyze(const ofSoundBuffer & inBuffer){
   
//    cout<<"SB info--"<<endl;
//    cout<<"SB samplerate: "<<inBuffer.getSampleRate()<<endl;
//    cout<<"SB buffersize: "<<inBuffer.getNumFrames()<<endl;
//    cout<<"SB channels: "<<inBuffer.getNumChannels()<<endl;
    
    if(inBuffer.getNumChannels() != _channels){
        ofLogError()<<"ofxAudioAnalyzer: inBuffer channels number not match the one set.";
        return;
    }
    
    if(channelAnalyzers.size()!= _channels){
        ofLogError()<<"ofxAudioAnalyzer: wrong number of audioAnalyzers";
        return;
    }
    
    for (int i=0; i<_channels; i++){
        ofSoundBuffer chBuff;
        inBuffer.getChannel(chBuff, i);//copy channel from inBuffer
        if(channelAnalyzers[i]!=nullptr){
            channelAnalyzers[i]->analyze(chBuff.getBuffer());
            //cout<<"analyzer-"<<i<<"rms"<<channelAnalyzers[i]->getRms()<<endl;
        }else{
            ofLogError()<<"ofxAudioAnalyzer: channelAnalyzer NULL pointer";
        }
        
    }
    
    
    
}
//---------------------------------------
void ofxAAMain::exit(){
    
    for(int i=0; i<channelAnalyzers.size();i++){
        channelAnalyzers[i]->exit();
    }
    
    
}