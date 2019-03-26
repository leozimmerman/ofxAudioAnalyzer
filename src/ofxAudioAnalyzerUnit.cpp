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

#include "ofxAudioAnalyzerUnit.h"
#include "ofxAudioAnalyzerUtils.h"

#pragma mark - Main funcs

ofxAudioAnalyzerUnit::ofxAudioAnalyzerUnit(int sampleRate, int bufferSize)
//cartesian2polar(CART_TO_POLAR, sampleRate, bufferSize),
//onsets(ONSETS, sampleRate, bufferSize),
//pitchDetect(PITCH_YIN_FREQ, sampleRate, bufferSize),
//fft(FFT, sampleRate, bufferSize),
//spectralPeaks(SPECTRAL_PEAKS, sampleRate, bufferSize),
//harmonicPeaks(HARMONIC_PEAKS, sampleRate, bufferSize),
//pitchSalienceFunctionPeaks(PITCH_SALIENCE_FUNC_PEAKS, sampleRate, bufferSize) {
    {
    _framesize = bufferSize;
    _samplerate = sampleRate;
    
    audioBuffer.resize(bufferSize);
    
    createAlgorithms();
    connectAlgorithms();
    
    //------Not very useful...
    /**
     pitchSalienceFunctionPeaks.init();
     setActive(PITCH_SALIENCE_FUNC_PEAKS, false);
     multiPitchKlapuri.init(MULTI_PITCH_KLAPURI, _samplerate, _framesize);
     setActive(MULTI_PITCHES, false);
     //------------------
     */
    
    
    //MultiPitch Kalpuri:
    ///multiPitchKlapuri.setup(&pitchSalienceFunctionPeaks, &spectrum, _samplerate);;
}
//--------------------------------------------------------------
void ofxAudioAnalyzerUnit::createAlgorithms(){
    fft = new ofxAAFftAlgorithm(FFT, _samplerate, _framesize);
    vectorAlgorithms.push_back(fft);
    pitchDetect = new ofxAAPitchDetectAlgorithm(PITCH_YIN_FREQ, _samplerate, _framesize);
    algorithms.push_back(pitchDetect);
    onsets = new ofxAAOnsetsAlgorithm(ONSETS, _samplerate, _framesize);
    algorithms.push_back(onsets);
    cartesian2polar = new ofxAACartToPolAlgorithm(CART_TO_POLAR, _samplerate, _framesize);
    algorithms.push_back(cartesian2polar);
    spectralPeaks = new ofxAAPeaksAlgorithm(SPECTRAL_PEAKS, _samplerate, _framesize);
    algorithms.push_back(spectralPeaks);
    harmonicPeaks = new ofxAAPeaksAlgorithm(HARMONIC_PEAKS, _samplerate, _framesize);
    algorithms.push_back(harmonicPeaks);
    pitchSalienceFunctionPeaks = new ofxAAPitchSalienceFunctionPeaksAlgorithm(PITCH_SALIENCE_FUNC_PEAKS, _samplerate, _framesize);
    algorithms.push_back(pitchSalienceFunctionPeaks);
    
    algorithms.push_back(new ofxAABaseAlgorithm(RMS, _samplerate, _framesize));
    algorithms.push_back(new ofxAABaseAlgorithm(ENERGY, _samplerate, _framesize));
    algorithms.push_back(new ofxAABaseAlgorithm(POWER, _samplerate, _framesize));
    algorithms.push_back(new ofxAABaseAlgorithm(PITCH_SALIENCE, _samplerate, _framesize));
    algorithms.push_back(new ofxAABaseAlgorithm(INHARMONICITY, _samplerate, _framesize));
    algorithms.push_back(new ofxAABaseAlgorithm(HFC, _samplerate, _framesize));
    algorithms.push_back(new ofxAABaseAlgorithm(CENTROID, _samplerate, _framesize));
    algorithms.push_back(new ofxAABaseAlgorithm(SPECTRAL_COMPLEXITY, _samplerate, _framesize));
    algorithms.push_back(new ofxAABaseAlgorithm(DISSONANCE, _samplerate, _framesize));
    algorithms.push_back(new ofxAABaseAlgorithm(ROLL_OFF, _samplerate, _framesize));
    algorithms.push_back(new ofxAABaseAlgorithm(ODD_TO_EVEN, _samplerate, _framesize));
    algorithms.push_back(new ofxAABaseAlgorithm(STRONG_PEAK, _samplerate, _framesize));
    algorithms.push_back(new ofxAABaseAlgorithm(STRONG_DECAY, _samplerate, _framesize));
    
    vectorAlgorithms.push_back(new ofxAAOneVectorOutputAlgorithm(SPECTRUM, _samplerate, _framesize, (_framesize/2)+1));
    vectorAlgorithms.push_back(new ofxAAOneVectorOutputAlgorithm(MEL_BANDS, _samplerate, _framesize, MELBANDS_BANDS_NUM));
    vectorAlgorithms.push_back(new ofxAAOneVectorOutputAlgorithm(DCT, _samplerate, _framesize, DCT_COEFF_NUM));
    vectorAlgorithms.push_back(new ofxAAOneVectorOutputAlgorithm(HPCP, _samplerate, _framesize, HPCP_SIZE));
    vectorAlgorithms.push_back(new ofxAAOneVectorOutputAlgorithm(PITCH_SALIENCE_FUNC, _samplerate, _framesize, PITCH_SALIENCE_FUNC_NUM));
    vectorAlgorithms.push_back(new ofxAAOneVectorOutputAlgorithm(TRISTIMULUS, _samplerate, _framesize, TRISTIMULUS_BANDS_NUM));
    vectorAlgorithms.push_back(new ofxAAOneVectorOutputAlgorithm(DC_REMOVAL, _samplerate, _framesize));
    vectorAlgorithms.push_back(new ofxAAOneVectorOutputAlgorithm(WINDOW, _samplerate, _framesize));
    
    //-:Set Default Max Estimated Values for Non Normalized Algorithms
    //default values set from testing with white noise.
    
    algorithm(ENERGY)->setMaxEstimatedValue(100.0);
    algorithm(HFC)->setMaxEstimatedValue(2000.0);
    algorithm(SPECTRAL_COMPLEXITY)->setMaxEstimatedValue(20.0);
    algorithm(CENTROID)->setMaxEstimatedValue(11000.0);
    algorithm(ROLL_OFF)->setMaxEstimatedValue(_samplerate/2);
    algorithm(ODD_TO_EVEN)->setMaxEstimatedValue(10.0);
    algorithm(STRONG_PEAK)->setMaxEstimatedValue(20.0);
    algorithm(STRONG_DECAY)->setMaxEstimatedValue(100.0);
    pitchDetect->setMaxEstimatedValue(4186.0);//C8
}

