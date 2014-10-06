#include <stdio.h>
#include <stdlib.h>
#include "Panorama.h"

int main(int argc, char* argv[]) {
	if (argc < 3) {
		fprintf(stderr, "Usage: Panorama <filepath> <number of images>\n");
		fprintf(stderr, "  e.g. Panorama images/image 7\n");
		fprintf(stderr, "       The image files, images/image1.tif, ..., images/image7.tif have to exist.\n");
		return 1;
	}

	char* filepath = argv[1];
	int n = atoi(argv[2]);

	Panorama panorama;
	for (int i = 0; i < n; i++) {
		char filename[256];
		sprintf(filename, "%s%d.tif", filepath, (i+1));
		try {
			panorama.AddImage(filename);
		} catch (const char* ex) {
			fprintf(stderr, "%s\n", ex);
			return 1;
		}
	}
	TIFFImage* result = panorama.Stitch();
	result->save("panorama.tif");

	return 0;
}
