#pragma once

#include <vector>
#include "TIFFImage.h"

class Panorama {
private:
	std::vector<TIFFImage*> images;
	TIFFImage* panorama;

public:
	Panorama();

	void AddImage(char* filename);
	TIFFImage* Stitch();
	int GetHeight(int image_id, int y, std::vector<int> offsetsY);
	void Correlation(TIFFImage* image1, TIFFImage* image2, int &offsetX, int &offsetY);
};

