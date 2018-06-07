#include "imageData.h"

void ImageData::createEmptyImage() {
	pixelData = new float**[width];
	for (unsigned int i = 0; i < width; i++) {
		pixelData[i] = new float*[height];
		for (unsigned int j = 0; j < height; j++) {
			pixelData[i][j] = new float[3];
		}
	}
}

ImageData::ImageData() {
	width = 0;
	height = 0;
	createEmptyImage();
	imageType = RGB;
}

ImageData::~ImageData() {
	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			delete[] pixelData[i][j];
		}
		delete[] pixelData[i];
	}
	delete[] pixelData;
}

ImageData::ImageData(wxImage& img) {
	width = img.GetWidth();
	height = img.GetHeight();
	createEmptyImage();
	imageType = RGB;
	unsigned char* rawPixelData = img.GetData();
	for (unsigned int x = 0; x < width; x++) {
		for (unsigned int y = 0; y < height; y++) {
			pixelData[x][y][0] = (float)rawPixelData[3 * width * y + 3 * x + 0] / 255.0;
			pixelData[x][y][1] = (float)rawPixelData[3 * width * y + 3 * x + 1] / 255.0;
			pixelData[x][y][2] = (float)rawPixelData[3 * width * y + 3 * x + 2] / 255.0;
		}
	}
}

wxImage ImageData::convertToWxImage() {
	wxImage result(width, height);
	if (imageType == RGB) {
		unsigned char* rawPixelData = result.GetData();
		for (unsigned int x = 0; x < width; x++) {
			for (unsigned int y = 0; y < height; y++) {
				rawPixelData[3 * width * y + 3 * x + 0] = (char)(255.0 * pixelData[x][y][0]);
				rawPixelData[3 * width * y + 3 * x + 1] = (char)(255.0 * pixelData[x][y][1]);
				rawPixelData[3 * width * y + 3 * x + 2] = (char)(255.0 * pixelData[x][y][2]);
			}
		}
	}

	return result;
}