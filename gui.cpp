#include "gui.h"

wxIMPLEMENT_APP(GFKProject);

bool GFKProject::OnInit() {
	GFKFrame* frame = new GFKFrame();
	frame->Show(true);
	return true;
}

GFKFrame::GFKFrame() : wxFrame(NULL, wxID_ANY, "Redukcja szumów") {
	SetSize(1000, 600);
	infoFrame = NULL;
	isInfo = new bool(false);

	mainSizer = new wxBoxSizer(wxHORIZONTAL);
	mainSizer2 = new wxBoxSizer(wxVERTICAL);
	panelSizer = new wxBoxSizer(wxVERTICAL);
	previewSizer = new wxBoxSizer(wxHORIZONTAL);
	this->SetSizer(mainSizer);
	this->SetAutoLayout(true);
	mainPanel = new wxPanel(this, wxID_ANY, wxPoint(2, 2), wxSize(500, 200));
	leftPreviewPanel = new wxPanel(this, wxID_ANY, wxPoint(2, 2), wxSize(160, 160));
	rightPreviewPanel = new wxPanel(this, wxID_ANY, wxPoint(2, 2), wxSize(160, 160));
	textPanel = new wxPanel(this, wxID_ANY, wxPoint(2, 2), wxSize(180, 110));
	blurPanel = new wxPanel(this, wxID_ANY, wxPoint(2, 2), wxSize(180, 25));

	panelSizer->Add(previewSizer, 2, wxALIGN_TOP | wxEXPAND | wxALL, 5);
	panelSizer->Add(mainPanel, 3, wxALIGN_LEFT | wxEXPAND | wxALL, 5);
	mainSizer->Add(panelSizer, 1, wxALIGN_TOP | wxEXPAND | wxALL, 5);
	mainSizer->Add(mainSizer2, 0, wxALIGN_TOP | wxALL, 5);

	previewSizer->Add(leftPreviewPanel, 1, wxALIGN_LEFT | wxEXPAND | wxALL, 5);
	previewSizer->Add(rightPreviewPanel, 1, wxALIGN_LEFT | wxEXPAND | wxALL, 5);

	chRepArray.Add("RGB");
	chRepArray.Add("HSL");
	chRepArray.Add("HSV");
	chRepArray.Add("LAB");

	btLoad = new wxButton(this, wxID_ANY, _("Wczytaj obraz"), wxPoint(10, 0), wxSize(180, 25), 0, wxDefaultValidator, _("btLoad"));
	btSave = new wxButton(this, wxID_ANY, _("Zapisz obraz"), wxPoint(10, 0), wxSize(180, 25), 0, wxDefaultValidator, _("btSave"));
	btReset = new wxButton(this, wxID_ANY, _("Zresetuj zaznaczenie"), wxPoint(10, 0), wxSize(180, 25), 0, wxDefaultValidator, _("btReset"));
	btFindNoise = new wxButton(this, wxID_ANY, _("Zbadaj szum"), wxPoint(10, 0), wxSize(180, 25), 0, wxDefaultValidator, _("btFindNoise"));
	sbArea = new wxScrollBar(this, wxID_ANY, wxPoint(10, 10), wxSize(180, 30), wxSB_HORIZONTAL, wxDefaultValidator, _("sbArea"));
	chRepresentation = new wxChoice(this, wxID_ANY, wxPoint(10, 10), wxSize(120, 30), chRepArray, 0, wxDefaultValidator, _("chRepresentation"));
	cbRed = new wxCheckBox(this, wxID_ANY, _("R"), wxPoint(10, 10), wxSize(180, 30), 0, wxDefaultValidator, _("cbRed"));
	cbGreen = new wxCheckBox(this, wxID_ANY, _("G"), wxPoint(10, 10), wxSize(180, 30), 0, wxDefaultValidator, _("cbGreen"));
	cbBlue = new wxCheckBox(this, wxID_ANY, _("B"), wxPoint(10, 10), wxSize(180, 30), 0, wxDefaultValidator, _("cbBlue"));
	sbBlur = new wxScrollBar(this, wxID_ANY, wxPoint(10, 10), wxSize(180, 30), wxSB_HORIZONTAL, wxDefaultValidator, _("sbBlur"));
	btRemNoise = new wxButton(this, wxID_ANY, _("Zastosuj"), wxPoint(10, 0), wxSize(180, 25), 0, wxDefaultValidator, _("btRemNoise"));
	btFinish = new wxButton(this, wxID_ANY, _("Zakoñcz odszumianie"), wxPoint(10, 0), wxSize(180, 25), 0, wxDefaultValidator, _("btFinish"));
	tcDebug = new wxTextCtrl(this, wxID_ANY, _(""), wxPoint(10, 0), wxSize(200, 200), wxTE_MULTILINE, wxDefaultValidator, _("tcDebug"));

	btSave->Enable(false);
	btReset->Enable(false);
	btFindNoise->Enable(false);

	originalImg.AddHandler(new wxJPEGHandler);
	originalImg.AddHandler(new wxBMPHandler);
	tcDebug->SetEditable(false);
	tcDebug->SetBackgroundColour(wxColor(200, 200, 200));
	sbArea->SetScrollbar(0, 0, 2, 10);
	sbBlur->SetScrollbar(50, 0, 100, 10);

	mainSizer2->Add(btLoad, 0, wxALIGN_CENTER, 5);
	mainSizer2->Add(btSave, 0, wxALIGN_CENTER | wxALL, 5);
	mainSizer2->Add(btReset, 0, wxALIGN_CENTER, 5);
	mainSizer2->Add(btFindNoise, 0, wxALIGN_CENTER | wxALL, 5);
	mainSizer2->Add(sbArea, 0, wxALIGN_CENTER, 5);
	mainSizer2->Add(textPanel, 1, wxALIGN_CENTER | wxALL, 5);
	mainSizer2->Add(chRepresentation, 0, wxALIGN_CENTER, 1);
	mainSizer2->Add(cbRed, 0, wxALIGN_CENTER, 5);
	mainSizer2->Add(cbGreen, 0, wxALIGN_CENTER, 5);
	mainSizer2->Add(cbBlue, 0, wxALIGN_CENTER, 5);
	mainSizer2->Add(sbBlur, 0, wxALIGN_CENTER, 5);
	mainSizer2->Add(blurPanel, 0, wxALIGN_CENTER | wxALL, 5);
	mainSizer2->Add(btRemNoise, 0, wxALIGN_CENTER, 5);
	mainSizer2->Add(btFinish, 0, wxALIGN_CENTER | wxALL, 5);
	mainSizer2->Add(tcDebug, 0, wxALIGN_CENTER, 5);

	Layout();
	GetSizer()->SetSizeHints(this);
	Center();

	btLoad->Bind(wxEVT_BUTTON, &GFKFrame::btLoad_Click, this);
	btSave->Bind(wxEVT_BUTTON, &GFKFrame::btSave_Click, this);
	btReset->Bind(wxEVT_BUTTON, &GFKFrame::btReset_Click, this);
	btFindNoise->Bind(wxEVT_BUTTON, &GFKFrame::btFindNoise_Click, this);
	sbArea->Bind(wxEVT_SCROLL_CHANGED, &GFKFrame::sbArea_Scroll, this);
	chRepresentation->Bind(wxEVT_CHOICE, &GFKFrame::chRepresentation_Changed, this);
	cbRed->Bind(wxEVT_CHECKBOX, &GFKFrame::cbRed_Click, this);
	cbGreen->Bind(wxEVT_CHECKBOX, &GFKFrame::cbGreen_Click, this);
	cbBlue->Bind(wxEVT_CHECKBOX, &GFKFrame::cbBlue_Click, this);
	sbBlur->Bind(wxEVT_SCROLL_CHANGED, &GFKFrame::sbBlur_Scroll, this);
	btRemNoise->Bind(wxEVT_BUTTON, &GFKFrame::btRemNoise_Click, this);
	btFinish->Bind(wxEVT_BUTTON, &GFKFrame::btFinish_Click, this);
	mainPanel->Bind(wxEVT_UPDATE_UI, &GFKFrame::mainPanel_Update, this);
	mainPanel->Bind(wxEVT_LEFT_DOWN, &GFKFrame::mainPanel_MouseDown, this);
	mainPanel->Bind(wxEVT_LEFT_UP, &GFKFrame::mainPanel_MouseUp, this);
	mainPanel->Bind(wxEVT_MOTION, &GFKFrame::mainPanel_MouseMove, this);
	leftPreviewPanel->Bind(wxEVT_UPDATE_UI, &GFKFrame::leftPreviewPanel_Update, this);
	rightPreviewPanel->Bind(wxEVT_UPDATE_UI, &GFKFrame::rightPreviewPanel_Update, this);
	textPanel->Bind(wxEVT_UPDATE_UI, &GFKFrame::textPanel_Update, this);
	blurPanel->Bind(wxEVT_UPDATE_UI, &GFKFrame::blurPanel_Update, this);
	Bind(wxEVT_CLOSE_WINDOW, &GFKFrame::onClose, this);

	selectedAreas = std::vector<RectSelection>(0, RectSelection(0, 0, 0, 0));
	isSelectionMode = false;

	dialLoad = new wxFileDialog(this, _("Podaj scie¿kê"), _(""), _(""), _("*"), wxFD_OPEN);
	dialSave = new wxFileDialog(this, _("Podaj scie¿kê"), _(""), _(""), _("*"), wxFD_SAVE);
	chRepresentation->SetSelection(0);
	isImgLoaded = false;
	isUpdateGUI = false;
	areaNumber = -1;
	blurLevel = 0.5;
	textPanelStr = "";
	areaImages = std::vector<ImageData*>(0);
	previewImgData = new ImageData();
	redirector = new wxStreamToTextRedirector(tcDebug);

	setGUI(false);

	std::cout << "Okienko do debugowania. Tu trafia wszystko co sie przekaze na std::cout" << std::endl << std::endl;
}

