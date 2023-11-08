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
    setupTiles();
    setupGraphicDimensions();
    setupLights();
    setupMaterials();
    setupBezierPath();
    mapFXShader.load("2d_fx_shaders/passThrough");
    tileLayer->setCenter(ofxGeo::Coordinate(MapsManagerSettings["map"]["mapCentre"]["lat"],MapsManagerSettings["map"]["mapCentre"]["lon"]), 4);
    
    myBezier.updatePixelCoordsFromGeo();

    
    ofLogVerbose("MapsManager") << "MapsManager Setup complete";
    
    
}

void MapsManager::update(){
    tileLayer->update();
    if(operationMode == PLAYER){
        
        tileLayer->setCenter(ofxGeo::Coordinate(tileLayer->pixelsToGeo(myBezier.getPolyline().getPointAtPercent(TimelineManager::timelineProgress))), TimelineManager::mapZoom);
        myBezier.updatePixelCoordsFromGeo();
        
        mapPathPartial.clear();
        float lastIndex = myBezier.getPolyline().getIndexAtPercent(TimelineManager::timelineProgress);
        //ofLogVerbose("MapsManager") << "lastIndex: " << lastIndex;

        for (int i = 0; i <= floor(lastIndex); i++) {
            if (!mapBoundsRect.inside(myBezier.getPolyline().getVertices()[i].x, myBezier.getPolyline().getVertices()[i].y)) {
                mapPathPartial.addVertex(myBezier.getPolyline().getVertices()[i]);
            }
            mapPathPartial.addVertex(myBezier.getPolyline().getVertices()[i]);
        }

        // Calculate the number of points to interpolate
        float lastSectionLength = lastIndex - floor(lastIndex);

        // Interpolate and add the points between the last real index and the final interpolated index
        for (int i = 1; i <= 10; i++) {
            float index = floor(lastIndex) + (i * lastSectionLength/10);
            ofVec3f interpolatedPoint = myBezier.getPolyline().getPointAtIndexInterpolated(index);
            if (mapBoundsRect.inside(interpolatedPoint.x, interpolatedPoint.y)) {
                mapPathPartial.addVertex(interpolatedPoint);
            }
        }
        mapPathMesh.clear();
        mapPathMesh = myBezier.getTubeMeshFromPolyline(mapPathPartial);
        currentMapPosition = glm::vec2( tileLayer->pixelsToGeo(mapPath.getPointAtPercent(TimelineManager::timelineProgress)).getLatitude(), tileLayer->pixelsToGeo(mapPath.getPointAtPercent(TimelineManager::timelineProgress)).getLongitude());
        //    if (!ofIsFloatEqual(animation, 0.f))
        //        tileLayer->setCenter(tileLayer->getCenter().getNeighbor(animation, 0));
    }
    mapFbo.begin();
    ofClear(0,0,0,0);
    if(operationMode == PLAYER){
        ofRotateYDeg(180);
        ofTranslate(-MapsManagerSettings["renderer"]["map_render_width"].get<float>(),0);
    }
  
//    mapFXShader.begin();
//    mapFXShader.setUniform1i        ("trueWidth"    ,int(MapsManagerSettings["renderer"]["map_draw_width"]));
//    mapFXShader.setUniform1i        ("trueHeight"    ,int(MapsManagerSettings["renderer"]["map_draw_height"]));
//    mapFXShader.setUniform1f        ("rand"            ,ofRandom(1));
//    mapFXShader.setUniform1f        ("mouseX"        ,ofGetMouseX());
//    mapFXShader.setUniform1f        ("mouseY"        ,ofGetMouseY());
//    mapFXShader.setUniform1f        ("val1"            ,ofRandom(100));
//    mapFXShader.setUniform1f        ("val2"            ,ofRandom(100));
//    mapFXShader.setUniform1f        ("val3"            ,ofRandom(100));
//    mapFXShader.setUniform1f        ("val4"            ,ofRandom(100));
//    mapFXShader.setUniform1f        ("timer"        ,ofGetElapsedTimef());
//    mapFXShader.setUniform1i("range", int(ofRandom(100)));
    
    tileLayer->draw(0, 0);
    
    //mapFXShader.end();
    mapFbo.end();
    
    if(MapsManagerSettings["NDI"]["enable_NDI"].get<bool>()){
        ndiManager.getMapSender().SendImage(mapSceneFbo);
    }
}

