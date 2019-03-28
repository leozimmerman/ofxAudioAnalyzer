#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(34);
    ofSetFrameRate(60);
    
    sampleRate = 44100;
    bufferSize = 512;
    int channels = 1;
    
    bAudioInput = true;
    
    audioAnalyzer.setup(sampleRate, bufferSize, channels);
    
    
    gui.setup();
    gui.setPosition(20, 150);
    gui.add(smoothAttack.setup  ("Smooth Attack", 0.0, 0.0, 1.0));
    gui.add(smoothRelease.setup  ("Smooth Release", 0.0, 0.0, 1.0));

    sound.printDeviceList();
    
    settings.sampleRate = sampleRate;
    settings.bufferSize = 512;
    settings.setInListener(this);
    settings.setOutListener(this);
    settings.numOutputChannels = 0;
    settings.numInputChannels = 1;
    
    sound.setup(settings);
    
    
    auto names = sound.getMatchingDevices("Microphone");
    if(!names.empty()) {
        ofLog() << "Setting audio input device to " << names[0];
        sound.stop();
        settings.setInDevice(names[0]);
        sound.setup(settings);
        sound.start();
    } else {
        ofLog() << "Could not find audio input device ";
    }
   
}

void ofApp::audioIn(ofSoundBuffer & buffer){
    
    if (bAudioInput) audioAnalyzer.analyze(buffer);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    if (!bAudioInput) {
        
        playerBuffer = player.getCurrentSoundBuffer(bufferSize);
        audioAnalyzer.analyze(playerBuffer);
    }
    
    
    
    rms     = audioAnalyzer.getValue(RMS, 0, smoothAttack, smoothRelease);
    power   = audioAnalyzer.getValue(POWER, 0, smoothAttack, smoothRelease);
    pitchFreq = audioAnalyzer.getValue(PITCH_FREQ, 0, smoothAttack, smoothRelease);
    pitchConf = audioAnalyzer.getValue(PITCH_CONFIDENCE, 0, smoothAttack, smoothRelease);
    pitchSalience  = audioAnalyzer.getValue(PITCH_SALIENCE, 0, smoothAttack, smoothRelease);
    inharmonicity   = audioAnalyzer.getValue(INHARMONICITY, 0, smoothAttack, smoothRelease);
    hfc = audioAnalyzer.getValue(HFC, 0, smoothAttack, smoothRelease);
    specComp = audioAnalyzer.getValue(SPECTRAL_COMPLEXITY, 0, smoothAttack, smoothRelease);
    centroid = audioAnalyzer.getValue(CENTROID, 0, smoothAttack, smoothRelease);
    rollOff = audioAnalyzer.getValue(ROLL_OFF, 0, smoothAttack, smoothRelease);
    oddToEven = audioAnalyzer.getValue(ODD_TO_EVEN, 0, smoothAttack, smoothRelease);
    strongPeak = audioAnalyzer.getValue(STRONG_PEAK, 0, smoothAttack, smoothRelease);
    strongDecay = audioAnalyzer.getValue(STRONG_DECAY, 0, smoothAttack, smoothRelease);
    //Normalized values for graphic meters:
    pitchFreqNorm   = audioAnalyzer.getValue(PITCH_FREQ, 0, smoothAttack, smoothRelease, TRUE);
    hfcNorm     = audioAnalyzer.getValue(HFC, 0, smoothAttack, smoothRelease, TRUE);
    specCompNorm = audioAnalyzer.getValue(SPECTRAL_COMPLEXITY, 0, smoothAttack, smoothRelease, TRUE);
    centroidNorm = audioAnalyzer.getValue(CENTROID, 0, smoothAttack, smoothRelease, TRUE);
    rollOffNorm  = audioAnalyzer.getValue(ROLL_OFF, 0, smoothAttack, smoothRelease, TRUE);
    oddToEvenNorm   = audioAnalyzer.getValue(ODD_TO_EVEN, 0, smoothAttack, smoothRelease, TRUE);
    strongPeakNorm  = audioAnalyzer.getValue(STRONG_PEAK, 0, smoothAttack, smoothRelease, TRUE);
    strongDecayNorm = audioAnalyzer.getValue(STRONG_DECAY, 0, smoothAttack, smoothRelease, TRUE);
    
    dissonance = audioAnalyzer.getValue(DISSONANCE, 0, smoothAttack, smoothRelease);
    
    spectrum = audioAnalyzer.getValues(SPECTRUM, 0, smoothAttack, smoothRelease);
    melBands = audioAnalyzer.getValues(MEL_BANDS, 0, smoothAttack, smoothRelease);
    mfcc = audioAnalyzer.getValues(MFCC, 0, smoothAttack, smoothRelease);
    hpcp = audioAnalyzer.getValues(HPCP, 0, smoothAttack, smoothRelease);
    
    tristimulus = audioAnalyzer.getValues(TRISTIMULUS, 0, smoothAttack, smoothRelease);
    
    isOnset = audioAnalyzer.getOnsetValue(0);
  
   
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //-Single value Algorithms:
    
    ofPushMatrix();
    ofTranslate(350, 0);
    int mw = 250;
    int xpos = 0;
    int ypos = 30;
    
    float value, valueNorm;
    
    ofSetColor(255);
    value = rms;
    string strValue = "RMS: " + ofToString(value, 2);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, value * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = power;
    strValue = "Power: " + ofToString(value, 2);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, value * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = pitchFreq;
    valueNorm = pitchFreqNorm;
    strValue = "Pitch Frequency: " + ofToString(value, 2) + " hz.";
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, valueNorm * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = pitchConf;
    strValue = "Pitch Confidence: " + ofToString(value, 2);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, value * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = pitchSalience;
    strValue = "Pitch Salience: " + ofToString(value, 2);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, value * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = inharmonicity;
    strValue = "Inharmonicity: " + ofToString(value, 2);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, value * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = hfc;
    valueNorm = hfcNorm;
    strValue = "HFC: " + ofToString(value, 2);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, valueNorm * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = specComp;
    valueNorm = specCompNorm;
    strValue = "Spectral Complexity: " + ofToString(value, 2);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, valueNorm * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = centroid;
    valueNorm = centroidNorm;
    strValue = "Centroid: " + ofToString(value, 2);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, valueNorm * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = dissonance;
    strValue = "Dissonance: " + ofToString(value, 2);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, value * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = rollOff;
    valueNorm = rollOffNorm;
    strValue = "Roll Off: " + ofToString(value, 2);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, valueNorm * mw , 10);
    
    ypos += 50;
    ofSetColor(255);
    value = oddToEven;
    valueNorm = oddToEvenNorm;
    strValue = "Odd To Even Harmonic Energy Ratio: " + ofToString(value, 2);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, valueNorm * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = strongPeak;
    valueNorm = strongPeakNorm;
    strValue = "Strong Peak: " + ofToString(value, 2);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, valueNorm * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = strongDecay;
    valueNorm = strongDecayNorm;
    strValue = "Strong Decay: " + ofToString(value, 2);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, valueNorm * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = isOnset;
    strValue = "Onsets: " + ofToString(value);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, value * mw, 10);
    
    ofPopMatrix();
    
    //-Vector Values Algorithms:
    
    ofPushMatrix();
    
    ofTranslate(700, 0);
    
    int graphH = 75;
    int yoffset = graphH + 50;
    ypos = 30;
    
    ofSetColor(255);
    ofDrawBitmapString("Spectrum: ", 0, ypos);
    ofPushMatrix();
    ofTranslate(0, ypos);
    ofSetColor(ofColor::cyan);
    float bin_w = (float) mw / spectrum.size();
    for (int i = 0; i < spectrum.size(); i++){
        float scaledValue = ofMap(spectrum[i], DB_MIN, DB_MAX, 0.0, 1.0, true);//clamped value
        float bin_h = -1 * (scaledValue * graphH);
        ofDrawRectangle(i*bin_w, graphH, bin_w, bin_h);
    }
    ofPopMatrix();
    
    ypos += yoffset;
    ofSetColor(255);
    ofDrawBitmapString("Mel Bands: ", 0, ypos);
    ofPushMatrix();
    ofTranslate(0, ypos);
    ofSetColor(ofColor::cyan);
    bin_w = (float) mw / melBands.size();
    for (int i = 0; i < melBands.size(); i++){
        float scaledValue = ofMap(melBands[i], DB_MIN, DB_MAX, 0.0, 1.0, true);//clamped value
        float bin_h = -1 * (scaledValue * graphH);
        ofDrawRectangle(i*bin_w, graphH, bin_w, bin_h);
    }
    ofPopMatrix();
    
    ypos += yoffset;
    ofSetColor(255);
    ofDrawBitmapString("MFCC: ", 0, ypos);
    ofPushMatrix();
    ofTranslate(0, ypos);
    ofSetColor(ofColor::cyan);
    bin_w = (float) mw / mfcc.size();
    for (int i = 0; i < mfcc.size(); i++){
        float scaledValue = ofMap(mfcc[i], 0, MFCC_MAX_ESTIMATED_VALUE, 0.0, 1.0, true);//clamped value
        float bin_h = -1 * (scaledValue * graphH);
        ofDrawRectangle(i*bin_w, graphH, bin_w, bin_h);
    }
    ofPopMatrix();
    
    ypos += yoffset;
    ofSetColor(255);
    ofDrawBitmapString("HPCP: ", 0, ypos);
    ofPushMatrix();
    ofTranslate(0, ypos);
    ofSetColor(ofColor::cyan);
    bin_w = (float) mw / hpcp.size();
    for (int i = 0; i < hpcp.size(); i++){
        //float scaledValue = ofMap(hpcp[i], DB_MIN, DB_MAX, 0.0, 1.0, true);//clamped value
        float scaledValue = hpcp[i];
        float bin_h = -1 * (scaledValue * graphH);
        ofDrawRectangle(i*bin_w, graphH, bin_w, bin_h);
    }
    ofPopMatrix();
    
    ypos += yoffset;
    ofSetColor(255);
    ofDrawBitmapString("Tristimulus: ", 0, ypos);
    ofPushMatrix();
    ofTranslate(0, ypos);
    ofSetColor(ofColor::cyan);
    bin_w = (float) mw / tristimulus.size();
    for (int i = 0; i < tristimulus.size(); i++){
        //float scaledValue = ofMap(hpcp[i], DB_MIN, DB_MAX, 0.0, 1.0, true);//clamped value
        float scaledValue = tristimulus[i];
        float bin_h = -1 * (scaledValue * graphH);
        ofDrawRectangle(i*bin_w, graphH, bin_w, bin_h);
    }
    ofPopMatrix();
    

    ofPopMatrix();
    
    //-Gui & info:
    
    gui.draw();
    ofSetColor(255);
    ofDrawBitmapString("ofxAudioAnalyzer\n\nALL ALGORITHMS EXAMPLE", 10, 32);
    ofSetColor(ofColor::hotPink);
    ofDrawBitmapString("Keys 1-6: Play audio tracks", 10, 100);
    ofDrawBitmapString("Key 0: Live audio input", 10, 120);
    

    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    
    switch (key) {
        case '0':
            bAudioInput = true;
            player.stop();
            break;
        case '1':
            player.load("test440mono.wav");
            player.play();
            bAudioInput = false;
            break;
        case '2':
            player.load("flute.wav");
            player.play();
            bAudioInput = false;
            break;
        case '3':
            player.load("chord.wav");
            player.play();
            bAudioInput = false;
            break;
        case '4':
            player.load("cadence.wav");
            player.play();
            bAudioInput = false;
            break;
        case '5':
            player.load("beatTrack.wav");
            player.play();
            bAudioInput = false;
            break;
        case '6':
            player.load("noise.wav");
            player.play();
            bAudioInput = false;
            break;


        default:
            break;
    }
    
}
//--------------------------------------------------------------
void ofApp::exit(){
    audioAnalyzer.exit();
//    player.stop();
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