void GFKFrame::fitInWindow(wxImage& img, int windowWidth, int windowHeight, int& newWidth, int& newHeight) {
	unsigned int width = img.GetWidth();
	unsigned int height = img.GetHeight();
	if ((float)width / (float)height > (float)windowWidth / (float)windowHeight) {
		newWidth = windowWidth;
		newHeight = (float)windowWidth * (float)height / (float)width;
	}
	else {
		newWidth = (float)windowHeight * (float)width / (float)height;
		newHeight = windowHeight;
	}
}

void GFKFrame::updatePreviewImg() {
	// Zwolnij pamiêæ starego obrazu
	delete previewImgData;
	// Skopiuj obraz
	previewImgData = new ImageData(areaImages[areaNumber]);
	// Jeœli wybrano kana³ R, H lub L(od LAB), zastosuj na nim gaussian blur
	int representation = chRepresentation->GetSelection();
	if (cbRed->GetValue()) {
		if (representation == 0) { previewImgData->convertToRGB(); areaImages[areaNumber]->convertToRGB(); }
		if (representation == 1) { previewImgData->convertToHSL(); areaImages[areaNumber]->convertToHSL(); }
		if (representation == 2) { previewImgData->convertToHSV(); areaImages[areaNumber]->convertToHSV(); }
		if (representation == 3) { previewImgData->convertToLAB(); areaImages[areaNumber]->convertToLAB(); }
		previewImgData->gaussianBlur(areaImages[areaNumber], pow(10, blurLevel * 4 - 2), 0);
	}
	// Jeœli wybrano kana³ G lub S lub A, zastosuj na nim gaussian blur
	if (cbGreen->GetValue()) {
		if (representation == 0) { previewImgData->convertToRGB(); areaImages[areaNumber]->convertToRGB(); }
		if (representation == 1) { previewImgData->convertToHSL(); areaImages[areaNumber]->convertToHSL(); }
		if (representation == 2) { previewImgData->convertToHSV(); areaImages[areaNumber]->convertToHSV(); }
		if (representation == 3) { previewImgData->convertToLAB(); areaImages[areaNumber]->convertToLAB(); }
		previewImgData->gaussianBlur(areaImages[areaNumber], pow(10, blurLevel * 4 - 2), 1);
	}
	// Jeœli wybrano kana³ B(od RGB), L(od HSL), V lub B(od LAB), zastosuj na nim gaussian blur
	if (cbBlue->GetValue()) {
		if (representation == 0) { previewImgData->convertToRGB(); areaImages[areaNumber]->convertToRGB(); }
		if (representation == 1) { previewImgData->convertToHSL(); areaImages[areaNumber]->convertToHSL(); }
		if (representation == 2) { previewImgData->convertToHSV(); areaImages[areaNumber]->convertToHSV(); }
		if (representation == 3) { previewImgData->convertToLAB(); areaImages[areaNumber]->convertToLAB(); }
		previewImgData->gaussianBlur(areaImages[areaNumber], pow(10, blurLevel * 4 - 2), 2);
	}
	// Upewnij siê, ¿e obraz jest w trybie RGB
	previewImgData->convertToRGB();
	areaImages[areaNumber]->convertToRGB();
	// Wyœwietl informacje o poziomie rozmycia w panel blurPanel
	std::stringstream str;
	str << "Poziom rozmycia: " << blurLevel;
	textPanelStr = str.str();
}

