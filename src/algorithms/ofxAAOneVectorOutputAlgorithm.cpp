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

#include "ofxAAOneVectorOutputAlgorithm.h"

ofxAAOneVectorOutputAlgorithm::ofxAAOneVectorOutputAlgorithm(ofxaa::AlgorithmType algorithmType, int samplerate, int framesize, int outputSize) : ofxAABaseAlgorithm(algorithmType, samplerate, framesize){
    
    assignOutputValuesSize(outputSize, 0.0);
    hasLogarithmicValues = false;
}
//-------------------------------------------
void ofxAAOneVectorOutputAlgorithm::assignOutputValuesSize(int size, int val){
    outputValues.assign(size, val);
    checkInternalValuesSizes();
}
//-------------------------------------------
void ofxAAOneVectorOutputAlgorithm::checkInternalValuesSizes(){
    auto size = outputValues.size();
    float val = 0.0;
    if (size != _normalizedValues.size()){
        _normalizedValues.assign(size, val);
        _smoothedValues.assign(size, val);
        _smoothedValuesNormalized.assign(size, val);
        _linearValues.assign(size, val);
    }
}
//-------------------------------------------
void ofxAAOneVectorOutputAlgorithm::compute(){
    ofxAABaseAlgorithm::compute();
    if (!isActive) {
        float zeroValue = hasLogarithmicValues ? dbSilenceCutoff : 0.0;
        static vector<float> zerosVec(outputValues.size(), zeroValue);
        outputValues = zerosVec;
    }
}
//-------------------------------------------
float ofxAAOneVectorOutputAlgorithm::getValueAtIndex(int index, float smooth, bool normalized){
    return getValues(smooth, normalized)[index];
}
//-------------------------------------------
vector<float>& ofxAAOneVectorOutputAlgorithm::getValues(float smooth, bool normalized){
    checkInternalValuesSizes();

    if (normalized){
        normalizeValues(outputValues, _normalizedValues);
        smoothValues(_normalizedValues, _smoothedValuesNormalized, smooth);
        return _smoothedValuesNormalized;
    } else {
        linValues(outputValues, _linearValues);
        smoothValues(_linearValues, _smoothedValues, smooth);
        return _smoothedValues;
    }
}
//-------------------------------------------
void ofxAAOneVectorOutputAlgorithm::linValues(vector<float>& valuesToLin, vector<float>& linearValues){
    if (hasLogarithmicValues){
        float dbMax = lin2db(maxEstimatedValue);
        for (int i=0; i<outputValues.size(); i++){
            linearValues[i] =  ofMap(lin2db(outputValues[i]), DB_MIN, dbMax, 0.0, 1.0, TRUE);
        }
    } else {
        linearValues = valuesToLin;
    }
}
//-------------------------------------------
void ofxAAOneVectorOutputAlgorithm::normalizeValues(vector<float>& valuesToNorm, vector<float>& normValues){
    
    if (isNormalizedByDefault || hasLogarithmicValues) {
        linValues(outputValues, _linearValues);
        normValues = _linearValues;
    } else {
        for (int i=0; i<valuesToNorm.size(); i++){
            if (hasLogarithmicValues){
                float dbMax = 0.0;
                normValues[i] = ofMap(lin2db(valuesToNorm[i]), dbSilenceCutoff, dbMax, 0.0, 1.0, TRUE);
            } else {
                float min = (_minEstimatedValues.size() == normValues.size()) ? _minEstimatedValues[i] : minEstimatedValue;
                float max = (_maxEstimatedValues.size() == normValues.size()) ? _maxEstimatedValues[i] : maxEstimatedValue;
                normValues[i] = ofMap(outputValues[i], min, max, 0.0, 1.0, TRUE);
            }
        }
    }
    
}
//-------------------------------------------
void ofxAAOneVectorOutputAlgorithm::smoothValues(vector<float>& valuesToSmooth, vector<float>& smoothedValues, float smthAmnt){
    if (smthAmnt == 0) {
        smoothedValues = valuesToSmooth;
        return;
    }
    
    for(int i=0; i<smoothedValues.size(); i++){
        smoothedValues[i] = smooth(valuesToSmooth[i], smoothedValues[i], smthAmnt);
    }
}

void ofxAAOneVectorOutputAlgorithm::setMaxEstimatedValues(vector<float> values){
    if (values.size() != outputValues.size()){ return ;}
    _maxEstimatedValues = values;
}

void ofxAAOneVectorOutputAlgorithm::setMinEstimatedValues(vector<float> values){
    if (values.size() != outputValues.size()){ return ;}
    _minEstimatedValues = values;
}


