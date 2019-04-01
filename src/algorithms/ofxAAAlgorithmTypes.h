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
//TODO: Differentiate algorithm types (will match essentia algorithms) and getValue types (may be more than one for each alg type)

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
    DERIVATIVE_SFX_BEFORE_MAX
};

namespace ofxaa {
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
        
        ///Standard
        FFT,
        WINDOWING,
        ZeroCrossingRate,
        
        ///SPECTRAL
        HFC,
        DCT,
        MEL_BANDS,
        ROLL_OFF,
        SPECTRAL_COMPLEXITY,
        SPECTRAL_PEAKS,
        SPECTRUM,
        STRONG_PEAK,
        
        ///RHYTHM
        ONSETS,
        ONSETS_DETECTION_HFC,
        ONSETS_DETECTION_COMPLEX,
        ONSETS_DETECTION_FLUX,
        
        ///MATH
        CART_TO_POLAR,
        
        ///STATISTICS
        CentralMoments,
        Centroid,
        ENERGY,
        Decrease,
        DistributionShape,
        InstantPower,
        Rms,
        
        ///TONAL
        DISSONANCE,
        HPCP,
        HARMONIC_PEAKS,
        INHARMONICITY,
        ODD_TO_EVEN,
        PITCH_SALIENCE,
        TRISTIMULUS,
        
        ///DURATION / SILENCE
        SilenceRate,
        
        ///LOUDNESS / DYNAMICS
        LoudnessVickers,
        Loudness,
        
        ///PITCH
        PITCH_YIN_FREQ,
        PITCH_YIN_CONFIDENCE,
        MULTI_PITCHES,
        PITCH_SALIENCE_FUNC_PEAKS,
        PITCH_SALIENCE_FUNC,
        MULTI_PITCH_KLAPURI,
        
    };
}




