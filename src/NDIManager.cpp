//
//  NDIManager.cpp
//  Synthetic Ornithology player
//
//  Created by Fred Rodrigues on 02/10/2023.
//

#include "NDIManager.h"

NDIManager::NDIManager() {
    updateSettings();
    ofLogVerbose("NDIManager") << "Constructor called" << endl;
}

NDIManager::~NDIManager() {
    
    ofLogVerbose("NDIManager") << "Destructor called" << endl;
}

void NDIManager::setup(){
    ofLogNotice("NDIManager") << "NDIManager setup";
    
    mapSender.SetAsync();
    mapSender.CreateSender(mapSenderName.c_str(), NDIManagerSettings["renderer"]["map_render_width"] , NDIManagerSettings["renderer"]["map_render_height"]);
    
    textSender1.SetAsync();
    textSender1.CreateSender(textSenderName1.c_str(), NDIManagerSettings["renderer"]["text_renderer_1_width"] , NDIManagerSettings["renderer"]["text_renderer_1_height"]);
    
    textSender2.SetAsync();
    textSender2.CreateSender(textSenderName2.c_str(), NDIManagerSettings["renderer"]["text_renderer_2_width"] , NDIManagerSettings["renderer"]["text_renderer_2_height"]);
    
    graphicsSender.SetAsync();
    graphicsSender.CreateSender(graphicsSenderName.c_str(), NDIManagerSettings["renderer"]["grapics_renderer_width"] , NDIManagerSettings["renderer"]["grapics_renderer_height"]);

    
    ofLogVerbose("NDIManager") << "NDIManager Setup complete";
    
}

void NDIManager::update(){
    
}

void NDIManager::draw(){
    
}

void NDIManager::exit(){
    ofLogVerbose("NDIManager") << "Exit";
    mapSender.ReleaseSender();
    textSender1.ReleaseSender();
    textSender2.ReleaseSender();
    graphicsSender.ReleaseSender();
}


void NDIManager::updateSettings() {
    // Connect a slot to listen for settings changes
    SettingsManager::getInstance().connectSettingsChanged([this](const ofJson& updatedSettings) {
        // Call the handleSettingsChange function in NDIManager
        handleSettingsChange(updatedSettings);
    });
}

void NDIManager::setNDIManagerSettings(const ofJson& updatedSettings) {
    // Merge the updated settings with the current NDIManagerSettings
    for (auto it = updatedSettings.begin(); it != updatedSettings.end(); ++it) {
        const std::string& key = it.key();
        const ofJson& newValue = it.value();
        
        // Check if the new value is different from the current value
        if (NDIManagerSettings["renderer"][key] != newValue) {
            NDIManagerSettings["renderer"][key] = newValue;
        }
    }
    
    // Update the main settings managed by the SettingsManager with the modified NDIManagerSettings
    SettingsManager::getInstance().updateSettings(NDIManagerSettings);
}


