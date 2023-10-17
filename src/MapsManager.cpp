//
//  MapsManager.cpp
//  OSC_MIDI_OSC
//
//  Created by Fred Rodrigues on 02/10/2023.
//

#include "MapsManager.h"

MapsManager::MapsManager() {
    
    
}
void MapsManager::setup(){
    
    ofLogNotice("MapsManager") << "MapsManager setup";
    // Get the instance of SettingsManager
    SettingsManager & settingsManager = SettingsManager::getInstance();
    
    // Access the settings
    MapsManagerSettings = settingsManager.getSettings();
    
    ofJson json = ofLoadJson("provider.json");
    ofLogVerbose("MapsManager") << "Loaded Provider JSON";
    
    tileProvider = std::make_shared<ofxMaps::MapTileProvider>(ofxMaps::MapTileProvider::fromJSON(json));
    ofLogVerbose("MapsManager") << "Created Provider";
    
    Poco::ThreadPool::defaultPool().addCapacity(64);
    bufferCache = std::make_shared<ofxMaps::MBTilesCache>(*tileProvider, "cache/");
    ofLogVerbose("MapsManager") << "Created Buffer cache";
    
    tileSet = std::make_shared<ofxMaps::MapTileSet>(1024,
                                                    tileProvider,
                                                    bufferCache);
    ofLogVerbose("MapsManager") << "Created Tile set";
    
    
    tileLayer = std::make_shared<ofxMaps::MapTileLayer>(tileSet, 600, 600);
    ofLogVerbose("MapsManager") << "Created Tile Layer";
    
    
    ofxGeo::Coordinate chicago(41.8827, -87.6233);
    ofLogVerbose("MapsManager") << "Created chicago";
    
    ofxGeo::Coordinate bethel(45.0579, -93.1605);
    ofLogVerbose("MapsManager") << "Created bethel";
    
    
    if (MapsManagerSettings["useVirtualPort"]) {
        
    } else {
        if (MapsManagerSettings["midiOutDevice"].is_string()) {
            
        }
        
        if (MapsManagerSettings["midiInDevice"].is_string()) {
            //Check if the port from the settings exists
            
        }
        
    }
    
    
    
    settingsManager.saveSettings("MIDI_OSC_SETTINGS.json", MapsManagerSettings);
    
    
    ofLogVerbose("MapsManager") << "MapsManager constructor called, midi out port: ";
    
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
    ofLogVerbose("MapsManager") << "tileLayer->setCenter";
    
    ofLogVerbose("MapsManager") << "Created Coordinates";
    
    fbo.allocate(600, 600, GL_RGBA);
    
    
}
void MapsManager::close(){
    
}

void MapsManager::update(){
    tileLayer->update();
    
    if (!ofIsFloatEqual(animation, 0.f))
        tileLayer->setCenter(tileLayer->getCenter().getNeighbor(animation, 0));
    
}
void MapsManager::draw(){
   
    fbo.begin();
    ofClear(0);
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
    fbo.draw(0, 400);

    
    ofDrawBitmapStringHighlight(tileLayer->getCenter().toString(0), 14, ofGetHeight() - 32);
    ofDrawBitmapStringHighlight("Task Queue:" + ofx::TaskQueue::instance().toString(), 14, ofGetHeight() - 16);
    ofDrawBitmapStringHighlight("Connection Pool: " + bufferCache->toString(), 14, ofGetHeight() - 2);
}

//destructors
MapsManager::~MapsManager() {
    
    ofLogVerbose("MapsManager") << "Destructor called" << endl;
}

void MapsManager::updateSettings(){
    SettingsManager & settingsManager = SettingsManager::getInstance();
    MapsManagerSettings = settingsManager.getSettings();
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
