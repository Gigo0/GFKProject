#pragma once

#include <wx/wxprec.h>
#include <wx/dcbuffer.h>
#include <wx/colordlg.h>
#include <vector>
#include <math.h>

#define CHANNEL_RED 0
#define CHANNEL_GREEN 1
#define CHANNEL_BLUE 2
#define CHANNEL_LIGHT 2
#define CHANNEL_VALUE 2
#define PI 3.14159265

// Po³o¿enie prostok¹tnego zaznaczenia (jako u³amek ca³kowitego rozmiaru panelu)
struct RectSelection {
	RectSelection(float _x1, float _y1, float _x2, float _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}
	float x1;
	float y1;
	float x2;
	float y2;
};

enum ImageType { RGB, HSL, HSV };

class ImageData{
public:
	// Domyœlny konstruktor, tworzy pust¹ tablicê
	ImageData();
	// Konstruktor który wype³nia tablicê pikselami z obiektu wxImage w trybie RGB
	ImageData(wxImage& img, RectSelection selection);
	// Konstruktor kopiuj¹cy obraz
	ImageData(ImageData* obj);
	// Domyslny destruktor
	~ImageData();
	// Funkcja która "wstawia" zmodyfikowany obszar z powrotem do wxImage
	void applyToWxImage(wxImage& img);
	// Funkcja która konwertuje obiekt ImageData do nowego wxImage
	wxImage convertToWxImage();
	// Konwersja do HSL
	void convertToHSL();
	// Konwersja do HSV
	void convertToHSV();
	// Konwersja do RGB
	void convertToRGB();
	// Odchylenie standardowe wybranego kana³u. Wyliczona przy okazji œrednia przekazana jest do zmiennej average
	float standardDeviation(short channel, double& average);
	// Gaussian blur - s³u¿y do odszumiania
	void gaussianBlur(ImageData* source, float deviation, short channel);
private:
	// Tablica wszystkich pikseli (szerokoœæ x wysokoœæ x kana³)
	float*** pixelData;
	// Wymiary obszaru
	unsigned int width, height;
	// Pocz¹tek zaznaczonego obszaru obrazu (wzglêdem orygina³u)
	unsigned int xOffset, yOffset;
	// Rodzaj reprezentacji obrazu (RGB, HSL, HSV)
	ImageType imageType;
	// Funkcja która tworzy pust¹ tablicê
	void createEmptyImage();
	//Funkcje pomocnicze do konwersji kolorów
	float getMin(float a, float b, float c);
	float getMax(float a, float b, float c);
	float negMod(float value, float maxVal);
};