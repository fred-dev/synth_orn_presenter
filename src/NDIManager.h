//
//  NDIManager.h
//  OSC_MIDI_OSC
//
//  Created by Fred Rodrigues on 02/10/2023.
//


#pragma once
#include <stdio.h>

#include "ofMain.h"
#include "ofxNdi.h"
#include "SettingsManager.h" // Include the SettingsManager header
#include "OscManager.h"

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
    void close();

    //public destructor
	~NDIManager();

    void updateSettings();

private:
    NDIManager();
    
    ofJson NDIManagerSettings;

};

