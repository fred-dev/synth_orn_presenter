//
//  MapsManager.cpp
//  Synthetic Ornithology player
//
//  Created by Fred Rodrigues on 02/10/2023.
//

#include "MapsManager.h"

glm::vec2 MapsManager::currentMapPosition = glm::vec2(0, 0);

MapsManager::MapsManager() {
    updateSettings();
    ofLogVerbose("MapsManager") << "Constructor called" << endl;
    
}

MapsManager::~MapsManager() {
    
    ofLogVerbose("MapsManager") << "Destructor called" << endl;
}

void MapsManager::setup(){
    ofLogNotice("MapsManager") << "MapsManager setup";
    
    tileProvider = std::make_shared<ofxMaps::MapTileProvider>(ofxMaps::MapTileProvider::fromJSON(MapsManagerSettings["map"]["provider"]));
    ofLogVerbose("MapsManager") << "tileProvider Setup complete";
    
    
    Poco::ThreadPool::defaultPool().addCapacity(64);
    bufferCache = std::make_shared<ofxMaps::MBTilesCache>(*tileProvider, "cache/");
    
    tileSet = std::make_shared<ofxMaps::MapTileSet>(1024,
                                                    tileProvider,
                                                    bufferCache);
    tileLayer = std::make_shared<ofxMaps::MapTileLayer>(tileSet, MapsManagerSettings["renderer"]["map_render_width"], MapsManagerSettings["renderer"]["map_render_height"]);
    
    ofFboSettings s;
    s.width = MapsManagerSettings["renderer"]["map_render_width"];
    s.height = MapsManagerSettings["renderer"]["map_render_height"];
    s.numColorbuffers = 2;
    s.numSamples = 4;
    fbo.allocate(s);
    //fbo.allocate(MapsManagerSettings["renderer"]["map_render_width"], MapsManagerSettings["renderer"]["map_render_height"], GL_RGBA);
    
    //go through the settings and get the lat and lon values for all the points and add them to coordinates
    for (auto point : MapsManagerSettings["map"]["points"]) {
        coordinates.push_back({point["lat"], point["lon"]});
    }
    ofLogVerbose("MapsManager") << "coordinates Setup complete";
    
    tileLayer->setCenter(ofxGeo::Coordinate(MapsManagerSettings["map"]["mapCentre"]["lat"],MapsManagerSettings["map"]["mapCentre"]["lon"]), 4);
    
    ofLogVerbose("MapsManager") << "MapsManager Setup complete";
    
    
}

void MapsManager::update(){
    tileLayer->update();
    
    if (!ofIsFloatEqual(animation, 0.f))
        tileLayer->setCenter(tileLayer->getCenter().getNeighbor(animation, 0));
    
    fbo.begin();
    ofClear(0,0,0,0);
    tileLayer->draw(0, 0);
    ofPushStyle();
    ofNoFill();
    ofSetColor(0, 255, 0);
    
    for (auto coordinate: coordinates)
    {
        auto tc = tileLayer->geoToPixels(coordinate);
        ofDrawCircle(tc.x, tc.y, 2);
    }
    ofPopStyle();
    ofSetColor(255, 0, 0);
    mapPath.clear();
    for (auto coordinate: coordinates)
    {
        auto tc = tileLayer->geoToPixels(coordinate);
        mapPath.lineTo(tc.x, tc.y);
    }
    
    currentMapPosition = glm::vec2( tileLayer->pixelsToGeo(mapPath.getPointAtPercent(TimelineManager::timelineProgress)).getLatitude(), tileLayer->pixelsToGeo(mapPath.getPointAtPercent(TimelineManager::timelineProgress)).getLongitude());
    
    
    
    ofPolyline mapPathPartial;
    
    
    for (float i = 0; i < TimelineManager::timelineProgress; i += 0.001) {
        mapPathPartial.curveTo(mapPath.getPointAtPercent(i),20); // Use curveTo
    }
    if(mapPathPartial.size() > 0){
        tileLayer->setCenter(ofxGeo::Coordinate(tileLayer->pixelsToGeo(mapPathPartial.getPointAtPercent(1))), TimelineManager::mapZoom);
        
    }
    ofSetColor(255, 0, 0);
    createCustomLine(mapPathPartial.getSmoothed(30), TimelineManager::mapZoom, 20).draw();
    
    fbo.end();
    ndiManager.getMapSender().SendImage(fbo);
}

