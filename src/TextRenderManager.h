//
//  TextRenderManager.h
//  Synthetic Ornithology player
//
//  Created by Fred Rodrigues on 02/10/2023.
//


#pragma once
#include <stdio.h>

#include "ofMain.h"
#include "SettingsManager.h" // Include the SettingsManager header
#include "TimelineManager.h"
#include "NDIManager.h"
#include "MapsManager.h"

class TextRenderManager {
public:
    static TextRenderManager& getInstance() {
        static TextRenderManager instance; // Guaranteed to be destroyed, instantiated on first use.
        return instance;
    }

    // Delete copy constructor and assignment operator
    TextRenderManager(TextRenderManager const&) = delete;
    void operator=(TextRenderManager const&) = delete;
    
    void setup();
    void update();
    void draw();
    void exit();
    
    std::string speculativeDate;
    // Public destructor
    ~TextRenderManager();

    void updateSettings();
    void setTextRenderManagerSettings(const ofJson& newSettings);
private:
    TextRenderManager();

    ofJson TextRenderManagerSettings;
    ofTrueTypeFont smallFont, mediumFont, largeFont;
    ofFbo textFbo1, textFbo2;
    std::string formatFloat(float value, int precision);

    // Function to handle settings changes
    void handleSettingsChange(const ofJson& updatedSettings) {
        // Update your TextRenderManagerSettings with the new settings
        TextRenderManagerSettings = updatedSettings;
        // Perform any other actions you need to do when settings change
        // This function will be called automatically when settings change.
    }
};

