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

#pragma once

//taken from essentia/src/algorithms/tonal/multipitchklapuri.cpp

#include "ofxAudioAnalyzerAlgorithms.h"

class ofxAAMultiPitchKlapuriAlgorithm : public ofxAABaseAlgorithm{
    
public:
    
    void setup(ofxAAPitchSalienceFunctionPeaksAlgorithm* saliencePeaksPtr,
               ofxAAOneVectorOutputAlgorithm* spectrumPtr,
               int sampleRate);
    void compute();
    
    vector<float>& getPitches();
    
    int frequencyToCentBin(Real frequency);
    Real getWeight(int centBin, int harmonicNumber);
    
private:
    
    ofxAAPitchSalienceFunctionPeaksAlgorithm* _saliencePeaksAlgthm;
    ofxAAOneVectorOutputAlgorithm* _spectrumAlgthm;
    
    Real _binResolution;
    int _binsInSemitone;
    std::vector<Real> _centSpectrum;
    Real _sampleRate;
    int _binsInOctave;
    Real _referenceTerm;
    Real _referenceFrequency;
    int _numberBins;
    Real _centToHertzBase;
    int _numberHarmonicsMax;
    vector<Real> nearestBinWeights;
    
    vector<float> pitches;
    
};