void ofxAudioAnalyzerUnit::connectAlgorithms(){

    vectorAlgorithm(DC_REMOVAL)->algorithm->input("signal").set(audioBuffer);
    vectorAlgorithm(DC_REMOVAL)->algorithm->output("signal").set(vectorAlgorithm(DC_REMOVAL)->realValues);

    algorithm(RMS)->algorithm->input("array").set(vectorAlgorithm(DC_REMOVAL)->realValues);
    algorithm(RMS)->algorithm->output("rms").set(algorithm(RMS)->realValue);

    algorithm(ENERGY)->algorithm->input("array").set(vectorAlgorithm(DC_REMOVAL)->realValues);
    algorithm(ENERGY)->algorithm->output("energy").set(algorithm(ENERGY)->realValue);

    algorithm(POWER)->algorithm->input("array").set(vectorAlgorithm(DC_REMOVAL)->realValues);
    algorithm(POWER)->algorithm->output("power").set(algorithm(POWER)->realValue);

    vectorAlgorithm(WINDOW)->algorithm->input("frame").set(vectorAlgorithm(DC_REMOVAL)->realValues);
    vectorAlgorithm(WINDOW)->algorithm->output("frame").set(vectorAlgorithm(WINDOW)->realValues);

    fft->algorithm->input("frame").set(vectorAlgorithm(WINDOW)->realValues);
    fft->algorithm->output("fft").set(fft->fftRealValues);
    cartesian2polar->algorithm->input("complex").set(fft->fftRealValues);
    cartesian2polar->algorithm->output("magnitude").set(cartesian2polar->magnitudes);
    cartesian2polar->algorithm->output("phase").set(cartesian2polar->phases);

    onsets->onsetHfc.algorithm->input("spectrum").set(cartesian2polar->magnitudes);
    onsets->onsetHfc.algorithm->input("phase").set(cartesian2polar->phases);
    onsets->onsetHfc.algorithm->output("onsetDetection").set(onsets->onsetHfc.realValue);

    onsets->onsetComplex.algorithm->input("spectrum").set(cartesian2polar->magnitudes);
    onsets->onsetComplex.algorithm->input("phase").set(cartesian2polar->phases);
    onsets->onsetComplex.algorithm->output("onsetDetection").set(onsets->onsetComplex.realValue);

    onsets->onsetFlux.algorithm->input("spectrum").set(cartesian2polar->magnitudes);
    onsets->onsetFlux.algorithm->input("phase").set(cartesian2polar->phases);
    onsets->onsetFlux.algorithm->output("onsetDetection").set(onsets->onsetFlux.realValue);

    vectorAlgorithm(SPECTRUM)->algorithm->input("frame").set(vectorAlgorithm(WINDOW)->realValues);
    vectorAlgorithm(SPECTRUM)->algorithm->output("spectrum").set(vectorAlgorithm(SPECTRUM)->realValues);

    algorithm(HFC)->algorithm->input("spectrum").set(vectorAlgorithm(SPECTRUM)->realValues);
    algorithm(HFC)->algorithm->output("hfc").set(algorithm(HFC)->realValue);

    algorithm(PITCH_SALIENCE)->algorithm->input("spectrum").set(vectorAlgorithm(SPECTRUM)->realValues);
    algorithm(PITCH_SALIENCE)->algorithm->output("pitchSalience").set(algorithm(PITCH_SALIENCE)->realValue);

    algorithm(CENTROID)->algorithm->input("array").set(vectorAlgorithm(SPECTRUM)->realValues);
    algorithm(CENTROID)->algorithm->output("centroid").set(algorithm(CENTROID)->realValue);

    algorithm(SPECTRAL_COMPLEXITY)->algorithm->input("spectrum").set(vectorAlgorithm(SPECTRUM)->realValues);
    algorithm(SPECTRAL_COMPLEXITY)->algorithm->output("spectralComplexity").set(algorithm(SPECTRAL_COMPLEXITY)->realValue);

    spectralPeaks->algorithm->input("spectrum").set(vectorAlgorithm(SPECTRUM)->realValues);
    spectralPeaks->algorithm->output("frequencies").set(spectralPeaks->frequencies);
    spectralPeaks->algorithm->output("magnitudes").set(spectralPeaks->magnitudes);

    vectorAlgorithm(HPCP)->algorithm->input("frequencies").set(spectralPeaks->frequencies);
    vectorAlgorithm(HPCP)->algorithm->input("magnitudes").set(spectralPeaks->magnitudes);
    vectorAlgorithm(HPCP)->algorithm->output("hpcp").set(vectorAlgorithm(HPCP)->realValues);

    vectorAlgorithm(MEL_BANDS)->algorithm->input("spectrum").set(vectorAlgorithm(SPECTRUM)->realValues);
    vectorAlgorithm(MEL_BANDS)->algorithm->output("bands").set(vectorAlgorithm(MEL_BANDS)->realValues);

    vectorAlgorithm(DCT)->algorithm->input("array").set(vectorAlgorithm(MEL_BANDS)->logRealValues);
    vectorAlgorithm(DCT)->algorithm->output("dct").set(vectorAlgorithm(DCT)->realValues);

    pitchDetect->algorithm->input("spectrum").set(vectorAlgorithm(SPECTRUM)->realValues);
    pitchDetect->algorithm->output("pitch").set(pitchDetect->pitchRealVal);
    pitchDetect->algorithm->output("pitchConfidence").set(pitchDetect->confidenceRealVal);

    harmonicPeaks->algorithm->input("frequencies").set(spectralPeaks->frequencies);
    harmonicPeaks->algorithm->input("magnitudes").set(spectralPeaks->magnitudes);
    harmonicPeaks->algorithm->input("pitch").set(pitchDetect->pitchRealVal);
    harmonicPeaks->algorithm->output("harmonicFrequencies").set(harmonicPeaks->frequencies);
    harmonicPeaks->algorithm->output("harmonicMagnitudes").set(harmonicPeaks->magnitudes);

    algorithm(INHARMONICITY)->algorithm->input("frequencies").set(harmonicPeaks->frequencies);
    algorithm(INHARMONICITY)->algorithm->input("magnitudes").set(harmonicPeaks->magnitudes);
    algorithm(INHARMONICITY)->algorithm->output("inharmonicity").set(algorithm(INHARMONICITY)->realValue);

    algorithm(DISSONANCE)->algorithm->input("frequencies").set(spectralPeaks->frequencies);
    algorithm(DISSONANCE)->algorithm->input("magnitudes").set(spectralPeaks->magnitudes);
    algorithm(DISSONANCE)->algorithm->output("dissonance").set(algorithm(DISSONANCE)->realValue);

    vectorAlgorithm(PITCH_SALIENCE_FUNC)->algorithm->input("frequencies").set(spectralPeaks->frequencies);
    vectorAlgorithm(PITCH_SALIENCE_FUNC)->algorithm->input("magnitudes").set(spectralPeaks->magnitudes);
    vectorAlgorithm(PITCH_SALIENCE_FUNC)->algorithm->output("salienceFunction").set(vectorAlgorithm(PITCH_SALIENCE_FUNC)->realValues);

    pitchSalienceFunctionPeaks->algorithm->input("salienceFunction").set(vectorAlgorithm(PITCH_SALIENCE_FUNC)->realValues);
    pitchSalienceFunctionPeaks->algorithm->output("salienceBins").set(pitchSalienceFunctionPeaks->realSalienceBins);
    pitchSalienceFunctionPeaks->algorithm->output("salienceValues").set(pitchSalienceFunctionPeaks->realSalienceValues);
    
    algorithm(ROLL_OFF)->algorithm->input("spectrum").set(vectorAlgorithm(SPECTRUM)->realValues);
    algorithm(ROLL_OFF)->algorithm->output("rollOff").set(algorithm(ROLL_OFF)->realValue);
    
    algorithm(STRONG_PEAK)->algorithm->input("spectrum").set(vectorAlgorithm(SPECTRUM)->realValues);
    algorithm(STRONG_PEAK)->algorithm->output("strongPeak").set(algorithm(STRONG_PEAK)->realValue);
    
    algorithm(STRONG_DECAY)->algorithm->input("signal").set(vectorAlgorithm(DC_REMOVAL)->realValues);
    algorithm(STRONG_DECAY)->algorithm->output("strongDecay").set(algorithm(STRONG_DECAY)->realValue);
    
    algorithm(ODD_TO_EVEN)->algorithm->input("frequencies").set(harmonicPeaks->frequencies);
    algorithm(ODD_TO_EVEN)->algorithm->input("magnitudes").set(harmonicPeaks->magnitudes);
    algorithm(ODD_TO_EVEN)->algorithm->output("oddToEvenHarmonicEnergyRatio").set(algorithm(ODD_TO_EVEN)->realValue);
    
    vectorAlgorithm(TRISTIMULUS)->algorithm->input("frequencies").set(harmonicPeaks->frequencies);
    vectorAlgorithm(TRISTIMULUS)->algorithm->input("magnitudes").set(harmonicPeaks->magnitudes);
    vectorAlgorithm(TRISTIMULUS)->algorithm->output("tristimulus").set(vectorAlgorithm(TRISTIMULUS)->realValues);
}

