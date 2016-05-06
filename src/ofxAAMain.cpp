
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
void ofxAAMain::setup(int sr, int bs, int ch){
    
   
    
    _samplerate = sr;
    _buffersize = bs;
    _channels = ch;
    
    if(_channels <= 0){
        ofLogWarning()<<"ofxAudioAnalyzer: channels cant be set to none. Setting 1 channel";
        _channels = 1;
    }
    
    //for(channels)
    
    
    for(int i=0; i<_channels; i++){
        ofxAudioAnalyzer * aa = new ofxAudioAnalyzer(_samplerate, _buffersize);
        channelAnalyzers.push_back(aa);
        
        ofSoundBuffer buff;
        channelBuffers.push_back(buff);
    }
    
    
    
}
//---------------------------------------
void ofxAAMain::analyze(const ofSoundBuffer & inBuffer){
    //for(channels)
    
    if(inBuffer.getNumChannels() != _channels){
        ofLogError()<<"ofxAudioAnalyzer: inBuffer channels number not match the one set.";
        return;
    }
    
    
    for (int i=0; i<_channels; i++){
        inBuffer.getChannel(channelBuffers[i], i);//copy channel from inBuffer
        channelAnalyzers[i]->analyze(channelBuffers[i].getBuffer());
//        cout<<"mainAnalyze-"<<audioAnalyzer->getRms()<<endl;
    }
    
    
    
}
//---------------------------------------
void ofxAAMain::exit(){
    
    for(int i=0; i<channelAnalyzers.size();i++){
        channelAnalyzers[i]->exit();
    }
    
    
}