void GFKFrame::setGUI(bool isEditMode) {
	// W zale¿noœci od argumentu isEditMode - poka¿ lub ukryj odpowiednie kontrolki
	previewSizer->Show(isEditMode);
	blurPanel->Show(isEditMode);
	chRepresentation->Show(isEditMode);
	sbArea->Show(isEditMode);
	cbRed->Show(isEditMode);
	cbGreen->Show(isEditMode);
	cbBlue->Show(isEditMode);
	sbBlur->Show(isEditMode);
	btRemNoise->Show(isEditMode);
	btFinish->Show(isEditMode);
	btLoad->Show(!isEditMode);
	btSave->Show(!isEditMode);
	btReset->Show(!isEditMode);
	btFindNoise->Show(!isEditMode);
	
	// Jeœli wy³¹czany jest tryb edycji (usuwanie szumów), zamknij okno InfoFrame
	if (!isEditMode && *isInfo) infoFrame->Close(true);
	if (!isEditMode) btFindNoise->Enable(false);
	isUpdateGUI = true;
}

void GFKFrame::onExit(wxCommandEvent& e) {
	if (isInfo != NULL) delete isInfo;
	Close(true);
}

void GFKFrame::onClose(wxCloseEvent& e) {
	// Jeœli okno InfoFrame jest wyœwietlone, zamknij je
	if (*isInfo) infoFrame->Close(true);
	// Zwolnij pamiêæ zajmowan¹ przez obrazy obszarów
	for (unsigned int i = 0; i < areaImages.size(); i++) {
		ImageData* readyToDelete = areaImages[i];
		delete readyToDelete;
	}
	// Oraz pamiêc zajmowan¹ przez obraz podgl¹du odszumiania
	delete previewImgData;
	Destroy();
}

