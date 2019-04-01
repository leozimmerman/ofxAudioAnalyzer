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

namespace ofxaa {
    
    /// https://essentia.upf.edu/documentation/reference/std_Windowing.html
    ///\param normalized (bool ∈ {true, false}, default = true) : a boolean value to specify whether to normalize windows (to have an area of 1) and then scale by a factor of 2
    ///\param size (integer ∈ [2, ∞), default = 1024)
    ///\param type (string ∈ {hamming, hann, hannnsgcq, triangular, square, blackmanharris62, blackmanharris70, blackmanharris74, blackmanharris92}, default = hann)
    ///\param zeroPadding (integer ∈ [0, ∞), default = 0)
    ///\param zeroPhase (bool ∈ {true, false}, default = true) : a boolean value that enables zero-phase windowing
    void configureWindow(Algorithm* algorithm, bool normalized, int size, string type, int zeroPadding, bool zeroPhase);
    
    ///https://essentia.upf.edu/documentation/reference/std_DCRemoval.html
    ///\param cutoffFrequency (real ∈ (0, ∞), default = 40)
    void configureDCRemoval(Algorithm* algorithm, Real size);
    
    ///https://essentia.upf.edu/documentation/reference/std_HFC.html
    ///\param type (string ∈ {Masri, Jensen, Brossier}, default = Masri) : the type of HFC coefficient to be computed
    void configureHFC(Algorithm* algorithm, string type);
    
    ///https://essentia.upf.edu/documentation/reference/std_PitchSalience.html
    ///\param highBoundary (real ∈ (0, ∞), default = 5000) : until which frequency we are looking for the minimum (must be smaller than half sampleRate) [Hz]
    ///\param lowBoundary (real ∈ (0, ∞), default = 100) : from which frequency we are looking for the maximum (must not be larger than highBoundary) [Hz]
    void configurePitchSalience(Algorithm* algorithm, Real highBoundary, Real lowBoundary);
    
    ///https://essentia.upf.edu/documentation/reference/std_Centroid.html
    ///\param range (real ∈ (0, ∞), default = 1) : the range of the input array, used for normalizing the results
    void configureCentroid(Algorithm* algorithm, Real range);
    
    ///https://essentia.upf.edu/documentation/reference/std_SpectralComplexity.html
    ///\param magnitudeThreshold (real ∈ [0, ∞), default = 0.005) : the minimum spectral-peak magnitude that contributes to spectral complexity
    void configureSpectralComplexity(Algorithm* algorithm, Real magnitudeThreshold);
    
    ///https://essentia.upf.edu/documentation/reference/std_RollOff.html
    ///\param cutoff (real ∈ (0, 1), default = 0.85) : the ratio of total energy to attain before yielding the roll-off frequency
    void configureRollOff(Algorithm* algorithm, Real cutoff);
    
    ///https://essentia.upf.edu/documentation/reference/std_SpectralPeaks.html
    ///\param magnitudeThreshold (real ∈ (-∞, ∞), default = 0) : peaks below this given threshold are not outputted
    ///\param maxFrequency (real ∈ (0, ∞), default = 5000) : the maximum frequency of the range to evaluate [Hz]
    ///\param maxPeaks (integer ∈ [1, ∞), default = 100) : the maximum number of returned peaks
    ///\param minFrequency (real ∈ [0, ∞), default = 0) : the minimum frequency of the range to evaluate [Hz]
    ///\param orderBy (string ∈ {frequency, magnitude}, default = frequency) : the ordering type of the outputted peaks (ascending by frequency or descending by magnitude)
    void configureSpectralPeaks(Algorithm* algorithm, Real magnitudeThreshold, Real maxFrequency, int maxPeaks, Real minFrequency, string orderBy);
    
