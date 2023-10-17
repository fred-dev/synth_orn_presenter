//
//  MapsManager.h
//  Synthetic Ornithology player
//
//  Created by Fred Rodrigues on 02/10/2023.
//


#pragma once
#include <stdio.h>

#include "ofMain.h"
#include "SettingsManager.h" // Include the SettingsManager header
#include "NDIManager.h"
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
    void update();
    void draw();
    void exit();
    void keyPressed(int key);

    // Public destructor
    ~MapsManager();

    void updateSettings();
    void setMapsManagerSettings(const ofJson& newSettings);
private:
    MapsManager();

    ofJson MapsManagerSettings;

    // Function to handle settings changes
    void handleSettingsChange(const ofJson& updatedSettings) {
        // Update your MapsManagerSettings with the new settings
        MapsManagerSettings = updatedSettings;
        // Perform any other actions you need to do when settings change
        // This function will be called automatically when settings change.
    }
    
    ofFbo fbo;

    std::shared_ptr<ofxMaps::MBTilesCache> bufferCache;
    std::shared_ptr<ofxMaps::MapTileLayer> tileLayer;
    std::shared_ptr<ofxMaps::MapTileSet> tileSet;
    std::shared_ptr<ofxMaps::MapTileProvider> tileProvider;

    std::vector<ofxGeo::Coordinate> coordinates;

    std::vector<std::string> sets;
    
    int setsIndex = 0;

    float animation = 0;
    
    NDIManager& ndiManager = NDIManager::getInstance();

};

