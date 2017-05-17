#pragma once

#include "ofMain.h"

#include "ofxImGui.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

static const float kWaveWidth = 300.0f;
class ReactionDiffusionSolver {
public:
	ReactionDiffusionSolver() {
		_values.resize(kWaveGrid * kWaveGrid);
		_u.resize(kWaveGrid * kWaveGrid);
		_v.resize(kWaveGrid * kWaveGrid);
		this->reset();
	}
	void reset() {
		std::fill(_u.begin(), _u.end(), 1.0f);
		std::fill(_v.begin(), _v.end(), 0.0f);

		for (int x = 1; x < kWaveGrid - 1; ++x) {
			for (int y = 1; y < kWaveGrid - 1; ++y) {
				int index = this->valueIndex(x, y);
				if (ofRandom(100.0f) < 1.0f) {
					_v[index] = 1.0f;
				}
			}
		}
	}
	void step() {
		if (ofRandom(1.0f) < 0.01f) {
			auto gauss = [](float x, float sigma) {
				return 1.0f / glm::sqrt(glm::two_pi<float>()) * sigma * glm::exp(-x * x / (2.0f * sigma * sigma));
			};

			int cx1 = ofRandom(2, kWaveGrid - 2);
			int cy1 = ofRandom(2, kWaveGrid - 2);
			for (int x = 1; x < kWaveGrid - 1; ++x) {
				for (int y = 1; y < kWaveGrid - 1; ++y) {
					int index = this->valueIndex(x, y);
					float value = _v[index];

					{
						float norm = glm::distance(glm::vec2(x, y), glm::vec2(cx1, cy1));
						value += gauss(norm, 7.0f) * 0.5;
					}
					
					_v[index] = value;
				}
			}
		}

		float deltaX = float(kWaveWidth) / float(kWaveGrid);
		float deltaT = 0.5;

		std::fill(_values.begin(), _values.end(), 0.0f);

		for (int x = 1; x < kWaveGrid - 1; ++x) {
			for (int y = 1; y < kWaveGrid - 1; ++y) {
				int index = this->valueIndex(x, y);

				float uL = _u[this->valueIndex(x - 1, y)];
				float uR = _u[this->valueIndex(x + 1, y)];
				float uT = _u[this->valueIndex(x, y - 1)];
				float uB = _u[this->valueIndex(x, y + 1)];

				float u = _u[index];
				float v = _v[index];

				_values[index] = u + deltaT * (
					(Du / (deltaX * deltaX)) * (uL + uR + uT + uB - 4 * u)
					- 
					u * v * v 
					+ 
					F * (1.0f - u)
				);
			}
		}
		std::swap(_values, _u);

		std::fill(_values.begin(), _values.end(), 0.0f);
		for (int x = 1; x < kWaveGrid - 1; ++x) {
			for (int y = 1; y < kWaveGrid - 1; ++y) {
				int index = this->valueIndex(x, y);

				float vL = _v[this->valueIndex(x - 1, y)];
				float vR = _v[this->valueIndex(x + 1, y)];
				float vT = _v[this->valueIndex(x, y - 1)];
				float vB = _v[this->valueIndex(x, y + 1)];

				float u = _u[index];
				float v = _v[index];

				float dd = (Dv / (deltaX * deltaX)) * (vL + vR + vT + vB - 4 * v);
				_values[index] = v + deltaT * (
					(Dv / (deltaX * deltaX)) * (vL + vR + vT + vB - 4 * v)
					+
					u * v * v
					-
					(F + K) * v
					);
			}
		}
		std::swap(_values, _v);
	}

	enum {
		kWaveGrid = 300
	};

	int valueIndex(int x, int y) const {
		return y * kWaveGrid + x;
	}
	float value(int x, int y) const {
		return _v[valueIndex(x, y)];
	}
	int widthN() const {
		return kWaveGrid;
	}
	int heightN() const {
		return kWaveGrid;
	}

	float width() const {
		return kWaveWidth;
	}
	float height() const {
		return kWaveWidth;
	}

	float Du = 0.2097f;
	float Dv = 0.105f;
	float F = 0.037f;
	float K = 0.06f;
private:
	std::vector<float> _values;

	std::vector<float> _u;
	std::vector<float> _v;
};

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofEasyCam _camera;
	ReactionDiffusionSolver _wave;
	ofMesh _mesh;
	ofxImGui _imgui;
};
