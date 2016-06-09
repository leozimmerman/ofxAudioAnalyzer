#include "ofxAudioAnalyzerUnit.h"

#pragma mark - Main funcs
//--------------------------------------------------------------
void ofxAudioAnalyzerUnit::setup(int sampleRate, int bufferSize){

    framesize = bufferSize;
    hopsize = framesize/2;
    sr = sampleRate;
    zeropadding = 0;
    framerate = (Real) sr / hopsize;

    ///init algorithms------------------
    rms.init();
    energy.init();
    power.init();
    pitchDetect.init();
    pitchSalience.init();
    tuning.init();
    inharmonicity.init();
    hfc.init();
    centroid.init();
    spectralComplex.init();
    
    spectrum.initAndAssignSize((bufferSize/2)+1, 0.0);
    melBands.initAndAssignSize(MELBANDS_BANDS_NUM,0);
    dct.initAndAssignSize(DCT_COEFF_NUM, 0);
    hpcp.initAndAssignSize(HPCP_SIZE, 0);

    dcremoval.init();
    window.init();
    fft.init();
    cartesian2polar.init();
    peaks.init();
    harmonicPeaks.init();
    
    onsetHfc.init();
    onsetComplex.init();
    onsetFlux.init();
    

    ///onsets:
    detecBufferSize = bufferSize; //TODO: revisar
    detection_sum.assign(detecBufferSize,0);
    detections.assign(3, vector<Real> (detecBufferSize));
    silenceTreshold = 0.02;
    alpha = 0.1;
    timeTreshold = 100.0;
    float lastOnsetTime = 0.0;
    addHfc = addComplex = addFlux = true;
    useTimeTreshold = false; //TODO: add frameNum timeTreshold option, for offline rendering
    
    ///Max estimated values------
    
    maxEnergyEstimatedValue = 100.0;
    maxHfcEstimatedValue = 500.0; //antes 1000
    maxSpecCompEstimatedValue = 15.0; //antes 30
    maxCentroidEstimatedValue = 5000.0;//antes 7000

    ///activations---------
    rms.setActive(TRUE);
    power.setActive(TRUE);
    energy.setActive(TRUE);

    pitchDetect.setActive(TRUE);
    pitchSalience.setActive(TRUE);
    tuning.setActive(TRUE);
    inharmonicity.setActive(TRUE);
    hfc.setActive(TRUE);
    centroid.setActive(TRUE);
    spectralComplex.setActive(TRUE);

    spectrum.setActive(TRUE);
    melBands.setActive(TRUE);
    dct.setActive(TRUE);
    hpcp.setActive(TRUE);

    doOnsets = true;

    audioBuffer.resize(bufferSize);

    essentia::init();

    /// instanciate factory and create algorithms---------------------------------------------------

    AlgorithmFactory& factory = AlgorithmFactory::instance();
    
    
    rms.algorithm = factory.create("RMS");
    energy.algorithm = factory.create("Energy");
    power.algorithm = factory.create("InstantPower");

    dcremoval.algorithm = factory.create("DCRemoval", "sampleRate", sr);

    window.algorithm = factory.create("Windowing",
                                 "type", "hann",
                                 "zeroPadding", zeropadding);

    fft.algorithm = factory.create("FFT", "size", framesize);
    cartesian2polar.algorithm = factory.create("CartesianToPolar");

    onsetHfc.algorithm     = factory.create("OnsetDetection", "method", "hfc", "sampleRate", sr);
    onsetComplex.algorithm = factory.create("OnsetDetection", "method", "complex", "sampleRate", sr);
    onsetFlux.algorithm    = factory.create("OnsetDetection", "method", "flux", "sampleRate", sr);

    spectrum.algorithm = factory.create("Spectrum",
                                   "size", framesize);

    hfc.algorithm = factory.create("HFC", "sampleRate", sr);

    pitchSalience.algorithm = factory.create("PitchSalience", "sampleRate",sr);

    centroid.algorithm = factory.create("Centroid", "range", sr/2);

    spectralComplex.algorithm = factory.create("SpectralComplexity", "sampleRate", sr);

    peaks.algorithm = factory.create("SpectralPeaks",
                                "maxPeaks", PEAKS_MAXPEAKS_NUM,
                                "magnitudeThreshold", 0.00001,
                                "minFrequency", PEAKS_MIN_FREQ,
                                "maxFrequency", PEAKS_MAX_FREQ,
                                "orderBy", "frequency");


    melBands.algorithm = factory.create("MelBands", "inputSize", (framesize/2)+1,
                              "sampleRate", sr);
    dct.algorithm = factory.create("DCT", "inputSize", MELBANDS_BANDS_NUM, "outputSize", DCT_COEFF_NUM);

    tuning.algorithm = factory.create("TuningFrequency");

    inharmonicity.algorithm = factory.create("Inharmonicity");

    pitchDetect.algorithm = factory.create("PitchYinFFT",
                                      "frameSize", framesize,
                                      "sampleRate", sr);

    harmonicPeaks.algorithm = factory.create("HarmonicPeaks");

    hpcp.algorithm = factory.create("HPCP",
                                   "size", HPCP_SIZE,
                                   "referenceFrequency", 440,
                                   "bandPreset", false,
                                   "minFrequency", HPCP_MIN_FREQ,
                                   "maxFrequency", HPCP_MAX_FREQ,
                                   "weightType", "squaredCosine",
                                   "nonLinear", false,
                                   "windowSize", 4.0/3.0);



///Algorithm diagram--------------------------------------------------------


    //DCRemoval
    dcremoval.algorithm->input("signal").set(audioBuffer);
    dcremoval.algorithm->output("signal").set(dcremoval.realValues);
    //RMS
    rms.algorithm->input("array").set(dcremoval.realValues);
    rms.algorithm->output("rms").set(rms.realValue);
    //Energy
    energy.algorithm->input("array").set(dcremoval.realValues);
    energy.algorithm->output("energy").set(energy.realValue);
    //Power
    power.algorithm->input("array").set(dcremoval.realValues);
    power.algorithm->output("power").set(power.realValue);
    //Window
    window.algorithm->input("frame").set(dcremoval.realValues);
    window.algorithm->output("frame").set(window.realValues);
    //Onsets
    fft.algorithm->input("frame").set(window.realValues);
    fft.algorithm->output("fft").set(fft.fftRealValues);
    cartesian2polar.algorithm->input("complex").set(fft.fftRealValues);
    cartesian2polar.algorithm->output("magnitude").set(cartesian2polar.magnitudes);
    cartesian2polar.algorithm->output("phase").set(cartesian2polar.phases);
    //-
    onsetHfc.algorithm->input("spectrum").set(cartesian2polar.magnitudes);
    onsetHfc.algorithm->input("phase").set(cartesian2polar.phases);
    onsetHfc.algorithm->output("onsetDetection").set(onsetHfc.realValue);
    //-
    onsetComplex.algorithm->input("spectrum").set(cartesian2polar.magnitudes);
    onsetComplex.algorithm->input("phase").set(cartesian2polar.phases);
    onsetComplex.algorithm->output("onsetDetection").set(onsetComplex.realValue);
    //-
    onsetFlux.algorithm->input("spectrum").set(cartesian2polar.magnitudes);
    onsetFlux.algorithm->input("phase").set(cartesian2polar.phases);
    onsetFlux.algorithm->output("onsetDetection").set(onsetFlux.realValue);
    //Spectrum
    spectrum.algorithm->input("frame").set(window.realValues);
    spectrum.algorithm->output("spectrum").set(spectrum.realValues);
    //HFC
    hfc.algorithm->input("spectrum").set(spectrum.realValues);
    hfc.algorithm->output("hfc").set(hfc.realValue);
    //Pitch Salience
    pitchSalience.algorithm->input("spectrum").set(spectrum.realValues);
    pitchSalience.algorithm->output("pitchSalience").set(pitchSalience.realValue);
    //Centroid
    centroid.algorithm->input("array").set(spectrum.realValues);
    centroid.algorithm->output("centroid").set(centroid.realValue);
    //Spectral Complexity
    spectralComplex.algorithm->input("spectrum").set(spectrum.realValues);
    spectralComplex.algorithm->output("spectralComplexity").set(spectralComplex.realValue);
    //Peak detection
    peaks.algorithm->input("spectrum").set(spectrum.realValues);
    peaks.algorithm->output("frequencies").set(peaks.frequencies);
    peaks.algorithm->output("magnitudes").set(peaks.magnitudes);
    //HPCP
    hpcp.algorithm->input("frequencies").set(peaks.frequencies);
    hpcp.algorithm->input("magnitudes").set(peaks.magnitudes);
    hpcp.algorithm->output("hpcp").set(hpcp.realValues);
    //MelBands
    melBands.algorithm->input("spectrum").set(spectrum.realValues);
    melBands.algorithm->output("bands").set(melBands.realValues);
    //DCT
    dct.algorithm->input("array").set(melBands.logRealValues);
    dct.algorithm->output("dct").set(dct.realValues);
    //PitchDetection
    pitchDetect.algorithm->input("spectrum").set(spectrum.realValues);
    pitchDetect.algorithm->output("pitch").set(pitchDetect.pitchRealVal);
    pitchDetect.algorithm->output("pitchConfidence").set(pitchDetect.confidenceRealVal);
    //Tuning frequency
    tuning.algorithm->input("frequencies").set(peaks.frequencies);
    tuning.algorithm->input("magnitudes").set(peaks.magnitudes);
    tuning.algorithm->output("tuningFrequency").set(tuning.freqRealVal);
    tuning.algorithm->output("tuningCents").set(tuning.centsRealVal);
    //HarmonicPeaks
    harmonicPeaks.algorithm->input("frequencies").set(peaks.frequencies);
    harmonicPeaks.algorithm->input("magnitudes").set(peaks.magnitudes);
    harmonicPeaks.algorithm->input("pitch").set(pitchDetect.pitchRealVal);
    harmonicPeaks.algorithm->output("harmonicFrequencies").set(harmonicPeaks.frequencies);
    harmonicPeaks.algorithm->output("harmonicMagnitudes").set(harmonicPeaks.magnitudes);
    //Inharmonicity
    inharmonicity.algorithm->input("frequencies").set(harmonicPeaks.frequencies);
    inharmonicity.algorithm->input("magnitudes").set(harmonicPeaks.magnitudes);
    inharmonicity.algorithm->output("inharmonicity").set(inharmonicity.realValue);

    //!!! testing this line...
    factory.shutdown();
}
//--------------------------------------------------------------
void ofxAudioAnalyzerUnit::analyze(const vector<float> & inBuffer){
    
    if(inBuffer.size() != framesize){
        ofLogWarning()<<"ofxAudioAnalyzerUnit: buffer requested to analyze size(" <<inBuffer.size()<<")doesnt match the buffer size already set: "<<framesize;
    }
    
    //Cast of incoming audio buffer to Real ------------
    for (int i=0; i<inBuffer.size();i++){
        audioBuffer[i] = (Real) inBuffer[i];
    }
    
    // Compute Algorithms--------------------------
    
    dcremoval.compute();
    
    rms.compute();
    energy.compute();
    power.compute();
    
    window.compute();
    if(doOnsets){
        fft.compute();
        cartesian2polar.compute();
        onsetHfc.compute();
        onsetComplex.compute();
        onsetFlux.compute();
    }
    
    //spectrum must always be computed as it is neede for other algorithms
    spectrum.algorithm->compute();
    
    hfc.compute();
    pitchSalience.compute();
    pitchDetect.compute();
    
    centroid.compute();
    spectralComplex.compute();
    
    if(melBands.getIsActive()){
        melBands.algorithm->compute();
        if(dct.getIsActive()){
            melBands.updateLogRealValues();
            dct.compute();
        }
    }else{
        dct.setActive(false);//dct needs melBands to be active
    }
    peaks.compute();
    hpcp.compute();
    tuning.compute();
    
    if (inharmonicity.getIsActive()){
        harmonicPeaks.compute();
        inharmonicity.algorithm->compute();
    }
    
    
    //Cast results to FLOATS--------------------------------------
    
    spectrum.castValuesToFloat(true);
    
    rms.castValueToFloat();
    energy.castValueToFloat();
    power.castValueToFloat();
    
    
    pitchDetect.castValuesToFloat();
    pitchSalience.castValueToFloat();
    
    tuning.castValuesToFloat();
    
    
    if(doOnsets){
        onsetHfc.castValueToFloat();
        onsetComplex.castValueToFloat();
        onsetFlux.castValueToFloat();
    
        //onsetEvaluation
        bool isThisBufferOnset = onsetEvaluation(onsetHfc.getValue(), onsetComplex.getValue(), onsetFlux.getValue());

        if(useTimeTreshold && isThisBufferOnset){
            isOnset = onsetTimeTresholdEvaluation();
        }
        else{
            isOnset = isThisBufferOnset;
        }
        
        
        
    }else{
        onsetHfc.setValueZero();
        onsetFlux.setValueZero();
        onsetComplex.setValueZero();
        isOnset = false;
    }
    
    //MelBands
    melBands.castValuesToFloat(true);
    dct.castValuesToFloat(false);
    
    hpcp.castValuesToFloat(false);
    hfc.castValueToFloat();
    centroid.castValueToFloat();
    spectralComplex.castValueToFloat();
    inharmonicity.castValueToFloat();
    
}