void MapsManager::draw(){
    if(operationMode == PLAYER){
        ofEnableDepthTest();
        if( light.shouldRenderShadowDepthPass() ) {
            int numShadowPasses = light.getNumShadowDepthPasses();
            for( int j = 0; j < numShadowPasses; j++ ) {
                light.beginShadowDepthPass(j);
                renderScene(true);
                light.endShadowDepthPass(j);
            }
        }
        
        
        mapSceneFbo.begin();
        mapSceneCam.begin();{
            ofClear(0,0,0,0);
            renderScene(false);
            
            if( cubeMap.hasPrefilteredMap() ) {
                cubeMap.drawPrefilteredCube(0.2f);
            }
            
        } mapSceneCam.end();
        
        mapSceneFbo.end();
        
        ofDisableDepthTest();
        mapSceneFbo.draw(MapsManagerSettings["renderer"]["map_draw_x_pos"], MapsManagerSettings["renderer"]["map_draw_y_pos"], MapsManagerSettings["renderer"]["map_draw_width"], MapsManagerSettings["renderer"]["map_draw_height"]);
    }
    else if(operationMode == EDITOR){
        ofSetColor(255);
        
        mapFbo.draw(0,0);
        ofPushStyle();
        if(!bdrawBezierInfo){
            myBezier.getTubeMesh().draw();
        }
        else{
            myBezier.drawHelp();
        }
        ofPopStyle();
    }
//    ofDrawBitmapStringHighlight(tileLayer->getCenter().toString(0), 14, ofGetHeight() - 32);
//    ofDrawBitmapStringHighlight("Task Queue:" + ofx::TaskQueue::instance().toString(), 14, ofGetHeight() - 16);
//    ofDrawBitmapStringHighlight("Connection Pool: " + bufferCache->toString(), 14, ofGetHeight() - 2);
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
    
    if (key == '-')
    {
        tileLayer->setCenter(tileLayer->getCenter().getZoomedBy(-1));
        myBezier.updatePixelCoordsFromGeo();

    }
    else if (key == '=')
    {
        tileLayer->setCenter(tileLayer->getCenter().getZoomedBy(1));
        myBezier.updatePixelCoordsFromGeo();

    }
    else if (key == 'w')
    {
        tileLayer->setCenter(tileLayer->getCenter().getNeighborUp());
        myBezier.updatePixelCoordsFromGeo();

    }
    else if (key == 'a')
    {
        tileLayer->setCenter(tileLayer->getCenter().getNeighborLeft());
        myBezier.updatePixelCoordsFromGeo();

    }
    else if (key == 's')
    {
        tileLayer->setCenter(tileLayer->getCenter().getNeighborDown());
        myBezier.updatePixelCoordsFromGeo();

    }
    else if (key == 'd')
    {
        tileLayer->setCenter(tileLayer->getCenter().getNeighborRight());
        myBezier.updatePixelCoordsFromGeo();

    }
    else if (key == '1')
    {
        animation -= 0.01;;
    }
    else if (key == '2')
    {
        animation += 0.01;;
    }
    else if (key == 'r')
    {
        reloadShader();
    }
    else if (key == 'W')
    {
        bWiggleVerts = !bWiggleVerts;
    }
    else if (key == '3')
    {
        animation = 0;
    }
    else if (key == 'h')
    {
        bdrawBezierInfo = !bdrawBezierInfo;
    }
  
    else if(key == OF_KEY_UP){
        if(operationMode == EDITOR){
            setMapSize(MapsManagerSettings["renderer"]["map_render_width"], MapsManagerSettings["renderer"]["map_render_height"]);
            myBezier.updatePixelCoordsFromGeo();
            operationMode = PLAYER;
            
        }
        else if(operationMode == PLAYER){
            setMapSize(ofGetWidth(), ofGetHeight());
           
            operationMode = EDITOR;
            ofxGeo::Coordinate centre(-24.978199, 133.100608);
            tileLayer->setCenter(centre, 5);
            myBezier.updatePixelCoordsFromGeo();

        }
    }
    
}

