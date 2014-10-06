#pragma once

#include "gui.h"
#include "FrameBuffer.h"
#include "V3.h"
#include "M33.h"
#include "PPC.h"
#include "TMesh.h"
#include "Light.h"
#include <vector>
#include <iostream>

#define SCREEN_SPACE_RASTERIZATION	0
#define MODEL_SPACE_RASTERIZATION	1

#define NO_SHADING					0
#define GOURAUD_SHADING				1
#define PHONG_SHADING				2

using namespace std;

class Scene {
public:
	/** Software frame buffer */
	FrameBuffer *fb;

	/** Planar pinhole camera */
	vector<PPC*> ppcs;
	PPC* currentPPC;

	/** Graphical user interface */
	GUI *gui;

	/** Triangle mesh object */
	vector<TMesh*> meshes;

	vector<Light*> lights;

	bool mipmap_mode;

public:
	Scene();
	void DBG();
	void Demo();
	void SaveTIFFs();
	void Render();
	TIFFImage* CreateCubeMap(int size);
	V3 RayTrace(PPC* ppc, const V3 &p, const V3 &dir, float &dist);
};

extern Scene *scene;

