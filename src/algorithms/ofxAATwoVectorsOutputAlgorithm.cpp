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

#include "ofxAATwoVectorsOutputAlgorithm.h"

ofxAATwoVectorsOutputAlgorithm::ofxAATwoVectorsOutputAlgorithm(ofxaa::AlgorithmType algorithmType, int samplerate, int framesize, int outputSize_1, int outputSize_2) :  ofxAAOneVectorOutputAlgorithm(algorithmType, samplerate, framesize, outputSize_1) {
    assignSecondOutpuValuesSize(outputSize_2, 0.0);
}

void ofxAATwoVectorsOutputAlgorithm::assignSecondOutpuValuesSize(int size, int val){
    outputValues_2.assign(size, val);
    //smoothedFloatValues_2.assign(size, val);
}

vector<float>& ofxAATwoVectorsOutputAlgorithm::getValues2(float smooth, bool normalized){
    if (normalized){
        return smooth ? smoothedValues2Normalized(smooth) : values2Normalized();
    } else {
        return smooth ? smoothedValues2(smooth) : outputValues_2;
    }
}
//-------------------------------------------
vector<float>& ofxAATwoVectorsOutputAlgorithm::values2Normalized(){
    
    if (isNormalizedByDefault) {
        return outputValues_2;
    } else {
        for (int i=0; i<outputValues_2.size(); i++){
            if (hasLogaritmicValues){
            _normalizedValues_2[i] = ofMap(lin2db(outputValues_2[i]), dbSilenceCutoff, DB_MAX, 0.0, 1.0, TRUE);
            } else {
                _normalizedValues_2[i] = ofMap(outputValues_2[i], minEstimatedValue, maxEstimatedValue, 0.0, 1.0, TRUE);
            }
        }
    }
    return _normalizedValues_2;
}
//-------------------------------------------
vector<float>& ofxAATwoVectorsOutputAlgorithm::smoothedValues2(float smthAmnt){
    for(int i=0; i<_smoothedValues_2.size(); i++){
        _smoothedValues_2[i] = smooth(outputValues_2[i], _smoothedValues_2[i], smthAmnt);
    }
    return _smoothedValues_2;
}
//-------------------------------------------
vector<float>& ofxAATwoVectorsOutputAlgorithm::smoothedValues2Normalized(float smthAmnt){
    auto normValues = values2Normalized();
    for(int i=0; i<_smoothedValuesNormalized_2.size(); i++){
        _smoothedValuesNormalized_2[i] = smooth(normValues[i], _smoothedValuesNormalized_2[i], smthAmnt);
    }
    return _smoothedValuesNormalized_2;
}
