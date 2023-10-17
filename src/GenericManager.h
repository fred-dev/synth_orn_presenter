//
//  GenericManager.h
//  Synthetic Ornithology player
//
//  Created by Fred Rodrigues on 02/10/2023.
//


#pragma once
#include <stdio.h>

#include "ofMain.h"
#include "SettingsManager.h" // Include the SettingsManager header

class GenericManager {
public:
    static GenericManager& getInstance() {
        static GenericManager instance; // Guaranteed to be destroyed, instantiated on first use.
        return instance;
    }

    // Delete copy constructor and assignment operator
    GenericManager(GenericManager const&) = delete;
    void operator=(GenericManager const&) = delete;
    
    void setup();
    void update();
    void draw();
    void exit();

    // Public destructor
    ~GenericManager();

    void updateSettings();
    void setGenericManagerSettings(const ofJson& newSettings);
private:
    GenericManager();

    ofJson GenericManagerSettings;

    // Function to handle settings changes
    void handleSettingsChange(const ofJson& updatedSettings) {
        // Update your GenericManagerSettings with the new settings
        GenericManagerSettings = updatedSettings;
        // Perform any other actions you need to do when settings change
        // This function will be called automatically when settings change.
    }
};

