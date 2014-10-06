#pragma once

#include "TMesh.h"

class Quad : public TMesh {
public:
	Quad(float w, float h, const V3 &c);
	Quad(float w, float h, const V3 &c, float s0, float t0, float s1, float t1);

private:
	void init(float w, float h, const V3 &c, float s0 = 0.0f, float t0 = 0.0f, float s1 = 1.0f, float t1 = 1.0f);
};

