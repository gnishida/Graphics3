#include "Box.h"

Box::Box(const V3 &p0, const V3 &p1, const V3 &col) {
	vertsN = 24;
	verts = new Vertex[vertsN];
	/*
	cols = new V3[vertsN];
	norms = new V3[vertsN];
	*/
	trisN = 12;
	tris = new unsigned int[trisN * 3];

	// bottom
	verts[0].v = p0;
	verts[1].v[0] = p1.x();
	verts[1].v[1] = p0.y();
	verts[1].v[2] = p0.z();
	verts[2].v[0] = p1.x();
	verts[2].v[1] = p0.y();
	verts[2].v[2] = p1.z();
	verts[3].v[0] = p0.x();
	verts[3].v[1] = p0.y();
	verts[3].v[2] = p1.z();
	for (int i = 0; i < 4; i++) verts[i].n = V3(0.0f, -1.0f, 0.0f);
	verts[0].t[0] = 0.0f;
	verts[0].t[1] = 0.0f;
	verts[1].t[0] = 1.0f;
	verts[1].t[1] = 0.0f;
	verts[2].t[0] = 1.0f;
	verts[2].t[1] = 1.0f;
	verts[3].t[0] = 0.0f;
	verts[3].t[1] = 1.0f;

	// right
	verts[4].v[0] = p1.x();
	verts[4].v[1] = p0.y();
	verts[4].v[2] = p1.z();
	verts[5].v[0] = p1.x();
	verts[5].v[1] = p0.y();
	verts[5].v[2] = p0.z();
	verts[6].v[0] = p1.x();
	verts[6].v[1] = p1.y();
	verts[6].v[2] = p0.z();
	verts[7].v[0] = p1.x();
	verts[7].v[1] = p1.y();
	verts[7].v[2] = p1.z();
	for (int i = 4; i < 8; i++) verts[i].n = V3(1.0f, 0.0f, 0.0f);
	verts[4].t[0] = 0.0f;
	verts[4].t[1] = 0.0f;
	verts[5].t[0] = 1.0f;
	verts[5].t[1] = 0.0f;
	verts[6].t[0] = 1.0f;
	verts[6].t[1] = 1.0f;
	verts[7].t[0] = 0.0f;
	verts[7].t[1] = 1.0f;


	// left
	verts[8].v = p0;
	verts[9].v = verts[3].v;
	verts[10].v[0] = p0.x();
	verts[10].v[1] = p1.y();
	verts[10].v[2] = p1.z();
	verts[11].v[0] = p0.x();
	verts[11].v[1] = p1.y();
	verts[11].v[2] = p0.z();
	for (int i = 8; i < 12; i++) verts[i].n = V3(-1.0f, 0.0f, 0.0f);
	verts[8].t[0] = 0.0f;
	verts[8].t[1] = 0.0f;
	verts[9].t[0] = 1.0f;
	verts[9].t[1] = 0.0f;
	verts[10].t[0] = 1.0f;
	verts[10].t[1] = 1.0f;
	verts[11].t[0] = 0.0f;
	verts[11].t[1] = 1.0f;


	// front
	verts[12].v = verts[3].v;
	verts[13].v = verts[2].v;
	verts[14].v = verts[7].v;
	verts[15].v = verts[10].v;
	for (int i = 12; i < 16; i++) verts[i].n = V3(0.0f, 0.0f, 1.0f);
	verts[12].t[0] = 0.0f;
	verts[12].t[1] = 0.0f;
	verts[13].t[0] = 1.0f;
	verts[13].t[1] = 0.0f;
	verts[14].t[0] = 1.0f;
	verts[14].t[1] = 1.0f;
	verts[15].t[0] = 0.0f;
	verts[15].t[1] = 1.0f;

	// back
	verts[16].v = verts[1].v;
	verts[17].v = verts[0].v;
	verts[18].v = verts[11].v;
	verts[19].v = verts[6].v;
	for (int i = 16; i < 20; i++) verts[i].n = V3(0.0f, 0.0f, -1.0f);
	verts[16].t[0] = 0.0f;
	verts[16].t[1] = 0.0f;
	verts[17].t[0] = 1.0f;
	verts[17].t[1] = 0.0f;
	verts[18].t[0] = 1.0f;
	verts[18].t[1] = 1.0f;
	verts[19].t[0] = 0.0f;
	verts[19].t[1] = 1.0f;

	// top
	verts[20].v = verts[10].v;
	verts[21].v = verts[7].v;
	verts[22].v = verts[6].v;
	verts[23].v = verts[11].v;
	for (int i = 20; i < 24; i++) verts[i].n = V3(0.0f, 1.0f, 0.0f);
	verts[20].t[0] = 0.0f;
	verts[20].t[1] = 0.0f;
	verts[21].t[0] = 1.0f;
	verts[21].t[1] = 0.0f;
	verts[22].t[0] = 1.0f;
	verts[22].t[1] = 1.0f;
	verts[23].t[0] = 0.0f;
	verts[23].t[1] = 1.0f;

	for (int i = 0; i < vertsN; i++) {
		verts[i].c = col;
	}

	// bottom
	tris[0] = 0;
	tris[1] = 1;
	tris[2] = 2;
	tris[3] = 0;
	tris[4] = 2;
	tris[5] = 3;

	// side
	tris[6] = 4;
	tris[7] = 5;
	tris[8] = 6;
	tris[9] = 4;
	tris[10] = 6;
	tris[11] = 7;

	tris[12] = 8;
	tris[13] = 9;
	tris[14] = 10;
	tris[15] = 8;
	tris[16] = 10;
	tris[17] = 11;

	tris[18] = 12;
	tris[19] = 13;
	tris[20] = 14;
	tris[21] = 12;
	tris[22] = 14;
	tris[23] = 15;

	tris[24] = 16;
	tris[25] = 17;
	tris[26] = 18;
	tris[27] = 16;
	tris[28] = 18;
	tris[29] = 19;

	// top
	tris[30] = 20;
	tris[31] = 21;
	tris[32] = 22;
	tris[33] = 20;
	tris[34] = 22;
	tris[35] = 23;
}
