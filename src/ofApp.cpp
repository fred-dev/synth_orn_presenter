#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(255*.15);
    ofEnableSmoothing();
    ofDisableArbTex();
    settingsManager.loadSettings("synth_orn_performer_settings.json");
    appSettings = settingsManager.getSettings();
    NDIManager.setup();
    oscManager.setup();
    mapsManager.setup();
    timelineManager.setup();
    textRenderManager.setup();
    
    ofLogVerbose() << "Main entry point all Setup complete";
    
    mode = 0;
    
}

//--------------------------------------------------------------
void ofApp::update() {
    oscManager.handleIncomingMessages();
    mapsManager.update();
    timelineManager.update();
    textRenderManager.update();
    
}

//--------------------------------------------------------------
void ofApp::draw() {
    if(mode == 0){
        timelineManager.draw();
        textRenderManager.draw();
        mapsManager.draw();
    }
       
       else{
        mapsManager.draw();
    }

}

//--------------------------------------------------------------
void ofApp::exit(){
    
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
        if(key=='1'){
            mode = 0;
            timelineManager.enableInteraction();
        }
        if(key=='2'){
            mode = 1;
            timelineManager.disableInteraction();
        }
        
        if(mode==0){
            mapsManager.keyPressed(key);
            timelineManager.keyPressed(key);
        }
        else{
            mapsManager.keyPressed(key);

        }
        

    
//    settingsManager.saveSettings();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
    if (msg.message.substr(0, 3) == "BTN" && buttonMessagesMap.find(msg.message) != buttonMessagesMap.end()) {
        
        ofLogVerbose() << "Button message received: " << msg.message;
        
        // Get the corresponding enum value from the map
        int buttonValue = buttonMessagesMap[msg.message];
        // Use a switch statement on the enum value
        switch (buttonValue) {
            case BTN_MSG_GOTOMODE_MODE_SETTING_MIDI_IN:
                break;
                
            case BTN_MSG_GOTOMODE_MODE_SETTING_MIDI_OUT:
                break;
                
            case BTN_MSG_GOTOMODE_MODE_SETTING_OSC_IN:
                break;
                
            case BTN_MSG_GOTOMODE_MODE_SETTING_OSC_OUT:
                break;
                
            case BTN_MSG_GOTOMODE_MODE_CONVERSION:
                break;
                
            case BTN_MSG_MIDI_IN_PORT_SET:
    
                NDIManager.exit();
                NDIManager.updateSettings();
                NDIManager.setup();
                break;
                
            case BTN_MSG_MIDI_IN_CHANNEL_SET:
                NDIManager.updateSettings();
                break;
                
            case BTN_MSG_MIDI_OUT_PORT_SET:
          
                NDIManager.exit();
                NDIManager.updateSettings();
                NDIManager.setup();
                
                break;
                
            case BTN_MSG_MIDI_OUT_CHANNEL_SET:
                NDIManager.updateSettings();
                break;
                
            case BTN_MSG_OSC_IN_PORT_SET:
                oscManager.closeReceiver();
                oscManager.updateSettings();
                oscManager.setupReceiver();
                
                break;
                
            case BTN_MSG_OST_OUT_PORT_SET:
                oscManager.closeSender();
                oscManager.updateSettings();
                oscManager.setupSender();
                
                break;
                
            case BTN_MSG_OST_OUT_IP_SET:
                oscManager.closeSender();
                oscManager.setupSender();
                break;
                
                
        }
    } else if (msg.message.substr(0, 3) != "BTN") {
        activityMessage = msg.message;
    }
}

//--------------------------------------------------------------
