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

#include "ofxAAConfigurations.h"

namespace ofxaa {
    
    void configureWindow(Algorithm* algorithm, bool normalized, int size, string type, int zeroPadding, bool zeroPhase){
        algorithm->configure("normalized", normalized, "size", size, "type", type, "zeroPadding", zeroPadding, "zeroPhase", zeroPhase);
    }
    
    void configureDCRemoval(Algorithm* algorithm, Real size){
        algorithm->configure("size", size);
    }
    
    void configureHFC(Algorithm* algorithm, string type){
        algorithm->configure("type", type);
    }
    
    void configurePitchSalience(Algorithm* algorithm, Real highBoundary, Real lowBoundary){
        algorithm->configure("highBoundary", highBoundary, "lowBoundary", lowBoundary);
    }
    
    void configureCentroid(Algorithm* algorithm, Real range){
        algorithm->configure("range", range);
    }
    
    void configureSpectralComplexity(Algorithm* algorithm, Real magnitudeThreshold){
        algorithm->configure("magnitudeThreshold", magnitudeThreshold);
    }
    
    void configureRollOff(Algorithm* algorithm, Real cutoff){
        algorithm->configure("cutoff", cutoff);
    }
    
    void configureSpectralPeaks(Algorithm* algorithm, Real magnitudeThreshold, Real maxFrequency, int maxPeaks, Real minFrequency, string orderBy){
        algorithm->configure("magnitudeThreshold", magnitudeThreshold, "maxFrequency", maxFrequency, "maxPeaks", maxPeaks, "minFrequency", minFrequency, "orderBy", orderBy);
    }
    
    void configureMelBands(Algorithm* algorithm, Real highFrequencyBound, int inputSize, bool log, Real lowFrequencyBound, string normalize, int numberBands, string type, string warpingFormula, string weighting){
        algorithm->configure("highFrequencyBound", highFrequencyBound, "inputSize", inputSize, "log", log, "lowFrequencyBound", lowFrequencyBound, "normalize", normalize, "numberBands", numberBands, "type", type, "warpingFormula", warpingFormula, "weighting", weighting);
    }
    
    void configureDCT(Algorithm* algorithm, int dctType, int inputSize, int liftering ,int outputSize){
        algorithm->configure("dctType", dctType, "inputSize", inputSize, "liftering", liftering, "outputSize", outputSize);
    }
    
    void configureHPCP(Algorithm* algorithm, bool bandPreset, Real bandSplitFrequency, int harmonics, Real maxFrequency, bool maxShifted, Real minFrequency, bool nonLinear, string normalized, Real referenceFrequency, int size, string weightType, Real windowSize){
        algorithm->configure("bandPreset", bandPreset, "bandSplitFrequency", bandSplitFrequency, "harmonics", harmonics, "maxFrequency", maxFrequency, "maxShifted", maxShifted, "minFrequency", minFrequency, "nonLinear", nonLinear, "normalized", normalized, "referenceFrequency", referenceFrequency, "size", size, "weightType", weightType, "windowSize", windowSize);
    }
    
    void configurePitchSalienceFunction(Algorithm* algorithm, Real binResolution, Real harmonicWeight, Real magnitudeCompression, Real magnitudeThreshold, Real numberHarmonics, Real referenceFrequency){
        algorithm->configure("binResolution", binResolution, "harmonicWeight", harmonicWeight, "magnitudeCompression", magnitudeCompression, "magnitudeThreshold", magnitudeThreshold, "numberHarmonics", numberHarmonics, "referenceFrequency", referenceFrequency);
    }
    
    void configurePitchSalienceFunctionPeaks(Algorithm* algorithm, Real binResolution, Real maxFrequency, Real minFrequency, Real referenceFrequency){
        algorithm->configure("binResolution", binResolution, "maxFrequency", maxFrequency, "minFrequency", minFrequency, "referenceFrequency", referenceFrequency);
    }
    
    void configureHarmonicPeaks(Algorithm* algorithm, int maxHarmonics, Real tolerance){
         algorithm->configure("maxHarmonics", maxHarmonics, "tolerance", tolerance);
    }
    
