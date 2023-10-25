//
//  MapsManager.h
//  Synthetic Ornithology player
//
//  Created by Fred Rodrigues on 02/10/2023.
//


#pragma once
#include <stdio.h>

#include "ofMain.h"
#include "SettingsManager.h" // Include the SettingsManager header
#include "NDIManager.h"
#include "ofxMaps.h"
#include "TimelineManager.h"
#include "ofxBezierEditor.h"

enum operationMode{
    EDITOR,
    PLAYER
};

class MapsManager {
public:
    static MapsManager& getInstance() {
        static MapsManager instance; // Guaranteed to be destroyed, instantiated on first use.
        return instance;
    }

    // Delete copy constructor and assignment operator
    MapsManager(MapsManager const&) = delete;
    void operator=(MapsManager const&) = delete;
    
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(int key);

    // Public destructor
    ~MapsManager();

    void updateSettings();
    void setMapsManagerSettings(const ofJson& newSettings);
    static glm::vec2 currentMapPosition;
    ofEasyCam& getMapSceneCam() {
            return mapSceneCam;
        }
    
    std::shared_ptr<glm::vec2> coordsToPixelsConvertor(glm::vec2 coords);

    std::shared_ptr<glm::vec2> pixelsToCoordsConvertor(glm::vec2 pixels);
private:
    MapsManager();

    ofJson MapsManagerSettings;

    // Function to handle settings changes
    void handleSettingsChange(const ofJson& updatedSettings) {
        // Update your MapsManagerSettings with the new settings
        MapsManagerSettings = updatedSettings;
        // Perform any other actions you need to do when settings change
        // This function will be called automatically when settings change.
    }
    
    ofFbo mapFbo, mapSceneFbo;
    


    std::shared_ptr<ofxMaps::MBTilesCache> bufferCache;
    std::shared_ptr<ofxMaps::MapTileLayer> tileLayer;
    std::shared_ptr<ofxMaps::MapTileSet> tileSet;
    std::shared_ptr<ofxMaps::MapTileProvider> tileProvider;
    std::vector<ofxGeo::Coordinate> coordinates;
    std::vector<std::string> sets;

    float animation = 0;
    
    ofPolyline mapPath;
    ofPolyline mapPathPartial;
    ofRectangle mapBoundsRect;
    ofVboMesh mapPathMesh;
    ofMesh createCustomLine(const ofPolyline& polyline, float lineWidth, int resolution);
    glm::vec3 cubicBezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t);

    NDIManager& ndiManager = NDIManager::getInstance();
    
    void setupTiles();
    void setupGraphicDimensions();
    
  
    
    ofEasyCam mapSceneCam;
    
    void setupMaterials();
    void setupLights();
    void renderScene(bool bShadowPass);
    bool reloadShader();
    
    ofMaterial matFloor;
    ofMaterial matPlywood;
    ofMaterial tubeMaterial;
    ofCubeMap cubeMap;


    ofVboMesh meshPlySphere;

    ofShader mDepthShader;

    ofLight light;
    bool bWiggleVerts = false;
    
    ofVideoGrabber grabber;
    ofShader mapFXShader;
    
    int operationMode = PLAYER;
    
    void setupBezierPath();
    ofxBezierEditor myBezier;
    bool bdrawBezierInfo;
    void setMapSize(float width, float height);

};

