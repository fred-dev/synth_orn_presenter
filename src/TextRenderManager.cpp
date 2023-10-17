//
//  TextRenderManager.cpp
//  Synthetic Ornithology player
//
//  Created by Fred Rodrigues on 02/10/2023.
//

#include "TextRenderManager.h"

TextRenderManager::TextRenderManager() {
    updateSettings();
    ofLogVerbose("TextRenderManager") << "Constructor called" << endl;
}

TextRenderManager::~TextRenderManager() {
    
    ofLogVerbose("TextRenderManager") << "Destructor called" << endl;
    
 

}

void TextRenderManager::setup(){
    ofLogNotice("TextRenderManager") << "TextRenderManager setup";
    
    if (smallFont.load(TextRenderManagerSettings["font_path"], TextRenderManagerSettings["font_size_small"])) {
        ofLogNotice("TextRenderManager") << "small font loaded";
    }
    
    if (mediumFont.load(TextRenderManagerSettings["font_path"], TextRenderManagerSettings["font_size_medium"])) {
        ofLogNotice("TextRenderManager") << "medium font loaded";
    }
    if (largeFont.load(TextRenderManagerSettings["font_path"], TextRenderManagerSettings["font_size_large"])) {
        ofLogNotice("TextRenderManager") << "large font loaded";
    }
    
    textFbo1.allocate(TextRenderManagerSettings["text_renderer_1_width"] , TextRenderManagerSettings["text_renderer_1_height"]);
    textFbo2.allocate(TextRenderManagerSettings["text_renderer_2_width"] , TextRenderManagerSettings["text_renderer_2_height"]);
    
    ofLogVerbose("TextRenderManager") << "TextRenderManager Setup complete";
    
}

void TextRenderManager::update(){
    speculativeDate = TimelineManager::getInstance().getSpecualtiveTime();

    textFbo1.begin();
    ofClear(0,0,0,0);
    largeFont.drawString(speculativeDate, 50, 50);
    textFbo1.end();
    
    
    textFbo2.begin();
    ofClear(0,0,0,0);
    mediumFont.drawString(speculativeDate, 50, 50);
    textFbo2.end();
    
    NDIManager::getInstance().getTextSender1().SendImage(textFbo1);
    NDIManager::getInstance().getTextSender2().SendImage(textFbo2);
}

void TextRenderManager::draw(){
    
}

void TextRenderManager::exit(){
    ofLogVerbose("TextRenderManager") << "Exit";
}


void TextRenderManager::updateSettings() {
    // Connect a slot to listen for settings changes
    SettingsManager::getInstance().connectSettingsChanged([this](const ofJson& updatedSettings) {
        // Call the handleSettingsChange function in TextRenderManager
        handleSettingsChange(updatedSettings);
    });
}

void TextRenderManager::setTextRenderManagerSettings(const ofJson& updatedSettings) {
    // Merge the updated settings with the current TextRenderManagerSettings
    for (auto it = updatedSettings.begin(); it != updatedSettings.end(); ++it) {
        const std::string& key = it.key();
        const ofJson& newValue = it.value();
        
        // Check if the new value is different from the current value
        if (TextRenderManagerSettings[key] != newValue) {
            TextRenderManagerSettings[key] = newValue;
        }
    }
    
    // Update the main settings managed by the SettingsManager with the modified TextRenderManagerSettings
    SettingsManager::getInstance().updateSettings(TextRenderManagerSettings);
}


