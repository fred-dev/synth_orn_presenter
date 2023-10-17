#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetLogLevel(OF_LOG_VERBOSE);
    font.load("frabk.ttf", 14);
    ofBackground(255*.15);
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    
    settingsManager.loadSettings("MIDI_OSC_SETTINGS.json");
    appSettings = settingsManager.getSettings();
    NDIManager.setup();
    oscManager.setup();
    mapsManager.setup();
    timelineManager.setup();
}

//--------------------------------------------------------------
void ofApp::update() {
    oscManager.handleIncomingMessages();
    mapsManager.update();
    timelineManager.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
    timelineManager.draw();
    
    mapsManager.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){

}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    mapsManager.keyPressed(key);
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
                appSettings["midiInDevice"] =  appSettings["inPortLabel"].get<string>();
                appSettings["midiInDeviceByString"] =  true;
                NDIManager.exit();
                NDIManager.updateSettings();
                NDIManager.setup();
                break;

            case BTN_MSG_MIDI_IN_CHANNEL_SET:
                NDIManager.updateSettings();
                break;

            case BTN_MSG_MIDI_OUT_PORT_SET:
                appSettings["midiOutDevice"] =  appSettings["outPortLabel"].get<string>();
                appSettings["midiOutDeviceByString"] =  true;
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
