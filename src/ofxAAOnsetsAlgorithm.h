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
