//
//  SettingsManager.h
//  Synthetic Ornithology player
//
//  Created by Fred Rodrigues on 02/10/2023.
//

#pragma once
#include <stdio.h>
#include "ofMain.h"

class SettingsManager {
public:
    static SettingsManager& getInstance() {
        static SettingsManager instance; // Guaranteed to be destroyed, instantiated on first use.
        return instance;
    }
    
    ~SettingsManager(); // Private destructor
    
    void loadSettings(const std::string& filename);
    ofJson getSettings() const;
    
    // Delete copy constructor and assignment operator
    SettingsManager(SettingsManager const&) = delete;
    void operator=(SettingsManager const&) = delete;
    
    // Define a signal type for settings change
    using SettingsChangedSignal = std::function<void(const ofJson&)>;
    
    // Connect a slot (a function) to the signal
    void connectSettingsChanged(SettingsChangedSignal slot);
    
    void saveSettings();
    void updateSettings(ofJson remoteSettings);
    
private:
    SettingsManager(); // Private constructor
    
    ofJson settings;
    std::vector<SettingsChangedSignal> settingsChangeListeners;
    std::string settingsPath;
    
    
    void setDefaultSettings();
    
    // Helper function to notify connected slots when settings change
    void notifySettingsChanged();
    bool loadSettingsFromJSON(std::string key);
    bool loadSettingsFromJSON(std::string key, std::string subKey1);
    bool loadSettingsFromJSON(std::string key, std::string subKey1, std::string subKey2);
    template <typename T>
    void setDefaultSetting(ofJson& settings, const std::string& key, const T& defaultValue);
};