    void configurePitchYinFFT(Algorithm* algorithm, bool interpolate, Real maxFrequency, Real minFrequency){
        algorithm->configure("interpolate", interpolate, "maxFrequency", maxFrequency, "minFrequency", minFrequency);
    }
    
    void configureZeroCrossingRate(Algorithm* algorithm, Real threshold){
        algorithm->configure("threshold", threshold);
    }
    
    void configureSilenceRate(Algorithm* algorithm, vector<Real>& thresholds){
        algorithm->configure("thresholds", thresholds);
    }
    
    void configureEnvelope(Algorithm* algorithm, bool applyRectification, Real attackTime, Real releaseTime){
        algorithm->configure("applyRectification", applyRectification, "attackTime", attackTime, "releaseTime", releaseTime);
    }
    
    void configureLogAttackTime(Algorithm* algorithm, Real startAttackThreshold, Real stopAttackThreshold){
           algorithm->configure("startAttackThreshold", startAttackThreshold, "stopAttackThreshold", stopAttackThreshold);
    }
    
    void configureCentralMoments(Algorithm* algorithm, string mode, Real range){
        algorithm->configure("mode", mode, "range", range);
    }
    
    void configureDecrease(Algorithm* algorithm, Real range){
        algorithm->configure("range", range);
    }
    
    void configureUnaryOperator(Algorithm* algorithm, Real scale, Real shift, string type){
        algorithm->configure("scale", scale, "shift", shift, "type", type);
    }
    
    void configureBarkBands(Algorithm* algorithm, int numberBands){
        algorithm->configure("numberBands", numberBands);
    }
    
    void configureEnergyBand(Algorithm* algorithm, Real startCutoffFrequency, Real stopCutoffFrequency){
        algorithm->configure("startCutoffFrequency", startCutoffFrequency, "stopCutoffFrequency", stopCutoffFrequency);
    }
    
    void configureFlux(Algorithm* algorithm, bool halfRectify, string norm){
        algorithm->configure("halfRectify", halfRectify, "norm", norm);
    }
    
    void configureMFCC(Algorithm* algorithm, int dctType, Real highFrequencyBound, int inputSize, int liftering, string logType, Real lowFrequencyBound, string normalize, int numberBands, int numberCoefficients, string type, string warpingFormula, string weighting){
        algorithm->configure("dctType", dctType, "highFrequencyBound", highFrequencyBound, "inputSize", inputSize, "liftering", liftering, "logType", logType, "lowFrequencyBound", lowFrequencyBound, "normalize", normalize, "numberBands", numberBands, "numberCoefficients", numberCoefficients, "type", type, "warpingFormula", warpingFormula, "weighting", weighting);
    }
    
    void configureGFCC(Algorithm* algorithm, int dctType, Real highFrequencyBound, int inputSize, string logType, Real lowFrequencyBound, int numberBands, int numberCoefficients, string type){
              algorithm->configure("dctType", dctType, "highFrequencyBound", highFrequencyBound, "inputSize", inputSize, "logType", logType, "lowFrequencyBound", lowFrequencyBound, "numberBands", numberBands, "numberCoefficients", numberCoefficients, "type", type);
    }
    
    void configureDynamicComplexity(Algorithm* algorithm, Real frameSize){
        algorithm->configure("frameSize", frameSize);
    }
    
    void configurePitchMelodia(Algorithm* algorithm, Real binResolution, int filterIterations, bool guessUnvoiced, Real harmonicWeight, int hopSize, Real magnitudeCompression, int magnitudeThreshold, Real maxFrequency, int minDuration, Real minFrequency, int numberHarmonics, Real peakDistributionThreshold, Real peakFrameThreshold, Real pitchContinuity, Real referenceFrequency, int timeContinuity){
        algorithm->configure("binResolution", binResolution, "filterIterations", filterIterations, "guessUnvoiced", guessUnvoiced, "harmonicWeight", harmonicWeight, "hopSize", hopSize, "magnitudeCompression", magnitudeCompression, "magnitudeThreshold", magnitudeThreshold, "maxFrequency", maxFrequency, "minDuration", minDuration, "minFrequency", minFrequency, "numberHarmonics", numberHarmonics, "peakDistributionThreshold", peakDistributionThreshold, "peakFrameThreshold", peakFrameThreshold, "pitchContinuity", pitchContinuity, "referenceFrequency", referenceFrequency, "timeContinuity", timeContinuity);
    }
    
