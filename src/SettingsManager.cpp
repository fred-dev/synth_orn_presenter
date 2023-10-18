//
//  SettingsManager.cpp
//  Synthetic Ornithology player
//
//  Created by Fred Rodrigues on 02/10/2023.
//

#include "SettingsManager.h"

SettingsManager::SettingsManager() {
    // Constructor code
    setDefaultSettings();
    cout<<"Settings constructor"<< endl;
}


void SettingsManager::loadSettings(const std::string& filename) {
    settingsPath = filename;
    if (ofFile::doesFileExist(settingsPath)) {
        settings = ofLoadJson(settingsPath);
        loadSettingsFromJSON("timeline","timeline_years");
        loadSettingsFromJSON("timeline","start_time_unix");
        loadSettingsFromJSON("timeline","main_duration_seconds");
        
        settings["timeline"]["end_time_unix"] = settings["timeline"]["start_time_unix"].get<long>() + (31536000 * settings["timeline"]["timeline_years"].get<long>());
        ofLogVerbose("loadSettings") << "end_time_unix set to " << settings["timeline"]["end_time_unix"] << endl;
        
        loadSettingsFromJSON("render","map_render_width");
        loadSettingsFromJSON("render","map_render_height");
        loadSettingsFromJSON("render","text_renderer_1_width");
        loadSettingsFromJSON("render","text_renderer_1_height");
        loadSettingsFromJSON("render","text_renderer_2_width");
        loadSettingsFromJSON("render","text_renderer_2_height");
        loadSettingsFromJSON("render","grapics_renderer_width");
        loadSettingsFromJSON("render","grapics_renderer_height");
        loadSettingsFromJSON("render","font_path");
        loadSettingsFromJSON("render","font_size_small");
        loadSettingsFromJSON("render","font_size_medium");
        loadSettingsFromJSON("render","font_size_large");
        loadSettingsFromJSON("render","font_path");
        
        if (!settings["Log_level"].is_null()) {
            ofLogVerbose("loadSettings") << "Log level set to " << settings["Log_level"] << endl;
            ofSetLogLevel(settings["Log_level"]);
        } else {
            settings["Log_level"] = OF_LOG_VERBOSE;
            ofLogVerbose("loadSettings fallback") << "Log level set to " << settings["Log_level"] << endl;
        }
        
    
        //set the incomingPortOsc from the settings
        if (!settings["osc"]["incomingPortOsc"].is_null()) {
            ofLogVerbose("loadSettings") << "incomingPortOsc set to " << settings["osc"]["incomingPortOsc"] << endl;
        } else {
            settings["osc"]["incomingPortOsc"] = 12345;
            ofLogVerbose("loadSettings fallback") << "incomingPortOsc set to " << settings["osc"]["incomingPortOsc"] << endl;
        }
        
        if (!settings["osc"]["outGoingPortOsc"].is_null()) {
            ofLogVerbose("loadSettings") << "outGoingPortOsc set to " << settings["osc"]["outGoingPortOsc"] << endl;
        } else {
            settings["osc"]["outGoingPortOsc"] = 1234;
            ofLogVerbose("loadSettings fallback") << "outGoingPortOsc set to " << settings["osc"]["outGoingPortOsc"] << endl;
        }
        
        if (!settings["osc"]["outgoingIpOSC"].is_null()) {
            ofLogVerbose("loadSettings") << "outgoingIpOSC set to " << settings["osc"]["outgoingIpOSC"] << endl;
        } else {
            settings["osc"]["outgoingIpOSC"] = "127.0.0.1";
            ofLogVerbose("loadSettings fallback") << "outgoingIpOSC set to " << settings["osc"]["outgoingIpOSC"] << endl;
        }
    } else {
        ofLogError("SettingsManager::LoadSettings") << "Settings file not found: " << filename;
        setDefaultSettings();
    }
    notifySettingsChanged(); // Notify all connected slots that settings have changed
    
}

ofJson SettingsManager::getSettings() const {
    return settings;
}