//--------------------------------------------------------------
void ofxAudioAnalyzerUnit::exit(){

    dcremoval.deleteAlgorithm();
    rms.deleteAlgorithm();
    energy.deleteAlgorithm();
    power.deleteAlgorithm();
    window.deleteAlgorithm();;
    spectrum.deleteAlgorithm();
    peaks.deleteAlgorithm();;
    pitchDetect.deleteAlgorithm();
    pitchSalience.deleteAlgorithm();
    
    tuning.deleteAlgorithm();
    
    melBands.deleteAlgorithm();
    dct.deleteAlgorithm();
    harmonicPeaks.deleteAlgorithm();;
    hpcp.deleteAlgorithm();
    hfc.deleteAlgorithm();
    inharmonicity.deleteAlgorithm();
    centroid.deleteAlgorithm();
    spectralComplex.deleteAlgorithm();
    fft.deleteAlgorithm();;
    cartesian2polar.deleteAlgorithm();;
    onsetComplex.deleteAlgorithm();;
    onsetHfc.deleteAlgorithm();;
    onsetFlux.deleteAlgorithm();;
    
    
    essentia::shutdown();
    
    ofLogVerbose()<<"AudioAnalyzer exit";

}
#pragma mark - Onset Funcs
//--------------------------------------------------------------
 bool ofxAudioAnalyzerUnit::onsetEvaluation (Real iDetectHfc, Real iDetectComplex, Real iDetectFlux){

    Real prop_hfc, prop_complex, prop_flux;

    if (iDetectHfc > hfc_max) {
        prop_hfc = iDetectHfc/hfc_max;
        hfc_max = iDetectHfc;
        for (int i=0; i<detections[0].size(); i++)
            detections[0][i] /= prop_hfc;
    }
    if (iDetectComplex > complex_max){
        prop_complex = iDetectComplex/complex_max;
        complex_max = iDetectComplex;
         for (int i=0; i<detections[1].size(); i++)
            detections[1][i] /= prop_complex;
    }
    if (iDetectFlux > flux_max){
        prop_flux = iDetectFlux/flux_max;
        flux_max = iDetectFlux;
         for (int i=0; i<detections[2].size(); i++)
            detections[2][i] /= prop_flux;
    }

    Real hfc_norm = iDetectHfc / hfc_max;
    Real complex_norm = iDetectComplex / complex_max;
    Real flux_norm = iDetectFlux / flux_max;

    detections[0].push_back(hfc_norm);
    detections[0].erase(detections[0].begin());

    detections[1].push_back(complex_norm);
    detections[1].erase(detections[1].begin());

    detections[2].push_back(flux_norm);
    detections[2].erase(detections[2].begin());

    for (int i=0; i<detection_sum.size(); i++){
        int n=0;
        detection_sum[i]=0;
        if(addHfc){
            detection_sum[i] += detections[0][i];
            n++;
        }
        if(addComplex){
            detection_sum[i] += detections[1][i];
            n++;
        }
        if(addFlux){
            detection_sum[i] += detections[2][i];
            n++;
        }
        if(n>0) detection_sum[i] /= n;
        if(detection_sum[i] < silenceTreshold) detection_sum[i] = 0.0;
    }

    Real buffer_median = median (detection_sum);
    Real buffer_mean = mean (detection_sum);
    Real onset_threshold = buffer_median + alpha * buffer_mean;

    bool onsetDetection = detection_sum[detection_sum.size()-1] > onset_threshold;

    return onsetDetection;

 }
