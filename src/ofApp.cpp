#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetLogLevel(OF_LOG_VERBOSE);
    font.load("frabk.ttf", 14);
    settingsManager.loadSettings("MIDI_OSC_SETTINGS.json");
    appSettings = settingsManager.getSettings();
    NDIManager.setup();
    oscManager.setup();
    mapsManager.setup();
    
    ofBackground(255*.15);
    ofSetVerticalSync(true);
    ofEnableSmoothing();

    timeline.setup();

    timeline.addVideoTrack("fingers", "fingers.mov");

  #ifdef TIMELINE_AUDIO_INCLUDED
    timeline.addAudioTrack("audio", "4chan.wav");
    timeline.setDurationInSeconds(timeline.getAudioTrack("audio")->getDuration());
  #endif

    timeline.addCurves("curves", ofRange(0, 255));
    timeline.addBangs("bangs");
    timeline.addFlags("flags");
    timeline.addColors("colors");
    timeline.addLFO("lfo");
    timeline.addSwitches("switches");

    timeline.setPageName("Page 1");
    timeline.addPage("Page 2");
    timeline.addPage("Page 3");
    timeline.addPage("Page 4");
    timeline.setCurrentPage(0);

    timeline.enableSnapToOtherKeyframes(false);
    timeline.setLoopType(OF_LOOP_NORMAL);
    
    ofAddListener(timeline.events().bangFired, this, &ofApp::bangFired);

  
}

//--------------------------------------------------------------
void ofApp::update() {
    oscManager.handleIncomingMessages();
    mapsManager.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
    if(timeline.isSwitchOn("switches")){
        ofBackground(timeline.getColor("colors"));
    }
    else{
        ofBackground(255*.15);
    }
    timeline.draw();
    mapsManager.draw();
}
void ofApp::bangFired(ofxTLBangEventArgs& args){
    cout << "bang fired!" << args.flag << endl;
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
                settingsManager.saveSettings("MIDI_OSC_SETTINGS.json", appSettings);
                NDIManager.close();
                NDIManager.updateSettings();
                NDIManager.setup();
                break;

            case BTN_MSG_MIDI_IN_CHANNEL_SET:
                settingsManager.saveSettings("MIDI_OSC_SETTINGS.json", appSettings);
                NDIManager.updateSettings();
                break;

            case BTN_MSG_MIDI_OUT_PORT_SET:
                appSettings["midiOutDevice"] =  appSettings["outPortLabel"].get<string>();
                appSettings["midiOutDeviceByString"] =  true;
                settingsManager.saveSettings("MIDI_OSC_SETTINGS.json", appSettings);
                NDIManager.close();
                NDIManager.updateSettings();
                NDIManager.setup();
                
                break;

            case BTN_MSG_MIDI_OUT_CHANNEL_SET:
                settingsManager.saveSettings("MIDI_OSC_SETTINGS.json", appSettings);
                NDIManager.updateSettings();
                break;

            case BTN_MSG_OSC_IN_PORT_SET:
                oscManager.closeReceiver();
                settingsManager.saveSettings("MIDI_OSC_SETTINGS.json", appSettings);
                oscManager.updateSettings();
                oscManager.setupReceiver();

                break;

            case BTN_MSG_OST_OUT_PORT_SET:
                oscManager.closeSender();
                settingsManager.saveSettings("MIDI_OSC_SETTINGS.json", appSettings);
                oscManager.updateSettings();
                oscManager.setupSender();

                break;

            case BTN_MSG_OST_OUT_IP_SET:
                oscManager.closeSender();
                settingsManager.saveSettings("MIDI_OSC_SETTINGS.json", appSettings);
                oscManager.setupSender();
                break;

            
        }
    } else if (msg.message.substr(0, 3) != "BTN") {
        activityMessage = msg.message;
    }
}

//--------------------------------------------------------------
