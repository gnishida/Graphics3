#pragma once

class TIFFImage {
private:
	int w;
	int h;
	
public:
	unsigned int* pix;

public:
	TIFFImage();
	TIFFImage(int w, int h);
	TIFFImage(const char* filename);

	int width() const;
	int height() const;
	void reset(int w, int h);
	void save(const char* filename);
};

