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
    
    ofLog() << "Receiving OSC on port: 7123";
    
    ofLog() << "Send: /smooth/attack";
    ofLog() << "Send: /smooth/release";
    ofLog() << "Send: /sensitivity/a";
    ofLog() << "Send: /sensitivity/b";
    
    receiver.setup(7123);
    
    ofLog() << "Sending OSC on port: 8123";
    
    sender.setup("localhost", 8123);
    
    ofLog() << "Receive: /rms";
    ofLog() << "Receive: /power";
    ofLog() << "Receive: /peak";
    ofLog() << "Receive: /decay";
    ofLog() << "Receive: /band/a";
    ofLog() << "Receive: /band/b";
    
    
    gui.setup();
    gui.setPosition(10, 180);
    gui.setSize(180, 120);
    gui.setDefaultWidth(180);
    gui.add(smoothAttack.set  ("Smooth Attack", 0.0, 0.0, 1.0));
    gui.add(smoothRelease.set  ("Smooth Release", 0.0, 0.0, 1.0));
    gui.add(melbandASensi.set  ("A Sensitivity", 0.0, 0.0, 1.6));
    gui.add(melbandBSensi.set  ("B Sensitivity", 0.0, 0.0, 1.6));

    settings.sampleRate = sampleRate;
    settings.bufferSize = 512;
    settings.setInListener(this);
    settings.setOutListener(this);
    settings.numOutputChannels = 0;
    settings.numInputChannels = 1;
    
    sound.setup(settings);
    
    auto names = sound.getMatchingDevices("default");
    if(!names.empty()) {
        ofLog() << "Setting audio input device to " << names[0];
        sound.stop();
        settings.setInDevice(names[0]);
        sound.setup(settings);
        sound.start();
    } else {
        ofLog() << "Could not find audio input device ";
    }
    
    ofSetWindowShape(200, 310);
    ofSetWindowTitle("VJ Tool");
   
}

