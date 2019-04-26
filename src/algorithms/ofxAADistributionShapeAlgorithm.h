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

#include "ofxAAOneVectorOutputAlgorithm.h"

class ofxAADistributionShapeAlgorithm : public ofxAAOneVectorOutputAlgorithm {
    
public:
    ofxAADistributionShapeAlgorithm(int samplerate, int framesize) : ofxAAOneVectorOutputAlgorithm(ofxaa::DistributionShape, samplerate, framesize, 3){}
    
    float getKurtosisValue(float smooth, bool normalized){
        return getValueAtIndex(0, smooth, normalized);
    }
    
    float getSpreadValue(float smooth, bool normalized){
        return getValueAtIndex(1, smooth, normalized);
    }
    
    float getSkewnessValue(float smooth, bool normalized){
        return getValueAtIndex(2, smooth, normalized);
    }
    
    void setMaxKurtosisValue(float value){
         setMaxEstimatedValues({value, getMaxEstimatedValues()[1], getMaxEstimatedValues()[2]});
    }
    
    void setMaxSpreadValue(float value){
        setMaxEstimatedValues({getMaxEstimatedValues()[0], value, getMaxEstimatedValues()[2]});
    }
    
    void setMaxSkewnessValue(float value){
        setMaxEstimatedValues({getMaxEstimatedValues()[0], getMaxEstimatedValues()[1], value});
    }
    
    float getMaxKurtosisValue(){
        return getMaxEstimatedValues()[0];
    }
    
    float getMaxSpreadValue(){
        return getMaxEstimatedValues()[1];
    }
    
    float getMaxSkewnessValue(){
        return getMaxEstimatedValues()[2];
    }
    
};
