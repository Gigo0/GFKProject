#pragma once

#include <wx/wxprec.h>
#include <wx/dcbuffer.h>
#include <wx/colordlg.h>
#include <vector>

enum ImageType { RGB, HSL, HSV };

class ImageData{
public:
	// Domyœlny konstruktor, tworzy pust¹ tablicê
	ImageData();
	// Konstruktor który wype³nia tablicê pikselami z obiektu wxImage w trybie RGB
	ImageData(wxImage& img);
	// Domyslny destruktor
	~ImageData();
	// Funkcja która konwertuje obiekt ImageData do wxImage
	wxImage convertToWxImage();
private:
	// Tablica wszystkich pikseli (szerokoœæ x wysokoœæ x kana³)
	float*** pixelData;
	// Wymiary obrazu
	unsigned int width;
	unsigned int height;
	// Rodzaj reprezentacji obrazu (RGB, HSL, HSV)
	ImageType imageType;
	// Funkcja która tworzy pust¹ tablicê
	void createEmptyImage();
};