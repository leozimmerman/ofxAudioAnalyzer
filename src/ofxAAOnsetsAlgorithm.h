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


enum OnsetsTimeThresholdMode{
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
    float getOnsetSilenceThreshold(){return silenceThreshold;}
    float getOnsetTimeThreshold(){return timeThreshold;}
    float getOnsetAlpha(){return alpha;}
    
    void setOnsetSilenceThreshold(float val){silenceThreshold=val;}
    void setOnsetAlpha(float val){alpha=val;}
    void setOnsetTimeThreshold(float ms){timeThreshold = ms;}
    void setOnsetBufferNumThreshold(int buffersNum){bufferNumThreshold = buffersNum;}
    void setUseTimeThreshold(bool doUse){usingTimeThreshold = doUse;}
    void setOnsetTimeThresholdsMode(OnsetsTimeThresholdMode mode){onsetsMode = mode;}

    ofxAABaseAlgorithm onsetHfc;
    ofxAABaseAlgorithm onsetComplex;
    ofxAABaseAlgorithm onsetFlux;
    
private:
    
    bool _value;//isOnset
    
    bool onsetBufferEvaluation (Real iDetectHfc, Real iDetectComplex, Real iDetectFlux);
    bool onsetTimeThresholdEvaluation();
    bool onsetBufferNumThresholdEvaluation();//framebased threshold eval.
    
    int detecBufferSize;
    vector<vector<Real> > detections;
    vector<Real> detection_sum;
    Real hfc_max, complex_max, flux_max;

    Real silenceThreshold, alpha;
    bool addHfc, addComplex, addFlux;
    
    bool usingTimeThreshold;
    float timeThreshold;
    float lastOnsetTime;
    int bufferNumThreshold;
    int lastOnsetBufferNum;
    
    OnsetsTimeThresholdMode onsetsMode;
    int bufferCounter;
    

};