void SettingsManager::setDefaultSettings() {
    ofLogVerbose("SettingsManager::setDefaultSettings") << "Loading defualt settings" << endl;
    
    settings["Log_level"] = OF_LOG_VERBOSE;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "Log level set to " << settings["Log_level"] << endl;
    
    settings["timeline"]["timeline_years"] = 1;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "timeline_years set to " << settings["timeline"]["timeline_years"]  << endl;
    
    settings["timeline"]["start_time_unix"] = ofGetUnixTime();
    ofLogVerbose("SettingsManager::setDefaultSettings") << "start_time_unix set to " << settings["timeline"]["start_time_unix"]  << endl;
    
    settings["timeline"]["end_time_unix"] = ( 31536000 * settings["timeline"]["timeline_years"].get<int>() )+ settings["timeline"]["start_time_unix"].get<long>();
    ofLogVerbose("SettingsManager::setDefaultSettings") << "end_time_unix set to " << settings["timeline"]["end_time_unix"]  << endl;
    
    settings["timeline"]["main_duration_seconds"] = 6000;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "main_duration_seconds set to " << settings["timeline"]["main_duration_seconds"]  << endl;
    
    settings["renderer"]["map_render_width"] = 600;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "map_render_width set to " << settings["renderer"]["map_render_width"]  << endl;
    
    settings["renderer"]["map_render_height"] = 600;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "map_render_height set to " << settings["renderer"]["map_render_height"]  << endl;
    
    settings["renderer"]["text_renderer_1_width"] = 1000;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "text_renderer_1_width set to " << settings["renderer"]["text_renderer_1_width"]  << endl;
    
    settings["renderer"]["text_renderer_1_height"] = 200;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "text_renderer_1_height set to " << settings["renderer"]["text_renderer_1_height"]  << endl;
    
    settings["renderer"]["text_renderer_2_width"] = 1000;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "text_renderer_2_width set to " << settings["renderer"]["text_renderer_2_width"]  << endl;
    
    settings["renderer"]["text_renderer_2_height"] = 200;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "text_renderer_2_height set to " << settings["renderer"]["text_renderer_2_height"]  << endl;
    
    settings["renderer"]["grapics_renderer_width"] = 1000;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "grapics_renderer_width set to " << settings["renderer"]["grapics_renderer_width"]  << endl;
    
    settings["renderer"]["grapics_renderer_height"] = 200;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "text_renderer_2_height set to " << settings["renderer"]["grapics_renderer_height"]  << endl;
    
    settings["renderer"]["font_path"] = "fonts/frabk.ttf";
    ofLogVerbose("SettingsManager::setDefaultSettings") << "font_path set to " << settings["renderer"]["font_path"]  << endl;
    
    settings["renderer"]["font_size_small"] = 8;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "font_size_small set to " << settings["renderer"]["font_size_small"]  << endl;
    
    settings["renderer"]["font_size_medium"] = 12;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "font_size_medium set to " << settings["renderer"]["font_size_medium"]  << endl;
    
    settings["renderer"]["font_size_large"] = 16;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "font_size_large set to " << settings["renderer"]["font_size_large"]  << endl;
    
    settings["osc"]["incomingPortOsc"] = 12345;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "incomingPortOsc set to " << settings["osc"]["incomingPortOsc"] << endl;
    
    settings["osc"]["outGoingPortOsc"] = 1234;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "outGoingPortOsc set to " << settings["osc"]["outGoingPortOsc"] << endl;
    
    settings["osc"]["outgoingIpOSC"] = "127.0.0.1";
    ofLogVerbose("SettingsManager::setDefaultSettings") << "outgoingIpOSC set to " << settings["osc"]["outgoingIpOSC"] << endl;
    
}

void SettingsManager::updateSettings(ofJson remoteSettings) {
    settings = remoteSettings;
    saveSettings(); // Save the updated settings
    notifySettingsChanged(); // Notify all connected slots that settings have changed
}

void SettingsManager::notifySettingsChanged() {
    for (const auto& listener : settingsChangeListeners) {
        listener(settings);
    }
}
//public destructor
SettingsManager::~SettingsManager() {
    // Destructor code
}
void SettingsManager::connectSettingsChanged(SettingsChangedSignal slot) {
    settingsChangeListeners.push_back(slot);
}
void SettingsManager::saveSettings(){
    ofSavePrettyJson(settingsPath, settings);
    notifySettingsChanged();
    
}
bool SettingsManager::loadSettingsFromJSON(std::string key) {
    if (!settings[key].is_null()) {
        ofLogVerbose("loadSettings") << key + " set to " << settings[key] << endl;
        return true;
    } else {
        ofLogVerbose("loadSettings fallback") << key + " not found in settings" << endl;
        return false;
    }
}

// Implement the second overload with two keys
bool SettingsManager::loadSettingsFromJSON(std::string key, std::string subKey1) {
    if (!settings[key][subKey1].is_null()) {
        ofLogVerbose("loadSettings") << key + "." + subKey1 + " set to " << settings[key][subKey1] << endl;
        return true;
    } else {
        ofLogVerbose("loadSettings fallback") << key + "." + subKey1 + " not found in settings" << endl;
        return false;
    }
}

// Implement the third overload with three keys
bool SettingsManager::loadSettingsFromJSON(std::string key, std::string subKey1, std::string subKey2) {
    if (!settings[key][subKey1][subKey2].is_null()) {
        ofLogVerbose("loadSettings") << key + "." + subKey1 + "." + subKey2 + " set to " << settings[key][subKey1][subKey2] << endl;
        return true;
    } else {
        ofLogVerbose("loadSettings fallback") << key + "." + subKey1 + "." + subKey2 + " not found in settings" << endl;
        return false;
    }
}