    ///https://essentia.upf.edu/documentation/reference/std_MelBands.html
    ///\param highFrequencyBound (real ∈ [0, ∞), default = 22050) : an upper-bound limit for the frequencies to be included in the bands
    ///\param inputSize (integer ∈ (1, ∞), default = 1025) : the size of the spectrum
    ///\param log (bool ∈ {true, false}, default = false) : compute log-energies (log10 (1 + energy))
    ///\param lowFrequencyBound (real ∈ [0, ∞), default = 0) : a lower-bound limit for the frequencies to be included in the bands
    ///\param normalize (string ∈ {unit_sum, unit_max}, default = unit_sum) : 'unit_max' makes the vertex of all the triangles equal to 1, 'unit_sum' makes the area of all the triangles equal to 1
    ///\param numberBands (integer ∈ (1, ∞), default = 24) : the number of output bands
    ///\param type (string ∈ {magnitude, power}, default = power) : 'power' to output squared units, 'magnitude' to keep it as the input
    ///\param warpingFormula (string ∈ {slaneyMel, htkMel}, default = slaneyMel) : The scale implementation type. use 'htkMel' to emulate its behaviour. Default slaneyMel.
    ///\param weighting (string ∈ {warping, linear}, default = warping) : type of weighting function for determining triangle area
    void configureMelBands(Algorithm* algorithm, Real highFrequencyBound, int inputSize, bool log, Real lowFrequencyBound, string normalize, int numberBands, string type, string warpingFormula, string weighting);
    
    ///https://essentia.upf.edu/documentation/reference/std_DCT.html
    ///\param dctType (integer ∈ [2, 3], default = 2) : the DCT type
    ///\param inputSize (integer ∈ [1, ∞), default = 10) : the size of the input array
    ///\param liftering (integer ∈ [0, ∞), default = 0) : the liftering coefficient. Use '0' to bypass it
    ///\param outputSize (integer ∈ [1, ∞), default = 10) : the number of output coefficients
    void configureDCT(Algorithm* algorithm, int dctType, int inputSize, int liftering ,int outputSize);
    
    ///https://essentia.upf.edu/documentation/reference/std_HPCP.html
    ///\param bandPreset (bool ∈ {true, false}, default = true) : enables whether to use a band preset
    ///\param bandSplitFrequency (real ∈ (0, ∞), default = 500) : the split frequency for low and high bands, not used if bandPreset is false [Hz]
    ///\param harmonics (integer ∈ [0, ∞), default = 0) : number of harmonics for frequency contribution, 0 indicates exclusive fundamental frequency contribution
    ///\param maxFrequency (real ∈ (0, ∞), default = 5000) : the maximum frequency that contributes to the HPCP [Hz] (the difference between the max and split frequencies must not be less than 200.0 Hz)
    ///\param maxShifted (bool ∈ {true, false}, default = false) : whether to shift the HPCP vector so that the maximum peak is at index 0
    ///\param minFrequency (real ∈ (0, ∞), default = 40) : the minimum frequency that contributes to the HPCP [Hz] (the difference between the min and split frequencies must not be less than 200.0 Hz)
    ///\param nonLinear (bool ∈ {true, false}, default = false) : apply non-linear post-processing to the output (use with normalized='unitMax'). Boosts values close to 1, decreases values close to 0.
    ///\param normalized (string ∈ {none, unitSum, unitMax}, default = unitMax) : whether to normalize the HPCP vector
    ///\param referenceFrequency (real ∈ (0, ∞), default = 440) : the reference frequency for semitone index calculation, corresponding to A3 [Hz]
    ///\param size (integer ∈ [12, ∞), default = 12) : the size of the output HPCP (must be a positive nonzero multiple of 12)
    ///\param weightType (string ∈ {none, cosine, squaredCosine}, default = squaredCosine) : type of weighting function for determining frequency contribution
    ///\param windowSize (real ∈ (0, 12], default = 1) : the size, in semitones, of the window used for the weighting
    void configureHPCP(Algorithm* algorithm, bool bandPreset, Real bandSplitFrequency, int harmonics, Real maxFrequency, bool maxShifted, Real minFrequency, bool nonLinear, string normalized, Real referenceFrequency, int size, string weightType, Real windowSize);
    
    ///https://essentia.upf.edu/documentation/reference/std_PitchSalienceFunction.html
    ///\param binResolution (real ∈ (0, ∞), default = 10) : salience function bin resolution [cents]
    ///\param harmonicWeight (real ∈ (0, 1), default = 0.8) : harmonic weighting parameter (weight decay ratio between two consequent harmonics, =1 for no decay)
    ///\param magnitudeCompression (real ∈ (0, 1], default = 1) : magnitude compression parameter (=0 for maximum compression, =1 for no compression)
    ///\param magnitudeThreshold (real ∈ [0, ∞), default = 40) : peak magnitude threshold (maximum allowed difference from the highest peak in dBs)
    ///\param numberHarmonics (integer ∈ [1, ∞), default = 20) : number of considered harmonics
    ///\param referenceFrequency (real ∈ (0, ∞), default = 55) : the reference frequency for Hertz to cent convertion [Hz], corresponding to the 0th cent bin
    void configurePitchSalienceFunction(Algorithm* algorithm, Real binResolution, Real harmonicWeight, Real magnitudeCompression, Real magnitudeThreshold, Real numberHarmonics, Real referenceFrequency);
    
