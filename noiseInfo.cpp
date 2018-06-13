#include "noiseInfo.h"

#define INFO_WIDTH 400
#define INFO_HEIGHT 400

InfoFrame::InfoFrame() : wxFrame(NULL, wxID_ANY, "Informacje o szumie", wxDefaultPosition, wxDefaultSize, wxCAPTION | wxCLOSE_BOX | wxMINIMIZE_BOX)
{
	SetSize(INFO_WIDTH, INFO_HEIGHT);

	mainSizer = new wxBoxSizer(wxHORIZONTAL);
	mainSizer2 = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(mainSizer);
	this->SetAutoLayout(true);
	mainSizer->Add(mainSizer2, 0, wxALIGN_TOP | wxALL, 5);

	tcInfo = new wxTextCtrl(this, wxID_ANY, _("Na razie nic tu nie ma. Lecz wkrótce..."), wxPoint(2, 2), wxSize(INFO_WIDTH, INFO_HEIGHT), wxTE_MULTILINE, wxDefaultValidator, _("tcInfo"));
	tcInfo->SetEditable(false);
	tcInfo->SetBackgroundColour(wxColor(200, 200, 200));
	mainSizer2->Add(tcInfo, 0, wxALIGN_CENTER, 5);

	Layout();
	GetSizer()->SetSizeHints(this);
	Center();

	Bind(wxEVT_CLOSE_WINDOW, &InfoFrame::onClose, this);
}

void InfoFrame::onClose(wxCloseEvent& e) { *isInfo = false; Destroy(); }

void InfoFrame::setPointer(bool* ptr) { isInfo = ptr; }

void InfoFrame::setText(std::string text) {
	tcInfo->SetValue(text);
}