void GFKFrame::btLoad_Click(wxCommandEvent& e) {
	dialLoad->SetWildcard("Obraz JPEG (*.jpg)|*.jpg|Obraz BMP (*.bmp)|*.bmp");
	// Wyswietl okno dialogowe i sprawdŸ czy podana zosta³a œcie¿ka
	if (dialLoad->ShowModal() == wxID_OK) {
		wxString filename = dialLoad->GetPath();
		// Jesli uda³o siê wczytaæ obraz
		if (originalImg.LoadFile(filename)) {
			isImgLoaded = true;
			// Utwórz kopiê obrazu, ¿eby mo¿na by³o go modyfikowaæ
			copyImg = originalImg.Copy();
			// Zwolnij pamiêæ zajmowan¹ przez obrazy obszarów
			for (unsigned int i = 0; i < areaImages.size(); i++) {
				ImageData* readyToDelete = areaImages[i];
				delete readyToDelete;
			}
			// Utwórz nowy, pusty wektor obrazów obszarów
			areaImages = std::vector<ImageData*>(0);

			// Zresetuj zaznaczone obszary
			selectedAreas = std::vector<RectSelection>(0, RectSelection(0, 0, 0, 0));

			// Ustaw przyciski na aktywne, ¿eby da³o siê je wciskaæ
			btSave->Enable(true);
			btReset->Enable(true);
			btFindNoise->Enable(false);
		}
	}
}

void GFKFrame::btSave_Click(wxCommandEvent& e) {
	dialSave->SetWildcard("Obraz JPEG (*.jpg)|*.jpg");
	// Wyswietl okno dialogowe i sprawdŸ czy podana zosta³a œcie¿ka
	if (dialSave->ShowModal() == wxID_OK) {
		wxString filename = dialSave->GetPath();
		// Zapisz obraz
		copyImg.SaveFile(filename, wxBITMAP_TYPE_JPEG);
	}
}

