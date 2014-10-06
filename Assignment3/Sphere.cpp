#include "Sphere.h"

Sphere::Sphere(float radius, const V3 &c, int nstack, int nslice) {
	vertsN = (nstack + 1) * (nslice + 1);
	verts = new Vertex[vertsN];

	trisN = nstack * nslice * 2;
	tris = new unsigned int[trisN * 3];

	int count = 0;
	for (int i = 0; i <= nstack; i++) {
		float y0 = -radius * cosf((float)i / (float)nstack * M_PI);

		for (int j = 0; j <= nslice; j++) {
			float x0 = radius * sinf((float)i / (float)nstack * M_PI) * cosf((float)j / (float)nslice * M_PI * 2.0f);
			float z0 = -radius * sinf((float)i / (float)nstack * M_PI) * sinf((float)j / (float)nslice * M_PI * 2.0f);

			verts[count].v[0] = x0;
			verts[count].v[1] = y0;
			verts[count].v[2] = z0;
			verts[count].c = c;
			verts[count].n = verts[count].v.UnitVector();
			verts[count].t[0] = (float)j / (float)nslice;
			verts[count].t[1] = (float)i / (float)nstack;
			count++;
		}
	}

	count = 0;
	for (int i = 0; i < nstack; i++) {
		for (int j = 0; j < nslice; j++) {

			tris[count] = i * (nslice + 1) + j;
			tris[count+1] = i * (nslice + 1) + j + 1;
			tris[count+2] = (i + 1) * (nslice + 1) + j + 1;

			tris[count+3] = i * (nslice + 1) + j;
			tris[count+4] = (i + 1) * (nslice + 1) + j + 1;
			tris[count+5] = (i + 1) * (nslice + 1) + j;

			count += 6;
		}
	}
}

