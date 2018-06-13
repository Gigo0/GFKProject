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
	xOffset = 0;
	yOffset = 0;
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

ImageData::ImageData(wxImage& img, RectSelection selection) {
	unsigned int fullWidth = img.GetWidth();
	unsigned int fullHeight = img.GetHeight();
	width = fullWidth * (selection.x2 - selection.x1);
	height = fullHeight * (selection.y2 - selection.y1);
	createEmptyImage();
	xOffset = fullWidth * selection.x1;
	yOffset = fullHeight * selection.y1;
	imageType = RGB;
	unsigned char* rawPixelData = img.GetData();
	for (unsigned int x = xOffset; x < xOffset + width; x++) {
		for (unsigned int y = yOffset; y < yOffset + height; y++) {
			pixelData[x - xOffset][y - yOffset][0] = (float)rawPixelData[3 * fullWidth * y + 3 * x + 0] / 255.0;
			pixelData[x - xOffset][y - yOffset][1] = (float)rawPixelData[3 * fullWidth * y + 3 * x + 1] / 255.0;
			pixelData[x - xOffset][y - yOffset][2] = (float)rawPixelData[3 * fullWidth * y + 3 * x + 2] / 255.0;
		}
	}
}

ImageData::ImageData(ImageData* obj) {
	width = obj->width;
	height = obj->height;
	xOffset = obj->xOffset;
	yOffset = obj->yOffset;
	imageType = obj->imageType;
	createEmptyImage();
	for (unsigned int x = 0; x < width; x++) {
		for (unsigned int y = 0; y < height; y++) {
			pixelData[x][y][0] = obj->pixelData[x][y][0];
			pixelData[x][y][1] = obj->pixelData[x][y][1];
			pixelData[x][y][2] = obj->pixelData[x][y][2];
		}
	}
}

void ImageData::applyToWxImage(wxImage& img) {
	unsigned int fullWidth = img.GetWidth();
	if (imageType == RGB || imageType != RGB) {
		unsigned char* rawPixelData = img.GetData();
		for (unsigned int x = xOffset; x < xOffset + width; x++) {
			for (unsigned int y = yOffset; y < yOffset + height; y++) {
				rawPixelData[3 * fullWidth * y + 3 * x + 0] = (char)(255.0 * pixelData[x - xOffset][y - yOffset][0]);
				rawPixelData[3 * fullWidth * y + 3 * x + 1] = (char)(255.0 * pixelData[x - xOffset][y - yOffset][1]);
				rawPixelData[3 * fullWidth * y + 3 * x + 2] = (char)(255.0 * pixelData[x - xOffset][y - yOffset][2]);
			}
		}
	}
}

wxImage ImageData::convertToWxImage() {
	wxImage img = wxImage(width, height);
	unsigned char* rawPixelData = img.GetData();
	for (unsigned int x = 0; x < width; x++) {
		for (unsigned int y = 0; y < height; y++) {
			rawPixelData[3 * width * y + 3 * x + 0] = (char)(255.0 * pixelData[x][y][0]);
			rawPixelData[3 * width * y + 3 * x + 1] = (char)(255.0 * pixelData[x][y][1]);
			rawPixelData[3 * width * y + 3 * x + 2] = (char)(255.0 * pixelData[x][y][2]);
		}
	}
	return img;
}

float ImageData::getMin(float a, float b, float c) {
	float minVal = a;
	if (b < minVal) minVal = b;
	if (c < minVal) minVal = c;
	return minVal;
}

float ImageData::getMax(float a, float b, float c) {
	float maxVal = a;
	if (b > maxVal) maxVal = b;
	if (c > maxVal) maxVal = c;
	return maxVal;
}

float ImageData::negMod(float value, float maxVal) {
	if (maxVal == 0) return 0;
	return value - maxVal * floor(value / maxVal);
}

void ImageData::convertToHSL() {
	if (imageType == HSV) convertToRGB();
	if (imageType == RGB) {
		float R, G, B, cMin, cMax, delta, H, S, L;
		for (unsigned int i = 0; i < width; i++) {
			for (unsigned int j = 0; j < height; j++) {
				R = pixelData[i][j][0];
				G = pixelData[i][j][1];
				B = pixelData[i][j][2];
				cMax = getMax(R, G, B);
				cMin = getMin(R, G, B);
				delta = cMax - cMin;

				if (delta == 0) H = 0;
				else if (cMax == R) H = negMod(((G - B) / delta), 6);
				else if (cMax == G) H = negMod(((B - R) / delta + 2), 6);
				else if (cMax == B) H = negMod(((R - G) / delta + 4), 6);
				H /= 6.0;

				L = (cMin + cMax) / 2.0;

				if (delta == 0) S = 0;
				else {
					if (1 - fabs(2 * L - 1) == 0) S = 1;
					else S = delta / (1 - fabs(2 * L - 1));
				}

				if (H < 0) H = 0;
				if (H > 1) H = 1;
				if (S < 0) S = 0;
				if (S > 1) S = 1;
				if (L < 0) L = 0;
				if (L > 1) L = 1;
				pixelData[i][j][0] = H;
				pixelData[i][j][1] = S;
				pixelData[i][j][2] = L;
			}
		}
		imageType = HSL;
	}
	/*else if (imageType == HSV) {
		float H, S, V, newS, newL;
		for (unsigned int i = 0; i < width; i++) {
			for (unsigned int j = 0; j < height; j++) {
				H = pixelData[i][j][0];
				S = pixelData[i][j][1];
				V = pixelData[i][j][2];

				if (S == 0 && V == 0) {
					pixelData[i][j][1] = S;
					pixelData[i][j][2] = V;
				}
				else {
					newS = S*V / ((H = (2 - S)*V) < 1 ? H : 2 - H);
					//if (!(newS >= 0 && newS <= 1)) std::cout << newS << std::endl;
					newL = H / 2;

					if (newS < 0) newS = 0;
					if (newS > 1) newS = 1;
					if (newL < 0) newL = 0;
					if (newL > 1) newL = 1;
					pixelData[i][j][1] = newS;
					pixelData[i][j][2] = newL;
				}
			}
		}
		imageType = HSL;
	}*/
}