    ///https://essentia.upf.edu/documentation/reference/std_PitchSalienceFunctionPeaks.html
    ///\param binResolution (real ∈ (0, ∞), default = 10) : salience function bin resolution [cents]
    ///\param maxFrequency (real ∈ [0, ∞), default = 1760) : the maximum frequency to evaluate (ignore peaks above) [Hz]
    ///\param minFrequency (real ∈ [0, ∞), default = 55) : the minimum frequency to evaluate (ignore peaks below) [Hz]
    ///\param referenceFrequency (real ∈ (0, ∞), default = 55) : the reference frequency for Hertz to cent convertion [Hz], corresponding to the 0th cent bin
    void configurePitchSalienceFunctionPeaks(Algorithm* algorithm, Real binResolution, Real maxFrequency, Real minFrequency, Real referenceFrequency);
    
    ///https://essentia.upf.edu/documentation/reference/std_HarmonicPeaks.html
    ///\param maxHarmonics (integer ∈ [1, ∞), default = 20) : the number of harmonics to return including F0
    ///\param tolerance (real ∈ (0, 0.5), default = 0.2) : the allowed ratio deviation from ideal harmonics
    void configureHarmonicPeaks(Algorithm* algorithm, int maxHarmonics, Real tolerance);
    
    ///https://essentia.upf.edu/documentation/reference/std_PitchYinFFT.html
    ///\param interpolate (bool ∈ {true, false}, default = true) : boolean flag to enable interpolation
    ///\param maxFrequency (real ∈ (0, ∞), default = 22050) : the maximum allowed frequency [Hz]
    ///\param minFrequency (real ∈ (0, ∞), default = 20) : the minimum allowed frequency [Hz]
    void configurePitchYinFFT(Algorithm* algorithm, bool interpolate, Real maxFrequency, Real minFrequency);
    
    ///https://essentia.upf.edu/documentation/reference/std_ZeroCrossingRate.html
    ///\param threshold (real ∈ [0, ∞], default = 0) : the threshold which will be taken as the zero axis in both positive and negative sign
    void configureZeroCrossingRate(Algorithm* algorithm, Real threshold);
    
    ///https://essentia.upf.edu/documentation/reference/std_SilenceRate.html
    ///\param thresholds (vector_real, default = []) : the threshold values
    void configureSilenceRate(Algorithm* algorithm, vector<Real>& thresholds);
    
    ///https://essentia.upf.edu/documentation/reference/std_Envelope.html
    ///\param applyRectification (bool ∈ {true, false}, default = true) : whether to apply rectification (envelope based on the absolute value of signal)
    ///\param attackTime (real ∈ [0, ∞), default = 10) : the attack time of the first order lowpass in the attack phase [ms]
    ///\param releaseTime (real ∈ [0, ∞), default = 1500) : the release time of the first order lowpass in the release phase [ms]
    void configureEnvelope(Algorithm* algorithm, bool applyRectification, Real attackTime, Real releaseTime);
    
    ///https://essentia.upf.edu/documentation/reference/std_LogAttackTime.html
    ///\param startAttackThreshold (real ∈ [0, 1], default = 0.2) : the percentage of the input signal envelope at which the starting point of the attack is considered
    ///\param stopAttackThreshold (real ∈ [0, 1], default = 0.9) : the percentage of the input signal envelope at which the ending point of the attack is considered
    void configureLogAttackTime(Algorithm* algorithm, Real startAttackThreshold, Real stopAttackThreshold);
    
    ///https://essentia.upf.edu/documentation/reference/std_CentralMoments.html
    ///\param mode (string ∈ {pdf, sample}, default = pdf) : compute central moments considering array values as a probability density function over array index or as sample points of a distribution
    ///\param range (real ∈ (0, ∞), default = 1) : the range of the input array, used for normalizing the results in the 'pdf' mode
    void configureCentralMoments(Algorithm* algorithm, string mode, Real range);
    
    ///https://essentia.upf.edu/documentation/reference/std_Decrease.html
    ///\param range (real ∈ (-∞, ∞), default = 1) : the range of the input array, used for normalizing the results
    void configureDecrease(Algorithm* algorithm, Real range);
    
    
    
    
}
