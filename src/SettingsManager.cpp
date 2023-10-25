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
        
        if (!settings["Log_level"].is_null()) {
            ofLogVerbose("loadSettings") << "Log level set to " << settings["Log_level"] << endl;
            ofSetLogLevel(settings["Log_level"]);
        } else {
            settings["Log_level"] = OF_LOG_VERBOSE;
            ofLogVerbose("loadSettings fallback") << "Log level set to " << settings["Log_level"] << endl;
        }
        
        loadSettingsFromJSON("timeline","timeline_years");
        loadSettingsFromJSON("timeline","start_time_unix");
        loadSettingsFromJSON("timeline","main_duration_seconds");
        
        settings["timeline"]["end_time_unix"] = settings["timeline"]["start_time_unix"].get<long>() + (31536000 * settings["timeline"]["timeline_years"].get<long>());
        ofLogVerbose("loadSettings") << "end_time_unix set to " << settings["timeline"]["end_time_unix"] << endl;
        
        loadSettingsFromJSON("renderer","map_render_width");
        loadSettingsFromJSON("renderer","map_render_height");
        loadSettingsFromJSON("renderer","map_draw_width");
        loadSettingsFromJSON("renderer","map_draw_height");
        loadSettingsFromJSON("renderer","map_draw_x_pos");
        loadSettingsFromJSON("renderer","map_draw_y_pos");
        loadSettingsFromJSON("renderer","text_renderer_1_width");
        loadSettingsFromJSON("renderer","text_renderer_1_height");
        loadSettingsFromJSON("renderer","text_renderer_2_width");
        loadSettingsFromJSON("renderer","text_renderer_2_height");
        loadSettingsFromJSON("renderer","grapics_renderer_width");
        loadSettingsFromJSON("renderer","grapics_renderer_height");
        loadSettingsFromJSON("renderer","font_path");
        loadSettingsFromJSON("renderer","font_size_small");
        loadSettingsFromJSON("renderer","font_size_medium");
        loadSettingsFromJSON("renderer","font_size_large");
        loadSettingsFromJSON("renderer","font_path");
        loadSettingsFromJSON("osc","incomingPortOsc");
        loadSettingsFromJSON("osc","outGoingPortOsc");
        loadSettingsFromJSON("osc","outgoingIpOSC");
//        loadSettingsFromJSON("map", "mapCentre", "lat");
//        loadSettingsFromJSON("map", "mapCentre", "lon");
        loadSettingsFromJSON("NDI","map_sender_name");
        loadSettingsFromJSON("NDI","GFX_sender_name");
        loadSettingsFromJSON("NDI","text_1_sender_name");
        loadSettingsFromJSON("NDI","text_2_sender_name");
        loadSettingsFromJSON("NDI","enable_NDI");

        

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
    
    setDefaultSetting(settings, "Log_level", OF_LOG_VERBOSE);
    setDefaultSetting(settings["timeline"], "timeline_years", 1);
    setDefaultSetting(settings["timeline"], "start_time_unix", ofGetUnixTime());
    setDefaultSetting(settings["timeline"], "end_time_unix", (31536000 * settings["timeline"]["timeline_years"].get<int>()) + settings["timeline"]["start_time_unix"].get<long>());
    setDefaultSetting(settings["timeline"], "main_duration_seconds", 6000);
    setDefaultSetting(settings["renderer"], "map_render_width", 600);
    setDefaultSetting(settings["renderer"], "map_render_height", 600);
    setDefaultSetting(settings["renderer"], "map_draw_width", 600);
    setDefaultSetting(settings["renderer"], "map_draw_height", 600);
    setDefaultSetting(settings["renderer"], "map_draw_x_pos", 1320);
    setDefaultSetting(settings["renderer"], "map_draw_y_pos", 480);
    setDefaultSetting(settings["renderer"], "text_renderer_1_width", 1000);
    setDefaultSetting(settings["renderer"], "text_renderer_1_height", 200);
    setDefaultSetting(settings["renderer"], "text_renderer_2_width", 1000);
    setDefaultSetting(settings["renderer"], "text_renderer_2_height", 200);
    setDefaultSetting(settings["renderer"], "grapics_renderer_width", 1000);
    setDefaultSetting(settings["renderer"], "grapics_renderer_height", 200);
    setDefaultSetting(settings["renderer"], "font_path", "fonts/frabk.ttf");
    setDefaultSetting(settings["renderer"], "font_size_small", 8);
    setDefaultSetting(settings["renderer"], "font_size_medium", 12);
    setDefaultSetting(settings["renderer"], "font_size_large", 16);
    setDefaultSetting(settings["osc"], "incomingPortOsc", 12345);
    setDefaultSetting(settings["osc"], "outGoingPortOsc", 1234);
    setDefaultSetting(settings["osc"], "outgoingIpOSC", "127.0.0.1");
        //setDefaultSetting(settings["map"], "mapCentre", ofJson::array({{ "lat", 0.0 }, { "lon", 0.0 }}));
    setDefaultSetting(settings["NDI"], "map_sender_name", "mapNDISend");
    setDefaultSetting(settings["NDI"], "GFX_sender_name", "GFXNDISend");
    setDefaultSetting(settings["NDI"], "text_1_sender_name", "text1NDISend");
    setDefaultSetting(settings["NDI"], "text_2_sender_name", "text2NDISend");
    setDefaultSetting(settings["NDI"], "enable_NDI", false);

    
    
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

template <typename T>
void SettingsManager::setDefaultSetting(ofJson& settings, const std::string& key, const T& defaultValue) {
    if (!settings[key].is_null()) {
        return; // Key already exists in settings, don't change it
    }
    
    settings[key] = defaultValue;
    
    // Log the change (you can customize the log message as needed)
    ofLogVerbose("SettingsManager::setDefaultSettings") << key << " set to " << settings[key];
}
