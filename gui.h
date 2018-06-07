#pragma once

#include "imageData.h"
#include <iostream>

class GFKProject : public wxApp
{
public:
	virtual bool OnInit();
};

class GFKFrame : public wxFrame
{
public:
	GFKFrame();
	// Dopasowuje rozmiar obrazu tak, by (z zachowaniem proporcji) zajmowa³ mo¿liwie najwiêksz¹ czêœæ pola o rozmiarach windowWidth, windowHeight
	void fitInWindow(wxImage& img, unsigned int windowWidth, unsigned int windowHeight);
private:
	// Sizery: poziomy i pionowy
	wxBoxSizer* mainSizer;
	wxBoxSizer* mainSizer2;
	// Panel na którym wyswietla sie obraz
	wxPanel* mainPanel;

	// Przycisk "Wczytaj:
	wxButton* btLoad;
	// Przycisk "Zapisz"
	wxButton* btSave;
	// Pole tekstowe do debugowania
	wxTextCtrl* tcDebug;

	// Orygina³ wczytanego obrazu. Nie modyfikowac!
	wxImage originalImg;
	// Kopia obrazu
	wxImage copyImg;
	// obiekt ImageData do przechowywania obrazu i modyfikowania go
	ImageData* image;

	// Okienka dialogowe do wczywywania i zapisywania
	wxFileDialog* dialLoad;
	wxFileDialog* dialSave;
	// Zmienna ktora mowi czy jest jakis obraz wczytany
	bool isImgLoaded;
	// Obiekt ktory przekierowywuje strumien std::cout do tcDebug
	wxStreamToTextRedirector* redirector;

	// Handlery do eventów
	void btLoad_Click(wxCommandEvent& e);
	void btSave_Click(wxCommandEvent& e);
	void mainPanel_Update(wxUpdateUIEvent& e);
	void onClose(wxCloseEvent& e);
	void onExit(wxCommandEvent& e);

};