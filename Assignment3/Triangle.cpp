#include "Triangle.h"


Triangle::Triangle(const V3 &p0, const V3 &c0, const V3 &t0, const V3 &p1, const V3 &c1, const V3 &t1, const V3 &p2, const V3 &c2, const V3 &t2) {
	vertsN = 3;
	verts = new Vertex[vertsN];
	trisN = 1;
	tris = new unsigned int[trisN * 3];

	V3 normal = ((p1 - p0) ^ (p2 - p0)).UnitVector();

	verts[0].v = p0;
	verts[1].v = p1;
	verts[2].v = p2;
	verts[0].c = c0;
	verts[1].c = c1;
	verts[2].c = c2;
	verts[0].n = normal;
	verts[1].n = normal;
	verts[2].n = normal;
	verts[0].t[0] = t0.x();
	verts[0].t[1] = t0.y();
	verts[1].t[0] = t1.x();
	verts[1].t[1] = t1.y();
	verts[2].t[0] = t2.x();
	verts[2].t[1] = t2.y();

	tris[0] = 0;
	tris[1] = 1;
	tris[2] = 2;
}

