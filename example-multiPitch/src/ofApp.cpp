#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(34);
    ofSetFrameRate(60);
    
    sampleRate = 44100;
    bufferSize = 512;
    int channels = 1;
    
    audioAnalyzer.setup(sampleRate, bufferSize, channels);
    //Multi pitch algorithms are deactived by default.
    audioAnalyzer.setActive(0, MULTI_PITCHES, TRUE);
    audioAnalyzer.setActive(0, PITCH_SALIENCE_FUNC_PEAKS, TRUE);
    

    player.load("test440mono.wav");
    
    gui.setup();
    gui.setPosition(20, 150);
    gui.add(smoothing.setup("Smoothing", 0.0, 0.0, 1.0));
    gui.add(maxPeaks.setup("Max Peaks", 40, 1, 50));
    gui.add(valueTreshold.setup("Value Treshold", 0.0, 0.0, 0.5));
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    //Get buffer from sound player:
    soundBuffer = player.getCurrentSoundBuffer(bufferSize);
    
    //ANALYZE SOUNDBUFFER:
    
    audioAnalyzer.setSalienceFunctionPeaksParameters(0, maxPeaks);
    
    audioAnalyzer.analyze(soundBuffer);
    saliencePeaks = audioAnalyzer.getSalienceFunctionPeaks(0, smoothing);
    multiPitches = audioAnalyzer.getValues(MULTI_PITCHES, 0);
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofPushStyle();
    
    for (int i = 0; i < saliencePeaks.size(); i++){
        
        if(saliencePeaks[i].value < valueTreshold)
            break;
        
        int x = (saliencePeaks[i].bin / 500) * ofGetWidth();
//        int x = (i/(float)saliencePeaks.size()) * ofGetWidth();
        int y = ofGetHeight();
        int w = 15;
        int h = saliencePeaks[i].value * ofGetHeight() * -1;
        
        ofSetColor(ofColor::cyan);
        ofDrawRectangle(x,y,w,h);
        
        ofSetColor(ofColor::hotPink);
        ofDrawBitmapString(ofToString(saliencePeaks[i].bin) + " cents\n" + ofToString(saliencePeaks[i].value, 2)  , x, y+h-30);
    }
    ofPopStyle();
    
    int xpos = ofGetWidth()/2;
    int ypos = 200;
    
    ofSetColor(255);
    ofDrawBitmapString("MultiPitches: ", xpos, ypos);
    ofPushMatrix();
    ofTranslate(0, ypos);
    ofSetColor(ofColor::cyan);
    for (int i = 0; i < multiPitches.size(); i++){
        int y =  5 + 15*i;
        float pitchVal = multiPitches[i] / 2000.0;
        //float pitchVal = 1;
        ofDrawRectangle(xpos, y, pitchVal* ofGetWidth()/2, 10);
    }
    ofPopMatrix();
    
    
    //Value Treshold
    ofSetColor(ofColor::yellow);
    int y = ofGetHeight() + valueTreshold * ofGetHeight() * -1;
    ofDrawLine(0, y, ofGetWidth(), y);
    
    //-Gui & info:
    
    gui.draw();
    ofSetColor(255);
    ofDrawBitmapString("ofxAudioAnalyzer\n\nMULTI PITCH EXAMPLE", 10, 32);
    ofSetColor(ofColor::hotPink);
    ofDrawBitmapString("Keys 1-5: Play audio tracks", 10, 100);

    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    player.stop();
    
    switch (key) {
       
        case '1':
            player.load("test440mono.wav");
            break;
        case '2':
            player.load("chordC.wav");
            break;
        case '3':
            player.load("chord.wav");
            break;
        case '4':
            player.load("cadence.wav");
            break;
        case '5':
            player.load("beethoven.wav");
            break;
        case '6':
            player.load("dosFlautas.wav");
            break;
        case '7':
            player.load("gtr2.wav");
            break;
            
        default:
            break;
    }
    
    player.play();
    
    
}
//--------------------------------------------------------------
void ofApp::exit(){
    audioAnalyzer.exit();
    player.stop();
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
