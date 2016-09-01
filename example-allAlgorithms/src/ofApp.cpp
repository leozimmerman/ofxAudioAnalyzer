#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(34);
    ofSetFrameRate(60);
    
    sampleRate = 44100;
    bufferSize = 512;
    int channels = 1;
    
    audioAnalyzer.setup(sampleRate, bufferSize, channels);
    
    player.load("beatTrack.wav");
    
    gui.setup("Smoothing Values");
    gui.setPosition(20, 150);
    gui.add(rms_smth.setup  ("RMS", 0.0, 0.0, 1.0));
    gui.add(power_smth.setup("Power", 0.0, 0.0, 1.0));
    gui.add(pitchFreq_smth.setup("Pitch Frequency", 0.0, 0.0, 1.0));
    gui.add(pitchConf_smth.setup("Pitch Confidence", 0.0, 0.0, 1.0));
    gui.add(melodySalience_smth.setup("Melody salience", 0.0, 0.0, 1.0));
    gui.add(inharmonicity_smth.setup("Inharmonicity", 0.0, 0.0, 1.0));
    gui.add(hfc_smth.setup("HFC", 0.0, 0.0, 1.0));
    gui.add(specComp_smth.setup("Spectral Complexity", 0.0, 0.0, 1.0));
    gui.add(centroid_smth.setup("Centroid", 0.0, 0.0, 1.0));
    
    gui.add(dissonance_smth.setup("Dissonance", 0.0, 0.0, 1.0));
    
    gui.add(spectrum_smth.setup("Spectrum", 0.0, 0.0, 1.0));
    gui.add(melBands_smth.setup("Mel Bands", 0.0, 0.0, 1.0));
    gui.add(mfcc_smth.setup("MFCC", 0.0, 0.0, 1.0));
    gui.add(hpcp_smth.setup("HPCP", 0.0, 0.0, 1.0));
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    //Get buffer from sound player:
    soundBuffer = player.getCurrentSoundBuffer(bufferSize);
    
    //ANALYZE SOUNDBUFFER:
    audioAnalyzer.analyze(soundBuffer);
    
    rms     = audioAnalyzer.getValue(RMS, 0, rms_smth);
    power   = audioAnalyzer.getValue(POWER, 0, power_smth);
    pitchFreq = audioAnalyzer.getValue(PITCH_FREQ, 0, pitchFreq_smth);
    pitchConf = audioAnalyzer.getValue(PITCH_CONFIDENCE, 0, pitchConf_smth);
    pitchSalience  = audioAnalyzer.getValue(PITCH_SALIENCE, 0, melodySalience_smth);
    inharmonicity   = audioAnalyzer.getValue(INHARMONICITY, 0, inharmonicity_smth);
    hfc = audioAnalyzer.getValue(HFC, 0, hfc_smth);
    specComp = audioAnalyzer.getValue(SPECTRAL_COMPLEXITY, 0, specComp_smth);
    centroid = audioAnalyzer.getValue(SPECTRAL_COMPLEXITY, 0, centroid_smth);
    //Normalized values for graphic meters:
    hfcNorm = audioAnalyzer.getValue(HFC, 0, hfc_smth, true);
    specCompNorm = audioAnalyzer.getValue(SPECTRAL_COMPLEXITY, 0, specComp_smth, true);
    centroidNorm = audioAnalyzer.getValue(SPECTRAL_COMPLEXITY, 0, centroid_smth, true);
    
    dissonance = audioAnalyzer.getValue(DISSONANCE, 0, dissonance_smth);
    
    spectrum = audioAnalyzer.getValues(SPECTRUM, 0, spectrum_smth);
    melBands = audioAnalyzer.getValues(MEL_BANDS, 0, melBands_smth);
    mfcc = audioAnalyzer.getValues(MFCC, 0, mfcc_smth);
    hpcp = audioAnalyzer.getValues(HPCP, 0, hpcp_smth);
    
    multiPitches = audioAnalyzer.getValues(MULTI_PITCHES, 0);
    saliencePeaks = audioAnalyzer.getSalienceFunctionPeaks(0);
    
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
    string strValue = "RMS: " + ofToString(value);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, value * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = power;
    strValue = "Power: " + ofToString(value);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, value * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = pitchFreq;
    strValue = "Pitch Frequency: " + ofToString(value) + " hz.";
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, (ofClamp(value,0,2000.0)/2000.0) * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = pitchConf;
    strValue = "Pitch Confidence: " + ofToString(value);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, value * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = pitchSalience;
    strValue = "Pitch Salience: " + ofToString(value);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, value * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = inharmonicity;
    strValue = "Inharmonicity: " + ofToString(value);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, value * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = hfc;
    valueNorm = hfcNorm;
    strValue = "HFC: " + ofToString(value);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, valueNorm * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = specComp;
    valueNorm = specCompNorm;
    strValue = "Spectral Complexity: " + ofToString(value);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, valueNorm * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = centroid;
    valueNorm = centroidNorm;
    strValue = "Centroid: " + ofToString(value);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, valueNorm * mw, 10);
    
    ypos += 50;
    ofSetColor(255);
    value = dissonance;
    strValue = "Dissonance: " + ofToString(value);
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos, ypos+5, value * mw, 10);
    
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
    
    ypos += 100;
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
    
    ypos += 100;
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
    
    ypos += 100;
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
    
    ypos += 100;
    ofSetColor(255);
    ofDrawBitmapString("Pitch Salience Function Peaks: ", 0, ypos);
    ofPushMatrix();
    ofTranslate(0, ypos);
    ofSetColor(ofColor::cyan);
    bin_w = (float) mw / saliencePeaks.size();
    for (int i = 0; i < saliencePeaks.size(); i++){
        //float scaledValue = ofMap(saliencePeaks[i].value, DB_MIN, DB_MAX, 0.0, 1.0, true);//clamped value
        float scaledValue = saliencePeaks[i].value;
        float bin_h = -1 * (scaledValue * graphH);
        
        float maxCents = 600.0;
        int xpos = (saliencePeaks[i].bin / maxCents) * mw;
        
        ofDrawRectangle(xpos, graphH, bin_w, bin_h);
    }
    ofPopMatrix();
    
    ypos += 100;
    ofSetColor(255);
    ofDrawBitmapString("Multi Pitches: ", 0, ypos);
    ofPushMatrix();
    ofTranslate(0, ypos);
    ofSetColor(ofColor::cyan);
    bin_w = 5;//cte.
    for (int i = 0; i < multiPitches.size(); i++){
        float bin_h = -0.75 * graphH;
        float maxPitch = 2000.0;
        int xpos = (multiPitches[i] / maxPitch) * mw;
        ofDrawRectangle(xpos, graphH, bin_w, bin_h);
    }
    ofPopMatrix();
    
    ofPopMatrix();
    ///---------------------------------------------
    
    gui.draw();
    ofSetColor(255);
    ofDrawBitmapString("ofxAudioAnalyzer\n\nALL ALGORITHMS EXAMPLE", 10, 32);
    ofSetColor(ofColor::hotPink);
    ofDrawBitmapString("Keys 1-4: Play audio tracks", 10, 100);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key) {
       
        case '1':
            player.stop();
            player.load("beatTrack.wav");
            player.play();
            break;
        case '2':
            player.stop();
            player.load("flute.wav");
            player.play();
            break;
        case '3':
            player.stop();
            player.load("chord.wav");
            player.play();
            break;
        case '4':
            player.stop();
            player.load("cadence.wav");
            player.play();
            break;
            
        default:
            break;
    }
    
    
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
