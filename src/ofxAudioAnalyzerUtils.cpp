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
    Algorithm* createAlgorithmWithType(ofxaa::AlgorithmType algorithmType, int samplerate, int framesize){
        Real thresholds_dB[] = { -20, -30, -60 };
        vector<Real> silenceRateThresholds(ARRAY_SIZE(thresholds_dB));
        for (int i=0; i<(int)silenceRateThresholds.size(); i++) {
            silenceRateThresholds[i] = db2lin(thresholds_dB[i]/2.0);
        }
        
        AlgorithmFactory& factory = AlgorithmFactory::instance();
        
        switch (algorithmType) {
            case Windowing:
                return factory.create("Windowing");
            case DCRemoval:
                return factory.create("DCRemoval", "sampleRate", samplerate);
            case Rms:
                return factory.create("RMS");
            case InstantPower:
                return factory.create("InstantPower");
            case StrongDecay:
                return factory.create("StrongDecay", "sampleRate", samplerate);
            case ZeroCrossingRate:
                return factory.create("ZeroCrossingRate");
            case LoudnessVickers:
                return factory.create("LoudnessVickers", "sampleRate", samplerate);
            case Loudness:
                return factory.create("Loudness");
            case SilenceRate:
                return factory.create("SilenceRate", "thresholds", silenceRateThresholds);
            case CentralMoments:
                return factory.create("CentralMoments");
            case Centroid:
                return factory.create("Centroid");
            case Decrease:
                return factory.create("Decrease");
            case DistributionShape:
                return factory.create("DistributionShape");
            case DerivativeSFX:
                return factory.create("DerivativeSFX");
            case Envelope:
                return factory.create("Envelope", "sampleRate", samplerate);
            case FlatnessSFX:
                return factory.create("FlatnessSFX");
            case LogAttackTime:
                return factory.create("LogAttackTime", "sampleRate", samplerate);
            case MaxToTotal:
                return factory.create("MaxToTotal");
            case TCToTotal:
                return factory.create("TCToTotal");
                
            case Spectrum:
                return factory.create("Spectrum", "size", framesize);
            case SpectrumCQ:
                return factory.create("SpectrumCQ",  "sampleRate", samplerate);
            case SpectralComplexity:
                return factory.create("SpectralComplexity", "sampleRate", samplerate);
            case StrongPeak:
                return factory.create("StrongPeak");
            case MelBands:
                return factory.create("MelBands", "sampleRate", samplerate, "inputSize", (framesize/2)+1, "highFrequencyBound", samplerate/2);
            case Hfc:
                return factory.create("HFC", "sampleRate", samplerate);
            case RollOff:
                return factory.create("RollOff", "sampleRate", samplerate);
            case Energy:
                return factory.create("Energy");
            case Dissonance:
                return factory.create("Dissonance");
            case PitchSalience:
                return factory.create("PitchSalience", "sampleRate", samplerate);
            
            case UnaryOperator:
                return factory.create("UnaryOperator", "type", "square");
            case BarkBands:
                return factory.create("BarkBands", "sampleRate", samplerate);
            case EnergyBand:
                return factory.create("EnergyBand", "sampleRate", samplerate);
            case FlatnessDB:
                return factory.create("FlatnessDB");
            case Flux:
                return factory.create("Flux");
            case Mfcc:
                return factory.create("MFCC", "sampleRate", samplerate, "inputSize", (framesize/2)+1, "highFrequencyBound", samplerate/4);
            case Gfcc:
                
                return factory.create("GFCC", "sampleRate", samplerate, "inputSize", (framesize/2)+1, "highFrequencyBound", samplerate/2);
            case Crest:
                return factory.create("Crest");
            case Entropy:
                return factory.create("Entropy");
            case DynamicComplexity:
                return factory.create("DynamicComplexity", "sampleRate", samplerate);
            case SpectralPeaks:
                return factory.create("SpectralPeaks", "sampleRate", samplerate, "minFrequency", 1.0);
            case HarmonicPeaks:
                return factory.create("HarmonicPeaks");
            case OddToEven:
                return factory.create("OddToEvenHarmonicEnergyRatio");
            case Inharmonicity:
                return factory.create("Inharmonicity");
            case Tristimulus:
                return factory.create("Tristimulus");
            
            case PitchYinFFT:
                return factory.create("PitchYinFFT", "sampleRate", samplerate, "frameSize", framesize);
            case PitchMelodia:
                return factory.create("PitchMelodia", "sampleRate", samplerate, "frameSize", framesize, "hopSize", framesize/16);
            case MultiPitchKlapuri:
                return factory.create("MultiPitchKlapuri", "sampleRate", samplerate, "frameSize", framesize, "hopSize", framesize/16);
            case MultiPitchMelodia:
                return factory.create("MultiPitchMelodia", "sampleRate", samplerate, "frameSize", framesize, "hopSize", framesize/16);
            case PredominantPitchMelodia:
                return factory.create("PredominantPitchMelodia", "sampleRate", samplerate, "frameSize", framesize, "hopSize", framesize/16);
            case EqualLoudness:
                return factory.create("EqualLoudness", "sampleRate", samplerate);
            case Hpcp:
                return factory.create("HPCP", "sampleRate", samplerate, "bandPreset", false, "size", 36);
            case ChordsDetection:
                return factory.create("ChordsDetection", "sampleRate", samplerate);
            
            case CartesianToPolar:
                return factory.create("CartesianToPolar");
                
            case Fft:
                return factory.create("FFT", "size", framesize);
            case OnsetDetection:
                return factory.create("OnsetDetection", "sampleRate", samplerate);
            
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
