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


//for scaling values:
#define DB_MIN -6
#define DB_MAX 0
#define MFCC_MAX_ESTIMATED_VALUE 300.0


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
    
    void setActive(ofxAAAlgorithmType algorithmType, bool state);
    void setMaxEstimatedValue(ofxAAAlgorithmType algorithmType, float value);
    
    
    float getValue(ofxAAAlgorithmType algorithmType, float smooth=0.0, bool normalized=false);
    vector<float>& getValues(ofxAAAlgorithmType algorithmType, float smooth=0.0);
   
    bool getIsActive(ofxAAAlgorithmType algorithmType);
    
    
    int getBinsNum(ofxAAAlgorithmType algorithmType);
    float getMaxEstimatedValue(ofxAAAlgorithmType algorithmType);
    
    
    ofxaa::Network* network; //TODO: Make private
private:
    
    
    
    vector<Real> audioBuffer;
    
    int samplerate;
    int framesize;
    
};


