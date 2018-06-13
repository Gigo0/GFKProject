#pragma once

#include <wx/wxprec.h>
#include <wx/dcbuffer.h>
#include <wx/colordlg.h>
#include <iostream>

class InfoFrame : public wxFrame
{
public:
	InfoFrame();
	// Funkcja która przekazuje okienku InfoFrame zmienn¹ isInfo z g³ównego okna
	void setPointer(bool* ptr);
	// Setter do wyœwietlanego w oknie tekstu
	void setText(std::string text);
private:
	// Sizery (poziomy i pionowy)
	wxBoxSizer* mainSizer;
	wxBoxSizer* mainSizer2;
	// WskaŸnik na zmienn¹ która mówi czy obecnie jest wyœwietlone okno infoFrame
	bool* isInfo;

	// Pole tekstowe wyœwietlaj¹ce tekst ustawiony przez setText()
	wxTextCtrl* tcInfo;

	void onClose(wxCloseEvent& e);
};