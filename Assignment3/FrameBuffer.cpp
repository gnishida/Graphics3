#include "framebuffer.h"
#include <libtiff/tiffio.h>
#include <iostream>
#include "scene.h"
#include <math.h>
#include <algorithm>

using namespace std;


// makes an OpenGL window that supports SW, HW rendering, that can be displayed on screen
//        and that receives UI events, i.e. keyboard, mouse, etc.
FrameBuffer::FrameBuffer(int u0, int v0, int _w, int _h) : Fl_Gl_Window(u0, v0, _w, _h, 0) {
	frame = new Frame(_w, _h);
}

FrameBuffer::~FrameBuffer() {
	delete frame;
}

// rendering callback; see header file comment
void FrameBuffer::draw() {
	// SW window, just transfer computed pixels from pix to HW for display
	glDrawPixels(frame->w, frame->h, GL_RGBA, GL_UNSIGNED_BYTE, frame->pix);
}

// function called automatically on event within window (callback)
int FrameBuffer::handle(int event)  {  
	switch(event) {   
	case FL_KEYBOARD:
		KeyboardHandle();
	case FL_PUSH:
		mousePushHandle();
	case FL_DRAG:
		mouseMoveHandle();
	default:
		break;
	}

	return 0;
}

void FrameBuffer::KeyboardHandle() {
	int key = Fl::event_key();

	switch (key) {
	case FL_Left:
		scene->currentPPC->TranslateLR(-1.0f);
		scene->Render();
		break;
	case FL_Right:
		scene->currentPPC->TranslateLR(1.0f);
		scene->Render();
		break;
	case FL_Up:
		scene->currentPPC->TranslateUD(1.0f);
		scene->Render();
		break;
	case FL_Down:
		scene->currentPPC->TranslateUD(-1.0f);
		scene->Render();
		break;
    case 'a':
		cerr << "pressed a" << endl;
		scene->SaveTIFFs();
		break;
	default:
		cerr << "INFO: do not understand keypress" << endl;
	}
}

void FrameBuffer::mousePushHandle() {
	make_current();
	lastPosition = V3(Fl::event_x(), Fl::event_y(), 0.0f);
}

void FrameBuffer::mouseMoveHandle() {
	if (Fl::event_state(FL_BUTTON1)) {	// revolution left-right / up-down
		float dx = Fl::event_x() - lastPosition.x();
		float dy = Fl::event_y() - lastPosition.y();

		scene->currentPPC->RotateAbout(scene->currentPPC->b, dx * 1.0f, V3(0.0f, 0.0f, 0.0f));
		scene->currentPPC->RotateAbout(scene->currentPPC->a, -dy * 1.0f, V3(0.0f, 0.0f, 0.0f));
	/*} else if (Fl::event_state(FL_BUTTON2)) {
		float dx = Fl::event_x() - lastPosition.x();
		float dy = Fl::event_y() - lastPosition.y();

		scene->currentPPC->TranslateLR(-dx);
		scene->currentPPC->TranslateUD(dy);*/
	} else if (Fl::event_state(FL_BUTTON3)) {	// roll
		float dx = Fl::event_x() - lastPosition.x();
		scene->currentPPC->Roll(dx);
		//float dy = Fl::event_y() - lastPosition.y();
		//scene->currentPPC->TranslateFB(dy);
	}

	lastPosition = V3(Fl::event_x(), Fl::event_y(), 0.0f);
	scene->Render();
}
