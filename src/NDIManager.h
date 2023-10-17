//
//  NDIManager.h
//  Synthetic Ornithology player
//
//  Created by Fred Rodrigues on 02/10/2023.
//


#pragma once
#include <stdio.h>

#include "ofMain.h"
#include "ofxNdi.h"
#include "SettingsManager.h" // Include the SettingsManager header

class NDIManager {
public:
    static NDIManager& getInstance() {
        static NDIManager instance; // Guaranteed to be destroyed, instantiated on first use.
        return instance;
    }

    // Delete copy constructor and assignment operator
    NDIManager(NDIManager const&) = delete;
    void operator=(NDIManager const&) = delete;
    
    void setup();
    void update();
    void draw();
    void exit();

    // Public destructor
    ~NDIManager();

    void updateSettings();
    void setNDIManagerSettings(const ofJson& newSettings);
    
    ofxNDIsender& getMapSender() { return mapSender; }
    ofxNDIsender& getTextSender1() { return textSender1; }
    ofxNDIsender& getTextSender2() { return textSender2; }
    ofxNDIsender& getGraphicsSender() { return graphicsSender; }
    
private:
    NDIManager();

    ofJson NDIManagerSettings;
    
    ofxNDIsender mapSender;        // NDI sender
    string mapSenderName = "Map Sender";
    
    ofxNDIsender textSender1;        // NDI sender
    string textSenderName1 = "Text Sender 1";
    
    ofxNDIsender textSender2;        // NDI sender
    string textSenderName2 = "Text Sender 2";
    
    ofxNDIsender graphicsSender;        // NDI sender
    string graphicsSenderName = "Graphics Sender";
    

    // Function to handle settings changes
    void handleSettingsChange(const ofJson& updatedSettings) {
        // Update your NDIManagerSettings with the new settings
        NDIManagerSettings = updatedSettings;
        // Perform any other actions you need to do when settings change
        // This function will be called automatically when settings change.
    }
};

