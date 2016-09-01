
#include "ofxAudioAnalyzer.h"

//TODO: Document all this funcs and classes. Add description
//TODO: Add throw error & exceptions with channel num checks??


//-------------------------------------------------------
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
//-------------------------------------------------------
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
//-------------------------------------------------------
void ofxAudioAnalyzer::analyze(const ofSoundBuffer & inBuffer){
   
    if(inBuffer.getNumChannels() != _channels){
        ofLogError()<<"ofxAudioAnalyzer: inBuffer channels number incorrect.";
        return;
    }
    
    if(channelAnalyzerUnits.size()!= _channels){
        ofLogError()<<"ofxAudioAnalyzer: wrong number of audioAnalyzerUnits";
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
//-------------------------------------------------------
void ofxAudioAnalyzer::exit(){
    
    for(int i=0; i<channelAnalyzerUnits.size();i++){
        channelAnalyzerUnits[i]->exit();
    }
    
}
//-------------------------------------------------------
float ofxAudioAnalyzer::getValue(ofxAAAlgorithm algorithm, int channel, float smooth, bool normalized){
    
    if (channel >= _channels){
        ofLogError()<<"ofxAudioAnalyzer: channel for getting value is incorrect.";
        return;
    }
    
    return channelAnalyzerUnits[channel]->getValue(algorithm, smooth, normalized);

}
//-------------------------------------------------------
vector<float>& ofxAudioAnalyzer::getValues(ofxAAAlgorithm algorithm, int channel, float smooth){
    
    if (channel >= _channels){
        ofLogError()<<"ofxAudioAnalyzer: channel for getting value is incorrect.";
        return;
    }
    
    return channelAnalyzerUnits[channel]->getValues(algorithm, smooth);
    
}
//-------------------------------------------------------
vector<SalienceFunctionPeak>& ofxAudioAnalyzer::getSalienceFunctionPeaks(int channel){
    if (channel >= _channels){
        ofLogError()<<"ofxAudioAnalyzer: channel for getting value is incorrect.";
        return;
    }
    
     return channelAnalyzerUnits[channel]->getPitchSaliencePeaksRef();

}
//-------------------------------------------------------
bool ofxAudioAnalyzer::getOnsetValue(int channel){
    
    if (channel >= _channels){
        ofLogError()<<"ofxAudioAnalyzer: channel for getting value is incorrect.";
        return;
    }
    
    return channelAnalyzerUnits[channel]->getOnsetValue();
    
    
}
//-------------------------------------------------------
void ofxAudioAnalyzer::resetOnsets(int channel){
    
    if (channel >= _channels){
        ofLogError()<<"ofxAudioAnalyzer: channel for getting value is incorrect.";
        return;
    }
    
    channelAnalyzerUnits[channel]->resetOnsets();
}

//-------------------------------------------------------
void ofxAudioAnalyzer::setOnsetsParameters(int channel, float alpha, float silenceTresh, float timeTresh, bool useTimeTresh){
    
    if (channel >= _channels){
        ofLogError()<<"ofxAudioAnalyzer: channel for getting value is incorrect.";
        return;
    }
    channelAnalyzerUnits[channel]->setOnsetsParameters(alpha, silenceTresh, timeTresh, useTimeTresh);

}
//-------------------------------------------------------




