void MapsManager::setupTiles(){
    ofLogVerbose("MapsManager::setupTiles") << "tileProvider Setup start";
    
    tileProvider = std::make_shared<ofxMaps::MapTileProvider>(ofxMaps::MapTileProvider::fromJSON(MapsManagerSettings["map"]["provider"]));
    Poco::ThreadPool::defaultPool().addCapacity(64);
    bufferCache = std::make_shared<ofxMaps::MBTilesCache>(*tileProvider, "cache/");
    tileSet = std::make_shared<ofxMaps::MapTileSet>(1024,
                                                    tileProvider,
                                                    bufferCache);
    tileLayer = std::make_shared<ofxMaps::MapTileLayer>(tileSet, MapsManagerSettings["renderer"]["map_render_width"], MapsManagerSettings["renderer"]["map_render_height"]);
    ofLogVerbose("MapsManager::setupTiles") << "tileProvider Setup complete";
    
}
void MapsManager::setupGraphicDimensions(){
    ofLogVerbose("MapsManager::setupGraphicDimensions") << "Setup graphics start";
    
    ofFboSettings mapFboS;
    mapFboS.width = MapsManagerSettings["renderer"]["map_render_width"];
    mapFboS.height = MapsManagerSettings["renderer"]["map_render_height"];
    mapFbo.allocate(mapFboS);
    
    ofFboSettings mapSceneFboS;
    mapSceneFboS.width = MapsManagerSettings["renderer"]["map_render_width"];
    mapSceneFboS.height = MapsManagerSettings["renderer"]["map_render_height"];
    mapSceneFboS.useDepth = true;
    mapSceneFboS.useStencil = true;
    mapSceneFbo.allocate(mapSceneFboS);
    
    mapSceneCam.setControlArea(ofRectangle(MapsManagerSettings["renderer"]["map_draw_x_pos"], MapsManagerSettings["renderer"]["map_draw_y_pos"], MapsManagerSettings["renderer"]["map_draw_width"], MapsManagerSettings["renderer"]["map_draw_height"]));
            
    mapBoundsRect.set(0, 0, MapsManagerSettings["renderer"]["map_render_width"], MapsManagerSettings["renderer"]["map_render_height"]);
    
    
    ofLogVerbose("MapsManager::setupGraphicDimensions") << "Setup graphics complete";
    
}

