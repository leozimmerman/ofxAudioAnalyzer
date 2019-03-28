/*
 * Copyright (C) 2016 Leo Zimmerman [http://www.leozimmerman.com.ar]
 *
 * ofxAudioAnalyzer is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License as published by the Free
 * Software Foundation (FSF), either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the Affero GNU General Public License
 * version 3 along with this program.  If not, see http://www.gnu.org/licenses/
 *
 * ---------------------------------------------------------------
 *
 * This project uses Essentia, copyrighted by Music Technology Group - Universitat Pompeu Fabra
 * using GNU Affero General Public License.
 * See http://essentia.upf.edu for documentation.
 *
 */

#include "ofxAAOnsetsAlgorithm.h"

//-------------------------------------------
void ofxAAOnsetsAlgorithm::setup(int bufferSize){
    /*
     at 44100:
     512 samples = 11.6 ms
     
     detetctBufferSize: 1 detection x buffers.
     512 x 64 = 742.4 ms
     */
    
//    detecBufferSize = bufferSize;
    detecBufferSize = 64;
    
    detection_sum.assign(detecBufferSize, 0.0);
    detections.assign(3, vector<Real> (detecBufferSize));
    silenceThreshold = 0.02;
    alpha = 0.1;
    timeThreshold = 100.0;
    bufferNumThreshold = 7; //116 ms at 60 fps
    lastOnsetTime = 0.0;
    lastOnsetBufferNum = 0;
    addHfc = addComplex = addFlux = true;
    hfc_max = complex_max = flux_max = 0.0;
    usingTimeThreshold = TRUE;
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
    
    //if current buffer is onset, check for timeThreshold evaluation
    if (usingTimeThreshold && isCurrentBufferOnset){
        
        switch (onsetsMode) {
            case TIME_BASED:
                _value = onsetTimeThresholdEvaluation();
                break;
            case BUFFER_NUM_BASED:
                _value = onsetBufferNumThresholdEvaluation();
                break;
            default:
                break;
        }
    
    }
    else{
        _value = isCurrentBufferOnset;
    }
    
    //update bufferCounter for frameBased timeThreshold evaluation:
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
        if(detection_sum[i] < silenceThreshold) detection_sum[i] = 0.0;
    }
    
    Real buffer_median = median (detection_sum);
    Real buffer_mean = mean (detection_sum);
    Real onset_thhreshold = buffer_median + alpha * buffer_mean;
    
    bool onsetDetection = detection_sum[detection_sum.size()-1] > onset_thhreshold;
    
    return onsetDetection;
    
}
//----------------------------------------------
bool ofxAAOnsetsAlgorithm::onsetTimeThresholdEvaluation(){
    
    bool onsetTimeEvaluation = false;
    
    float currentTime = ofGetElapsedTimeMillis();
    
    //elapsed time since last onset:
    float elapsed = currentTime - lastOnsetTime;
    
    if (elapsed>timeThreshold){
        onsetTimeEvaluation = true;
        lastOnsetTime = currentTime;
    }
    
    return onsetTimeEvaluation;
}
//----------------------------------------------
bool ofxAAOnsetsAlgorithm::onsetBufferNumThresholdEvaluation(){
    
    bool onsetBufferNumEvaluation = false;
    
    //elapsed frames/buffers since last onset:
    int elapsedBuffers = bufferCounter - lastOnsetBufferNum;
    
    if (elapsedBuffers > bufferNumThreshold){
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