//--------------------------------------------------------------
void ofApp::sendMessage(string address, float value) {
    
    ofxOscMessage m;
    m.setAddress(address);
    m.addFloatArg(value);
    sender.sendMessage(m, false);
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer & buffer){
    
    if (bAudioInput) audioAnalyzer.analyze(buffer);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (!receiver.hasWaitingMessages()) bMessages = false;
    
    while(receiver.hasWaitingMessages()){
        
        
        bMessages = true;
        ofxOscMessage m;
        receiver.getNextMessage(m);
        float val =  m.getArgAsFloat(0);
        ofLog() << "Receiving " << m.getAddress();
        // check for mouse moved message
        if(m.getAddress() == "/smooth/attack") smoothAttack.set(val );
        if(m.getAddress() == "/smooth/release") smoothRelease.set( val );
        if(m.getAddress() == "/sensitivity/a") melbandASensi.set( val );
        if(m.getAddress() == "/sensitivity/b") melbandBSensi.set( val );
        
    }
    
    
    
    rms     = audioAnalyzer.getValue(RMS, 0, smoothAttack, smoothRelease);
    power   = audioAnalyzer.getValue(POWER, 0, smoothAttack, smoothRelease);
    strongPeak = audioAnalyzer.getValue(STRONG_PEAK, 0, smoothAttack, smoothRelease);
    strongDecay = audioAnalyzer.getValue(STRONG_DECAY, 0, smoothAttack, smoothRelease);
    strongPeakNorm  = audioAnalyzer.getValue(STRONG_PEAK, 0, smoothAttack, smoothRelease, TRUE) * 4;
    strongDecayNorm = audioAnalyzer.getValue(STRONG_DECAY, 0, smoothAttack, smoothRelease, TRUE);
    
    spectrum = audioAnalyzer.getValues(SPECTRUM, 0, smoothAttack, smoothRelease);
    melBands = audioAnalyzer.getValues(MEL_BANDS, 0, smoothAttack, smoothRelease);
    
    int melCountA = 0;
    int melCountB = 0;
    melbandA = 0.0;
    melbandB = 0.0;
    
    for (int i = 0; i < melBands.size(); i++){
        
        float scaledValue = ofMap(melBands[i], DB_MIN, DB_MAX, 0.0, 1.0, true);//clamped value
        
        if (i < melBands.size()/2) {
            melbandA += scaledValue;
            melCountA += 1;
        }
        if (i >= melBands.size()/2) {
            melbandB += scaledValue;
            melCountB += 1;
        }
    }
    
    melbandA /= (float)melCountA/2.0;
    melbandB /= (float)melCountB/2.0;
    
    melbandTrigA = triggerA.update(melbandA, melbandASensi);
    melbandTrigB = triggerB.update(melbandB, melbandBSensi);
    
    tristimulus = audioAnalyzer.getValues(TRISTIMULUS, 0, smoothAttack, smoothRelease);
    
    isOnset = audioAnalyzer.getOnsetValue(0);
    
    sendMessage("/rms", rms);
    sendMessage("/power", power);
    sendMessage("/peak", strongPeakNorm);
    sendMessage("/decay", strongDecayNorm);
    sendMessage("/band/a", melbandA);
    sendMessage("/band/b", melbandB);
    if (melbandTrigA) sendMessage("/trigger/a", melbandTrigA);
    if (melbandTrigB) sendMessage("/trigger/b", melbandTrigB);
    if (isOnset) sendMessage("/onset", 1.0);
  
   
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    int xpos = 10;
    int iter = 20;
    int ypos = 10;
    int hh = 10;
    int hhh = (float)hh * 0.8;
    int col = 60;
    int mw = 180 - col;
    
    
    ofPushMatrix();
    
    
    int graphH = 20;
    
    ofSetColor(ofColor::yellow);
    float bin_w = (float) (mw + col) / spectrum.size();
    
    for (int i = 0; i < spectrum.size(); i++){
        float scaledValue = ofMap(spectrum[i], DB_MIN, DB_MAX, 0.0, 1.0, true);//clamped value
        float bin_h = -1 * (scaledValue * graphH);
        ofDrawRectangle( (i*bin_w) + 10, graphH + 10, bin_w, bin_h);
    }
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    ofSetColor(ofColor::cyan, 200);
    bin_w = (float) (mw + col) / melBands.size();
    for (int i = 0; i < melBands.size(); i++){
        float scaledValue = ofMap(melBands[i], DB_MIN, DB_MAX, 0.0, 1.0, true);//clamped value
        float bin_h = -1 * (scaledValue * graphH);
        ofDrawRectangle( (i*bin_w) + 10, graphH + 10, bin_w, bin_h);
    }
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ypos += 40;
    
    float value, valueNorm;
    
    ofSetColor(255);
    value = rms;
    string strValue = "RMS: ";
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos + col, ypos - hhh, value * mw, hh);
    
    ypos += iter;
    ofSetColor(255);
    value = power;
    strValue = "Power: ";
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos + col, ypos - hhh, value * mw, hh);
    
    ypos += iter;
    ofSetColor(255);
    value = strongPeak;
    valueNorm = strongPeakNorm;
    strValue = "Peak: ";
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos + col, ypos - hhh, valueNorm * mw, hh);
    
    ypos += iter;
    ofSetColor(255);
    value = strongDecay;
    valueNorm = strongDecayNorm;
    strValue = "Decay: ";
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(xpos + col, ypos - hhh, valueNorm * mw, hh);
    
    ypos += iter;
    ofSetColor(255);
    value = isOnset;
    strValue = "Onsets: ";
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::cyan);
    if (value) ofDrawCircle(xpos + col + 5, ypos - hhh + 5, 5);
    
    
    
    
    ypos += iter;
    ofSetColor(255);
    value = melbandA;
    strValue = "Low: ";
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::hotPink);
    ofDrawRectangle(xpos + col, ypos - hhh, value * mw, hh);
    ofSetColor(ofColor::yellow);
    if (melbandTrigA) ofDrawCircle(190, ypos - hhh + 5, 5);
    
    ypos += iter;
    ofSetColor(255);
    value = melbandB;
    strValue = "High: ";
    ofDrawBitmapString(strValue, xpos, ypos);
    ofSetColor(ofColor::hotPink);
    ofDrawRectangle(xpos + col, ypos - hhh, value * mw, hh);
    ofSetColor(ofColor::yellow);
    if (melbandTrigB) ofDrawCircle(190, ypos - hhh + 5, 5);
    
    
    ypos+= iter + gui.getHeight();
    (bMessages) ? ofSetColor(ofColor::limeGreen) : ofSetColor(100);
    ofDrawRectangle(xpos, ypos, 8, 8);
    ofDrawBitmapString("7123", xpos + 20, ypos + 10);
    
    ofSetColor(ofColor::limeGreen);
    int extra = 70;
    ofDrawRectangle(xpos + mw - 20 + extra, ypos, 8, 8);
    ofDrawBitmapString("8123", xpos + mw - 60 + extra, ypos + 10);
    
    
    
    
    gui.draw();
    

    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    
}
//--------------------------------------------------------------
void ofApp::exit(){
    audioAnalyzer.exit();
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
