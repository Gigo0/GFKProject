#pragma once

#include <wx/wxprec.h>
#include <wx/dcbuffer.h>
#include <wx/colordlg.h>
#include <iostream>

class InfoFrame : public wxFrame
{
public:
	InfoFrame();
	// Funkcja kt�ra przekazuje okienku InfoFrame zmienn� isInfo z g��wnego okna
	void setPointer(bool* ptr);
	// Setter do wy�wietlanego w oknie tekstu
	void setText(std::string text);
private:
	// Sizery (poziomy i pionowy)
	wxBoxSizer* mainSizer;
	wxBoxSizer* mainSizer2;
	// Wska�nik na zmienn� kt�ra m�wi czy obecnie jest wy�wietlone okno infoFrame
	bool* isInfo;

	// Pole tekstowe wy�wietlaj�ce tekst ustawiony przez setText()
	wxTextCtrl* tcInfo;

	void onClose(wxCloseEvent& e);
};