//--------------------------------------------------------------
void ofxAudioAnalyzerUnit::analyze(const vector<float> & inBuffer){
    
    if(inBuffer.size() != _framesize){
        ofLogWarning()<<"ofxAudioAnalyzerUnit: buffer requested to analyze size(" <<inBuffer.size()<<")doesnt match the buffer size already set: "<<_framesize;
    }
    
    //Cast of incoming audio buffer to Real
    for (int i=0; i<inBuffer.size();i++){
        audioBuffer[i] = (Real) inBuffer[i];
    }
    
    #pragma mark -Compute Algorithms
    
    vectorAlgorithm(DC_REMOVAL)->compute();
    algorithm(RMS)->compute();
    algorithm(ENERGY)->compute();
    algorithm(POWER)->compute();
    vectorAlgorithm(WINDOW)->compute();
    
    if(onsets->getIsActive()){
        fft->compute();
        cartesian2polar->compute();
        onsets->compute();
    }
    
    //spectrum must always be computed as it is neede for other algorithms
    vectorAlgorithm(SPECTRUM)->algorithm->compute();
    
    algorithm(HFC)->compute();
    algorithm(PITCH_SALIENCE)->compute();
    pitchDetect->compute();
    algorithm(CENTROID)->compute();
    algorithm(SPECTRAL_COMPLEXITY)->compute();
    if(vectorAlgorithm(MEL_BANDS)->getIsActive()){
        vectorAlgorithm(MEL_BANDS)->algorithm->compute();
        if(vectorAlgorithm(DCT)->getIsActive()){
            vectorAlgorithm(MEL_BANDS)->updateLogRealValues();
            vectorAlgorithm(DCT)->compute();
        }
    }else{
        vectorAlgorithm(DCT)->setActive(false);//dct needs melBands to be active
    }
    spectralPeaks->compute();
    vectorAlgorithm(HPCP)->compute();
    
    if (algorithm(INHARMONICITY)->getIsActive()){
        harmonicPeaks->compute();
        algorithm(INHARMONICITY)->algorithm->compute();
    }

    algorithm(DISSONANCE)->compute();
    vectorAlgorithm(PITCH_SALIENCE_FUNC)->compute();
    pitchSalienceFunctionPeaks->compute();
    
    ///multiPitchKlapuri.compute();
    
    algorithm(ROLL_OFF)->compute();
    algorithm(ODD_TO_EVEN)->compute();
    algorithm(STRONG_PEAK)->compute();
    
    vectorAlgorithm(TRISTIMULUS)->compute();
    if(vectorAlgorithm(DC_REMOVAL)->realValues[0] != 0.0){
        //the strong decay is not defined for a zero signal
        algorithm(STRONG_DECAY)->compute();
    }
    
    
    #pragma mark -Cast results to float
    
    vectorAlgorithm(SPECTRUM)->castValuesToFloat(true);
    vectorAlgorithm(MEL_BANDS)->castValuesToFloat(true);
    vectorAlgorithm(DCT)->castValuesToFloat(false);
    vectorAlgorithm(HPCP)->castValuesToFloat(false);
    vectorAlgorithm(TRISTIMULUS)->castValuesToFloat(false);
    
    algorithm(RMS)->castValueToFloat();
    algorithm(ENERGY)->castValueToFloat();
    algorithm(POWER)->castValueToFloat();
    algorithm(PITCH_SALIENCE)->castValueToFloat();
    algorithm(HFC)->castValueToFloat();
    algorithm(CENTROID)->castValueToFloat();
    algorithm(SPECTRAL_COMPLEXITY)->castValueToFloat();
    algorithm(INHARMONICITY)->castValueToFloat();
    algorithm(DISSONANCE)->castValueToFloat();
    algorithm(ROLL_OFF)->castValueToFloat();
    algorithm(ODD_TO_EVEN)->castValueToFloat();
    algorithm(STRONG_PEAK)->castValueToFloat();
    algorithm(STRONG_DECAY)->castValueToFloat();
    
    pitchSalienceFunctionPeaks->castValuesToFloat();
    pitchDetect->castValuesToFloat();
    
    onsets->castValuesToFloat();
    onsets->evaluate();
}

