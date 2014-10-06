#pragma once

class TIFFImage {
private:
	int w;
	int h;
	
public:
	unsigned int* pix;

public:
	TIFFImage(int w, int h);
	TIFFImage(char* filename);

	int width();
	int height();
	void reset(int w, int h);
};

