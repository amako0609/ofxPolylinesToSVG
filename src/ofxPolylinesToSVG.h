#pragma mark once

#include "ofMain.h"
#include "ofxSVGXml.h"

//-------------------------------------------------

class ofxPolylinesToSVG{
    
public:
    ofxPolylinesToSVG();
    ~ofxPolylinesToSVG();

    void addPolyline(ofPolyline poly, ofColor color);
    void addCircle(ofPolyline poly, ofColor color);
    
    // Save to svg
    //----------------------------------
    void saveToFile(string filename);
    
private:
    // save stuffs
    int						currentSaveNode;
    ofxSVGXml				saveXml;
    map<string, string>		currentAttributes;

    //  create root
    //----------------------------------
    void createRootSvg();
    void addLayer(string layerName);
    
    void beginPath();
    void vertex(float x, float y);
    void endPath();
    void circle(float x, float y,float radius);

    void stroke(string colorHex, int weight);
    void fill(string colorHex);
    void noFill();
    void noStroke();
};
