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
        NSGConstantQ,
        
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




