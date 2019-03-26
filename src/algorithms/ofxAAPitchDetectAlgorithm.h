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

class ofxAAPitchDetectAlgorithm : public ofxAABaseAlgorithm{
    
public:
    
    ofxAAPitchDetectAlgorithm(ofxAAAlgorithmType algorithmType, int samplerate, int framesize);
    
    void castValuesToFloat();
    
    float getPitchValue();
    float getPitchValueNormalized();
    float getConfidenceValue();
    
    float getSmoothedPitchValue(float smthAmnt);
    float getSmoothedPitchValueNormalized(float smthAmnt);
    float getSmoothedConfidenceValue(float smthAmnt);
    
    float getMaxEstimatedValue() override {return pitchMaxEstimatedValue;}
    
    void setMaxEstimatedValue(float value) override;
    
    Real pitchRealVal, confidenceRealVal;
    
private:
    
    float pitchFloatVal, confidenceFloatVal;
    
    float smoothedPitchFloatValue;
    float smoothedNormPitchFloatValue;
    float smoothedConfidenceFloatValue;
    
    float pitchMaxEstimatedValue;
    
};
