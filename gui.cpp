#include "gui.h"

wxIMPLEMENT_APP(GFKProject);

bool GFKProject::OnInit() {
	GFKFrame* frame = new GFKFrame();
	frame->Show(true);
	return true;
}

GFKFrame::GFKFrame() : wxFrame(NULL, wxID_ANY, "Redukcja szum�w") {
	SetSize(1000, 600);

	mainSizer = new wxBoxSizer(wxHORIZONTAL);
	mainSizer2 = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(mainSizer);
	this->SetAutoLayout(true);
	mainPanel = new wxPanel(this, wxID_ANY, wxPoint(4, 4), wxSize(500, 200));
	mainSizer->Add(mainPanel, 1, wxALIGN_LEFT | wxEXPAND | wxALL, 5);
	mainSizer->Add(mainSizer2, 0, wxALIGN_TOP | wxALL, 5);

	btLoad = new wxButton(this, wxID_ANY, _("Wczytaj obraz"), wxPoint(10, 0), wxSize(180, 25), 0, wxDefaultValidator, _("btLoad"));
	btSave = new wxButton(this, wxID_ANY, _("Zapisz obraz"), wxPoint(10, 0), wxSize(180, 25), 0, wxDefaultValidator, _("btSave"));
	tcDebug = new wxTextCtrl(this, wxID_ANY, _(""), wxPoint(10, 0), wxSize(200, 200), wxTE_MULTILINE, wxDefaultValidator, _("tcDebug"));

	btSave->Enable(false);

	originalImg.AddHandler(new wxJPEGHandler);
	originalImg.AddHandler(new wxBMPHandler);
	tcDebug->SetEditable(false);
	tcDebug->SetBackgroundColour(wxColor(200, 200, 200));

	mainSizer2->Add(btLoad, 0, wxALIGN_CENTER, 5);
	mainSizer2->Add(btSave, 0, wxALIGN_CENTER | wxALL, 5);
	mainSizer2->Add(tcDebug, 0, wxALIGN_CENTER, 5);

	Layout();
	GetSizer()->SetSizeHints(this);
	Center();

	btLoad->Bind(wxEVT_BUTTON, &GFKFrame::btLoad_Click, this);
	btSave->Bind(wxEVT_BUTTON, &GFKFrame::btSave_Click, this);
	mainPanel->Bind(wxEVT_UPDATE_UI, &GFKFrame::mainPanel_Update, this);
	Bind(wxEVT_CLOSE_WINDOW, &GFKFrame::onClose, this);

	dialLoad = new wxFileDialog(this, _("Podaj sciezke"), _(""), _(""), _("*"), wxFD_OPEN);
	dialSave = new wxFileDialog(this, _("Podaj sciezke"), _(""), _(""), _("*"), wxFD_SAVE);
	isImgLoaded = false;
	image = new ImageData();
	redirector = new wxStreamToTextRedirector(tcDebug);

	std::cout << "Okienko do debugowania. Tu trafia wszystko co sie przekaze na std::cout" << std::endl << std::endl;
}

void GFKFrame::fitInWindow(wxImage& img, unsigned int windowWidth, unsigned int windowHeight) {
	unsigned int width = img.GetWidth();
	unsigned int height = img.GetHeight();
	if ((double)width / (double)height > (double)windowWidth / (double)windowHeight) img.Rescale(windowWidth, (double)windowWidth * (double)height / (double)width);
	else img.Rescale((double)windowHeight * (double)width / (double)height, windowHeight);
}

void GFKFrame::onExit(wxCommandEvent& e) {
	Close(true);
}

void GFKFrame::onClose(wxCloseEvent& e) {
	delete image;
	Destroy();
}

void GFKFrame::btLoad_Click(wxCommandEvent& e) {
	dialLoad->SetWildcard("Obraz JPEG (*.jpg)|*.jpg|Obraz BMP (*.bmp)|*.bmp");
	// Wyswietl okno dialogowe
	if (dialLoad->ShowModal() == wxID_OK) {
		wxString filename = dialLoad->GetPath();
		// Jesli uda�o si� wczyta� obraz
		if (originalImg.LoadFile(filename)) {
			isImgLoaded = true;
			// Utw�rz kopi� obrazu, �eby mo�na by�o go modyfikowa�
			copyImg = originalImg.Copy();
			delete image;
			image = new ImageData(copyImg);
			//image.loadFromWxImage(copyImg);

			// Ustaw przyciski na aktywne, �eby da�o si� je wciska�
			btSave->Enable(true);
		}
	}
}

void GFKFrame::btSave_Click(wxCommandEvent& e) {
	dialSave->SetWildcard("Obraz JPEG (*.jpg)|*.jpg");
	// Wyswietl okno dialogowe
	if (dialSave->ShowModal() == wxID_OK) {
		wxString filename = dialSave->GetPath();
		// Zapisz obraz
		copyImg.SaveFile(filename, wxBITMAP_TYPE_JPEG);
	}
}

void GFKFrame::mainPanel_Update(wxUpdateUIEvent& e) {
	// Przygotowanie obiektu bufferedDC na panelu g��wnym do rysowania
	wxClientDC myOtherDC(mainPanel);
	wxBufferedDC myDC(&myOtherDC);
	myDC.Clear();
	// Uzyskanie szeroko�ci i wysoko�ci panelu oraz znalezienie wsp�rz�dnych �rodka
	int panelWidth, panelHeight;
	int xMiddle, yMiddle;
	mainPanel->GetClientSize(&panelWidth, &panelHeight);
	xMiddle = panelWidth / 2.;
	yMiddle = panelHeight / 2.;
	// Je�li obraz jest wczytany
	if (isImgLoaded) {
		// Stw�rz tymczasowy obiekt wxImage do przechowywania obrazu, kt�rego rozmiar jest dopasowany do wielko�ci okna
		wxImage resizedImg = copyImg.Copy();
		// Dopasuj do okna
		fitInWindow(resizedImg, panelWidth, panelHeight);
		// Wy�wietl obraz na �rodku panelu za pomoc� obiektu wxBitmap
		wxBitmap mainBMP(resizedImg);
		myDC.DrawBitmap(mainBMP, xMiddle - resizedImg.GetWidth() / 2.0, yMiddle - resizedImg.GetHeight() / 2.0);
	}
}