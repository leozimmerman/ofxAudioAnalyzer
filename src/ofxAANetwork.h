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

#include "ofxAudioAnalyzerAlgorithms.h"
#include "ofxAAValues.h"

namespace ofxaa {
    class Network {
    public:
        Network(int sampleRate, int bufferSize);
        ~Network();
        
        void computeAlgorithms(vector<Real>& signal, vector<Real>& accumulatedSignal);
        
        
        float getValue(ofxAAValue value, float smooth, bool normalized);
        float getValue(ofxAAValue value){ return getValue(value, 0.0, false); }
        
        vector<float>& getValues(ofxAABinsValue value, float smooth, bool normalized);
        vector<float>& getValues(ofxAABinsValue value){ return getValues(value, 0.0, false); }

        ofxAAOnsetsAlgorithm* getOnsetsPtr(){ return onsets;}
        ofxAABaseAlgorithm* getAlgorithmWithType(ofxaa::AlgorithmType algorithmType);
        
    private:
        
        void createAlgorithms();
        void setDefaultMaxEstimatedValues();
        void connectAlgorithms();
        void deleteAlgorithms();
        
        
        
        int _samplerate;
        int _framesize;
        
        vector<Real> _audioSignal;
        vector<Real> _accumulatedAudioSignal;
        
        vector<ofxAABaseAlgorithm*> algorithms;
        
        ofxAAOneVectorOutputAlgorithm* dcRemoval;
        ofxAAOneVectorOutputAlgorithm* windowing;
        
        ofxAAOneVectorOutputAlgorithm* equalLoudness;
        
        ofxAASingleOutputAlgorithm* rms;
        ofxAASingleOutputAlgorithm* power;
        ofxAASingleOutputAlgorithm* strongDecay;
        ofxAASingleOutputAlgorithm* zeroCrossingRate;
        ofxAASingleOutputAlgorithm* loudness;
        ofxAASingleOutputAlgorithm* loudnessVickers;
        ofxAAOneVectorOutputAlgorithm* silenceRate;
        
        ofxAAOneVectorOutputAlgorithm* centralMoments;
        ofxAASingleOutputAlgorithm* sfx_decrease;
        ofxAAOneVectorOutputAlgorithm* distributionShape;
        ofxAAOneVectorOutputAlgorithm* derivativeSFX;
        ofxAAOneVectorOutputAlgorithm* envelope;
        ofxAAOneVectorOutputAlgorithm* envelope_acummulated;
        ofxAASingleOutputAlgorithm* flatnessSFX;
        ofxAAOneVectorOutputAlgorithm* logAttackTime;
        ofxAASingleOutputAlgorithm* maxToTotal;
        ofxAASingleOutputAlgorithm* tcToTotal;
        
        ofxAAOneVectorOutputAlgorithm* spectrum;
        ofxAANSGConstantQAlgorithm* nsgConstantQ;
        ofxAATwoVectorsOutputAlgorithm* mfcc;
        ofxAAOneVectorOutputAlgorithm* melBands_centralMoments;
        ofxAAOneVectorOutputAlgorithm* melBands_distributionShape;
        ofxAASingleOutputAlgorithm* melBands_flatnessDb;
        ofxAASingleOutputAlgorithm* melBands_crest;
        
        ofxAATwoVectorsOutputAlgorithm* gfcc;
        ofxAAOneVectorOutputAlgorithm* erbBands_centralMoments;
        ofxAAOneVectorOutputAlgorithm* erbBands_distributionShape;
        ofxAASingleOutputAlgorithm* erbBands_flatnessDb;
        ofxAASingleOutputAlgorithm* erbBands_crest;
        
        ofxAAOneVectorOutputAlgorithm* barkBands;
        ofxAAOneVectorOutputAlgorithm* barkBands_centralMoments;
        ofxAAOneVectorOutputAlgorithm* barkBands_distributionShape;
        ofxAASingleOutputAlgorithm* barkBands_flatnessDb;
        ofxAASingleOutputAlgorithm* barkBands_crest;
        
        ofxAAOneVectorOutputAlgorithm* unaryOperator_square;
        ofxAASingleOutputAlgorithm* spectral_decrease;
        
        ofxAASingleOutputAlgorithm* rollOff;
        ofxAASingleOutputAlgorithm* spectral_energy;
        
        ofxAASingleOutputAlgorithm* ebr_low;
        ofxAASingleOutputAlgorithm* ebr_mid_low;
        ofxAASingleOutputAlgorithm* ebr_mid_hi;
        ofxAASingleOutputAlgorithm* ebr_hi;
        
        ofxAASingleOutputAlgorithm* hfc;
        ofxAASingleOutputAlgorithm* spectral_flux;
        ofxAASingleOutputAlgorithm* strongPeak;
        ofxAASingleOutputAlgorithm* spectralComplexity;
        ofxAASingleOutputAlgorithm* pitchSalience;
        ofxAATwoVectorsOutputAlgorithm* spectralPeaks;
        ofxAASingleOutputAlgorithm* dissonance;
        ofxAASingleOutputAlgorithm* spectral_entropy;
        ofxAASingleOutputAlgorithm* spectral_centroid;
        
        ofxAAOneVectorOutputAlgorithm* spectral_centralMoments;
        ofxAAOneVectorOutputAlgorithm* spectral_distributionShape;
        
        ofxAAOneVectorOutputAlgorithm* dynamicComplexity;
        
        ofxAATwoVectorsOutputAlgorithm* harmonicPeaks;
        ofxAASingleOutputAlgorithm* oddToEven;
        ofxAASingleOutputAlgorithm* inharmonicity;
        ofxAAOneVectorOutputAlgorithm* tristimulus;
        
        ofxAAOneVectorOutputAlgorithm* pitchYinFFT;
        ofxAATwoVectorsOutputAlgorithm* pitchMelodia;
        ofxAATwoVectorsOutputAlgorithm* predominantPitchMelodia;
        ofxAAVectorVectorOutputAlgorithm* multiPitchMelodia;
        ofxAAVectorVectorOutputAlgorithm* multiPitchKlapuri;
        
        ofxAATwoVectorsOutputAlgorithm* spectralPeaks_hpcp;
        ofxAAOneVectorOutputAlgorithm* hpcp;
        ofxAASingleOutputAlgorithm* hpcp_entropy;
        ofxAASingleOutputAlgorithm* hpcp_crest;
        ofxAATwoTypesVectorOutputAlgorithm* chordsDetection;
        
        ofxAAOnsetsAlgorithm* onsets;
        
    };
}
