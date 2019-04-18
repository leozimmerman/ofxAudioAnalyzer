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

#include "ofxAudioAnalyzerUtils.h"
#include "ofxAudioAnalyzerAlgorithms.h"

namespace ofxaa {
    class Network {
    public:
        Network(int sampleRate, int bufferSize);
        ~Network();
        
        void computeAlgorithms(vector<Real>& signal, vector<Real>& accumulatedSignal);
        
        float getValue(ofxAAValueType valueType, float smooth, bool normalized);
        vector<float>& getValues(ofxaa::AlgorithmType algorithmType, float smooth);
        int getBinsNum(ofxaa::AlgorithmType algorithmType);
        
        void resetOnsets();
        bool getOnsetValue();
        
        void setOnsetsParameters(float alpha, float silenceTresh, float timeTresh, bool useTimeTresh = true);
        
    private:
        
        void createAlgorithms();
        void setDefaultMaxEstimatedValues();
        void connectAlgorithms();
        void deleteAlgorithms();
        
        int samplerate;
        int framesize;
        
        vector<Real> _audioSignal;
        vector<Real> _accumulatedAudioSignal;
        
        vector<ofxAABaseAlgorithm*> algorithms;
        
        ofxAAOneVectorOutputAlgorithm* dcRemoval;
        ofxAAOneVectorOutputAlgorithm* windowing;
        
        ofxAAOneVectorOutputAlgorithm* equalLoudness;
        
        ofxAABaseAlgorithm* rms;
        ofxAABaseAlgorithm* power;
        ofxAABaseAlgorithm* strongDecay;
        ofxAABaseAlgorithm* zeroCrossingRate;
        ofxAABaseAlgorithm* loudness;
        ofxAABaseAlgorithm* loudnessVickers;
        ofxAAOneVectorOutputAlgorithm* silenceRate;
        
        ofxAAOneVectorOutputAlgorithm* centralMoments;
        ofxAABaseAlgorithm* sfx_decrease;
        ofxAAOneVectorOutputAlgorithm* distributionShape;
        ofxAAOneVectorOutputAlgorithm* derivativeSFX;
        ofxAAOneVectorOutputAlgorithm* envelope;
        ofxAAOneVectorOutputAlgorithm* envelope_acummulated;
        ofxAABaseAlgorithm* flatnessSFX;
        ofxAAOneVectorOutputAlgorithm* logAttackTime;
        ofxAABaseAlgorithm* maxToTotal;
        ofxAABaseAlgorithm* tcToTotal;
        
        ofxAAOneVectorOutputAlgorithm* spectrum;
        ofxAANSGConstantQAlgorithm* nsgConstantQ;
        //https://essentia.upf.edu/documentation/reference/streaming_NSGConstantQStreaming.html
        //https://github.com/MTG/essentia/blob/master/src/examples/python/NSGConstantQ-example.ipynb
        //NSGConstantQStreaming
        ofxAATwoVectorsOutputAlgorithm* mfcc;
        ofxAAOneVectorOutputAlgorithm* melBands_centralMoments;
        ofxAAOneVectorOutputAlgorithm* melBands_distributionShape;
        ofxAABaseAlgorithm* melBands_flatnessDb;
        ofxAABaseAlgorithm* melBands_crest;
        
        ofxAATwoVectorsOutputAlgorithm* gfcc;
        ofxAAOneVectorOutputAlgorithm* erbBands_centralMoments;
        ofxAAOneVectorOutputAlgorithm* erbBands_distributionShape;
        ofxAABaseAlgorithm* erbBands_flatnessDb;
        ofxAABaseAlgorithm* erbBands_crest;
        
        ofxAAOneVectorOutputAlgorithm* barkBands;
        ofxAAOneVectorOutputAlgorithm* barkBands_centralMoments;
        ofxAAOneVectorOutputAlgorithm* barkBands_distributionShape;
        ofxAABaseAlgorithm* barkBands_flatnessDb;
        ofxAABaseAlgorithm* barkBands_crest;
        
        ofxAAOneVectorOutputAlgorithm* unaryOperator_square;
        ofxAABaseAlgorithm* spectral_decrease;
        
        ofxAABaseAlgorithm* rollOff;
        ofxAABaseAlgorithm* spectral_energy;
        
        ofxAABaseAlgorithm* ebr_low;
        ofxAABaseAlgorithm* ebr_mid_low;
        ofxAABaseAlgorithm* ebr_mid_hi;
        ofxAABaseAlgorithm* ebr_hi;
        
        ofxAABaseAlgorithm* hfc;
        ofxAABaseAlgorithm* spectral_flux;
        ofxAABaseAlgorithm* strongPeak;
        ofxAABaseAlgorithm* spectralComplexity;
        ofxAABaseAlgorithm* pitchSalience;
        ofxAATwoVectorsOutputAlgorithm* spectralPeaks;
        ofxAABaseAlgorithm* dissonance;
        ofxAABaseAlgorithm* spectral_entropy;
        ofxAABaseAlgorithm* spectral_centroid;
        
        ofxAAOneVectorOutputAlgorithm* spectral_centralMoments;
        ofxAAOneVectorOutputAlgorithm* spectral_distributionShape;
        
        ofxAAOneVectorOutputAlgorithm* dynamicComplexity;
        
        ofxAATwoVectorsOutputAlgorithm* harmonicPeaks;
        ofxAABaseAlgorithm* oddToEven;
        ofxAABaseAlgorithm* inharmonicity;
        ofxAAOneVectorOutputAlgorithm* tristimulus;
        
        ofxAAOneVectorOutputAlgorithm* pitchYinFFT;
        ofxAATwoVectorsOutputAlgorithm* pitchMelodia;
        ofxAATwoVectorsOutputAlgorithm* predominantPitchMelodia;
        ofxAAVectorVectorOutputAlgorithm* multiPitchMelodia;
        ofxAAVectorVectorOutputAlgorithm* multiPitchKlapuri;
        
        ofxAATwoVectorsOutputAlgorithm* spectralPeaks_hpcp;
        ofxAAOneVectorOutputAlgorithm* hpcp;
        ofxAABaseAlgorithm* hpcp_entropy;
        ofxAABaseAlgorithm* hpcp_crest;
        ofxAATwoTypesVectorOutputAlgorithm* chordsDetection;
        
        ofxAAOnsetsAlgorithm* onsets;
        
      
        
    };
}
