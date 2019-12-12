#include "ofxPolylinesToSVG.h"

ofxPolylinesToSVG::ofxPolylinesToSVG() {
    addLayer("rootLayer");
}

ofxPolylinesToSVG::~ofxPolylinesToSVG() {
}

void ofxPolylinesToSVG::createRootSvg() {
    saveXml.addTag("svg");
    saveXml.addAttribute("svg", "xmlns", "http://www.w3.org/2000/svg", 0);
    saveXml.addAttribute("svg", "xmlns:xlink", "http://www.w3.org/1999/xlink", 0);
    saveXml.addAttribute("svg", "version", "1.1", 0);
    saveXml.addAttribute("svg", "x", "0", 0);
    saveXml.addAttribute("svg", "y", "0", 0);
    saveXml.addAttribute("svg", "width", "800px", 0);
    saveXml.addAttribute("svg", "height", "800px", 0);
    saveXml.pushTag("svg", 0);
}

void ofxPolylinesToSVG::addLayer(string layerName){
    if(saveXml.getValue("svg", "", 0) == "") {
        createRootSvg();
    }
    
    saveXml.addTag("g");
    saveXml.addAttribute("g", "id", layerName, 0);
    saveXml.pushTag("g", 0);
    currentSaveNode = 0;
}

void ofxPolylinesToSVG::saveToFile(string filename) {
    saveXml.saveFile(filename);
}

void ofxPolylinesToSVG::addPolyline(ofPolyline poly, ofColor color) {
    string hexColor = "#" + ofToHex(color.getHex()).erase(0, 2);
        
    noFill();
    stroke(hexColor, 3);
    std::vector<ofDefaultVec3> vertices = poly.getVertices();
    
    if (vertices.size()>0) {
        beginPath();
        for(size_t j = 0; j<vertices.size(); j++) {
            vertex(vertices[j].x, vertices[j].y);
        }
        endPath();
    }
}

void ofxPolylinesToSVG::addCircle(ofPolyline poly, ofColor color) {
    string hexColor = "#" + ofToHex(color.getHex()).erase(0, 2);
    stroke(hexColor, 1);
    std::vector<ofDefaultVec3> vertices = poly.getVertices();
    if (vertices.size()>0) {
        for(size_t j = 0; j<vertices.size(); j++) {
            circle(vertices[j].x, vertices[j].y, 3);
        }
    }
}

void ofxPolylinesToSVG::beginPath() {
	currentAttributes["drawingpath"] = "true";
	saveXml.addTag("path");
	saveXml.setAttribute("path", "fill", currentAttributes["color"], currentSaveNode);
	saveXml.setAttribute("path", "stroke", currentAttributes["stroke"], currentSaveNode);
	saveXml.setAttribute("path", "stroke-width", currentAttributes["strokewidth"], currentSaveNode);
}

void ofxPolylinesToSVG::endPath() {
	currentAttributes["drawingpath"] = "false";
	string currentString;
	stringstream s;
	currentString = saveXml.getAttribute("path", "d", currentSaveNode);
    
	if(currentString.length() > 1) { // i.e. has a path already been started?
		currentString += " Z";
		saveXml.setAttribute("path", "d", s.str(), currentSaveNode);
	}
    
    currentSaveNode++;
}

void ofxPolylinesToSVG::vertex(float x, float y){
	string currentPath;
    
	if(currentAttributes["drawingpath"] != "" && currentAttributes["drawingpath"] != "false") {
		currentPath = saveXml.getAttribute("path", "d", "", currentSaveNode);
		stringstream s;
        
        if(currentPath.length() == 0) {
            s << "M " << x << " " << y << " ";
        } else {
            s << "L " << x << " " << y << " ";
        }
        
		currentPath+=s.str();
		saveXml.setAttribute("path", "d", currentPath, currentSaveNode);
	}

	if(currentAttributes["drawingpolygon"] != "" && currentAttributes["drawingpolygon"] != "false") {
		//saveXml.pushTag("svg", 0);
		int numPolygons = saveXml.getNumTags("polygon");
		currentPath = saveXml.getAttribute("polygon", "points", "", numPolygons-1);
		stringstream s;
		s << x << "," << y << " ";
		currentPath += s.str();
		saveXml.setAttribute("polygon", "points", currentPath, numPolygons-1);
	}
}

void ofxPolylinesToSVG::circle(float x, float y, float radius){
    saveXml.addTag("circle");
    saveXml.setAttribute("circle", "cx", x, currentSaveNode);
    saveXml.setAttribute("circle", "cy", y, currentSaveNode);
    saveXml.setAttribute("circle", "r", radius, currentSaveNode);
    saveXml.setAttribute("circle", "fill", currentAttributes["color"], currentSaveNode);
    saveXml.setAttribute("circle", "stroke", currentAttributes["stroke"], currentSaveNode);
    saveXml.setAttribute("circle", "stroke-width", currentAttributes["strokewidth"], currentSaveNode);
    currentSaveNode++;
}

void ofxPolylinesToSVG::stroke(string colorHex, int weight) {
	currentAttributes["stroke"] = colorHex;

	stringstream s;
	s << weight;
	currentAttributes["strokewidth"] = s.str();
}

void ofxPolylinesToSVG::fill(string colorHex) {
	// figure out if it has an alpha
	int rgb = strtol(("0x"+colorHex.substr(1, colorHex.length()-1)).c_str(), NULL, 0);
	float r = (rgb >> 16) & 0xFF;
	float g = (rgb >> 8) & 0xFF;
	float b = (rgb) & 0xFF;
	
	currentAttributes["color"] = colorHex;
}

void ofxPolylinesToSVG::noFill() {
	currentAttributes["color"] = "none";
}

void ofxPolylinesToSVG::noStroke(){
	currentAttributes["stroke"] = "none";
}
