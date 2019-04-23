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

//#include <iostream>
//#include "algorithmfactory.h"
//#include "essentiamath.h"
//#include "pool.h"
//
//using namespace std;
//using namespace essentia;
//using namespace standard;

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
    
    void resetOnsets();
    bool getOnsetValue();
    
    int getSampleRate() {return samplerate;}
    int getBufferSize() {return framesize;}
    
    void setActive(ofxaa::AlgorithmType algorithmType, bool state);
    void setMaxEstimatedValue(ofxaa::AlgorithmType algorithmType, float value);
    
    
    float getValue(ofxAAValue value, float smooth, bool normalized);
    vector<float>& getValues(ofxAABinsValue value, float smooth , bool normalized);
    int getBinsNum(ofxAABinsValue value);
    
    bool getIsActive(ofxaa::AlgorithmType algorithmType);
    
    
    float getMaxEstimatedValue(ofxaa::AlgorithmType algorithmType);
    
    
    ofxaa::Network* network; //TODO: Make private
private:
    
    
    
    vector<Real> audioBuffer;
    vector<Real> accumulatedAudioBuffer;
    
    int samplerate;
    int framesize;
    
};


