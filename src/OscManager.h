//
//  OSCManager.h
//  OSC_MIDI_OSC
//
//  Created by Fred Rodrigues on 02/10/2023.
//

#pragma once
#include "ofMain.h"
#include "ofxOsc.h"
#include "OscManager.h"
#include "SettingsManager.h" // Include the SettingsManager header

class OscManager {
public:
    static OscManager& getInstance() {
        static OscManager instance; // Guaranteed to be destroyed, instantiated on first use.
        return instance;
    }

    // Delete copy constructor and assignment operator
    OscManager(OscManager const&) = delete;
    void operator=(OscManager const&) = delete;
    
    void setup();
    void update();
    void draw();
    void exit();

    // Public destructor
    ~OscManager();

    void updateSettings();
    void setOscManagerSettings(const ofJson& newSettings);
    ofxOscSender& getOSCSender() { return oscSender; }
    ofxOscReceiver& getOscReciever() { return oscReceiver; }
    
    void closeReceiver();
    void setupReceiver();
    void closeSender();
    void setupSender();
    void handleIncomingMessages();

    
private:
    OscManager();

    ofJson OscManagerSettings;
    string message;
    ofxOscSender oscSender;
    ofxOscReceiver oscReceiver;
    // Function to handle settings changes
    void handleSettingsChange(const ofJson& updatedSettings) {
        
        // Update your OscManagerSettings with the new settings
        OscManagerSettings = updatedSettings;
        // Perform any other actions you need to do when settings change
        // This function will be called automatically when settings change.
    }
};

