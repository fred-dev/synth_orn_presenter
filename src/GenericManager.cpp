//
//  GenericManager.cpp
//  OSC_MIDI_OSC
//
//  Created by Fred Rodrigues on 02/10/2023.
//

#include "GenericManager.h"

GenericManager::GenericManager() {
    updateSettings();
    ofLogVerbose("GenericManager") << "Constructor called" << endl;
}

GenericManager::~GenericManager() {
    
    ofLogVerbose("GenericManager") << "Destructor called" << endl;
}

void GenericManager::setup(){
    ofLogNotice("GenericManager") << "GenericManager setup";
    
    if (GenericManagerSettings["NDI"]["NDI_MAP_STREAM_LABEL"].is_string()) {
        
    }
    
    ofLogVerbose("GenericManager") << "GenericManager Setup complete";
    
}

void GenericManager::update(){
    
}

void GenericManager::draw(){
    
}

void GenericManager::exit(){
    ofLogVerbose("GenericManager") << "Exit";
}


void GenericManager::updateSettings() {
    // Connect a slot to listen for settings changes
    SettingsManager::getInstance().connectSettingsChanged([this](const ofJson& updatedSettings) {
        // Call the handleSettingsChange function in GenericManager
        handleSettingsChange(updatedSettings);
    });
}

void GenericManager::setGenericManagerSettings(const ofJson& updatedSettings) {
    // Merge the updated settings with the current GenericManagerSettings
    for (auto it = updatedSettings.begin(); it != updatedSettings.end(); ++it) {
        const std::string& key = it.key();
        const ofJson& newValue = it.value();
        
        // Check if the new value is different from the current value
        if (GenericManagerSettings[key] != newValue) {
            GenericManagerSettings[key] = newValue;
        }
    }
    
    // Update the main settings managed by the SettingsManager with the modified GenericManagerSettings
    SettingsManager::getInstance().updateSettings(GenericManagerSettings);
}


