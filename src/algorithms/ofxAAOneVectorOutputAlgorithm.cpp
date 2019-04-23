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
    hasLogaritmicValues = false;
}
//-------------------------------------------
void ofxAAOneVectorOutputAlgorithm::assignOutputValuesSize(int size, int val){
    outputValues.assign(size, val);
    _normalizedValues.assign(size, val);
    _smoothedValues.assign(size, val);
    _smoothedValuesNormalized.assign(size, val);
}
//-------------------------------------------
void ofxAAOneVectorOutputAlgorithm::compute(){
    ofxAABaseAlgorithm::compute();
    if (!isActive) {
        float zeroValue = hasLogaritmicValues ? dbSilenceCutoff : 0.0; ///???
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
    if (normalized){
        return smooth ? smoothedValuesNormalized(smooth) : valuesNormalized();
    } else {
        return smooth ? smoothedValues(smooth) : outputValues;
    }
}
//-------------------------------------------
vector<float>& ofxAAOneVectorOutputAlgorithm::valuesNormalized(){
    
    if (isNormalizedByDefault) {
        return outputValues;
    } else {
        for (int i=0; i<outputValues.size(); i++){
            if (hasLogaritmicValues){
                _normalizedValues[i] = ofMap(lin2db(outputValues[i]), dbSilenceCutoff, DB_MAX, 0.0, 1.0, TRUE);
            } else {
                _normalizedValues[i] = ofMap(outputValues[i], minEstimatedValue, maxEstimatedValue, 0.0, 1.0, TRUE);
            }
        }
    }
    return _normalizedValues;
}
//-------------------------------------------
vector<float>& ofxAAOneVectorOutputAlgorithm::smoothedValues(float smthAmnt){
    for(int i=0; i<_smoothedValues.size(); i++){
        _smoothedValues[i] = smooth(outputValues[i], _smoothedValues[i], smthAmnt);
    }
    return _smoothedValues;
}
//-------------------------------------------
vector<float>& ofxAAOneVectorOutputAlgorithm::smoothedValuesNormalized(float smthAmnt){
    auto normValues = valuesNormalized();
    for(int i=0; i<_smoothedValuesNormalized.size(); i++){
        _smoothedValuesNormalized[i] = smooth(normValues[i], _smoothedValuesNormalized[i], smthAmnt);
    }
    return _smoothedValuesNormalized;
}
//-------------------------------------------
//int ofxAAOneVectorOutputAlgorithm::getBinsNum(){
//    return outputValues.size();
//}
//-------------------------------------------
/*
void ofxAAOneVectorOutputAlgorithm::castValues(vector<Real>& reals, vector<float>& floats){
    if (floats.size() != reals.size()) { return; }
    
    for (int i=0; i<reals.size(); i++){
        if(getIsActive()){
            if(hasLogaritmicValues){
                if(reals[i] == 0.0){
                    floats[i] = log10(0.000001);//DB_MIN
                }else{
                    floats[i] = log10((float) reals[i]);
                }
            }else{
                floats[i] = (float) reals[i];
            }
        }else{
            if(hasLogaritmicValues){
                floats[i] = log10(0.000001);//DB_MIN
            }else{
                floats[i] = 0.0;
            }
        }
    }
    
}
*/
//-------------------------------------------
//void ofxAAOneVectorOutputAlgorithm::updateLogRealValues(){
//    logRealValues.resize(realValues.size());
//    for (int i=0; i<realValues.size(); ++i)
//        logRealValues[i] = amp2db(realValues[i]);
//
//}


