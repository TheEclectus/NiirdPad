#include "NiirdPad.h"

#include "QReferenceEditWindow.h"

NiirdPad::NiirdPad(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.actionEditReferenceWindowNew, &QAction::triggered, [this]() {
		std::string Res = "";
		QReferenceEditWindow::NewReference(this, Res);
	});
	connect(ui.actionEditReferenceWindowEdit, &QAction::triggered, [this]() {
		std::string Res = "";
		QReferenceEditWindow::EditReference(this, Res);
	});
	
	//ui.widget->setFocus();
	//this->setMouseTracking(true);
}
