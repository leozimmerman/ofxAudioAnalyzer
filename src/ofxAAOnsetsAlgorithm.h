
#pragma once

#include "ofxAudioAnalyzerAlgorithms.h"


enum OnsetsTimeTresholdMode{
    TIME_BASED,
    BUFFER_NUM_BASED
};

class ofxAAOnsetsAlgorithm : public ofxAABaseAlgorithm{

public:
    
    void setup(int bufferSize);
    void compute();
    void castValuesToFloat();
    void evaluate();
    
    void reset();
    
    bool getValue(){return _value;}
    float getOnsetSilenceTreshold(){return silenceTreshold;}
    float getOnsetTimeTreshold(){return timeTreshold;}
    float getOnsetAlpha(){return alpha;}
    
    void setOnsetSilenceTreshold(float val){silenceTreshold=val;}
    void setOnsetAlpha(float val){alpha=val;}
    void setOnsetTimeTreshold(float ms){timeTreshold = ms;}
    void setOnsetBufferNumTreshold(int buffersNum){bufferNumTreshold = buffersNum;}
    void setUseTimeTreshold(bool doUse){usingTimeTreshold = doUse;}
    void setOnsetTimeTresholdsMode(OnsetsTimeTresholdMode mode){onsetsMode = mode;}

    ofxAABaseAlgorithm onsetHfc;
    ofxAABaseAlgorithm onsetComplex;
    ofxAABaseAlgorithm onsetFlux;
    
private:
    
    bool _value;//isOnset
    
    bool onsetBufferEvaluation (Real iDetectHfc, Real iDetectComplex, Real iDetectFlux);
    bool onsetTimeTresholdEvaluation();
    bool onsetBufferNumTresholdEvaluation();//framebased treshold eval.
    
    int detecBufferSize;
    vector<vector<Real> > detections;
    vector<Real> detection_sum;
    Real hfc_max, complex_max, flux_max;

    Real silenceTreshold, alpha;
    bool addHfc, addComplex, addFlux;
    
    bool usingTimeTreshold;
    float timeTreshold;
    float lastOnsetTime;
    int bufferNumTreshold;
    int lastOnsetBufferNum;
    
    OnsetsTimeTresholdMode onsetsMode;
    int bufferCounter;
    

};