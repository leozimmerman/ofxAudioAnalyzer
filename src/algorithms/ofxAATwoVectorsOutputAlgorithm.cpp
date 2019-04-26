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
    _smoothedValuesNormalized_2.assign(size, val);
    _smoothedValues_2.assign(size, val);
    _linearValues_2.assign(size, val);
    _normalizedValues_2.assign(size, val);
}

void ofxAATwoVectorsOutputAlgorithm::checkInternalValuesSizes() {
    ofxAAOneVectorOutputAlgorithm::checkInternalValuesSizes();
    auto size = outputValues_2.size();
    float val = 0.0;
    if (size != _normalizedValues_2.size()){
        _smoothedValuesNormalized_2.assign(size, val);
        _smoothedValues_2.assign(size, val);
        _linearValues_2.assign(size, val);
        _normalizedValues_2.assign(size, val);
    }
}

vector<float>& ofxAATwoVectorsOutputAlgorithm::getValues2(float smooth, bool normalized){
    checkInternalValuesSizes();
    
    if (normalized){
        normalizeValues(outputValues_2, _normalizedValues_2);
        smoothValues(_normalizedValues_2, _smoothedValuesNormalized_2, smooth);
        return _smoothedValuesNormalized_2;
    } else {
        linValues(outputValues_2, _linearValues_2);
        smoothValues(_linearValues_2, _smoothedValues_2, smooth);
        return _smoothedValues_2;
    }
}