void ImageData::convertToHSV() {
	if (imageType == RGB) convertToHSL();
	if (imageType == HSL) {
		float S, L, newS, newV;
		for (unsigned int i = 0; i < width; i++) {
			for (unsigned int j = 0; j < height; j++) {
				S = pixelData[i][j][1];
				L = pixelData[i][j][2];

				if (S == 0 && L == 0) {
					pixelData[i][j][1] = S;
					pixelData[i][j][2] = L;
				}
				else {
					S *= (L <= 0.5 ? L : 1 - L);

					newS = 2 * S / (L + S);
					newV = L + S;
					if (newS < 0) newS = 0;
					if (newS > 1) newS = 1;
					if (newV < 0) newV = 0;
					if (newV > 1) newV = 1;
					pixelData[i][j][1] = newS;
					pixelData[i][j][2] = newV;
				}
			}
		}
		imageType = HSV;
	}
}

void ImageData::convertToRGB() {
	if (imageType == HSL) convertToHSV();
	if (imageType == HSV) {
		float H, S, V;
		for (unsigned int i = 0; i < width; i++) {
			for (unsigned int j = 0; j < height; j++) {
				H = pixelData[i][j][0];
				S = pixelData[i][j][1];
				V = pixelData[i][j][2];
				float chroma = V * S;
				float tempH = H * 6;
				float valX = chroma * (1 - abs(fmod(tempH, 2) - 1));
				float R1, G1, B1;
				if (tempH >= 0 && tempH < 1) { R1 = chroma; G1 = valX; B1 = 0; }
				else if (tempH >= 1 && tempH < 2) { R1 = valX; G1 = chroma; B1 = 0; }
				else if (tempH >= 2 && tempH < 3) { R1 = 0; G1 = chroma; B1 = valX; }
				else if (tempH >= 3 && tempH < 4) { R1 = 0; G1 = valX; B1 = chroma; }
				else if (tempH >= 4 && tempH < 5) { R1 = valX; G1 = 0; B1 = chroma; }
				else if (tempH >= 5 && tempH < 6) { R1 = chroma; G1 = 0; B1 = valX; }
				else { R1 = 0; G1 = 0; B1 = 0; }
				float valM = V - chroma;

				float newR = R1 + valM;
				float newG = G1 + valM;
				float newB = B1 + valM;
				if (newR < 0) newR = 0;
				if (newR > 1) newR = 1;
				if (newG < 0) newG = 0;
				if (newG > 1) newG = 1;
				if (newB < 0) newB = 0;
				if (newB > 1) newB = 1;
				pixelData[i][j][0] = newR;
				pixelData[i][j][1] = newG;
				pixelData[i][j][2] = newB;
			}
		}
		imageType = RGB;
	}
}

float ImageData::standardDeviation(short channel, double& average) {
	double avg = 0;
	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			avg += pixelData[i][j][channel];
		}
	}
	avg /= (double)(width * height);
	
	double variance = 0;
	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			variance += pow(pixelData[i][j][channel] - avg, 2);
		}
	}
	variance /= (double)(width * height);
	average = avg;
	return sqrt(variance);
}

void ImageData::gaussianBlur(ImageData* source, float deviation, short channel) {
	std::vector<std::vector<float>> gaussianFilter(5, std::vector<float>(5));
	double sum = 0;
	for (float x = -2.0; x <= 2.0; x += 1.0) {
		for (float y = -2.0; y <= 2.0; y += 1.0) {
			float newValue = exp(-1.0 * (pow(x, 2) + pow(y, 2)) / (2.0 *  deviation)) / sqrt(2.0 * PI * deviation);
			gaussianFilter[x + 2][y + 2] = newValue;
			sum += newValue;
		}
	}
	for (float x = -2.0; x <= 2.0; x += 1.0) {
		for (float y = -2.0; y <= 2.0; y += 1.0) {	
			gaussianFilter[x + 2][y + 2] /= sum;
		}
	}
	for (unsigned int x = 0; x < width; x++) {
		for (unsigned int y = 0; y < height; y++) {
			float newPixel = 0.0;
			double filterSum = 1;
			for (int xFilt = -2; xFilt <= 2; xFilt++) {
				for (int yFilt = -2; yFilt <= 2; yFilt++) {
					if ((x + xFilt) >= 0 && (x + xFilt) < width && (y + yFilt) >= 0 && (y + yFilt) < height) {
						newPixel += gaussianFilter[xFilt + 2][yFilt + 2] * source->pixelData[x + xFilt][y + yFilt][channel];
						/*if (source->pixelData[x + xFilt][y + yFilt][channel] < 0) std::cout << source->pixelData[x + xFilt][y + yFilt][channel] << std::endl;*/
					}
					else filterSum -= gaussianFilter[xFilt + 2][yFilt + 2];
				}
			}
			//std::cout << source->pixelData[x][y][channel] << std::endl;
			//if (x == 20) std::cout << newPixel << " ";
			if (newPixel < 0) newPixel = 0;
			if (newPixel > 1) newPixel = 1;
			if (filterSum != 0) pixelData[x][y][channel] = newPixel / filterSum;
			else pixelData[x][y][channel] = newPixel;
		}
	}
}