//--------------------------------------------------------------
void ofxAudioAnalyzerUnit::exit(){
    for (auto a : algorithms){
        a->deleteAlgorithm();
    }
    for (auto a :vectorAlgorithms){
        a->deleteAlgorithm();
    }
}

//--------------------------------------------------------------
#pragma mark - Activates
//----------------------------------------------
void ofxAudioAnalyzerUnit::setActive(ofxAAAlgorithmType algorithmType, bool state){
    
    switch (algorithmType) {
        case SPECTRUM:
            ofLogWarning()<<"ofxAudioAnalyzerUnit: Spectrum Algorithm cant be turned off.";
            break;
        case MEL_BANDS:
            vectorAlgorithm(MEL_BANDS)->setActive(state);
            if(state==false)vectorAlgorithm(DCT)->setActive(state);//dct needs melBands to be active.
            break;
        case DCT:
            //dct needs melBands to be active.
            vectorAlgorithm(MEL_BANDS)->setActive(state);
            vectorAlgorithm(DCT)->setActive(state);
            break;
        case PITCH_SALIENCE_FUNC_PEAKS:
            vectorAlgorithm(PITCH_SALIENCE_FUNC)->setActive(state);
            pitchSalienceFunctionPeaks->setActive(state);
            break;
        default:
            algorithm(algorithmType)->setActive(state);
            break;
    }
}
//----------------------------------------------
bool ofxAudioAnalyzerUnit::getIsActive(ofxAAAlgorithmType algorithmType){
    return  algorithm(algorithmType)->getIsActive();
}
//----------------------------------------------
#pragma mark - Get values
//----------------------------------------------
float ofxAudioAnalyzerUnit::getValue(ofxAAAlgorithmType algorithmType, float smooth, bool normalized){
   
    //TODO: Check this exception cases...
    switch (algorithmType) {
        case PITCH_YIN_FREQ:
            if (normalized){
                return smooth ? pitchDetect->getSmoothedPitchValueNormalized(smooth): pitchDetect->getPitchValueNormalized();
            }else{
                return smooth ? pitchDetect->getSmoothedPitchValue(smooth) : pitchDetect->getPitchValue();
            }
            break;
            
        case PITCH_YIN_CONFIDENCE:
            return smooth ? pitchDetect->getSmoothedConfidenceValue(smooth) : pitchDetect->getConfidenceValue();
            break;
            
        case ODD_TO_EVEN:
            if (normalized){
                return smooth ? algorithm(ODD_TO_EVEN)->getSmoothedValueNormalized(smooth) : algorithm(ODD_TO_EVEN)->getValueNormalized();
            }else{
                float r = smooth ? algorithm(ODD_TO_EVEN)->getSmoothedValue(smooth) : algorithm(ODD_TO_EVEN)->getValue();
                //limit value, because this algorithm reaches huge values (eg: 3.40282e+38)
                return ofClamp(r, 0.0, algorithm(ODD_TO_EVEN)->getMaxEstimatedValue());
            }
            break;
            
        default:
            ofxAABaseAlgorithm* baseAlgorithm =  algorithm(algorithmType);
            
            if (ofxaa::algorithmHasVectorOutput(baseAlgorithm)){
                ofLogWarning()<<"ofxAudioAnalyzerUnit: wrong algorithm for Getting values. This is algorithm outputs a vector of values" << baseAlgorithm->algorithm->name();
                return 0.0;
            }
            if (normalized && !ofxaa::algorithmHasNormalizedSingleOutputByDefault(baseAlgorithm)){
                return smooth ? baseAlgorithm->getSmoothedValueNormalized(smooth): baseAlgorithm->getValueNormalized();
            } else if (ofxaa::algorithmHasOutputInDbs(baseAlgorithm)) {
                return smooth ? baseAlgorithm->getSmoothedValueDbNormalized(smooth, DB_MIN, DB_MAX) : baseAlgorithm->getValueDbNormalized(DB_MIN, DB_MAX);
            } else {
                return smooth ? baseAlgorithm->getSmoothedValue(smooth): baseAlgorithm->getValue();
            }
            break;
    }
}

