#pragma once

#include "ofMain.h"
#include "SettingsManager.h"
#include "NDIManager.h"
#include "OscManager.h"
#include "MapsManager.h"
#include "TimelineManager.h"
#include "colours.h"



enum OperationMode {
    MODE_SETTING_MIDI_IN,
    MODE_SETTING_MIDI_OUT,
    MODE_SETTING_OSC_IN,
    MODE_SETTING_OSC_OUT,
    MODE_CONVERSION
};

enum buttonMesssages {
    BTN_MSG_MIDI_IN_PORT_SET,
    BTN_MSG_MIDI_IN_CHANNEL_SET,
    BTN_MSG_MIDI_OUT_PORT_SET,
    BTN_MSG_MIDI_OUT_CHANNEL_SET,
    BTN_MSG_OSC_IN_PORT_SET,
    BTN_MSG_OST_OUT_PORT_SET,
    BTN_MSG_OST_OUT_IP_SET,
    BTN_MSG_GOTOMODE_MODE_SETTING_MIDI_IN,
    BTN_MSG_GOTOMODE_MODE_SETTING_MIDI_OUT,
    BTN_MSG_GOTOMODE_MODE_SETTING_OSC_IN,
    BTN_MSG_GOTOMODE_MODE_SETTING_OSC_OUT,
    BTN_MSG_GOTOMODE_MODE_CONVERSION
};



class ofApp : public ofBaseApp {

    public:
		void setup();
		void update();
		void draw();
        void exit();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void gotMessage(ofMessage msg);
    
        SettingsManager& settingsManager = SettingsManager::getInstance();
        NDIManager& NDIManager = NDIManager::getInstance();
        OscManager& oscManager = OscManager::getInstance();
        MapsManager& mapsManager = MapsManager::getInstance();
        TimelineManager& timelineManager = TimelineManager::getInstance();
    
    
        ofJson appSettings;
        
        std::string activityMessage;
    
    ofTrueTypeFont font;
    // Helper function to handle port selection

    
    std::unordered_map<std::string, int> buttonMessagesMap = {
        {"BTN_MSG_MIDI_IN_PORT_SET", 0},
        {"BTN_MSG_MIDI_IN_CHANNEL_SET", 1},
        {"BTN_MSG_MIDI_OUT_PORT_SET", 2},
        {"BTN_MSG_MIDI_OUT_CHANNEL_SET", 3},
        {"BTN_MSG_OSC_IN_PORT_SET", 4},
        {"BTN_MSG_OST_OUT_PORT_SET", 5},
        {"BTN_MSG_OST_OUT_IP_SET", 6},
        {"BTN_MSG_GOTOMODE_MODE_SETTING_MIDI_IN", 7},
        {"BTN_MSG_GOTOMODE_MODE_SETTING_MIDI_OUT", 8},
        {"BTN_MSG_GOTOMODE_MODE_SETTING_OSC_IN", 9},
        {"BTN_MSG_GOTOMODE_MODE_SETTING_OSC_OUT", 10},
        {"BTN_MSG_GOTOMODE_MODE_CONVERSION", 11}
    };
	
  Colours colours;
    
    

};

