#pragma once

#include "TMesh.h"
#include "Texture.h"
#include "TIFFImage.h"

class SkyBox : public TMesh {
private:
	Texture** textures;

public:
	SkyBox(const TIFFImage &image);
};