//----------------------------------------------
vector<float>& ofxAudioAnalyzerUnit::getValues(ofxAAAlgorithmType algorithmType, float smooth){
    
    ofxAAOneVectorOutputAlgorithm* algorithm = vectorAlgorithm(algorithmType);
    
    if (!ofxaa::algorithmHasVectorOutput(algorithm)){
        ofLogWarning()<<"ofxAudioAnalyzerUnit: wrong algorithm for Getting values. This is algorithm outputs a single of values" << algorithm->algorithm->name();
        static vector<float>r (1, 0.0);
        return r;
    }
    
    return smooth ? algorithm->getSmoothedValues(smooth) : algorithm->getValues();
}
//----------------------------------------------
int ofxAudioAnalyzerUnit::getBinsNum(ofxAAAlgorithmType algorithmType){
    ofxAAOneVectorOutputAlgorithm* algorithm = vectorAlgorithm(algorithmType);
    
    if (!ofxaa::algorithmHasVectorOutput(algorithm)){
        ofLogWarning()<<"ofxAudioAnalyzerUnit: wrong algorithm for Getting values. This is algorithm outputs a single of values" << algorithm->algorithm->name();
        return 0;
    }
    
    return algorithm->getBinsNum();
}
//----------------------------------------------
float ofxAudioAnalyzerUnit::getMaxEstimatedValue(ofxAAAlgorithmType algorithmType){
    ofxAABaseAlgorithm* baseAlgorithm = algorithm(algorithmType);
    
    if (ofxaa::algorithmHasVectorOutput(baseAlgorithm)){
        ofLogWarning()<<"ofxAudioAnalyzerUnit: wrong algorithm for Getting max estimated value. This is algorithm outputs a vector" << baseAlgorithm->algorithm->name();
    }
    
    if (ofxaa::algorithmHasNormalizedSingleOutputByDefault(baseAlgorithm)){
        ofLogWarning()<<"ofxAudioAnalyzerUnit: wrong algorithm for Getting max estimated value. This is algorithm outputs a normalized value by default." << baseAlgorithm->algorithm->name();
    }
    
    return baseAlgorithm->getMaxEstimatedValue();
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setMaxEstimatedValue(ofxAAAlgorithmType algorithmType, float value){
    ofxAABaseAlgorithm* baseAlgorithm = algorithm(algorithmType);
    
    if (ofxaa::algorithmHasVectorOutput(baseAlgorithm)){
         ofLogWarning()<<"ofxAudioAnalyzerUnit: wrong algorithm for Setting max estimated value. This is algorithm outputs a vector" << baseAlgorithm->algorithm->name();
    }
    
    if (ofxaa::algorithmHasNormalizedSingleOutputByDefault(baseAlgorithm)){
        ofLogWarning()<<"ofxAudioAnalyzerUnit: wrong algorithm for Setting max estimated value. This is algorithm outputs a normalized value by default." << baseAlgorithm->algorithm->name();
    }
    
    baseAlgorithm->setMaxEstimatedValue(value);
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setSalienceFunctionPeaksParameters(int maxPeaks){
    pitchSalienceFunctionPeaks->setMaxPeaksNum(maxPeaks);
}
//----------------------------------------------
ofxAABaseAlgorithm* ofxAudioAnalyzerUnit::algorithm(ofxAAAlgorithmType type){
    if (type == NONE){
         ofLogError()<<"ofxAudioAnalyzerUnit: algorithm type NONE requested.";
    }
    for (int i=0; i<algorithms.size(); i++){
        if (type == algorithms[i]->getType()){
            return algorithms[i];
        }
    }
     ofLogError()<<"ofxAudioAnalyzerUnit: algorithm type is NOT a Base Algorithm.";
}
//----------------------------------------------
ofxAAOneVectorOutputAlgorithm* ofxAudioAnalyzerUnit::vectorAlgorithm(ofxAAAlgorithmType type){
    for (int i=0; i<vectorAlgorithms.size(); i++){
        if (type == vectorAlgorithms[i]->getType()){
            return vectorAlgorithms[i];
        }
    }
}
//----------------------------------------------
#pragma mark - Onsets
//----------------------------------------------
bool ofxAudioAnalyzerUnit::getOnsetValue(){
    return onsets->getValue();
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::resetOnsets(){
    onsets->reset();
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setOnsetsParameters(float alpha, float silenceTresh, float timeTresh, bool useTimeTresh){
    onsets->setOnsetAlpha(alpha);
    onsets->setOnsetSilenceThreshold(silenceTresh);
    onsets->setOnsetTimeThreshold(timeTresh);
    onsets->setUseTimeThreshold(useTimeTresh);
}
//----------------------------------------------
#pragma mark - Pitch
//----------------------------------------------
int ofxAudioAnalyzerUnit::getPitchFreqAsMidiNote(float smooth){
    return ofxaa::pitchToMidi(getValue(PITCH_YIN_FREQ, smooth));
}
//----------------------------------------------
string ofxAudioAnalyzerUnit::getPitchFreqAsNoteName(float smooth){
    return ofxaa::midiToNoteName(getValue(PITCH_YIN_FREQ, smooth));
}
//----------------------------------------------
vector<SalienceFunctionPeak>& ofxAudioAnalyzerUnit::getPitchSaliencePeaksRef(float smooth){
    return smooth ? pitchSalienceFunctionPeaks->getSmoothedPeaks(smooth) : pitchSalienceFunctionPeaks->getPeaks();
}


