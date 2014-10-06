#pragma once

#include "V3.h"
#include "PPC.h"
#include "Texture.h"
#include "CubeMap.h"

class FrameBuffer;

typedef struct {
	V3 v;
	V3 pv;
	V3 c;
	V3 n;
	float t[2];
} Vertex;

class TMesh {
protected:
	/** the numver of vertices */
	int vertsN;

	/** the list of vertices */
	Vertex* verts;

	/** the number of triangles */
	int trisN;

	/** the list of triangles */
	unsigned int* tris;

	bool raytracing;

	Texture* texture;
	CubeMap* cubeMap;

public:
	TMesh();
	~TMesh();

	void SetRaytracing();
	void Load(char *filename);
	void ComputeAABB(AABB &aabb);
	void Translate(const V3 &v);
	void Scale(float t);
	void Scale(const V3 &centroid, const V3 &size);
	void RenderWireframe(Frame *fb, PPC *ppc);
	void Render(Frame *fb, PPC *ppc);

	void Clear();
	void RotateAbout(const V3 &axis, float angle);
	void RotateAbout(const V3 &axis, float angle, const V3 &orig);
	V3 GetCentroid();

	bool isInside2D(const V3 &p0, const V3 &p1, const V3 &p2, const V3 p) const;
	void SetTexture(const char* filename);
	void SetCubeMap(const TIFFImage &image);
	bool RayTrace(PPC* ppc, const V3 &p, const V3 &dir, V3 &col, float &dist);
};