void MapsManager::draw(){
    
    fbo.draw(ofGetWidth() -600, ofGetHeight()-600, 600,600);
    ofDrawBitmapStringHighlight(tileLayer->getCenter().toString(0), 14, ofGetHeight() - 32);
    ofDrawBitmapStringHighlight("Task Queue:" + ofx::TaskQueue::instance().toString(), 14, ofGetHeight() - 16);
    ofDrawBitmapStringHighlight("Connection Pool: " + bufferCache->toString(), 14, ofGetHeight() - 2);
}

void MapsManager::exit(){
    ofLogVerbose("MapsManager") << "Exit";
    
}


void MapsManager::updateSettings() {
    // Connect a slot to listen for settings changes
    SettingsManager::getInstance().connectSettingsChanged([this](const ofJson& updatedSettings) {
        // Call the handleSettingsChange function in MapsManager
        handleSettingsChange(updatedSettings);
    });
}

void MapsManager::setMapsManagerSettings(const ofJson& updatedSettings) {
    // Merge the updated settings with the current MapsManagerSettings
    for (auto it = updatedSettings.begin(); it != updatedSettings.end(); ++it) {
        const std::string& key = it.key();
        const ofJson& newValue = it.value();
        
        // Check if the new value is different from the current value
        if (MapsManagerSettings[key] != newValue) {
            MapsManagerSettings[key] = newValue;
        }
    }
    
    // Update the main settings managed by the SettingsManager with the modified MapsManagerSettings
    SettingsManager::getInstance().updateSettings(MapsManagerSettings);
}


void MapsManager::keyPressed(int key){
    
    if (key == 'f' || key == 'F')
    {
        ofToggleFullscreen();
    }
    else if (key == '-')
    {
        tileLayer->setCenter(tileLayer->getCenter().getZoomedBy(-1));
    }
    else if (key == '=')
    {
        tileLayer->setCenter(tileLayer->getCenter().getZoomedBy(1));
    }
    else if (key == 'w')
    {
        tileLayer->setCenter(tileLayer->getCenter().getNeighborUp());
    }
    else if (key == 'a')
    {
        tileLayer->setCenter(tileLayer->getCenter().getNeighborLeft());
    }
    else if (key == 's')
    {
        tileLayer->setCenter(tileLayer->getCenter().getNeighborDown());
    }
    else if (key == 'd')
    {
        tileLayer->setCenter(tileLayer->getCenter().getNeighborRight());
    }
    else if (key == '1')
    {
        animation -= 0.01;;
    }
    else if (key == '2')
    {
        animation += 0.01;;
    }
    else if (key == '3')
    {
        animation = 0;
    }
    
}
ofMesh MapsManager::createCustomLine(const ofPolyline& polyline, float lineWidth, int resolution) {
    ofMesh customLineMesh;
    
    vector<glm::vec3> linePoints = polyline.getVertices();
    if (linePoints.size() < 2) {
        // The polyline must have at least two points to create a line
        return customLineMesh;
    }
    
    for (size_t i = 0; i < linePoints.size() - 1; ++i) {
        glm::vec3 p0 = linePoints[i];
        glm::vec3 p1 = linePoints[i + 1];
        
        // Calculate the direction from the current point to the next point
        glm::vec3 dir = glm::normalize(p1 - p0);
        
        // Calculate the perpendicular vector (adjust line width here)
        glm::vec3 offset = glm::vec3(-dir.y, dir.x, 0) * (lineWidth / 2.0);
        
        for (int j = 0; j < resolution; ++j) {
            float t = static_cast<float>(j) / (resolution - 1);
            
            // Calculate control points for the cubic Bezier curve
            glm::vec3 cp1 = p0 + offset;
            glm::vec3 cp2 = p1 + offset;
            
            // Interpolate along the Bezier curve
            glm::vec3 point = cubicBezier(p0, cp1, cp2, p1, t);
            
            // Offset the point to create the curve
            glm::vec3 vertex1 = point + offset;
            glm::vec3 vertex2 = point - offset;
            
            // Add vertices to the mesh
            customLineMesh.addVertex(vertex1);
            customLineMesh.addVertex(vertex2);
        }
    }
    
    // Set the draw mode to triangle strip
    customLineMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    
    return customLineMesh;
}

glm::vec3 MapsManager::cubicBezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t) {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;
    
    glm::vec3 p = uuu * p0; // (1-t)^3 * p0
    p += 3.0f * uu * t * p1; // 3(1-t)^2 * t * p1
    p += 3.0f * u * tt * p2; // 3(1-t) * t^2 * p2
    p += ttt * p3; // t^3 * p3
    
    return p;
}
