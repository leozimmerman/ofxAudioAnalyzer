/*
 * Copyright (C) 2019 Leo Zimmerman [http://www.leozimmerman.com.ar]
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

#include "ofxAABaseAlgorithm.h"

//---------------------------------------------------------------------
struct SalienceFunctionPeak{
    float bin;//cents
    float value;
    
    SalienceFunctionPeak(){
        bin = 0.0;
        value = 0.0;
    }
};

class ofxAAPitchSalienceFunctionPeaksAlgorithm : public ofxAABaseAlgorithm{
public:
    
    ofxAAPitchSalienceFunctionPeaksAlgorithm(ofxAAAlgorithmType algorithmType, int samplerate, int framesize) : ofxAABaseAlgorithm(algorithmType, samplerate, framesize){
        
        limitPeaksNum = TRUE;
        maxPeaksNum = 4;
    }
    
    void castValuesToFloat();
    
    vector<SalienceFunctionPeak>& getPeaks();
    vector<SalienceFunctionPeak>& getSmoothedPeaks(float smthAmnt);
    
    ///change to maxPeaks!!!!
    void setMaxPeaksNum(int maxBins){maxPeaksNum = maxBins;}
    
    vector<Real> realSalienceBins;
    vector<Real> realSalienceValues;
    
private:
    vector<SalienceFunctionPeak> peaks;
    vector<SalienceFunctionPeak> smoothedPeaks;
    
    bool limitPeaksNum;
    
    int maxPeaksNum;
};
