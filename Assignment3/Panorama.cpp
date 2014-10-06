#include "Panorama.h"
#include "V3.h"

#define SQR(x)		((x) * (x))

Panorama::Panorama() {
	panorama = NULL;
}

void Panorama::AddImage(char* filename) {
	TIFFImage* image = new TIFFImage(filename);
	images.push_back(image);
}

TIFFImage* Panorama::Stitch() {
	if (panorama != NULL) delete panorama;

	std::vector<int> offsetsX;
	std::vector<int> offsetsY;
	std::vector<int> cumulativeOffsetsX;
	std::vector<int> cumulativeWidths;
	int w = 0;
	int max_h1 = 0;
	int min_h2 = images[0]->height();
	int h1 = 0;
	int h2 = images[0]->height();

	for (int i = 0; i < images.size() - 1; i++) {
		int offsetX, offsetY;
		Correlation(images[i], images[i + 1], offsetX, offsetY);

		fprintf(stderr, "Offset: (%d, %d)\n", offsetX, offsetY);

		offsetsX.push_back(offsetX);
		offsetsY.push_back(offsetY);
		w += offsetX;
		cumulativeOffsetsX.push_back(w);

		h1 += offsetY;
		if (h1 > max_h1) max_h1 = h1;
		h2 = h1 + images[i]->height();
		if (h2 < min_h2) min_h2 = h2;
	}

	h1 = max_h1;
	h2 = min_h2;

	cumulativeWidths.push_back(images[0]->width());
	for (int i =  1; i < images.size(); i++) {
		cumulativeWidths.push_back(cumulativeOffsetsX[i - 1] + images[i]->width());
	}

	w += images[images.size() - 1]->width();
	cumulativeOffsetsX.push_back(w);

	TIFFImage* result = new TIFFImage(w, h2 - h1);
	for (int i = h1; i < h2; i++) {
		for (int j = 0; j < w; j++) {
			int x1, x2;
			int y1, y2;

			int image1, image2;
			for (image1 = 0; image1 < cumulativeWidths.size(); image1++) {
				if (j < cumulativeWidths[image1]) {
					x1 = images[image1]->width() - cumulativeWidths[image1] + j;
					y1 = GetHeight(image1, i, offsetsY);
					break;
				}
			}
			for (image2 = 0; image2 < cumulativeOffsetsX.size(); image2++) {
				if (j < cumulativeOffsetsX[image2]) {
					if (image2 > 0) {
						x2 = j - cumulativeOffsetsX[image2 - 1];
						y2 = GetHeight(image2, i, offsetsY);
					}
					break;
				}
			}

			if (image1 == image2) {
				result->pix[(i-h1) * w + j] = images[image1]->pix[y1 * images[image1]->width() + x1];
			} else {
				V3 c1, c2;
				c1.SetColor(images[image1]->pix[y1 * images[image1]->width() + x1]);
				c2.SetColor(images[image2]->pix[y2 * images[image2]->width() + x2]);
				
				float s1 = (float)(images[image1]->width() - x1) / (float)(images[image1]->width() - x1 + x2);
				float s2 = (float)x2 / (float)(images[image1]->width() - x1 + x2);
				result->pix[(i-h1) * w + j] = (c1 * s1 + c2 * s2).GetColor();
			}
		}
	}

	return result;
}

int Panorama::GetHeight(int image_id, int y, std::vector<int> offsetsY) {
	int offsetY = 0;
	for (int i = 0; i < image_id; i++) {
		offsetY += offsetsY[i];
	}

	return y - offsetY;
}

void Panorama::Correlation(TIFFImage* image1, TIFFImage* image2, int &offsetX, int &offsetY) {
	float total_min = std::numeric_limits<float>::max();
	offsetX = -1;
	offsetY = -1;

	for (int osX = 0; osX < image1->width(); osX++) {
		for (int osY = -10; osY <= 10; osY++) {
			float total = 0.0f;
			int count = 0;

			for (int x = 0; x < image1->width(); x++) {
				if (x + osX >= image1->width()) continue;
				if (x >= image2->width()) continue;

				for (int y = 0; y < image1->height(); y++) {
					if (y + osY < 0) continue;
					if (y + osY >= image1->height()) continue;
					if (y >= image2->height()) continue;

					V3 diff;

					V3 col1, col2;
					col1.SetColor(image1->pix[x + osX + (y + osY) * image1->width()]);
					col2.SetColor(image2->pix[x + y * image2->width()]);

					V3 col = col1 - col2;
					total += col * col;
					count++;
				}
			}

			total /= (float)count;
			if (count < image1->height() * image1->width() * 0.05) continue;

			if (total < total_min) {
				total_min = total;
				offsetX = osX;
				offsetY = osY;
			}
		}
	}
}