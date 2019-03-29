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

#include "ofMain.h"
#include "ofxAudioAnalyzerUtils.h"
#include "ofxAudioAnalyzerAlgorithms.h"

namespace ofxaa {
    class Network {
    public:
        Network(int sampleRate, int bufferSize);
        ~Network();
        
        void computeAlgorithms(vector<Real>& signal);
        
        float getValue(ofxAAAlgorithmType algorithmType, float smooth, bool normalized);
        
        vector<float>& getValues(ofxAAAlgorithmType algorithmType, float smooth);
        
        int getBinsNum(ofxAAAlgorithmType algorithmType);
        
        void resetOnsets();
        bool getOnsetValue();
        //vector<SalienceFunctionPeak>& getPitchSaliencePeaksRef(float smooth=0.0);
        //void setSalienceFunctionPeaksParameters(int maxPeaks);
        void setOnsetsParameters(float alpha, float silenceTresh, float timeTresh, bool useTimeTresh = true);
        
        
    private:
        void createAlgorithms();
        void setDefaultMaxEstimatedValues();
        void connectAlgorithms();
        void deleteAlgorithms();
        
        vector<ofxAABaseAlgorithm*> algorithms;
        vector<ofxAAOneVectorOutputAlgorithm*> vectorAlgorithms;
        
        ofxAABaseAlgorithm* rms;
        ofxAABaseAlgorithm* energy;
        ofxAABaseAlgorithm* power;
        ofxAAPitchDetectAlgorithm* pitchDetect;
        ofxAABaseAlgorithm* pitchSalience;
        //ofxAATuningFrequencyAlgorithm tuning;
        ofxAABaseAlgorithm* inharmonicity;
        ofxAABaseAlgorithm* hfc;
        ofxAABaseAlgorithm* centroid;
        ofxAABaseAlgorithm* spectralComplexity;
        ofxAABaseAlgorithm* dissonance;
        ofxAABaseAlgorithm* rollOff;
        ofxAABaseAlgorithm* oddToEven;
        ofxAABaseAlgorithm* strongPeak;
        ofxAABaseAlgorithm* strongDecay;
        
        ofxAAOneVectorOutputAlgorithm* spectrum;
        ofxAAOneVectorOutputAlgorithm* melBands;
        ofxAAOneVectorOutputAlgorithm* dct;//MFCC
        ofxAAOneVectorOutputAlgorithm* hpcp;
        ofxAAOneVectorOutputAlgorithm* tristimulus;
        //MultiPitch
        ofxAAPitchSalienceFunctionPeaksAlgorithm* pitchSalienceFunctionPeaks;
        ofxAAMultiPitchKlapuriAlgorithm* multiPitchKlapuri;
        
        //Algorithms for internal functionality:
        ofxAAOneVectorOutputAlgorithm* dcRemoval;
        ofxAAOneVectorOutputAlgorithm* windowing;
        ofxAAFftAlgorithm* fft;
        ofxAACartToPolAlgorithm* cartesian2polar;
        ofxAAPeaksAlgorithm* spectralPeaks;
        ofxAAPeaksAlgorithm* harmonicPeaks;
        ofxAAOneVectorOutputAlgorithm* pitchSalienceFunction;
        ofxAAOnsetsAlgorithm* onsets;
        
        int _samplerate;
        int _framesize;
        
        vector<Real> _audioSignal;
        
    };
}
