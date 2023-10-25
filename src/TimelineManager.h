//
//  TimelineManager.h
//  Synthetic Ornithology player
//
//  Created by Fred Rodrigues on 02/10/2023.
//


#pragma once
#include <stdio.h>

#include "ofMain.h"
#include "SettingsManager.h" // Include the SettingsManager header
#include "ofxTextInputField.h"
#include "ofxTimeline.h"
#include "MapsManager.h"
#include <iostream>
#include <ctime>
#include <chrono>


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
    void keyPressed(int key);

    
    // Public destructor
    ~TimelineManager();

    void updateSettings();
    void setTimelineManagerSettings(const ofJson& newSettings);
    void disableInteraction();
    void enableInteraction();
    std::string getSpecualtiveTime() {return specualtiveTime;};
    
    static float timelineProgress;
    static int mapZoom;

    
private:
    TimelineManager();
    ofxTimeline timeline;
    void bangFired(ofxTLBangEventArgs& args);
    ofxTLCameraTrack* cameraTrack;

    ofJson TimelineManagerSettings;
    
    std::string unixTimeToHumanReadable(long long unixTime);
    std::string specualtiveTime;

    // Function to handle settings changes
    void handleSettingsChange(const ofJson& updatedSettings) {
        // Update your TimelineManagerSettings with the new settings
        TimelineManagerSettings = updatedSettings;
        // Perform any other actions you need to do when settings change
        // This function will be called automatically when settings change.
    }
    
    
};