void GFKFrame::btReset_Click(wxCommandEvent& e) {
	// Jeœli nie jesteœmy w trybie ustawiania obszaru (wciœniêta mysz) - ustaw wektor obszarów na pusty
	if (!isSelectionMode) selectedAreas = std::vector<RectSelection>(0, RectSelection(0, 0, 0, 0));
	// Ustaw przycisk "Zbadaj szum" na aktywny, by da³o siê go wcisn¹æ
	btFindNoise->Enable(false);
}

void GFKFrame::btFindNoise_Click(wxCommandEvent& e) {
	// SprawdŸ czy zaznaczone zosta³y jakiekolwiek obszary
	if (isSelectionMode || selectedAreas.size() == 0) return;
	// Zwolnij pamiêæ zajmowan¹ przez obrazy obszarów
	for (unsigned int i = 0; i < areaImages.size(); i++) {
		ImageData* readyToDelete = areaImages[i];
		delete readyToDelete;
	}
	// Utwórz pusty wektor obrazów obszarów
	areaImages = std::vector<ImageData*>(0);
	// I dodaj do niego obrazy na podstawie zaznaczonych obszarów
	for (unsigned int i = 0; i < selectedAreas.size(); i++) {
		areaImages.push_back(new ImageData(copyImg, selectedAreas[i]));
	}

	// report bêdzie zawiera³ tekst do wyœwietlenia w oknie InfoFrame
	std::stringstream report;
	for (unsigned int i = 0; i < areaImages.size(); i++) {
		report << "OBSZAR " << (i + 1) << std::endl << "Informacje o kana³ach:" << std::endl;
		report << std::fixed << std::setprecision(2);
		float devRed, devGreen, devBlue, devLight, devValue;
		double avgRed, avgGreen, avgBlue, avgLight, avgValue;

		// Oblicz œrednie wartoœci kana³ów oraz ich odchylenia standardowe
		devRed = areaImages[i]->standardDeviation(CHANNEL_RED, avgRed);
		devGreen = areaImages[i]->standardDeviation(CHANNEL_GREEN, avgGreen);
		devBlue = areaImages[i]->standardDeviation(CHANNEL_BLUE, avgBlue);
		areaImages[i]->convertToHSL();
		devLight = areaImages[i]->standardDeviation(CHANNEL_LIGHT, avgLight);
		areaImages[i]->convertToHSV();
		devValue = areaImages[i]->standardDeviation(CHANNEL_VALUE, avgValue);
		areaImages[i]->convertToRGB();

		// Wypisz uzyskane wartoœci
		report << "R: œrednie: " << avgRed << ", odchylenie: " << devRed << std::endl;
		report << "G: œrednie: " << avgGreen << ", odchylenie: " << devGreen << std::endl;
		report << "B: œrednie: " << avgBlue << ", odchylenie: " << devBlue << std::endl;
		report << "L: œrednie: " << avgLight << ", odchylenie: " << devLight << std::endl;
		report << "V: œrednie: " << avgValue << ", odchylenie: " << devValue << std::endl;

		// Wykryj rodzaj szumów na podstawie uzyskanych wartoœci
		short recommendedImgType = -1;
		double maxDev = devRed;
		if (devGreen > maxDev) maxDev = devGreen;
		if (devBlue > maxDev) maxDev = devBlue;
		if (devLight > maxDev) maxDev = devLight;
		if (devValue > maxDev) maxDev = devValue;
		if (maxDev >= 0.08) {
			if (maxDev == devRed || maxDev == devGreen || maxDev == devBlue) recommendedImgType = 0;
			if (maxDev == devLight) recommendedImgType = 1;
			if (maxDev == devValue) recommendedImgType = 2;
		}

		// Wypisz informacje o rodzaju szumów
		if (recommendedImgType == -1)
			report << "Brak istotnych szumów" << std::endl;
		else {
			if (recommendedImgType == 0)
				report << "Wykryto szumy na kana³ach:" << (devRed >= 0.08 ? " R," : "") << (devGreen >= 0.08 ? " G," : "") << (devBlue >= 0.08 ? " B" : "") << std::endl;
			if (recommendedImgType == 1)
				report << "Wykryto szum poziomu jasnoœci (kana³ L w reprezentacji HSL)" << std::endl;
			if (recommendedImgType == 2)
				report << "Wykryto szum wartoœci koloru (kana³u V w reprezentacji HSV)" << std::endl;
		}
		report << std::endl;
	}

	// Jeœli okno InfoFrame nie jest wyœwietlone, wyœwietl je i przeka¿ mu tekst przechowywany w report
	if (*isInfo == false) {
		infoFrame = new InfoFrame();
		infoFrame->setPointer(isInfo);
		*isInfo = true;
		infoFrame->setText(report.str());
		infoFrame->Show(true);
	}

	// Dostosuj GUI do trybu usuwania szumów
	previewSizer->Show(true);
	areaNumber = 0;
	sbArea->SetScrollbar(0, 0, areaImages.size()-1, 10);
	setGUI(true);
	updatePreviewImg();
}

