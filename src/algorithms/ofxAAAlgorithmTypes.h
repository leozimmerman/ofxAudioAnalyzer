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

enum ofxAAAlgorithmType {
    DC_REMOVAL,
    WINDOW,
    FFT,
    
    RMS,
    ENERGY,
    POWER,
    
    PITCH_YIN_FREQ,
    PITCH_YIN_CONFIDENCE,
    
    PITCH_SALIENCE,
    INHARMONICITY,
    HFC,
    CENTROID,
    SPECTRAL_COMPLEXITY,
    DISSONANCE,
    ROLL_OFF,
    ODD_TO_EVEN,
    STRONG_PEAK,
    STRONG_DECAY,
    
    ONSETS,
    ONSETS_DETECTION_HFC,
    ONSETS_DETECTION_COMPLEX,
    ONSETS_DETECTION_FLUX,
    
    SPECTRUM,
    MEL_BANDS,
    //MFCC,
    HPCP,
    MULTI_PITCHES,
    PITCH_SALIENCE_FUNC_PEAKS,
    PITCH_SALIENCE_FUNC,
    TRISTIMULUS,
    
    MULTI_PITCH_KLAPURI,
    CART_TO_POLAR,
    SPECTRAL_PEAKS,
    HARMONIC_PEAKS,
    DCT
};
