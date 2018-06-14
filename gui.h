#pragma once

#include "imageData.h"
#include "noiseInfo.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

class GFKProject : public wxApp {
public:
	virtual bool OnInit();
};

class GFKFrame : public wxFrame {
public:
	GFKFrame();
	// Dobiera wymiary obrazu tak, by (z zachowaniem proporcji) zajmowa� mo�liwie najwi�ksz� cz�� pola o rozmiarach windowWidth, windowHeight
	void fitInWindow(wxImage& img, int windowWidth, int windowHeight, int& newWidth, int& newHeight);
	// Funkcja kt�ra uaktualnia obrazek w trybie podgl�du odszumiania
	void updatePreviewImg();
	// Funkcja kt�ra ustawia kontrolki w GUI
	void setGUI(bool isEditMode);
private:
	// Sizery
	wxBoxSizer* mainSizer;
	wxBoxSizer* mainSizer2;
	wxBoxSizer* panelSizer;
	wxBoxSizer* previewSizer;
	
	// Panel na kt�rym wy�wietlany jest obraz
	wxPanel* mainPanel;
	// Panele do podgl�du odszumiania przed potwierdzeniem go
	wxPanel* leftPreviewPanel;
	wxPanel* rightPreviewPanel;
	// Panel do wy�wietlania dodatkowych informacji
	wxPanel* textPanel;
	// Tekst wy�wietlany w textPanel
	std::string textPanelStr;
	// Panel do wy�wietlania informacji o poziomie rozmycia
	wxPanel* blurPanel;

	// Przycisk "Wczytaj obraz"
	wxButton* btLoad;
	// Przycisk "Zapisz obraz"
	wxButton* btSave;
	// Przycisk "Zresetuj zaznaczenie"
	wxButton* btReset;
	// Przycisk "Zbadaj szum"
	wxButton* btFindNoise;
	// Suwak do wyboru zaznaczonego obszaru
	wxScrollBar* sbArea;
	// Kontrolka do wyboru reprezentacji kolor�w
	wxChoice* chRepresentation;
	// Lista reprezentacji kolor�w
	wxArrayString chRepArray;
	// Checkboxy do zaznaczania rodzaju odszumiania
	wxCheckBox* cbRed;
	wxCheckBox* cbGreen;
	wxCheckBox* cbBlue;
	// Suwak do wyboru poziomu rozmycia
	wxScrollBar* sbBlur;
	// Obecna warto�� poziomu rozmycia
	float blurLevel;
	// Przycisk "Usu� szum"
	wxButton* btRemNoise;
	// Przycisk "Zako�cz odszumianie"
	wxButton* btFinish;
	// Pole tekstowe do debugowania
	wxTextCtrl* tcDebug;

	// Orygina� wczytanego obrazu. Nie modyfikowa�!
	wxImage originalImg;
	// Kopia obrazu
	wxImage copyImg;

	// Wektor ze wsp�rz�dnymi zaznaczonych obszar�w
	std::vector<RectSelection> selectedAreas;
	// Wektor z obrazami zaznaczonych obszar�w
	std::vector<ImageData*> areaImages;
	// Obraz wy�wietlany w trybie podgl�du
	ImageData* previewImgData;
	// Zmienna kt�ra m�wi czy obecnie jeste�my w trybie ustawiania prostok�tnego obszaru
	bool isSelectionMode;
	// Zmienna do sprawdzenia czy jest potrzeba od�wie�enia GUI
	bool isUpdateGUI;
	// Numer wybranego obszaru (w trybie por�wnywania przed i po usuni�ciu szumu). -1 przed wci�ni�ciem "Zbadaj szum"
	int areaNumber;

	// Okienka dialogowe do wczywywania i zapisywania
	wxFileDialog* dialLoad;
	wxFileDialog* dialSave;
	// Zmienna kt�ra m�wi czy obraz zosta� ju� wczytany
	bool isImgLoaded;
	// Obiekt ktory przekierowywuje strumie� std::cout do tcDebug
	wxStreamToTextRedirector* redirector;

	// Wska�nik na dodatkowe okno w kt�rym znajduj� si� informacje o szumie
	InfoFrame* infoFrame;
	// Wska�nik na zmienn� kt�ra m�wi czy obecnie jest wy�wietlone okno infoFrame
	bool* isInfo;

	// Handlery do zdarze�
	void btLoad_Click(wxCommandEvent& e);
	void btSave_Click(wxCommandEvent& e);
	void btReset_Click(wxCommandEvent& e);
	void btFindNoise_Click(wxCommandEvent& e);
	void sbArea_Scroll(wxScrollEvent& e);
	void chRepresentation_Changed(wxCommandEvent& e);
	void cbRed_Click(wxCommandEvent& e);
	void cbGreen_Click(wxCommandEvent& e);
	void cbBlue_Click(wxCommandEvent& e);
	void sbBlur_Scroll(wxScrollEvent& e);
	void btRemNoise_Click(wxCommandEvent& e);
	void btFinish_Click(wxCommandEvent& e);
	void mainPanel_Update(wxUpdateUIEvent& e);
	void mainPanel_MouseDown(wxMouseEvent& e);
	void mainPanel_MouseUp(wxMouseEvent& e);
	void mainPanel_MouseMove(wxMouseEvent& e);
	void leftPreviewPanel_Update(wxUpdateUIEvent& e);
	void rightPreviewPanel_Update(wxUpdateUIEvent& e);
	void textPanel_Update(wxUpdateUIEvent& e);
	void blurPanel_Update(wxUpdateUIEvent& e);
	void onClose(wxCloseEvent& e);
	void onExit(wxCommandEvent& e);
};