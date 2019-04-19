#include "NiirdPad.h"

#include "QImportConsole.h"
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
	
	connect(ui.actionImportConsole, &QAction::triggered, [this]() {
		QImportConsole Con(this);
		Con.exec();
	});
	
	//ui.widget->setFocus();
	//this->setMouseTracking(true);
}
