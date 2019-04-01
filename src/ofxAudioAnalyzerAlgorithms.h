/*
 * Copyright (C) 2016 Leo Zimmerman [http://www.leozimmerman.com.ar]
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

# pragma once


#include "ofxAABaseAlgorithm.h"
#include "ofxAAOneVectorOutputAlgorithm.h"
#include "ofxAAPitchSalienceFunctionPeaksAlgorithm.h"
#include "ofxAAPitchDetectAlgorithm.h"
#include "ofxAATuningFrequencyAlgorithm.h"
#include "ofxAAOnsetsAlgorithm.h"
#include "ofxAAMultiPitchKlapuriAlgorithm.h"

//---------------------------------------------------------------------
class ofxAACartToPolAlgorithm : public ofxAABaseAlgorithm{
public:
    
    ofxAACartToPolAlgorithm(ofxaa::AlgorithmType algorithmType, int samplerate, int framesize) : ofxAABaseAlgorithm(algorithmType, samplerate, framesize){}
    
    vector<Real> magnitudes;
    vector<Real> phases;
};
//---------------------------------------------------------------------
//class used for SpectralPeaks & HarmonicPeaks
class ofxAAPeaksAlgorithm : public ofxAABaseAlgorithm{
public:
    ofxAAPeaksAlgorithm(ofxaa::AlgorithmType algorithmType, int samplerate, int framesize) : ofxAABaseAlgorithm(algorithmType, samplerate, framesize){}
    
    vector<Real> frequencies;
    vector<Real> magnitudes;
};
//---------------------------------------------------------------------
class ofxAAFftAlgorithm : public ofxAAOneVectorOutputAlgorithm{
    public:
    ofxAAFftAlgorithm(ofxaa::AlgorithmType algorithmType, int samplerate, int framesize) : ofxAAOneVectorOutputAlgorithm(algorithmType, samplerate, framesize){}
    
    vector<complex<Real> > fftRealValues;
};

