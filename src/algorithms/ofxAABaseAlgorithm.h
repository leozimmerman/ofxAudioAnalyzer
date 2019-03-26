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

#include "ofMain.h"
#include "ofxAAAlgorithmTypes.h"

#include "algorithmfactory.h"
#include "essentiamath.h"
#include "pool.h"

using namespace std;
using namespace essentia;
using namespace standard;

class ofxAABaseAlgorithm{
    
public:
    
    ofxAABaseAlgorithm(ofxAAAlgorithmType algorithmType, int samplerate, int framesize);
    
    virtual ~ofxAABaseAlgorithm() = default;
    
    float getValue();
    float getValueDb();
    
    ///Gets the value normalized from 0 to maxEstimatedValue with Clamping
    float getValueNormalized();
    
    float getValueNormalized(float min, float max, bool doClamp=TRUE);
    float getValueDbNormalized(float min, float max, bool doClamp=TRUE);
    float getSmoothedValue(float smthAmnt);
    
    ///Gets the value normalized and smoothed from 0 to maxEstimatedValue with Clamping
    float getSmoothedValueNormalized(float smthAmnt);
    
    float getSmoothedValueNormalized(float smthAmnt, float min, float max, bool doClamp=TRUE);
    float getSmoothedValueDbNormalized(float smthAmnt, float min, float max, bool doClamp=TRUE);
    
    virtual float getMaxEstimatedValue();
    bool getIsActive();
    ofxAAAlgorithmType getType();
    
    void setActive(bool state);
    void setValueZero();
    virtual void setMaxEstimatedValue(float value);
    
    void compute();
    
    void castValueToFloat();
    
    virtual void deleteAlgorithm();
    
    Algorithm* algorithm;
    Real realValue;
    
protected:
    
    bool isActivated;
    ofxAAAlgorithmType type;
    
private:
    
    float floatValue;
    float smoothedFloatValue;
    float smoothedNormFloatValue;
    float smoothedNormFloatValueDb;
    
    float maxEstimatedValue;
    
};
