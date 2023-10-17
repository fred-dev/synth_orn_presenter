#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    
    // this example uses compute shaders which are only supported since
    // openGL 4.3
    ofGLWindowSettings settings;
    settings.setSize(1920, 1080);
    settings.setGLVersion(4,1);
    settings.windowMode = OF_WINDOW;

    auto window = ofCreateWindow(settings);

    ofRunApp(window, std::make_shared<ofApp>());
    ofRunMainLoop();

}