    void configureMultiPitchMelodia(Algorithm* algorithm, Real binResolution, int filterIterations, bool guessUnvoiced, Real harmonicWeight, int hopSize, Real magnitudeCompression, int magnitudeThreshold, Real maxFrequency, int minDuration, Real minFrequency, int numberHarmonics, Real peakDistributionThreshold, Real peakFrameThreshold, Real pitchContinuity, Real referenceFrequency, int timeContinuity){
        algorithm->configure("binResolution", binResolution, "filterIterations", filterIterations, "guessUnvoiced", guessUnvoiced, "harmonicWeight", harmonicWeight, "hopSize", hopSize, "magnitudeCompression", magnitudeCompression, "magnitudeThreshold", magnitudeThreshold, "maxFrequency", maxFrequency, "minDuration", minDuration, "minFrequency", minFrequency, "numberHarmonics", numberHarmonics, "peakDistributionThreshold", peakDistributionThreshold, "peakFrameThreshold", peakFrameThreshold, "pitchContinuity", pitchContinuity, "referenceFrequency", referenceFrequency, "timeContinuity", timeContinuity);
    }
    
    void configurePredominantPitchMelodia(Algorithm* algorithm, Real binResolution, int filterIterations, bool guessUnvoiced, Real harmonicWeight, int hopSize, Real magnitudeCompression, int magnitudeThreshold, Real maxFrequency, int minDuration, Real minFrequency, int numberHarmonics, Real peakDistributionThreshold, Real peakFrameThreshold, Real pitchContinuity, Real referenceFrequency, int timeContinuity, bool voiceVibrato, Real voicingTolerance){
         algorithm->configure("binResolution", binResolution, "filterIterations", filterIterations, "guessUnvoiced", guessUnvoiced, "harmonicWeight", harmonicWeight, "hopSize", hopSize, "magnitudeCompression", magnitudeCompression, "magnitudeThreshold", magnitudeThreshold, "maxFrequency", maxFrequency, "minDuration", minDuration, "minFrequency", minFrequency, "numberHarmonics", numberHarmonics, "peakDistributionThreshold", peakDistributionThreshold, "peakFrameThreshold", peakFrameThreshold, "pitchContinuity", pitchContinuity, "referenceFrequency", referenceFrequency, "timeContinuity", timeContinuity);
        algorithm->configure("voiceVibrato", voiceVibrato, "voicingTolerance", voicingTolerance);
    }
   
    void configureMultiPitchKlapuri(Algorithm* algorithm, Real binResolution, Real harmonicWeight, int hopSize, Real magnitudeCompression, int magnitudeThreshold, Real maxFrequency, Real minFrequency, int numberHarmonics, Real referenceFrequency){
         algorithm->configure("binResolution", binResolution, "harmonicWeight", harmonicWeight, "hopSize", hopSize, "magnitudeCompression", magnitudeCompression, "magnitudeThreshold", magnitudeThreshold, "maxFrequency", maxFrequency, "minFrequency", minFrequency, "numberHarmonics", numberHarmonics, "referenceFrequency", referenceFrequency);
    }
    
    void configureSpectrumCQ(Algorithm* algorithm, int binsPerOctave, Real maxFrequency, Real minFrequency, Real threshold){
         algorithm->configure("binsPerOctave", binsPerOctave, "maxFrequency", maxFrequency, "minFrequency", minFrequency, "threshold", threshold);
    }
    
    void configureChordsDetection(Algorithm* algorithm, int hopSize, Real windowSize){
        algorithm->configure("hopSize", hopSize, "windowSize", windowSize);
    }
    
    void configureOnsetDetection(Algorithm* algorithm, string method){
        algorithm->configure("method", method);
    }
}
