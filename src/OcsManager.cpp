//
//  OSCManager.cpp
//  OSC_MIDI_OSC
//
//  Created by Fred Rodrigues on 02/10/2023.
//

#include "OscManager.h"

OscManager::OscManager() {
    updateSettings();
    cout<<"OSC Constrcutor"<< endl;
}

OscManager::~OscManager() {
    ofLogVerbose("OscManager") << "Destructor called" << endl;
    oscSender.clear();
    ofLogVerbose() << "OSC Manager destructor called" << endl;
}

void OscManager::setup(){
    ofLogNotice("OscManager") << "OscManager setup";

    oscSender.setup(OscManagerSettings["outgoingIpOSC"], OscManagerSettings["outGoingPortOsc"]);
    ofLogVerbose() << "OSC Sender initialised and set to port: " << oscSender.getPort() << " With host: " << oscSender.getHost() << endl;
    oscReceiver.setup(OscManagerSettings["incomingPortOsc"]);
    ofLogVerbose() << "OSC Receiver initialised and set to port: " << oscReceiver.getPort() << endl;
    
    ofLogVerbose("OscManager") << "OscManager Setup complete";
    
}
void OscManager::closeReceiver(){
}
void OscManager::setupReceiver(){
    oscReceiver.setup(OscManagerSettings["incomingPortOsc"]);
    ofLogVerbose() << "OSC Receiver initialised and set to port: " << oscReceiver.getPort() << endl;
    oscReceiver.start();

}
void OscManager::closeSender(){
    //oscSender.clear();
    
}
void OscManager::setupSender(){
    oscSender.setup(OscManagerSettings["outgoingIpOSC"], OscManagerSettings["outGoingPortOsc"]);
    ofLogVerbose() << "OSC Sender initialised and set to port: " << oscSender.getPort() << " With host: " << oscSender.getHost() << endl;

}
void OscManager::handleIncomingMessages() {
    

    while (oscReceiver.hasWaitingMessages()) {
        ofxOscMessage m;
        oscReceiver.getNextMessage(m);

        if (m.getAddress() == "/noteOn") {
            
        }
            
        ofSendMessage(message);
    }
}
void OscManager::update(){
    
}

void OscManager::draw(){
    
}

void OscManager::exit(){
    ofLogVerbose("OscManager") << "Exit";
    
}


void OscManager::updateSettings() {
    // Connect a slot to listen for settings changes
    SettingsManager::getInstance().connectSettingsChanged([this](const ofJson& updatedSettings) {
        // Call the handleSettingsChange function in OscManager
        handleSettingsChange(updatedSettings);
    });
}

void OscManager::setOscManagerSettings(const ofJson& updatedSettings) {
    // Merge the updated settings with the current OscManagerSettings
    for (auto it = updatedSettings.begin(); it != updatedSettings.end(); ++it) {
        const std::string& key = it.key();
        const ofJson& newValue = it.value();
        
        // Check if the new value is different from the current value
        if (OscManagerSettings[key] != newValue) {
            OscManagerSettings[key] = newValue;
        }
    }
    
    // Update the main settings managed by the SettingsManager with the modified OscManagerSettings
    SettingsManager::getInstance().updateSettings(OscManagerSettings);
}
