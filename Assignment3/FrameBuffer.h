#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>
#include "Frame.h"
#include "V3.h"
#include "M33.h"
#include "TMesh.h"
#include "PPC.h"
#include "Texture.h"

// framebuffer + window class

class FrameBuffer : public Fl_Gl_Window {
public:
	Frame* frame;

	/** the last position of the mouse pointer */
	V3 lastPosition;

public:
	FrameBuffer(int u0, int v0, int _w, int _h); // constructor, top left coords and resolution
	~FrameBuffer();

	// function that is always called back by system and never called directly by programmer
	// programmer triggers framebuffer update by calling FrameBuffer::redraw(), which makes
	//            system call draw
	void draw();

	// function called back when event occurs (mouse, keyboard, etc)
	int handle(int event);
	void KeyboardHandle();
	void mousePushHandle();
	void mouseMoveHandle();
};


