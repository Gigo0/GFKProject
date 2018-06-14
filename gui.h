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
	// Dobiera wymiary obrazu tak, by (z zachowaniem proporcji) zajmowa³ mo¿liwie najwiêksz¹ czêœæ pola o rozmiarach windowWidth, windowHeight
	void fitInWindow(wxImage& img, int windowWidth, int windowHeight, int& newWidth, int& newHeight);
	// Funkcja która uaktualnia obrazek w trybie podgl¹du odszumiania
	void updatePreviewImg();
	// Funkcja która ustawia kontrolki w GUI
	void setGUI(bool isEditMode);
private:
	// Sizery
	wxBoxSizer* mainSizer;
	wxBoxSizer* mainSizer2;
	wxBoxSizer* panelSizer;
	wxBoxSizer* previewSizer;
	
	// Panel na którym wyœwietlany jest obraz
	wxPanel* mainPanel;
	// Panele do podgl¹du odszumiania przed potwierdzeniem go
	wxPanel* leftPreviewPanel;
	wxPanel* rightPreviewPanel;
	// Panel do wyœwietlania dodatkowych informacji
	wxPanel* textPanel;
	// Tekst wyœwietlany w textPanel
	std::string textPanelStr;
	// Panel do wyœwietlania informacji o poziomie rozmycia
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
	// Kontrolka do wyboru reprezentacji kolorów
	wxChoice* chRepresentation;
	// Lista reprezentacji kolorów
	wxArrayString chRepArray;
	// Checkboxy do zaznaczania rodzaju odszumiania
	wxCheckBox* cbRed;
	wxCheckBox* cbGreen;
	wxCheckBox* cbBlue;
	// Suwak do wyboru poziomu rozmycia
	wxScrollBar* sbBlur;
	// Obecna wartoœæ poziomu rozmycia
	float blurLevel;
	// Przycisk "Usuñ szum"
	wxButton* btRemNoise;
	// Przycisk "Zakoñcz odszumianie"
	wxButton* btFinish;
	// Pole tekstowe do debugowania
	wxTextCtrl* tcDebug;

	// Orygina³ wczytanego obrazu. Nie modyfikowaæ!
	wxImage originalImg;
	// Kopia obrazu
	wxImage copyImg;

	// Wektor ze wspó³rzêdnymi zaznaczonych obszarów
	std::vector<RectSelection> selectedAreas;
	// Wektor z obrazami zaznaczonych obszarów
	std::vector<ImageData*> areaImages;
	// Obraz wyœwietlany w trybie podgl¹du
	ImageData* previewImgData;
	// Zmienna która mówi czy obecnie jesteœmy w trybie ustawiania prostok¹tnego obszaru
	bool isSelectionMode;
	// Zmienna do sprawdzenia czy jest potrzeba odœwie¿enia GUI
	bool isUpdateGUI;
	// Numer wybranego obszaru (w trybie porównywania przed i po usuniêciu szumu). -1 przed wciœniêciem "Zbadaj szum"
	int areaNumber;

	// Okienka dialogowe do wczywywania i zapisywania
	wxFileDialog* dialLoad;
	wxFileDialog* dialSave;
	// Zmienna która mówi czy obraz zosta³ ju¿ wczytany
	bool isImgLoaded;
	// Obiekt ktory przekierowywuje strumieñ std::cout do tcDebug
	wxStreamToTextRedirector* redirector;

	// WskaŸnik na dodatkowe okno w którym znajduj¹ siê informacje o szumie
	InfoFrame* infoFrame;
	// WskaŸnik na zmienn¹ która mówi czy obecnie jest wyœwietlone okno infoFrame
	bool* isInfo;

	// Handlery do zdarzeñ
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