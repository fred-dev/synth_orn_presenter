//
//  NDIManager.cpp
//  OSC_MIDI_OSC
//
//  Created by Fred Rodrigues on 02/10/2023.
//

#include "NDIManager.h"

NDIManager::NDIManager() {
    
}
void NDIManager::setup(){
    ofLogNotice("NDIManager") << "NDIManager setup";
    // Get the instance of SettingsManager
    SettingsManager & settingsManager = SettingsManager::getInstance();
    
    // Access the settings
    NDIManagerSettings = settingsManager.getSettings();
    
    if (NDIManagerSettings["useVirtualPort"]) {
        
    } else {
        if (NDIManagerSettings["midiOutDevice"].is_string()) {
            
        }
        
        if (NDIManagerSettings["midiInDevice"].is_string()) {
            //Check if the port from the settings exists
            
        }
        
    }

 

    settingsManager.saveSettings("MIDI_OSC_SETTINGS.json", NDIManagerSettings);
    
    
    ofLogVerbose("NDIManager") << "NDIManager constructor called, midi out port: ";
    
}
void NDIManager::close(){

}
//destructors
NDIManager::~NDIManager() {

	ofLogVerbose("NDIManager") << "Destructor called" << endl;
}

void NDIManager::updateSettings(){
    SettingsManager & settingsManager = SettingsManager::getInstance();
    NDIManagerSettings = settingsManager.getSettings();
}
