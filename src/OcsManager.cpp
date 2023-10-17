//
//  OSCManager.cpp
//  OSC_MIDI_OSC
//
//  Created by Fred Rodrigues on 02/10/2023.
//

#include "OscManager.h"

OscManager::OscManager() {

}

void OscManager::setup() {
    ofLogVerbose() << "OSC Manager setup" << endl;
	SettingsManager & settingsManager = SettingsManager::getInstance();
	// Access the settings
	oscManagerSettings = settingsManager.getSettings();

	oscSender.setup(oscManagerSettings["outgoingIpOSC"], oscManagerSettings["outGoingPortOsc"]);
	ofLogVerbose() << "OSC Sender initialised and set to port: " << oscSender.getPort() << " With host: " << oscSender.getHost() << endl;
	oscReceiver.setup(oscManagerSettings["incomingPortOsc"]);
	ofLogVerbose() << "OSC Receiver initialised and set to port: " << oscReceiver.getPort() << endl;
}
void OscManager::closeReceiver(){
    ///oscReceiver.stop();
}
void OscManager::setupReceiver(){
    SettingsManager & settingsManager = SettingsManager::getInstance();
    oscManagerSettings = settingsManager.getSettings();
    oscReceiver.setup(oscManagerSettings["incomingPortOsc"]);
    ofLogVerbose() << "OSC Receiver initialised and set to port: " << oscReceiver.getPort() << endl;
    oscReceiver.start();

}
void OscManager::closeSender(){
    //oscSender.clear();
    
}
void OscManager::setupSender(){
    SettingsManager & settingsManager = SettingsManager::getInstance();
    oscManagerSettings = settingsManager.getSettings();
    oscSender.setup(oscManagerSettings["outgoingIpOSC"], oscManagerSettings["outGoingPortOsc"]);
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

//destructor
OscManager::~OscManager() {
    oscSender.clear();

	ofLogVerbose() << "OSC Manager destructor called" << endl;
}
void OscManager::updateSettings(){
    SettingsManager & settingsManager = SettingsManager::getInstance();
    oscManagerSettings = settingsManager.getSettings();
}
