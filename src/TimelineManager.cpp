//
//  TimelineManager.cpp
//  OSC_MIDI_OSC
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

    timeline.addVideoTrack("fingers", "fingers.mov");

    timeline.addAudioTrack("audio", "4chan.wav");
    timeline.setDurationInSeconds(timeline.getAudioTrack("audio")->getDuration());

    timeline.addCurves("curves", ofRange(0, 255));
    timeline.addBangs("bangs");
    timeline.addFlags("flags");
    timeline.addColors("colors");
    timeline.addLFO("lfo");
    timeline.addSwitches("switches");

    timeline.setPageName("Page 1");
    timeline.addPage("Page 2");
    timeline.addPage("Page 3");
    timeline.addPage("Page 4");
    timeline.setCurrentPage(0);

    timeline.enableSnapToOtherKeyframes(false);
    timeline.setLoopType(OF_LOOP_NORMAL);
    
    ofAddListener(timeline.events().bangFired, this, &TimelineManager::bangFired);
    
    ofLogVerbose("TimelineManager") << "TimelineManager Setup complete";
    
}

void TimelineManager::update(){
    
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


