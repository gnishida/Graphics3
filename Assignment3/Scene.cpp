#include "Scene.h"
#include "V3.h"
#include "Box.h"
#include "Triangle.h"
#include "Quad.h"
#include "Sphere.h"
#include "Light.h"
#include "Frame.h"
#include <time.h>
#include <float.h>
#include <iostream>
#include <fstream>

using namespace std;

Scene *scene;// = new Scene();


Scene::Scene() {
	mipmap_mode = true;

	lights.push_back(new Light(V3(-30.0f, 0.0f, -30.0f), Light::TYPE_POINT_LIGHT, 0.4f, 0.6f, 40.0f));
	lights.push_back(new Light(V3(30.0f, 0.0f, -30.0f), Light::TYPE_DIRECTIONAL_LIGHT, 0.4f, 0.6f, 40.0f));

	// create user interface
	gui = new GUI();
	gui->show();

	// create SW framebuffer
	int u0 = 20;
	int v0 = 50;
	int sci = 2;
	int w = sci*240;//640;
	int h = sci*180;//360;
	fb = new FrameBuffer(u0, v0, w, h);
	fb->label("SW Framebuffer");
	fb->show();
  
	// position UI window
	gui->uiw->position(fb->frame->w+u0 + 2*20, v0);

	TMesh* tms = new TMesh();
	tms->Load("geometry/auditorium.bin");
	tms->Translate(tms->GetCentroid() * -1.0f);
	tms->RotateAbout(V3(1.0f, 0.0f, 0.0f), -90.0f);
	meshes.push_back(tms);
	/*
	TMesh* tms = new Quad(230, 80, V3(0, 0, 1));
	tms->RotateAbout(V3(0.0f, 1.0f, 0.0f), -90.0f);
	tms->Translate(V3(100.0f, 0.0f, 0.0f));
	tms->SetTexture("texture/billboard.tif");
	meshes.push_back(tms);

	tms = new Sphere(40.0f, V3(1, 0, 0));
	tms->SetRaytracing();
	tms->SetCubeMap("texture/uffizi_cross.tiff");
	tms->Translate(tms->GetCentroid() * -1.0f);
	meshes.push_back(tms);
	*/

	// create three cameras
	float hfov = 60.0f;
	PPC* ppc = new PPC(hfov, fb->frame->w, fb->frame->h);
	ppc->LookAt(V3(0.0f, 0.0f, 0.0f), V3(0.0f, 0.0f, -1.0f), V3(0.0f, 1.0f, 0.0f), 2.0f);
	//ppc->LookAt(V3(0.0f, 0.0f, 0.0f), V3(0.0f, 0.0f, -1.0f), V3(0.0f, 1.0f, 0.0f), 130.0f);
	ppcs.push_back(ppc);
	currentPPC = ppc;

	//SaveTIFFs();
}

// function linked to the DBG GUI button for testing new features
void Scene::DBG() {
	if (meshes.size() == 1) {
		TIFFImage* image = CreateCubeMap(512);

		TMesh* tms = new TMesh();
		tms->Load("geometry/teapot1K.bin");
		tms->Translate(tms->GetCentroid() * -1.0f);
		AABB aabb;
		tms->ComputeAABB(aabb);
		tms->Scale(V3(0, 0, 0), aabb.Size() * 0.02f);
		tms->SetCubeMap(*image);
		meshes.push_back(tms);

		delete image;
	}

	Render();
}

/**
 * This function is called when "Demo" button is clicked.
 */
void Scene::Demo() {
	/*
	for (int i = 0; i < 50; i++) {
		meshes[0]->Translate(V3(-1.0f, 0.0f, 0.0f));

		Render();
		Fl::wait();
	}
	*/

	if (meshes.size() == 1) {
		TIFFImage* image = CreateCubeMap(512);

		TMesh* tms = new TMesh();
		tms->Load("geometry/teapot1K.bin");
		tms->Translate(tms->GetCentroid() * -1.0f);
		AABB aabb;
		tms->ComputeAABB(aabb);
		tms->Scale(V3(0, 0, 0), aabb.Size() * 0.02f);
		tms->SetCubeMap(*image);
		meshes.push_back(tms);

		delete image;
	}

	currentPPC = ppcs[0];

	for (int i = 0; i < 600; i++) {
		currentPPC->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.5f, V3(0.0f, 0.0f, 0.0f));

		Render();
		Fl::wait();
	}
}

/**
 * This function is called when "SaveTIFFs" button is clicked.
 * The scene is animated with total 600 frames, which are corresponding to 20 seconds of animation,
 * and all the frames are stored in "captured" folder.
 */
