//
//  TimelineManager.cpp
//  Synthetic Ornithology player
//
//  Created by Fred Rodrigues on 02/10/2023.
//

#include "TimelineManager.h"

TimelineManager::TimelineManager() {
    updateSettings();
    ofLogVerbose("TimelineManager") << "Constructor called" << endl;
}

TimelineManager::~TimelineManager() {
    
    ofLogVerbose("TimelineManager") << "Destructor called" << endl;
}

void TimelineManager::setup(){
    ofLogNotice("TimelineManager") << "TimelineManager setup";
    
    timeline.setup();
    timeline.setWidth(ofGetWidth());

    timeline.addAudioTrack("audio", "4chan.wav");
    timeline.setDurationInSeconds(TimelineManagerSettings["main_duration_seconds"]);
    timeline.addCurves("Unixtime", ofRange(TimelineManagerSettings["start_time_unix"], TimelineManagerSettings["end_time_unix"]));
    
//    timeline.addBangs("bangs");
//    timeline.addFlags("flags");
//    timeline.addColors("colors");
//    timeline.addLFO("lfo");
    timeline.addSwitches("switches");
//
//    timeline.setPageName("Page 1");
//    timeline.addPage("Page 2");
//    timeline.addPage("Page 3");
//    timeline.addPage("Page 4");
//    timeline.setCurrentPage(0);

    timeline.enableSnapToOtherKeyframes(false);
    timeline.setLoopType(OF_LOOP_NORMAL);
    
    ofAddListener(timeline.events().bangFired, this, &TimelineManager::bangFired);
    
    ofLogVerbose("TimelineManager") << "TimelineManager Setup complete";
    
}

void TimelineManager::update(){
  
    specualtiveTime = unixTimeToHumanReadable(timeline.getValue("Unixtime"));
    
}

void TimelineManager::draw(){
    if(timeline.isSwitchOn("switches")){
        ofBackground(timeline.getColor("colors"));
    }
    else{
        ofBackground(255*.15);
    }
    timeline.draw();
}

void TimelineManager::disableInteraction(){
    timeline.disableEvents();
}
void TimelineManager::enableInteraction(){
    timeline.enableEvents();
}

void TimelineManager::bangFired(ofxTLBangEventArgs& args){
    cout << "bang fired!" << args.flag << endl;
}

void TimelineManager::exit(){
    ofLogVerbose("TimelineManager") << "Exit";
}


void TimelineManager::updateSettings() {
    // Connect a slot to listen for settings changes
    SettingsManager::getInstance().connectSettingsChanged([this](const ofJson& updatedSettings) {
        // Call the handleSettingsChange function in TimelineManager
        handleSettingsChange(updatedSettings);
    });
}

void TimelineManager::setTimelineManagerSettings(const ofJson& updatedSettings) {
    // Merge the updated settings with the current TimelineManagerSettings
    for (auto it = updatedSettings.begin(); it != updatedSettings.end(); ++it) {
        const std::string& key = it.key();
        const ofJson& newValue = it.value();
        
        // Check if the new value is different from the current value
        if (TimelineManagerSettings[key] != newValue) {
            TimelineManagerSettings[key] = newValue;
        }
    }
    
    // Update the main settings managed by the SettingsManager with the modified TimelineManagerSettings
    SettingsManager::getInstance().updateSettings(TimelineManagerSettings);
}


std::string TimelineManager::unixTimeToHumanReadable(long long unixTime) {
    // Convert Unix timestamp to a time_point
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(unixTime);

    // Convert time_point to a time_t (not necessary, but useful for printing)
    std::time_t tt = std::chrono::system_clock::to_time_t(tp);

    // Format the time as a human-readable string
    std::string timeStr = std::ctime(&tt);

    return timeStr;
}
