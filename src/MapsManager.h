//
//  MapsManager.h
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
#include "ofxMaps.h"


class MapsManager {
public:
    static MapsManager& getInstance() {
        static MapsManager instance; // Guaranteed to be destroyed, instantiated on first use.
        return instance;
    }

    // Delete copy constructor and assignment operator
    MapsManager(MapsManager const&) = delete;
    void operator=(MapsManager const&) = delete;
    void setup();
    void close();
    void update();
    void draw();
    void keyPressed(int key);
    
    // Send MIDI message
  

    //public destructor
	~MapsManager();

    void updateSettings();

private:
    MapsManager();
    
    ofJson MapsManagerSettings;
    ofFbo fbo;

    std::shared_ptr<ofxMaps::MBTilesCache> bufferCache;
    std::shared_ptr<ofxMaps::MapTileLayer> tileLayer;
    std::shared_ptr<ofxMaps::MapTileSet> tileSet;
    std::shared_ptr<ofxMaps::MapTileProvider> tileProvider;

    std::vector<ofxGeo::Coordinate> coordinates;

    std::vector<std::string> sets;
    int setsIndex = 0;

    float animation = 0;

};