//----------------------------------------------
bool ofxAudioAnalyzerUnit::onsetTimeTresholdEvaluation(){
    
    bool onsetTimeEvaluation = false;

    float currentTime = ofGetElapsedTimeMillis();
    
    //elapsed time since last onset:
    float elapsed = currentTime - lastOnsetTime;
    
    if (elapsed>timeTreshold){
        onsetTimeEvaluation = true;
        lastOnsetTime = currentTime;
    }
    
    return onsetTimeEvaluation;
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::resetOnsets(){
    
    hfc_max = complex_max = flux_max = 0.0;
    for (int i=0; i<detection_sum.size(); i++)
        detection_sum[i] = 0.0;

}
//----------------------------------------------
#pragma mark - Setters
//----------------------------------------------
void ofxAudioAnalyzerUnit::setOnsetSilenceTreshold(float val){
    silenceTreshold=val;
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setOnsetAlpha(float val){
    alpha=val;
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setOnsetTimeTreshold(float val){
    timeTreshold = val;
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setActiveRms(bool state){
    rms.setActive(state);
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setActiveEnergy(bool state){
    energy.setActive(state);
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setActivePower(bool state){
    power.setActive(state);
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setActivePitch(bool state){
    pitchDetect.setActive(state);
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setActiveMelodySalience(bool state){
    pitchSalience.setActive(state);
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setActiveTuning(bool state){
    tuning.setActive(state);
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setActiveInharmonicity(bool state){
    inharmonicity.setActive(state);
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setActiveHfc(bool state){
    hfc.setActive(state);
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setActiveSpectralComplex(bool state){
    spectralComplex.setActive(state);
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setActiveCentroid(bool state){
    centroid.setActive(state);
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setActiveMelbandsAndMfcc(bool state){
    melBands.setActive(state);
    dct.setActive(state);
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setActiveHpcp(bool state){
    hpcp.setActive(state);
}
//----------------------------------------------
void ofxAudioAnalyzerUnit::setActiveOnsets(bool state){
    doOnsets = state;
}
//----------------------------------------------
#pragma mark - Getters
//----------------------------------------------
float ofxAudioAnalyzerUnit::getRms(float smooth){
    float r = smooth ?
        rms.getSmoothedValueDbNormalized(smooth, DB_MIN, DB_MAX) :
        rms.getValueDbNormalized(DB_MIN, DB_MAX);
    return r;
}
//----------------------------------------------
float ofxAudioAnalyzerUnit::getEnergy(float smooth){
    float r =   smooth ?
                    energy.getSmoothedValueNormalized(smooth, 0.0, maxEnergyEstimatedValue) :
                    energy.getValueNormalized(0.0, maxEnergyEstimatedValue);
    return r;
}
//----------------------------------------------
float ofxAudioAnalyzerUnit::getPower(float smooth){
    float r = smooth ?
                power.getSmoothedValueDbNormalized(smooth, DB_MIN, DB_MAX) :
                power.getValueDbNormalized(DB_MIN, DB_MAX);
    return r;
}
//----------------------------------------------
float ofxAudioAnalyzerUnit::getPitchFreq(float smooth){
    return smooth ? pitchDetect.getSmoothedPitchValue(smooth) : pitchDetect.getPitchValue();
}
//----------------------------------------------
int ofxAudioAnalyzerUnit::getPitchFreqAsMidiNote(float smooth){
    return pitchToMidi(getPitchFreq(smooth));
}
//----------------------------------------------
string ofxAudioAnalyzerUnit::getPitchFreqAsNoteName(float smooth){
    return midiToNoteName(pitchToMidi(getPitchFreq(smooth)));
}
//----------------------------------------------
float ofxAudioAnalyzerUnit::getPitchConfidence(float smooth){
    return smooth ? pitchDetect.getSmoothedConfidenceValue(smooth) : pitchDetect.getConfidenceValue();
}
//----------------------------------------------
float ofxAudioAnalyzerUnit::getMelodySalience(float smooth){
    return smooth ? pitchSalience.getSmoothedValue(smooth) : pitchSalience.getValue();
}
//----------------------------------------------
float ofxAudioAnalyzerUnit::getTuningFreq(){
    return tuning.getFreqValue();
}
//----------------------------------------------
float ofxAudioAnalyzerUnit::getTuningCents(){
    return tuning.getCentsValue();
}
//----------------------------------------------
float ofxAudioAnalyzerUnit::getInharmonicity(float smooth){
    return smooth ? inharmonicity.getSmoothedValue(smooth) : inharmonicity.getValue();
}
//----------------------------------------------
float ofxAudioAnalyzerUnit::getHfc(float smooth){
    return smooth ? hfc.getSmoothedValue(smooth) : hfc.getValue();
}
//----------------------------------------------
float ofxAudioAnalyzerUnit::getHfcNormalized(float smooth){
    float r =   smooth ?
                hfc.getSmoothedValueNormalized(smooth, 0.0, maxHfcEstimatedValue) :
                hfc.getValueNormalized(0.0, maxHfcEstimatedValue);
    return r;
    
}
//----------------------------------------------
float ofxAudioAnalyzerUnit::getSpectralComplex(float smooth){
    return smooth ? spectralComplex.getSmoothedValue(smooth) : spectralComplex.getValue();
}
//----------------------------------------------
float ofxAudioAnalyzerUnit::getSpectralComplexNormalized(float smooth){
    float r =   smooth ?
                spectralComplex.getSmoothedValueNormalized(smooth, 0.0, maxSpecCompEstimatedValue) :
                spectralComplex.getValueNormalized(0.0, maxSpecCompEstimatedValue);
    return r;
}
//----------------------------------------------
float ofxAudioAnalyzerUnit::getCentroid(float smooth){
    return smooth ? centroid.getSmoothedValue(smooth) : centroid.getValue();
}
//----------------------------------------------
float ofxAudioAnalyzerUnit::getCentroidNormalized(float smooth){
    float r =   smooth ?
                centroid.getSmoothedValueNormalized(smooth, 0.0, maxCentroidEstimatedValue) :
                centroid.getValueNormalized(0.0, maxCentroidEstimatedValue);
    return r;
}
//----------------------------------------------
bool ofxAudioAnalyzerUnit::getIsOnset(){
    return isOnset;
}
//----------------------------------------------
int ofxAudioAnalyzerUnit::getSpectrumBinsNum(){
    return spectrum.getBinsNum();
}
//----------------------------------------------
int ofxAudioAnalyzerUnit::getMelBandsBinsNum(){
    return melBands.getBinsNum();
}
//----------------------------------------------
int ofxAudioAnalyzerUnit::getMfccBinsNum(){
    return dct.getBinsNum();
}
//----------------------------------------------
int ofxAudioAnalyzerUnit::getHpcpBinsNum(){
    return hpcp.getBinsNum();
}
//----------------------------------------------
vector<float>& ofxAudioAnalyzerUnit::getSpectrumRef(float smooth){
    return smooth ? spectrum.getSmoothedValues(smooth) : spectrum.getValues();
}
//----------------------------------------------
vector<float>& ofxAudioAnalyzerUnit::getMelBandsRef(float smooth){
    return smooth ? melBands.getSmoothedValues(smooth) : melBands.getValues();
}
//----------------------------------------------
vector<float>& ofxAudioAnalyzerUnit::getDctRef(float smooth){
    return smooth ? dct.getSmoothedValues(smooth) : dct.getValues();
}
//----------------------------------------------
vector<float>& ofxAudioAnalyzerUnit::getHpcpRef(float smooth){
    return smooth ? hpcp.getSmoothedValues(smooth) : hpcp.getValues();
}
//----------------------------------------------
#pragma mark - Utils
//----------------------------------------------
int ofxAudioAnalyzerUnit::pitchToMidi(float pitch){
    return round (12*log2(pitch/440) + 69);
}
//--------------------------------------------------------------
string ofxAudioAnalyzerUnit::midiToNoteName(int midiNote){
    
    string noteName;
    int mod = midiNote%12;
    switch (mod){
        case 0: noteName = "C";
            break;
        case 1: noteName = "C#";
            break;
        case 2: noteName = "D";
            break;
        case 3: noteName = "D#";
            break;
        case 4: noteName = "E";
            break;
        case 5: noteName = "F";
            break;
        case 6: noteName = "F#";
            break;
        case 7: noteName = "G";
            break;
        case 8: noteName = "G#";
            break;
        case 9: noteName = "A";
            break;
        case 10: noteName = "Bb";
            break;
        case 11: noteName = "B";
            break;
        default:
            break;
            
    }
    return (noteName);
    
}


