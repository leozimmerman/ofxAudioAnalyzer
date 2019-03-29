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

#include "ofxAudioAnalyzerUtils.h"

namespace ofxaa {
    
    void initializeEssentia(){
        if (!essentia::isInitialized()){
            essentia::init();
        }
    }
    //----------------------------------------------
    void shutEssentiaFactoryDown(){
        AlgorithmFactory& factory = AlgorithmFactory::instance();
        factory.shutdown();
    }
    //----------------------------------------------
    Algorithm* createAlgorithmWithType(ofxAAAlgorithmType algorithmType, int samplerate, int framesize){
        AlgorithmFactory& factory = AlgorithmFactory::instance();
        
        switch (algorithmType) {
            case RMS:
                return factory.create("RMS");
            case ENERGY:
                return factory.create("Energy");;
            case POWER:
                return factory.create("InstantPower");
            case DC_REMOVAL:
                return factory.create("DCRemoval", "sampleRate", samplerate);
            case WINDOWING:
                return factory.create("Windowing");
            case FFT:
                return factory.create("FFT", "size", framesize);
            case CART_TO_POLAR:
                return factory.create("CartesianToPolar");
            case ONSETS_DETECTION_HFC:
                return factory.create("OnsetDetection", "method", "hfc", "sampleRate", samplerate);
            case ONSETS_DETECTION_COMPLEX:
                return factory.create("OnsetDetection", "method", "complex", "sampleRate", samplerate);
            case ONSETS_DETECTION_FLUX:
                return factory.create("OnsetDetection", "method", "flux", "sampleRate", samplerate);
            case SPECTRUM:
                return factory.create("Spectrum", "size", framesize);
            case HFC:
                return factory.create("HFC", "sampleRate", samplerate);
            case PITCH_SALIENCE:
                return factory.create("PitchSalience", "sampleRate", samplerate);
            case CENTROID:
                return factory.create("Centroid");
            case SPECTRAL_COMPLEXITY:
                return factory.create("SpectralComplexity", "sampleRate", samplerate);
            case DISSONANCE:
                return factory.create("Dissonance");
            case ROLL_OFF:
                return factory.create("RollOff", "sampleRate", samplerate);
            case ODD_TO_EVEN:
                return factory.create("OddToEvenHarmonicEnergyRatio");
            case STRONG_PEAK:
                return factory.create("StrongPeak");
            case STRONG_DECAY:
                return factory.create("StrongDecay", "sampleRate", samplerate);
            case SPECTRAL_PEAKS:
                return factory.create("SpectralPeaks", "minFrequency", 1.0);
            case MEL_BANDS:
                return factory.create("MelBands", "inputSize", framesize+1, "sampleRate", samplerate, "highFrequencyBound", samplerate/2);
            case INHARMONICITY:
                return factory.create("Inharmonicity");
            case DCT:
                return factory.create("DCT");
            case HPCP:
                return factory.create("HPCP");
            case PITCH_SALIENCE_FUNC:
                return factory.create("PitchSalienceFunction");
            case PITCH_SALIENCE_FUNC_PEAKS:
                return factory.create("PitchSalienceFunctionPeaks");
            case TRISTIMULUS:
                return factory.create("Tristimulus");
            case HARMONIC_PEAKS:
                return factory.create("HarmonicPeaks");
            case PITCH_YIN_FREQ:
                return factory.create("PitchYinFFT", "frameSize", framesize, "sampleRate", samplerate);
            case PITCH_YIN_CONFIDENCE://Duplicate
                return factory.create("PitchYinFFT", "frameSize", framesize, "sampleRate", samplerate);

            case MULTI_PITCHES:
                ///???
                break;
            case MULTI_PITCH_KLAPURI:
                ///???
                break;
            
                
            default:
                return NULL;
                break;
        }
    }
 
    //----------------------------------------------
    bool algorithmHasVectorOutput(ofxAABaseAlgorithm* algorithm){
        ofxAAOneVectorOutputAlgorithm* vectorAlgorithm = dynamic_cast<ofxAAOneVectorOutputAlgorithm*>(algorithm);
        return vectorAlgorithm != NULL;
    }
    //----------------------------------------------
    bool algorithmHasOutputInDbs(ofxAABaseAlgorithm* algorithm){
        for (auto a : algorithmsWithOutputInDbs){
            if (algorithm->getType() == a){
                return true;
            }
        }
        return false;
    }
    
    //----------------------------------------------
    bool algorithmHasNormalizedSingleOutputByDefault(ofxAABaseAlgorithm* algorithm){
        if (algorithmHasVectorOutput(algorithm)){
            return false;
        }
        for (auto a : algorithmsWithNoNormalizedSingleOutput){
            if (algorithm->getType() == a){
                return false;
            }
        }
        return true;
    }
}
