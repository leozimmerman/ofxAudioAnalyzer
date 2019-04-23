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

#include "ofMain.h"

#include "ofxAudioAnalyzerAlgorithms.h"
#include "ofxAANetwork.h"

class ofxAudioAnalyzerUnit
{

public:
    
    ofxAudioAnalyzerUnit(int sampleRate, int bufferSize);
    
    ~ofxAudioAnalyzerUnit(){
        exit();
    }
    
    void analyze(const vector<float> &  inBuffer);
    void exit();
    
    int getSampleRate() {return samplerate;}
    int getBufferSize() {return framesize;}
    
    
    float getValue(ofxAAValue value, float smooth, bool normalized);
    float getValue(ofxAAValue value){ return getValue(value, 0.0, false); }
    vector<float>& getValues(ofxAABinsValue value, float smooth , bool normalized);
    vector<float>& getValues(ofxAABinsValue value){ return getValues(value, 0.0, false); }
    
    int getBinsNum(ofxAABinsValue value);
    
    void setActive(ofxAAValue valueType, bool state);
    void setActive(ofxAABinsValue valueType, bool state);
    
    bool getIsActive(ofxAAValue valueType);
    bool getIsActive(ofxAABinsValue valueType);
    
    float getMaxEstimatedValue(ofxAAValue valueType);
    float getMaxEstimatedValue(ofxAABinsValue valueType);
    
    float getMinEstimatedValue(ofxAAValue valueType);
    float getMinEstimatedValue(ofxAABinsValue valueType);
    
    void setMaxEstimatedValue(ofxAAValue valueType, float value);
    void setMaxEstimatedValue(ofxAABinsValue valueType, float value);
    
    ofxAAOnsetsAlgorithm* getOnsetsPtr(){ return network->getOnsetsPtr();}
    
private:
    ofxaa::Network* network; 
    
    vector<Real> audioBuffer;
    vector<Real> accumulatedAudioBuffer;
    
    int samplerate;
    int framesize;
};