void GFKFrame::sbArea_Scroll(wxScrollEvent& e) {
	if (areaNumber == -1) return;
	// Uzyskaj numer zaznaczonego obszaru
	areaNumber = sbArea->GetThumbPosition();
	updatePreviewImg();
}

// Jeœli zmieniony zosta³ kana³, uaktualnij podgl¹d
void GFKFrame::cbRed_Click(wxCommandEvent& e) { updatePreviewImg(); }
void GFKFrame::cbGreen_Click(wxCommandEvent& e) { updatePreviewImg(); }
void GFKFrame::cbBlue_Click(wxCommandEvent& e) { updatePreviewImg(); }

void GFKFrame::chRepresentation_Changed(wxCommandEvent& e) {
	updatePreviewImg();
	if (chRepresentation->GetSelection() == 0) {
		cbRed->SetLabelText("R");
		cbGreen->SetLabelText("G");
		cbBlue->SetLabelText("B");
	}
	if (chRepresentation->GetSelection() == 1) {
		cbRed->SetLabelText("H");
		cbGreen->SetLabelText("S");
		cbBlue->SetLabelText("L");
	}
	if (chRepresentation->GetSelection() == 2) {
		cbRed->SetLabelText("H");
		cbGreen->SetLabelText("S");
		cbBlue->SetLabelText("V");
	}
	if (chRepresentation->GetSelection() == 3) {
		cbRed->SetLabelText("L");
		cbGreen->SetLabelText("A");
		cbBlue->SetLabelText("B");
	}
}

void GFKFrame::btRemNoise_Click(wxCommandEvent& e) {
	// Przeka¿ obraz z obiektu ImageData do wxImage
	previewImgData->applyToWxImage(copyImg);
	textPanelStr = "Zmiany wprowadzone!";
}

void GFKFrame::btFinish_Click(wxCommandEvent& e) {
	// Dostosuj GUI do trybu zaznaczania obszarów
	selectedAreas = std::vector<RectSelection>(0, RectSelection(0, 0, 0, 0));
	isSelectionMode = false;
	areaNumber = -1;
	setGUI(false);
	isUpdateGUI = true;
}

void GFKFrame::sbBlur_Scroll(wxScrollEvent& e) {
	// Uzyskaj wartoœæ poziomu rozmycia
	blurLevel = sbBlur->GetThumbPosition() / 100.0;
	if (areaNumber != -1) updatePreviewImg();
}

void GFKFrame::mainPanel_MouseDown(wxMouseEvent& e) {
	if (!isImgLoaded || areaNumber != -1) return;
	wxPoint mousePos = e.GetPosition();
	int panelWidth, panelHeight, imgWidth, imgHeight, xOffset, yOffset;
	mainPanel->GetClientSize(&panelWidth, &panelHeight);
	fitInWindow(copyImg, panelWidth, panelHeight, imgWidth, imgHeight);
	xOffset = (panelWidth / 2) - (imgWidth / 2);
	yOffset = (panelHeight / 2) - (imgHeight / 2);
	if (!isSelectionMode && isImgLoaded) {
		isSelectionMode = true;
		RectSelection curSel((float)(mousePos.x - xOffset) / (float)imgWidth, (float)(mousePos.y - yOffset) / (float)imgHeight, (float)(mousePos.x - xOffset) / (float)imgWidth, (float)(mousePos.y - yOffset) / (float)imgHeight);
		selectedAreas.push_back(curSel);
	}
}