void Scene::SaveTIFFs() {
	/*
	char filename[32];
	for (int i = 0; i < 150; i++) {
		meshes[0]->Translate(V3(-1.0f, 0.0f, 0.0f));
		Render();
		sprintf(filename, "captured\\scene%03d.tif", i);
		fb->frame->Save(filename);
		Fl::wait();
	}
	*/

	TIFFImage* image = CreateCubeMap(512);

	TMesh* tms = new TMesh();
	tms->Load("geometry/teapot1K.bin");
	tms->Translate(tms->GetCentroid() * -1.0f);
	AABB aabb;
	tms->ComputeAABB(aabb);
	tms->Scale(V3(0, 0, 0), aabb.Size() * 0.02f);
	tms->SetCubeMap(*image);
	meshes.push_back(tms);

	delete image;

	char filename[32];

	currentPPC = ppcs[0];

	for (int i = 0; i < 60; i++) {
		Render();
		sprintf(filename, "captured\\scene%03d.tif", i);
		fb->frame->Save(filename);
		Fl::wait();
	}

	for (int i = 60; i < 600; i++) {
		currentPPC->RotateAbout(V3(0.0f, 1.0f, 0.0f), 0.5f, V3(0.0f, 0.0f, 0.0f));
		Render();
		sprintf(filename, "captured\\scene%03d.tif", i);
		fb->frame->Save(filename);
		Fl::wait();
	}
}

/**
 * Render all the models.
 */
void Scene::Render() {
	fb->frame->SetZB(0.0f);
	fb->frame->Set(WHITE);

	for (int i = 0; i < meshes.size(); i++) {
		meshes[i]->Render(fb->frame, currentPPC);
	}

	fb->redraw();
}

TIFFImage* Scene::CreateCubeMap(int size) {
	PPC ppc(90.0f, size, size);

	// render the back scene
	Frame fb1(size, size);
	fb1.SetZB(0.0f);
	fb1.Set(BLACK);
	for (int i= 0; i < meshes.size(); i++) {
		meshes[i]->Render(&fb1, &ppc);
	}

	// render the left scene
	Frame fb2(size, size);
	ppc.Pan(90.0f);
	fb2.SetZB(0.0f);
	fb2.Set(BLACK);
	for (int i= 0; i < meshes.size(); i++) {
		meshes[i]->Render(&fb2, &ppc);
	}

	// render the front scene
	Frame fb3(size, size);
	ppc.Pan(90.0f);
	fb3.SetZB(0.0f);
	fb3.Set(BLACK);
	for (int i= 0; i < meshes.size(); i++) {
		meshes[i]->Render(&fb3, &ppc);
	}

	// render the right scene
	Frame fb4(size, size);
	ppc.Pan(90.0f);
	fb4.SetZB(0.0f);
	fb4.Set(BLACK);
	for (int i= 0; i < meshes.size(); i++) {
		meshes[i]->Render(&fb4, &ppc);
	}

	// render the top scene
	Frame fb5(size, size);
	ppc.Pan(90.0f);
	ppc.Tilt(90.0f);
	fb5.SetZB(0.0f);
	fb5.Set(BLACK);
	for (int i= 0; i < meshes.size(); i++) {
		meshes[i]->Render(&fb5, &ppc);
	}

	// render the bottom scene
	Frame fb6(size, size);
	ppc.Tilt(-180.0f);
	fb6.SetZB(0.0f);
	fb6.Set(BLACK);
	for (int i= 0; i < meshes.size(); i++) {
		meshes[i]->Render(&fb6, &ppc);
	}

	int w = size * 3;
	int h = size * 4;
	TIFFImage* result = new TIFFImage(w, h);
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			result->pix[x + size + y * w] = fb1.pix[x + (size - y - 1) * size];
			result->pix[x + (y + size * 2) * w] = fb2.pix[(size - x - 1) + y * size];
			result->pix[x + size + (y + size * 2) * w] = fb3.pix[(size - x - 1) + y * size];
			result->pix[x + size * 2 + (y + size * 2) * w] = fb4.pix[(size - x - 1) + y * size];
			result->pix[x + size + (y + size * 3) * w] = fb5.pix[(size - x - 1) + y * size];
			result->pix[x + size + (y + size) * w] = fb6.pix[x + (size - y - 1) * size];
		}
	}

	return result;
}

V3 Scene::RayTrace(PPC* ppc, const V3 &p, const V3 &dir, float &dist) {
	float dist_min = std::numeric_limits<float>::max();
	V3 c_min;

	for (int i = 0; i < meshes.size(); i++) {
		float d;
		V3 c;
		if (!meshes[i]->RayTrace(ppc, p, dir, c, d)) continue;
		if (d < dist_min) {
			dist_min = d;
			c_min = c;
		}
	}

	dist = dist_min;

	return c_min;
}