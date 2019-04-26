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

enum ofxAAValue {
    //TEMPORAL
    RMS,
    POWER,
    ZERO_CROSSING_RATE,
    LOUDNESS,
//    LOUDNESS_VICKERS, //only at 44100 sr
    SILENCE_RATE_20dB,
    SILENCE_RATE_30dB,
    SILENCE_RATE_60dB,
    //SFX
    DYNAMIC_COMPLEXITY,
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
    
    //SPECTRAL
    MEL_BANDS_KURTOSIS,
    MEL_BANDS_SPREAD,
    MEL_BANDS_SKEWNESS,
    MEL_BANDS_FLATNESS_DB,
    MEL_BANDS_CREST,
    
    ERB_BANDS_KURTOSIS,
    ERB_BANDS_SPREAD,
    ERB_BANDS_SKEWNESS,
    ERB_BANDS_FLATNESS_DB,
    ERB_BANDS_CREST,
    
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
    SPECTRAL_FLUX,
    DISSONANCE,
    HFC,
    PITCH_SALIENCE,
    
    INHARMONICITY,
    ODD_TO_EVEN,
    STRONG_PEAK,
    
    HPCP_CREST,
    HPCP_ENTROPY,
    
    PITCH_YIN_FREQUENCY,
    PITCH_YIN_CONFIDENCE,
    
    ONSETS,
    NONE
};

enum ofxAABinsValue {
    SPECTRUM,
    MFCC_MEL_BANDS,
    GFCC_ERB_BANDS,
    BARK_BANDS,
    
    TRISTIMULUS,
    HPCP,
    
    PITCH_MELODIA_FREQUENCIES,
    PITCH_MELODIA_CONFIDENCES,
    PREDOMINANT_PITCH_MELODIA_FREQUENCIES,
    PREDOMINANT_PITCH_MELODIA_CONFIDENCES,
    NONE_BINS
};

enum ofxAAOtherValue {
    MULTI_PITCH_KLAPURI_FREQUENCIES,
    NSG_CONSTANT_Q,
    CHORDS_DETECTION
};


