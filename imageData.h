#pragma once

#include <wx/wxprec.h>
#include <wx/dcbuffer.h>
#include <wx/colordlg.h>
#include <vector>

enum ImageType { RGB, HSL, HSV };

class ImageData{
public:
	// Domy�lny konstruktor, tworzy pust� tablic�
	ImageData();
	// Konstruktor kt�ry wype�nia tablic� pikselami z obiektu wxImage w trybie RGB
	ImageData(wxImage& img);
	// Domyslny destruktor
	~ImageData();
	// Funkcja kt�ra konwertuje obiekt ImageData do wxImage
	wxImage convertToWxImage();
private:
	// Tablica wszystkich pikseli (szeroko�� x wysoko�� x kana�)
	float*** pixelData;
	// Wymiary obrazu
	unsigned int width;
	unsigned int height;
	// Rodzaj reprezentacji obrazu (RGB, HSL, HSV)
	ImageType imageType;
	// Funkcja kt�ra tworzy pust� tablic�
	void createEmptyImage();
};