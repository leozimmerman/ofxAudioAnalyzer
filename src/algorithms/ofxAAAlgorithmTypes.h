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



enum ofxAAValueType {
    //TEMPORAL
    RMS,
    POWER,
    ZERO_CROSSING_RATE,
    LOUDNESS,
    LOUDNESS_VICKERS,
    SILENCE_RATE_20dB,
    SILENCE_RATE_30dB,
    SILENCE_RATE_60dB,
    //SFX
    DECREASE,
    DISTRIBUTION_SHAPE_KURTOSIS,
    DISTRIBUTION_SHAPE_SPREAD,
    DISTRIBUTION_SHAPE_SKEWNESS,
    LOG_ATTACK_TIME,
    STRONG_DECAY,
    FLATNESS_SFX,
    MAX_TO_TOTAL,
    TC_TO_TOTAL,
    DERIVATIVE_SFX_AFTER_MAX,
    DERIVATIVE_SFX_BEFORE_MAX,
    
    DYNAMIC_COMPLEXITY,
    
    //SPECTRAL
    SPECTRUM,
    MFCC_MEL_BANDS,
    MFCC_COEFFICIENTS,
    MEL_BANDS_KURTOSIS,
    MEL_BANDS_SPREAD,
    MEL_BANDS_SKEWNESS,
    MEL_BANDS_FLATNESS_DB,
    MEL_BANDS_CREST,
    
    GFCC_ERB_BANDS,
    GFCC_COEFFICIENTS,
    ERB_BANDS_KURTOSIS,
    ERB_BANDS_SPREAD,
    ERB_BANDS_SKEWNESS,
    ERB_BANDS_FLATNESS_DB,
    ERB_BANDS_CREST,
    
    BARK_BANDS,
    BARK_BANDS_KURTOSIS,
    BARK_BANDS_SPREAD,
    BARK_BANDS_SKEWNESS,
    BARK_BANDS_FLATNESS_DB,
    BARK_BANDS_CREST,
    
    ENERGY_BAND_LOW,
    ENERGY_BAND_MID_LOW,
    ENERGY_BAND_MID_HI,
    ENERGY_BAND_HI,
    
    SPECTRAL_KURTOSIS,
    SPECTRAL_SPREAD,
    SPECTRAL_SKEWNESS,
    SPECTRAL_DECREASE,
    SPECTRAL_ROLLOFF,
    SPECTRAL_ENERGY,
    SPECTRAL_ENTROPY,
    SPECTRAL_CENTROID,
    SPECTRAL_COMPLEXITY,
    DISSONANCE,
    HFC,
    
    TRISTIMULUS,
    INHARMONICITY,
    ODD_TO_EVEN,
    
    PITCH_SALIENCE,
    PITCH_FREQUENCY,
    PITCH_CONFIDENCE
};

namespace ofxaa {
    //TODO: Move to BaseAlgorithm class?
    enum AlgorithmType {
        ///Envelope/SFX
        DerivativeSFX,
        Envelope,
        FlatnessSFX,
        LogAttackTime,
        MaxToTotal,
        TCToTotal,
        StrongDecay,
        
        ///Filters
        DCRemoval,
        EqualLoudness,
        
        ///Standard
        UnaryOperator,
        Fft,
        Windowing,
        ZeroCrossingRate,
        
        ///SPECTRAL
        BarkBands,
        EnergyBand,

        FlatnessDB,
        Flux,
        MelBands,
        Mfcc,
        Gfcc,
        Hfc,
        RollOff,
        SpectralComplexity,
        SpectralPeaks,
        Spectrum,
        SpectrumCQ,
        StrongPeak,
        
        ///RHYTHM
        Onsets,
        OnsetDetection,
        
        ///MATH
        CartesianToPolar,
        
        ///STATISTICS
        CentralMoments,
        Centroid,
        Crest,
        Energy,
        Entropy,
        Decrease,
        DistributionShape,
        InstantPower,
        Rms,
        
        ///TONAL
        Dissonance,
        HarmonicPeaks,
        Inharmonicity,
        OddToEven,
        PitchSalience,
        Tristimulus,
        Hpcp,
        ChordsDetection,
        
        ///DURATION / SILENCE
        SilenceRate,
        
        ///LOUDNESS / DYNAMICS
        DynamicComplexity,
        LoudnessVickers,
        Loudness,
        
        ///PITCH
        PitchYinFFT,
        PitchMelodia,
        MultiPitchKlapuri,
        MultiPitchMelodia,
        PredominantPitchMelodia
        
        
    };
}




