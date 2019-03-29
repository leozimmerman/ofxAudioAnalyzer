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

enum ofxAAAlgorithmType {
    ///Envelope/SFX
    STRONG_DECAY,
    
    ///Filters
    DC_REMOVAL,
    
    ///Standard
    FFT,
    WINDOWING,
    
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
    CENTROID,
    ENERGY,
    POWER,
    RMS,
    
    ///TONAL
    DISSONANCE,
    HPCP,
    HARMONIC_PEAKS,
    INHARMONICITY,
    ODD_TO_EVEN,
    PITCH_SALIENCE,
    TRISTIMULUS,
    
    ///LOUDNESS / DYNAMICS
    
    ///PITCH
    PITCH_YIN_FREQ,
    PITCH_YIN_CONFIDENCE,
    MULTI_PITCHES,
    PITCH_SALIENCE_FUNC_PEAKS,
    PITCH_SALIENCE_FUNC,
    MULTI_PITCH_KLAPURI,
    
};


