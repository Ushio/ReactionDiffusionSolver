#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	_camera.setDistance(100);
}

//--------------------------------------------------------------
void ofApp::update() {
	for (int i = 0; i < 10; ++i) {
		_wave.step();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofClear(0);
	
	_camera.begin();
	ofPushMatrix();
	ofRotateZ(90.0f);
	ofSetColor(255);
	ofDrawGridPlane(10.0f);
	ofPopMatrix();

	ofPushMatrix();
	float s = 0.5f;
	ofScale(s, s, s);
	ofDrawAxis(50);

	_mesh.clear();

	float stepX = _wave.width() / _wave.widthN();
	float stepY = _wave.height() / _wave.heightN();

	for (int x = 0; x < _wave.widthN(); ++x) {
		for (int y = 0; y < _wave.heightN(); ++y) {
			float value = _wave.value(x, y);
			_mesh.addVertex(ofVec3f(x * stepX, value, y * stepY));
			_mesh.addColor(ofColor::fromHsb(value * 255.0f, 255.0f, 255.0f));
		}
	}

	ofPushMatrix();
	glPointSize(2);
	ofTranslate(-_wave.width() * 0.5f, 0, -_wave.height() * 0.5f);
	_mesh.drawVertices();
	ofPopMatrix();

	ofPopMatrix();
	_camera.end();


	_imgui.begin();
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ofVec4f(0.0f, 0.2f, 0.2f, 1.0f));
	ImGui::SetNextWindowPos(ofVec2f(10, 30), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ofVec2f(500, ofGetHeight() * 0.8), ImGuiSetCond_Once);

	ImGui::Begin("Config Panel");
	ImGui::SliderFloat("Du", &_wave.Du, 0.0f, 2.0f);
	ImGui::SliderFloat("Dv", &_wave.Dv, 0.0f, 2.0f);
	ImGui::SliderFloat("F", &_wave.F, 0.0f, 1.0f);
	ImGui::SliderFloat("K", &_wave.K, 0.0f, 1.0f);
	ImGui::End();
	ImGui::PopStyleColor();

	_imgui.end();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		_wave.reset();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
