//
//  TextRenderManager.cpp
//  Synthetic Ornithology player
//
//  Created by Fred Rodrigues on 02/10/2023.
//

#include "TextRenderManager.h"
#include <iomanip> // Include the <iomanip> header for setprecision

TextRenderManager::TextRenderManager() {
    updateSettings();
    ofLogVerbose("TextRenderManager") << "Constructor called" << endl;
}

TextRenderManager::~TextRenderManager() {
    
    ofLogVerbose("TextRenderManager") << "Destructor called" << endl;
    
 

}

void TextRenderManager::setup(){
    ofLogNotice("TextRenderManager") << "TextRenderManager setup";
    
    if (smallFont.load(TextRenderManagerSettings["renderer"]["font_path"], TextRenderManagerSettings["renderer"]["font_size_small"])) {
        ofLogNotice("TextRenderManager") << "small font loaded";
    }
    
    if (mediumFont.load(TextRenderManagerSettings["renderer"]["font_path"], TextRenderManagerSettings["renderer"]["font_size_medium"])) {
        ofLogNotice("TextRenderManager") << "medium font loaded";
    }
    if (largeFont.load(TextRenderManagerSettings["renderer"]["font_path"], TextRenderManagerSettings["renderer"]["font_size_large"])) {
        ofLogNotice("TextRenderManager") << "large font loaded";
    }
    
    textFbo1.allocate(TextRenderManagerSettings["renderer"]["text_renderer_1_width"] , TextRenderManagerSettings["renderer"]["text_renderer_1_height"]);
    textFbo2.allocate(TextRenderManagerSettings["renderer"]["text_renderer_2_width"] , TextRenderManagerSettings["renderer"]["text_renderer_2_height"]);
    
    ofLogVerbose("TextRenderManager") << "TextRenderManager Setup complete";
    
}

void TextRenderManager::update(){
    speculativeDate = TimelineManager::getInstance().getSpecualtiveTime();

    textFbo1.begin();
    ofClear(0,0,0);
    ofSetColor(85, 186,54);
    largeFont.drawString("Current date: " + speculativeDate, 50, 50);
    textFbo1.end();
    
    
    textFbo2.begin();
    ofClear(0,0,0);
    ofSetColor(85, 186,54);
    mediumFont.drawString("Location Lat: " + ofToString(formatFloat(MapsManager::currentMapPosition.x, 4)) + " Lon: " + ofToString(formatFloat(MapsManager::currentMapPosition.y, 4)), 50, 50);
    textFbo2.end();
    
    NDIManager::getInstance().getTextSender1().SendImage(textFbo1);
    NDIManager::getInstance().getTextSender2().SendImage(textFbo2);
}

void TextRenderManager::draw(){
    textFbo1.draw(0, ofGetHeight() - TextRenderManagerSettings["renderer"]["text_renderer_1_height"].get<float>());
    textFbo2.draw(0, ofGetHeight() - (TextRenderManagerSettings["renderer"]["text_renderer_2_height"].get<float>() +TextRenderManagerSettings["renderer"]["text_renderer_1_height"].get<float>()));

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


std::string TextRenderManager::formatFloat(float value, int precision) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;
    return stream.str();
}
