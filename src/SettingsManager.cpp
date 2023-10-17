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
        loadSettingsFromJSON("timeline_years");
        loadSettingsFromJSON("start_time_unix");
        loadSettingsFromJSON("main_duration_seconds");
        
        settings["end_time_unix"] = settings["start_time_unix"].get<long>() + (31536000 * settings["timeline_years"].get<long>());
        ofLogVerbose("loadSettings") << "end_time_unix set to " << settings["end_time_unix"] << endl;
        
        loadSettingsFromJSON("map_render_width");
        loadSettingsFromJSON("map_render_height");
        loadSettingsFromJSON("text_renderer_1_width");
        loadSettingsFromJSON("text_renderer_1_height");
        loadSettingsFromJSON("text_renderer_2_width");
        loadSettingsFromJSON("text_renderer_2_height");
        loadSettingsFromJSON("grapics_renderer_width");
        loadSettingsFromJSON("grapics_renderer_height");
        loadSettingsFromJSON("font_path");
        loadSettingsFromJSON("font_size_small");
        loadSettingsFromJSON("font_size_medium");
        loadSettingsFromJSON("font_size_large");
        loadSettingsFromJSON("font_path");
        loadSettingsFromJSON("font_path");
        
        if (!settings["Log_level"].is_null()) {
            ofLogVerbose("loadSettings") << "Log level set to " << settings["Log_level"] << endl;
            ofSetLogLevel(settings["Log_level"]);
        } else {
            settings["Log_level"] = OF_LOG_VERBOSE;
            ofLogVerbose("loadSettings fallback") << "Log level set to " << settings["Log_level"] << endl;
        }
        
        
        
        if (!settings["useVirtualPort"].is_null()) {
            ofLogVerbose("loadSettings") << "useVirtualPort set to " << settings["useVirtualPort"] << endl;
        } else {
            settings["useVirtualPort"] = true;
            ofLogVerbose("loadSettings fallback") << "useVirtualPort set to " << settings["useVirtualPort"] << endl;
        }
        
        if (!settings["midiInDevice"].is_null()) {
            if (settings["midiInDevice"].is_number()) {
                settings["midiInDeviceByString"] = false;
                ofLogVerbose("loadSettings") << "midiInDevice set to number: " << settings["midiInDeviceNum"] << endl;
            } else if (settings["midiInDevice"].is_string()) {
                settings["midiInDeviceByString"] = true;
                ofLogVerbose("loadSettings") << "midiInDevice set to string: " << settings["midiInDeviceName"] << endl;
            }
        } else {
            settings["midiInDeviceByString"] = false;
            settings["midiInDevice"] = 0;
            ofLogVerbose("loadSettings") << "midiInDevice set to number" << settings["midiInDevice"] << endl;
        }
        
        if (!settings["midiOutDevice"].is_null()) {
            if (settings["midiOutDevice"].is_number()) {
                settings["midiOutDeviceByString"] = false;
                ofLogVerbose("loadSettings") << "midiOutDevice set to number: " << settings["midiOutDeviceNum"]  << endl;
            } else if (settings["midiOutDevice"].is_string()) {
                settings["midiOutDeviceByString"] = true;
                ofLogVerbose("loadSettings") << "midiOutDevice set to string: " << settings["midiOutDeviceName"] << endl;
            }
        } else {
            settings["midiOutDeviceByString"] = false;
            settings["midiOutDevice"] = 0;
            ofLogVerbose("loadSettings") << "midiOutDevice set to " << settings["midiOutDevice"]  << endl;
        }
        
        if (!settings["midiOutChannel"].is_null()) {
            ofLogVerbose("loadSettings") << "midiOutChannel set to " << settings["midiOutChannel"] << endl;
        } else {
            settings["midiOutChannel"] = 1;
            ofLogVerbose("loadSettings fallback") << "midiOutChannel set to " << settings["midiOutChannel"] << endl;
        }
        
        if (!settings["midiInChannel"].is_null()) {
            ofLogVerbose("loadSettings") << "midiInChannel set to " << settings["midiInChannel"] << endl;
        } else {
            settings["midiInChannel"] = 1;
            ofLogVerbose("loadSettings fallback") << "midiInChannel set to " << settings["midiInChannel"] << endl;
        }
        
        //set the incomingPortOsc from the settings
        if (!settings["incomingPortOsc"].is_null()) {
            ofLogVerbose("loadSettings") << "incomingPortOsc set to " << settings["incomingPortOsc"] << endl;
        } else {
            settings["incomingPortOsc"] = 12345;
            ofLogVerbose("loadSettings fallback") << "incomingPortOsc set to " << settings["incomingPortOsc"] << endl;
        }
        
        if (!settings["outGoingPortOsc"].is_null()) {
            ofLogVerbose("loadSettings") << "outGoingPortOsc set to " << settings["outGoingPortOsc"] << endl;
        } else {
            settings["outGoingPortOsc"] = 1234;
            ofLogVerbose("loadSettings fallback") << "outGoingPortOsc set to " << settings["outGoingPortOsc"] << endl;
        }
        
        if (!settings["outgoingIpOSC"].is_null()) {
            ofLogVerbose("loadSettings") << "outgoingIpOSC set to " << settings["outgoingIpOSC"] << endl;
        } else {
            settings["outgoingIpOSC"] = "127.0.0.1";
            ofLogVerbose("loadSettings fallback") << "outgoingIpOSC set to " << settings["outgoingIpOSC"] << endl;
        }
        
        if (!settings["frameRate"].is_null()) {
            ofLogVerbose("loadSettings") << "frameRate set to " << settings["frameRate"] << endl;
        } else {
            settings["frameRate"] = 25;
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
    
    settings["timeline_years"] = 1;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "timeline_years set to " << settings["timeline_years"]  << endl;
    
    settings["start_time_unix"] = ofGetUnixTime();
    ofLogVerbose("SettingsManager::setDefaultSettings") << "start_time_unix set to " << settings["start_time_unix"]  << endl;
    
    settings["end_time_unix"] = ( 31536000 * settings["timeline_years"].get<int>() )+ settings["start_time_unix"].get<long>();
    ofLogVerbose("SettingsManager::setDefaultSettings") << "end_time_unix set to " << settings["end_time_unix"]  << endl;
    
    settings["main_duration_seconds"] = 6000;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "main_duration_seconds set to " << settings["main_duration_seconds"]  << endl;
    
    settings["map_render_width"] = 600;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "map_render_width set to " << settings["map_render_width"]  << endl;
    
    settings["map_render_height"] = 600;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "map_render_height set to " << settings["map_render_height"]  << endl;
    
    settings["text_renderer_1_width"] = 1000;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "text_renderer_1_width set to " << settings["text_renderer_1_width"]  << endl;
    
    settings["text_renderer_1_height"] = 200;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "text_renderer_1_height set to " << settings["text_renderer_1_height"]  << endl;
    
    settings["text_renderer_2_width"] = 1000;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "text_renderer_2_width set to " << settings["text_renderer_2_width"]  << endl;
    
    settings["text_renderer_2_height"] = 200;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "text_renderer_2_height set to " << settings["text_renderer_2_height"]  << endl;
    
    settings["grapics_renderer_width"] = 1000;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "grapics_renderer_width set to " << settings["grapics_renderer_width"]  << endl;
    
    settings["grapics_renderer_height"] = 200;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "text_renderer_2_height set to " << settings["grapics_renderer_height"]  << endl;
    
    settings["font_path"] = "fonts/frabk.ttf";
    ofLogVerbose("SettingsManager::setDefaultSettings") << "font_path set to " << settings["font_path"]  << endl;
    
    settings["font_size_small"] = 8;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "font_size_small set to " << settings["font_size_small"]  << endl;
    
    settings["font_size_medium"] = 12;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "font_size_medium set to " << settings["font_size_medium"]  << endl;
    
    settings["font_size_large"] = 16;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "font_size_large set to " << settings["font_size_large"]  << endl;
    
    
    settings["useVirtualPort"] = true;
    
    ofLogVerbose("SettingsManager::setDefaultSettings") << "useVirtualPort set to " << settings["useVirtualPort"]  << endl;
    
    settings["incomingPortOsc"] = 12345;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "incomingPortOsc set to " << settings["incomingPortOsc"] << endl;
    
    settings["outGoingPortOsc"] = 1234;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "outGoingPortOsc set to " << settings["outGoingPortOsc"] << endl;
    
    settings["outgoingIpOSC"] = "127.0.0.1";
    ofLogVerbose("SettingsManager::setDefaultSettings") << "outgoingIpOSC set to " << settings["outgoingIpOSC"] << endl;
    
    settings["midiInDevice"] = 0;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "midiInDevice set to " << settings["midiInDevice"] << endl;
    
    settings["midiOutDevice"] = 0;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "midiOutDevice set to " << settings["midiOutDevice"] << endl;
    
    settings["midiOutChannel"] = 1;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "midiOutChannel set to " << settings["midiOutChannel"] << endl;
    
    
    settings["midiInChannel"] = 1;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "midiInChannel set to " << settings["midiInChannel"] << endl;
    
    settings["frameRate"] = 25;
    ofLogVerbose("SettingsManager::setDefaultSettings") << "frameRate set to " << settings["frameRate"] << endl;
    notifySettingsChanged(); // Notify all connected slots that settings have changed
    
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
bool SettingsManager::loadSettingsFromJSON(std::string key){
    if (!settings[key].is_null()) {
        ofLogVerbose("loadSettings") << key + " set to " << settings[key] << endl;
        return true;
    }
    else{
        ofLogVerbose("loadSettings fallback") << key + " set to " << settings[key] << endl;
        return false;
    }
}