void GFKFrame::mainPanel_MouseUp(wxMouseEvent& e) {
	if (!isImgLoaded) return;
	int areas = selectedAreas.size();
	if (areas == 0) return;
	isSelectionMode = false;
	if (selectedAreas[areas - 1].x2 < selectedAreas[areas - 1].x1) {
		float tempCoord = selectedAreas[areas - 1].x1;
		selectedAreas[areas - 1].x1 = selectedAreas[areas - 1].x2;
		selectedAreas[areas - 1].x2 = tempCoord;
	}
	if (selectedAreas[areas - 1].y2 < selectedAreas[areas - 1].y1) {
		float tempCoord = selectedAreas[areas - 1].y1;
		selectedAreas[areas - 1].y1 = selectedAreas[areas - 1].y2;
		selectedAreas[areas - 1].y2 = tempCoord;
	}
	if (selectedAreas[areas - 1].x1 < 0 || selectedAreas[areas - 1].y1 < 0 || selectedAreas[areas - 1].x2 >= 1 || selectedAreas[areas - 1].y2 >= 1) selectedAreas.pop_back();
	if ((selectedAreas[areas - 1].x2 - selectedAreas[areas - 1].x1) * copyImg.GetWidth() <= 2 || (selectedAreas[areas - 1].y2 - selectedAreas[areas - 1].y1) * copyImg.GetHeight() <= 2) selectedAreas.pop_back();
	else btFindNoise->Enable(true);
}

void GFKFrame::mainPanel_MouseMove(wxMouseEvent& e) {
	if (!isImgLoaded) return;
	wxPoint mousePos = e.GetPosition();
	int panelWidth, panelHeight, imgWidth, imgHeight, xOffset, yOffset;
	mainPanel->GetClientSize(&panelWidth, &panelHeight);
	fitInWindow(copyImg, panelWidth, panelHeight, imgWidth, imgHeight);
	xOffset = (panelWidth / 2) - (imgWidth / 2);
	yOffset = (panelHeight / 2) - (imgHeight / 2);
	if (isSelectionMode) {
		selectedAreas[selectedAreas.size() - 1].x2 = (float)(mousePos.x - xOffset) / (float)imgWidth;
		selectedAreas[selectedAreas.size() - 1].y2 = (float)(mousePos.y - yOffset) / (float)imgHeight;
	}
}

void GFKFrame::textPanel_Update(wxUpdateUIEvent& e) {
	wxClientDC myOtherDC(textPanel);
	wxBufferedDC myDC(&myOtherDC);
	myDC.Clear();
	if (areaNumber == -1) myDC.DrawText("Po za³adowaniu obrazu\r\nzaznacz kursorem\nprostok¹tne obszary,\na nastêpnie kliknij\n\"Zbadaj szum\".", 5, 3);
	else {
		std::stringstream str;
		str << "Wybrany obszar: " << (areaNumber + 1) << "/" << areaImages.size() << "." << std::endl;
		str << "Zaznacz kana³y do" << std::endl << "odszumienia. Wybór" << std::endl << "ograniczony do jednej" << std::endl << "reprezentacji kolorów!";
		myDC.DrawText(str.str(), 5, 3);
	}
	if (isUpdateGUI) { Refresh(); isUpdateGUI = false; }
}

void GFKFrame::blurPanel_Update(wxUpdateUIEvent& e) {
	wxClientDC myOtherDC(blurPanel);
	wxBufferedDC myDC(&myOtherDC);
	myDC.Clear();
	myDC.DrawText(textPanelStr, 5, 4);
}

