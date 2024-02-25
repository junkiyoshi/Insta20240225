#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < 10; i++) {

		auto noise_location = glm::vec2(
			ofMap(ofNoise(39 + i * 1000, ofGetFrameNum() * 0.0085), 0, 1, -300, 300),
			ofMap(ofNoise(239 + i * 1000, ofGetFrameNum() * 0.0085), 0, 1, -300, 300));
		noise_location = glm::length(noise_location) < 250 ? noise_location : glm::normalize(noise_location) * 250;

		auto next_noise_location = glm::vec2(
			ofMap(ofNoise(39 + i * 1000, (ofGetFrameNum() + 1) * 0.0085), 0, 1, -300, 300),
			ofMap(ofNoise(239 + i * 1000, (ofGetFrameNum() + 1) * 0.0085), 0, 1, -300, 300));
		next_noise_location = glm::length(next_noise_location) < 250 ? next_noise_location : glm::normalize(next_noise_location) * 250;

		for (int i = 0; i < 2; i++) {

			vector<glm::vec2> log;
			log.push_back(noise_location);
			this->log_list.push_back(log);

			auto deg = glm::atan(next_noise_location.y - noise_location.y, next_noise_location.x - noise_location.x) * RAD_TO_DEG + 180 + ofRandom(-15, 15);
			glm::vec2 velocity = glm::vec2(cos(deg * DEG_TO_RAD), sin(deg * DEG_TO_RAD));
			this->velocity_list.push_back(velocity);

			this->noise_seed_list.push_back(ofRandom(1000));
		}
	}

	for (int i = 0; i < this->log_list.size(); i++) {

		auto future = this->velocity_list[i] * 200;
		auto random_deg = ofMap(ofNoise(glm::vec2(this->noise_seed_list[i], ofGetFrameNum() * 0.01)), 0, 1, 0, 360);
		future += glm::vec2(50 * cos(random_deg * DEG_TO_RAD), 50 * sin(random_deg * DEG_TO_RAD));

		auto next = this->log_list[i].back() + glm::normalize(future) * 4;
		next = glm::length(next) < 280 ? next  : glm::normalize(next) * 280;
		this->log_list[i].push_back(next);

		if (this->log_list[i].size() > 20) {

			this->log_list.erase(this->log_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->noise_seed_list.erase(this->noise_seed_list.begin() + i);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	ofNoFill();
	for (int i = 0; i < this->log_list.size(); i++) {

		ofSetColor(39);
		auto size = this->log_list[i].size() < 3 ? 13 : ofMap(this->log_list[i].size(), 3, 20, 13, 0);
		ofDrawCircle(this->log_list[i].back(), size);
	}

	ofFill();
	for (int i = 0; i < this->log_list.size(); i++) {

		ofSetColor(239);
		auto size = this->log_list[i].size() < 3 ? 12 : ofMap(this->log_list[i].size(), 3, 20, 12, 0);
		ofDrawCircle(this->log_list[i].back(), size);
	}

	/*
	int start = 50;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}