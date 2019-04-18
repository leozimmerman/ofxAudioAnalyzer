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

class ofxAATwoVectorsOutputAlgorithm : public ofxAAOneVectorOutputAlgorithm{
public:
    
      ofxAATwoVectorsOutputAlgorithm(ofxaa::AlgorithmType algorithmType, int samplerate, int framesize) : ofxAAOneVectorOutputAlgorithm(algorithmType, samplerate, framesize){}
    
    ofxAATwoVectorsOutputAlgorithm(ofxaa::AlgorithmType algorithmType, int samplerate, int framesize, int outputSize_1, int outputSize_2);
    
    void castToFloat() override;
    
    void assignSecondOutpuValuesSize(int size, int val);
    
    vector<Real> realValues_2;
    
protected:
    vector<float> floatValues_2;
    vector<float> smoothedFloatValues_2;
};