void GFKFrame::mainPanel_Update(wxUpdateUIEvent& e) {
	// Przygotowanie obiektu bufferedDC na panelu g³ównym do rysowania
	wxClientDC myOtherDC(mainPanel);
	wxBufferedDC myDC(&myOtherDC);
	if (areaNumber == -1) myDC.SetBrush(wxBrush(wxColour(255, 255, 255), wxBRUSHSTYLE_BDIAGONAL_HATCH));
	else myDC.SetBrush(*wxTRANSPARENT_BRUSH);
	myDC.SetPen(wxPen(RGB(255, 0, 0), 3));
	myDC.Clear();
	// Uzyskanie szerokoœci i wysokoœci panelu oraz znalezienie wspó³rzêdnych œrodka
	int panelWidth, panelHeight, xMiddle, yMiddle;
	mainPanel->GetClientSize(&panelWidth, &panelHeight);
	xMiddle = panelWidth / 2;
	yMiddle = panelHeight / 2;
	// Jeœli obraz jest wczytany
	if (isImgLoaded && !this->IsIconized()) {
		// Stwórz tymczasowy obiekt wxImage do przechowywania obrazu, którego rozmiar jest dopasowany do wielkoœci okna
		wxImage resizedImg = copyImg.Copy();
		// Dopasuj obraz do okna
		int newWidth, newHeight, xOffset, yOffset;
		fitInWindow(resizedImg, panelWidth, panelHeight, newWidth, newHeight);
		xOffset = xMiddle - (newWidth / 2);
		yOffset = yMiddle - (newHeight / 2);
		resizedImg.Rescale(newWidth, newHeight);
		// Wyœwietl obraz na œrodku panelu za pomoc¹ obiektu wxBitmap
		wxBitmap mainBMP(resizedImg);
		myDC.DrawBitmap(mainBMP, xMiddle - resizedImg.GetWidth() / 2, yMiddle - resizedImg.GetHeight() / 2);

		// Rysowanie zaznaczenia obszaru
		if (areaNumber == -1) {
			for (unsigned int i = 0; i < selectedAreas.size(); i++) {
				myDC.DrawRectangle(selectedAreas[i].x1 * newWidth + xOffset, selectedAreas[i].y1 * newHeight + yOffset, selectedAreas[i].x2 * newWidth - selectedAreas[i].x1 * newWidth, selectedAreas[i].y2 * newHeight - selectedAreas[i].y1 * newHeight);
			}
		}
		else myDC.DrawRectangle(selectedAreas[areaNumber].x1 * newWidth + xOffset, selectedAreas[areaNumber].y1 * newHeight + yOffset, selectedAreas[areaNumber].x2 * newWidth - selectedAreas[areaNumber].x1 * newWidth, selectedAreas[areaNumber].y2 * newHeight - selectedAreas[areaNumber].y1 * newHeight);
	}
}

void GFKFrame::leftPreviewPanel_Update(wxUpdateUIEvent& e) {
	if (areaNumber == -1 || this->IsIconized()) return;
	wxClientDC myOtherDC(leftPreviewPanel);
	wxBufferedDC myDC(&myOtherDC);
	myDC.Clear();
	int panelWidth, panelHeight, xMiddle, yMiddle, newWidth, newHeight;
	leftPreviewPanel->GetClientSize(&panelWidth, &panelHeight);
	xMiddle = panelWidth / 2;
	yMiddle = panelHeight / 2;
	wxImage previewImg = areaImages[areaNumber]->convertToWxImage();
	fitInWindow(previewImg, panelWidth, panelHeight, newWidth, newHeight);
	previewImg.Rescale(newWidth, newHeight);
	wxBitmap mainBMP(previewImg);
	myDC.DrawBitmap(mainBMP, xMiddle - previewImg.GetWidth() / 2, yMiddle - previewImg.GetHeight() / 2);
	myDC.SetBrush(wxBrush(wxColour(255, 255, 200), wxBRUSHSTYLE_SOLID));
	myDC.SetPen(wxPen(RGB(255, 0, 0), 2));
	myDC.DrawRectangle(0, 0, 150, 25);
	myDC.DrawText("Przed odszumieniem", 5, 2);
}

void GFKFrame::rightPreviewPanel_Update(wxUpdateUIEvent& e) { 
	Layout();
	if (areaNumber == -1 || this->IsIconized()) return;
	wxClientDC myOtherDC(rightPreviewPanel);
	wxBufferedDC myDC(&myOtherDC);
	myDC.Clear();
	int panelWidth, panelHeight, xMiddle, yMiddle, newWidth, newHeight;
	rightPreviewPanel->GetClientSize(&panelWidth, &panelHeight);
	xMiddle = panelWidth / 2;
	yMiddle = panelHeight / 2;
	wxImage previewImg = previewImgData->convertToWxImage();
	fitInWindow(previewImg, panelWidth, panelHeight, newWidth, newHeight);
	previewImg.Rescale(newWidth, newHeight);
	wxBitmap mainBMP(previewImg);
	myDC.DrawBitmap(mainBMP, xMiddle - previewImg.GetWidth() / 2, yMiddle - previewImg.GetHeight() / 2);
	myDC.SetBrush(wxBrush(wxColour(255, 255, 200), wxBRUSHSTYLE_SOLID));
	myDC.SetPen(wxPen(RGB(255, 0, 0), 2));
	myDC.DrawRectangle(0, 0, 116, 25);
	myDC.DrawText("Po odszumieniu", 5, 2);
}