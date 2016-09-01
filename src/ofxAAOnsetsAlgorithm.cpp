
#include "ofxAAOnsetsAlgorithm.h"

//-------------------------------------------
void ofxAAOnsetsAlgorithm::setup(int bufferSize){
    
    //-Onsets:
    detecBufferSize = bufferSize; //FIXME: Check this value. Too big??? 
    detection_sum.assign(detecBufferSize, 0.0);
    detections.assign(3, vector<Real> (detecBufferSize));
    silenceTreshold = 0.02;
    alpha = 0.1;
    timeTreshold = 100.0;
    bufferNumTreshold = 7; //116 ms at 60 fps
    lastOnsetTime = 0.0;
    lastOnsetBufferNum = 0;
    addHfc = addComplex = addFlux = true;
    hfc_max = complex_max = flux_max = 0.0;
    usingTimeTreshold = false;
    onsetsMode = TIME_BASED;
    bufferCounter = 0;
    
    _value = false;
    
    init();
    
    onsetHfc.init();
    onsetComplex.init();
    onsetFlux.init();
}
//-------------------------------------------
void ofxAAOnsetsAlgorithm::compute(){
    
    onsetHfc.compute();
    onsetComplex.compute();
    onsetFlux.compute();

}
//-------------------------------------------
void ofxAAOnsetsAlgorithm::castValuesToFloat(){
    
    if(isActivated){
        onsetHfc.castValueToFloat();
        onsetComplex.castValueToFloat();
        onsetFlux.castValueToFloat();
    }else{
        onsetHfc.setValueZero();
        onsetFlux.setValueZero();
        onsetComplex.setValueZero();
    }

}
//-------------------------------------------
void ofxAAOnsetsAlgorithm::evaluate(){
    
    //is current buffer an Onset?
    bool isCurrentBufferOnset = onsetBufferEvaluation(onsetHfc.getValue(), onsetComplex.getValue(), onsetFlux.getValue());
    
    //if current buffer is onset, check for timeTreshold evaluation
    if (usingTimeTreshold && isCurrentBufferOnset){
        
        switch (onsetsMode) {
            
            case TIME_BASED:
                _value = onsetTimeTresholdEvaluation();
                break;
            
            case BUFFER_NUM_BASED:
                _value = onsetBufferNumTresholdEvaluation();
                break;
                
            default:
                break;
        }
    
    }
    else{
        _value = isCurrentBufferOnset;
    }
    
    //update bufferCounter for frameBased timeTreshold evaluation:
    if (onsetsMode == BUFFER_NUM_BASED) bufferCounter++;
}

//--------------------------------------------------------------
bool ofxAAOnsetsAlgorithm::onsetBufferEvaluation (Real iDetectHfc, Real iDetectComplex, Real iDetectFlux){
    
    Real prop_hfc, prop_complex, prop_flux;
    
    if (iDetectHfc > hfc_max) {
        prop_hfc = iDetectHfc/hfc_max;
        hfc_max = iDetectHfc;
        for (int i=0; i<detections[0].size(); i++)
            detections[0][i] /= prop_hfc;
    }
    if (iDetectComplex > complex_max){
        prop_complex = iDetectComplex/complex_max;
        complex_max = iDetectComplex;
        for (int i=0; i<detections[1].size(); i++)
            detections[1][i] /= prop_complex;
    }
    if (iDetectFlux > flux_max){
        prop_flux = iDetectFlux/flux_max;
        flux_max = iDetectFlux;
        for (int i=0; i<detections[2].size(); i++)
            detections[2][i] /= prop_flux;
    }
    
    Real hfc_norm = iDetectHfc / hfc_max;
    Real complex_norm = iDetectComplex / complex_max;
    Real flux_norm = iDetectFlux / flux_max;
    
    detections[0].push_back(hfc_norm);
    detections[0].erase(detections[0].begin());
    
    detections[1].push_back(complex_norm);
    detections[1].erase(detections[1].begin());
    
    detections[2].push_back(flux_norm);
    detections[2].erase(detections[2].begin());
    
    for (int i=0; i<detection_sum.size(); i++){
        int n=0;
        detection_sum[i]=0;
        if(addHfc){
            detection_sum[i] += detections[0][i];
            n++;
        }
        if(addComplex){
            detection_sum[i] += detections[1][i];
            n++;
        }
        if(addFlux){
            detection_sum[i] += detections[2][i];
            n++;
        }
        if(n>0) detection_sum[i] /= n;
        if(detection_sum[i] < silenceTreshold) detection_sum[i] = 0.0;
    }
    
    Real buffer_median = median (detection_sum);
    Real buffer_mean = mean (detection_sum);
    Real onset_threshold = buffer_median + alpha * buffer_mean;
    
    bool onsetDetection = detection_sum[detection_sum.size()-1] > onset_threshold;
    
    return onsetDetection;
    
}
//----------------------------------------------
bool ofxAAOnsetsAlgorithm::onsetTimeTresholdEvaluation(){
    
    bool onsetTimeEvaluation = false;
    
    float currentTime = ofGetElapsedTimeMillis();
    
    //elapsed time since last onset:
    float elapsed = currentTime - lastOnsetTime;
    
    if (elapsed>timeTreshold){
        onsetTimeEvaluation = true;
        lastOnsetTime = currentTime;
    }
    
    return onsetTimeEvaluation;
}
//----------------------------------------------
bool ofxAAOnsetsAlgorithm::onsetBufferNumTresholdEvaluation(){
    
    bool onsetBufferNumEvaluation = false;
    
    //elapsed frames/buffers since last onset:
    int elapsedBuffers = bufferCounter - lastOnsetBufferNum;
    
    if (elapsedBuffers > bufferNumTreshold){
        onsetBufferNumEvaluation = true;
        lastOnsetBufferNum = bufferCounter;
    }
    
    return onsetBufferNumEvaluation;
    
    
}
//----------------------------------------------
void ofxAAOnsetsAlgorithm::reset(){
    
    hfc_max = complex_max = flux_max = 0.0;
    for (int i=0; i<detection_sum.size(); i++){
        detection_sum[i] = 0.0;
    }
    
    //necessary?
    onsetHfc.algorithm->reset();
    onsetComplex.algorithm->reset();
    onsetFlux.algorithm->reset();
    
    
    bufferCounter = 0;
}


