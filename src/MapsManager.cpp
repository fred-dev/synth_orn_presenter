//
//  MapsManager.cpp
//  Synthetic Ornithology player
//
//  Created by Fred Rodrigues on 02/10/2023.
//

#include "MapsManager.h"

MapsManager::MapsManager() {
    updateSettings();
    ofLogVerbose("MapsManager") << "Constructor called" << endl;
    
}

MapsManager::~MapsManager() {
    
    ofLogVerbose("MapsManager") << "Destructor called" << endl;
}

void MapsManager::setup(){
    ofLogNotice("MapsManager") << "MapsManager setup";
    
    ofJson json = ofLoadJson("provider.json");
    tileProvider = std::make_shared<ofxMaps::MapTileProvider>(ofxMaps::MapTileProvider::fromJSON(json));
    
    Poco::ThreadPool::defaultPool().addCapacity(64);
    bufferCache = std::make_shared<ofxMaps::MBTilesCache>(*tileProvider, "cache/");
    
    tileSet = std::make_shared<ofxMaps::MapTileSet>(1024,
                                                    tileProvider,
                                                    bufferCache);
    tileLayer = std::make_shared<ofxMaps::MapTileLayer>(tileSet, 600, 600);

    ofLogVerbose("MapsManager") << "MapsManager constructor called, midi out port: ";
    
    fbo.allocate(600, 600, GL_RGBA);
    coordinates =
    {
        { 42.2610492, -91.4501953 },
        { 43.0046471, -90.4833984 },
        { 43.0367759, -89.3847656 },
        { 41.9676592, -88.4619141 },
        { 41.2117215, -89.0332031 },
        { 40.5805847, -90.1318359 },
        { 40.6806380, -91.1865234 },
        { 41.1124688, -92.4169922 },
        { 42.1959688, -93.2958984 },
        { 43.2932003, -92.1972656 },
        { 44.0560117, -90.7470703 }
    };
    
    tileLayer->setCenter(coordinates[3], 21);
    
    ofLogVerbose("MapsManager") << "MapsManager Setup complete";
    
}

void MapsManager::update(){
    tileLayer->update();
    
    if (!ofIsFloatEqual(animation, 0.f))
        tileLayer->setCenter(tileLayer->getCenter().getNeighbor(animation, 0));
    
    fbo.begin();
    tileLayer->draw(0, 0);
    ofPushStyle();
    ofNoFill();
    ofSetColor(0, 255, 0);
    
    for (auto coordinate: coordinates)
    {
        auto tc = tileLayer->geoToPixels(coordinate);
        ofDrawCircle(tc.x, tc.y, 10);
    }
    ofPopStyle();
    fbo.end();
    ndiManager.getMapSender().SendImage(fbo);
}

void MapsManager::draw(){
    
    fbo.draw(ofGetWidth() -600, ofGetHeight()-600);
    
    
    
    ofDrawBitmapStringHighlight(tileLayer->getCenter().toString(0), 14, ofGetHeight() - 32);
    ofDrawBitmapStringHighlight("Task Queue:" + ofx::TaskQueue::instance().toString(), 14, ofGetHeight() - 16);
    ofDrawBitmapStringHighlight("Connection Pool: " + bufferCache->toString(), 14, ofGetHeight() - 2);
}

void MapsManager::exit(){
    ofLogVerbose("MapsManager") << "Exit";
    
}


void MapsManager::updateSettings() {
    // Connect a slot to listen for settings changes
    SettingsManager::getInstance().connectSettingsChanged([this](const ofJson& updatedSettings) {
        // Call the handleSettingsChange function in MapsManager
        handleSettingsChange(updatedSettings);
    });
}

void MapsManager::setMapsManagerSettings(const ofJson& updatedSettings) {
    // Merge the updated settings with the current MapsManagerSettings
    for (auto it = updatedSettings.begin(); it != updatedSettings.end(); ++it) {
        const std::string& key = it.key();
        const ofJson& newValue = it.value();
        
        // Check if the new value is different from the current value
        if (MapsManagerSettings[key] != newValue) {
            MapsManagerSettings[key] = newValue;
        }
    }
    
    // Update the main settings managed by the SettingsManager with the modified MapsManagerSettings
    SettingsManager::getInstance().updateSettings(MapsManagerSettings);
}


void MapsManager::keyPressed(int key){
    
    if (key == 'f' || key == 'F')
    {
        ofToggleFullscreen();
    }
    else if (key == '-')
    {
        tileLayer->setCenter(tileLayer->getCenter().getZoomedBy(-1));
    }
    else if (key == '=')
    {
        tileLayer->setCenter(tileLayer->getCenter().getZoomedBy(1));
    }
    else if (key == 'w')
    {
        tileLayer->setCenter(tileLayer->getCenter().getNeighborUp());
    }
    else if (key == 'a')
    {
        tileLayer->setCenter(tileLayer->getCenter().getNeighborLeft());
    }
    else if (key == 's')
    {
        tileLayer->setCenter(tileLayer->getCenter().getNeighborDown());
    }
    else if (key == 'd')
    {
        tileLayer->setCenter(tileLayer->getCenter().getNeighborRight());
    }
    else if (key == '1')
    {
        animation -= 0.01;;
    }
    else if (key == '2')
    {
        animation += 0.01;;
    }
    else if (key == '3')
    {
        animation = 0;
    }
    
}
