#include "Quad.h"

Quad::Quad(float w, float h, const V3 &c) {
	init(w, h, c);
}

Quad::Quad(float w, float h, const V3 &c, float s0, float t0, float s1, float t1) {
	init(w, h, c, s0, t0, s1, t1);
}

void Quad::init(float w, float h, const V3 &c, float s0, float t0, float s1, float t1) {
	vertsN = 4;
	verts = new Vertex[vertsN];
	trisN = 2;
	tris = new unsigned int[trisN * 3];

	verts[0].v[0] = -w / 2.0f;
	verts[0].v[1] = -h / 2.0f;
	verts[0].v[2] = 0.0f;
	verts[0].n = V3(0.0f, 0.0f, 1.0f);
	verts[0].c = c;
	verts[0].t[0] = s0;
	verts[0].t[1] = t0;

	verts[1].v[0] = w / 2.0f;
	verts[1].v[1] = -h / 2.0f;
	verts[1].v[2] = 0.0f;
	verts[1].n = V3(0.0f, 0.0f, 1.0f);
	verts[1].c = c;
	verts[1].t[0] = s1;
	verts[1].t[1] = t0;

	verts[2].v[0] = w / 2.0f;
	verts[2].v[1] = h / 2.0f;
	verts[2].v[2] = 0.0f;
	verts[2].n = V3(0.0f, 0.0f, 1.0f);
	verts[2].c = c;
	verts[2].t[0] = s1;
	verts[2].t[1] = t1;

	verts[3].v[0] = -w / 2.0f;
	verts[3].v[1] = h / 2.0f;
	verts[3].v[2] = 0.0f;
	verts[3].n = V3(0.0f, 0.0f, 1.0f);
	verts[3].c = c;
	verts[3].t[0] = s0;
	verts[3].t[1] = t1;
	
	tris[0] = 0;
	tris[1] = 1;
	tris[2] = 2;
	tris[3] = 0;
	tris[4] = 2;
	tris[5] = 3;
}