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

#include "ofxAASingleOutputAlgorithm.h"

ofxAASingleOutputAlgorithm::ofxAASingleOutputAlgorithm(ofxaa::AlgorithmType algorithmType, int samplerate, int framesize) : ofxAABaseAlgorithm(algorithmType, samplerate, framesize) {
    _smoothedValue = 0.0;
    _smoothedNormValue = 0.0;
}

//-------------------------------------------
void ofxAASingleOutputAlgorithm::compute(){
    ofxAABaseAlgorithm::compute();
    if (!isActive){
         outputValue = 0.0;
    }
}
//-------------------------------------------
float ofxAASingleOutputAlgorithm::getValue(float smooth, bool normalized){
    if (normalized){
        float normValue = normalizedValue();
        smoothValue(normValue, _smoothedNormValue, smooth);
        return _smoothedNormValue;
    } else {
        float linValue = linearValue();
        smoothValue(linValue, _smoothedValue, smooth);
        return _smoothedValue;
    }
}
//-------------------------------------------
float ofxAASingleOutputAlgorithm::linearValue(){
    if (hasLogarithmicValues){
        /*
        lin2db-> 0.001 = -30
        lin2db-> 1.5 = 17.6
        lin2db-> 1 = 0
        lin2db-> -0.001 = No existe
        */
        float dbMax = lin2db(maxEstimatedValue);
        return ofMap(lin2db(outputValue), DB_MIN, dbMax, 0.0, 1.0, TRUE);
    } else {
        return outputValue;
    }
}
//-------------------------------------------
float ofxAASingleOutputAlgorithm::normalizedValue(){
    if (isNormalizedByDefault || hasLogarithmicValues) {
        return linearValue();
    } else if (hasDbValues){
        float dbMax = 0.0;
        return ofMap(outputValue, dbSilenceCutoff, dbMax, 0.0, 1.0, TRUE);
    } else {
        return ofMap(outputValue, minEstimatedValue, maxEstimatedValue, 0.0, 1.0, TRUE);
    }
}
//-------------------------------------------
void ofxAASingleOutputAlgorithm::smoothValue(float& valueToSmooth, float& smoothedValue, float smthAmnt){
    if (smthAmnt == 0){
        smoothedValue = valueToSmooth;
    }
    smoothedValue = smooth(valueToSmooth, smoothedValue, smthAmnt);
}