void MapsManager::setupMaterials(){
    ofLogVerbose("MapsManager::setupMaterials") << "Materials Setup begin";
    
    
    tubeMaterial.setCustomUniform1f("iElapsedTime", 1.0);
    //    // set the material to PBR, default if phong
    tubeMaterial.setPBR(true);
    
    matFloor.setPBR(true);
    
    // try commenting this out to see the effect that cube maps have on lighting
    // https://polyhaven.com/a/kloppenheim_06_puresky
    cubeMap.load( "kloppenheim_06_puresky_1k.exr", 512 );
    
    matFloor.setMetallic(0.0);
    matFloor.setReflectance(0.01);
    matFloor.setRoughness(0.8);
    
    matFloor.setTexture(OF_MATERIAL_TEXTURE_DIFFUSE, mapFbo.getTexture());
    matFloor.setTexture(OF_MATERIAL_TEXTURE_SPECULAR, mapFbo.getTexture());

    reloadShader();
    
    ofLogVerbose("MapsManager::setupMaterials") << "Materials Setup complete";
    
}
void MapsManager::setupLights(){
    ofLogVerbose("MapsManager::setupLights") << "Light Setup begin";
    
    
    light.setDirectional();
    light.enable();
    light.setPosition(100.1, 400, 600 );
    light.lookAt(glm::vec3(0,0,0));
    light.getShadow().setEnabled(true);
    light.getShadow().setGlCullingEnabled(true);
    light.getShadow().setDirectionalBounds(2000, 1000);
    light.getShadow().setNearClip(200);
    light.getShadow().setFarClip(2000);
    light.getShadow().setStrength(1.0);
    // increase alpha value to increase strength of light
    light.setDiffuseColor(ofFloatColor(1.0, 2.0));
    light.getShadow().setShadowType(OF_SHADOW_TYPE_PCF_MED);
    ofLogVerbose("MapsManager::setupLights") << "Light Setup complete";
    
}
void MapsManager::renderScene(bool bShadowPass){
    
    matFloor.begin();
    ofDrawBox(0, -275, 0, MapsManagerSettings["renderer"]["map_render_width"].get<float>() *2, 1 ,MapsManagerSettings["renderer"]["map_render_height"].get<float>()*2);
    
    matFloor.end();
    

    
    if( !tubeMaterial.hasDepthShader() && bShadowPass && bWiggleVerts ) {
        mDepthShader.begin();
        mDepthShader.setUniform1f("iElapsedTime", ofGetElapsedTimef());
        mDepthShader.setUniform1f("uWiggleVerts", bWiggleVerts ? 1.0f : 0.0f);
    }
    // setting custom uniforms on a material automatically adds it to the shader
    tubeMaterial.setCustomUniform1f("iElapsedTime", ofGetElapsedTimef());
    tubeMaterial.setCustomUniform1f("uWiggleVerts", bWiggleVerts ? 1.0f : 0.0f);
    tubeMaterial.begin();
    
    //Draw the tube here
    ofPushMatrix();
    ofScale(-2,-2,-2);
    
    ofRotateX(90);
    ofTranslate(-MapsManagerSettings["renderer"]["map_render_width"].get<float>()/2, -MapsManagerSettings["renderer"]["map_render_width"].get<float>()/2, -100);
    mapPathMesh.draw();
    
    ofPopMatrix();
    
    tubeMaterial.end();
    if(!tubeMaterial.hasDepthShader() && bShadowPass && bWiggleVerts ) {
        mDepthShader.end();
    }
}

bool MapsManager::reloadShader(){
    // load the shader main functions //
    string vname = "shaders/main.vert";
    ofBuffer vbuffer = ofBufferFromFile(vname);
    string fname = "shaders/main.frag";
    ofBuffer fbuffer = ofBufferFromFile(fname);
    if( vbuffer.size() && fbuffer.size() ) {
        tubeMaterial.setShaderMain(vbuffer.getText(), GL_VERTEX_SHADER, "main.vert");
        tubeMaterial.setShaderMain(fbuffer.getText(), GL_FRAGMENT_SHADER, "main.frag");
        tubeMaterial.setDepthShaderMain(vbuffer.getText(), "main.glsl");
        // configure the shader to include shadow functions for passing depth
        // #define OF_SHADOW_DEPTH_PASS gets added by OF so we can use the same shader file and run different bits of code for the shadow pass
        // we add #define NON_MATERIAL_DEPTH_PASS because ofMaterial adds variables that we need
        // to add manually when not using a materil, see main.vert
        //        light.getShadow().setupShadowDepthShader(mDepthShader, "#define NON_MATERIAL_DEPTH_PASS\n"+vbuffer.getText());
        return true;
    }
    return false;
}

std::shared_ptr<glm::vec2> MapsManager::coordsToPixelsConvertor(glm::vec2 coords) {
    auto result = std::make_shared<glm::vec2>(tileLayer->geoToPixels(ofxGeo::Coordinate(coords)));
    return result;
}

std::shared_ptr<glm::vec2> MapsManager::pixelsToCoordsConvertor(glm::vec2 pixels) {
    ofxGeo::Coordinate geoCoords = tileLayer->pixelsToGeo(pixels);
    auto result = std::make_shared<glm::vec2>(geoCoords.getLatitude(), geoCoords.getLongitude());
    return result;
}
void MapsManager::setupBezierPath(){
    myBezier.loadPoints("ofxBezierInfo.json");
    bdrawBezierInfo = false;
}
void MapsManager::setMapSize(float width, float height){
    tileLayer->setSize(glm::vec2(width, height));
    mapFbo.allocate(width, height);
}
