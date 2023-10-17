//
//  TimelineManager.h
//  OSC_MIDI_OSC
//
//  Created by Fred Rodrigues on 02/10/2023.
//


#pragma once
#include <stdio.h>

#include "ofMain.h"
#include "SettingsManager.h" // Include the SettingsManager header
#include "ofxTextInputField.h"
#include "ofxTimeline.h"

class TimelineManager {
public:
    static TimelineManager& getInstance() {
        static TimelineManager instance; // Guaranteed to be destroyed, instantiated on first use.
        return instance;
    }

    // Delete copy constructor and assignment operator
    TimelineManager(TimelineManager const&) = delete;
    void operator=(TimelineManager const&) = delete;
    
    void setup();
    void update();
    void draw();
    void exit();

    // Public destructor
    ~TimelineManager();

    void updateSettings();
    void setTimelineManagerSettings(const ofJson& newSettings);
private:
    TimelineManager();
    ofxTimeline timeline;
    void bangFired(ofxTLBangEventArgs& args);

    ofJson TimelineManagerSettings;

    // Function to handle settings changes
    void handleSettingsChange(const ofJson& updatedSettings) {
        // Update your TimelineManagerSettings with the new settings
        TimelineManagerSettings = updatedSettings;
        // Perform any other actions you need to do when settings change
        // This function will be called automatically when settings change.
    }
};
