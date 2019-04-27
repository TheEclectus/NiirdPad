#include "NiirdPad.h"

#include <filesystem>

#include <QFileDialog>
#include <QProgressDialog>
#include <QThread>

#include "format.h"
//#include "QImportConsole.h"
#include "ImportWorker.h"
#include "QReferenceEditWindow.h"

void NiirdPad::Import()
{
	QFileDialog Dialog;
	Dialog.setWindowTitle("Select /Teraurge/database/ folder...");
	Dialog.setFileMode(QFileDialog::Directory);
	Dialog.setOption(QFileDialog::ShowDirsOnly);

	if (Dialog.exec() != QDialog::Accepted)
		return;
	
	/*namespace fs = std::experimental::filesystem;

	fs::path AbsoluteImportPath = fs::absolute(Dialog.directory().absolutePath().toStdString());
	QProgressDialog Prg(fmt::format("Importing {}", AbsoluteImportPath.string()).c_str(), "Cancel", 0, 0, this);
	Prg.setWindowModality(Qt::WindowModality::ApplicationModal);
	Prg.resize(600, Prg.height());
	Prg.exec();

	while (true)
	{
		//QApplication::processEvents();

		if (Prg.wasCanceled())
			break;
	}*/

	QThread *ImportThread = new QThread(this);
	ImportWorker *Worker = new ImportWorker("sus");
	QProgressDialog *Prg = new QProgressDialog("", "Cancel", 0, 0, this);
	Prg->setWindowTitle(fmt::format("Importing '{0}'", Dialog.directory().absolutePath().toStdString()).c_str());
	Prg->setWindowModality(Qt::WindowModality::ApplicationModal);
	Prg->setModal(true);

	Worker->moveToThread(ImportThread);

	connect(ImportThread, &QThread::started, Worker, &ImportWorker::Start);
	connect(Worker, &ImportWorker::SetProgress, Prg, &QProgressDialog::setValue);
	connect(Worker, &ImportWorker::SetMessage, Prg, &QProgressDialog::setLabelText);
	connect(Prg, &QProgressDialog::cancel, Worker, &ImportWorker::Cancel);
}

NiirdPad::NiirdPad(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.actionImportProject, &QAction::triggered, [this]() {
		this->Import();

		/*QFileDialog Dialog;
		Dialog.setWindowTitle("Select /Teraurge/database/ folder...");
		Dialog.setFileMode(QFileDialog::Directory);
		Dialog.setOption(QFileDialog::ShowDirsOnly);

		if (Dialog.exec() == QDialog::Accepted)
		{
			QImportConsole Con(Dialog.directory().absolutePath().toStdString());
			Con.exec();
		}*/
	});

	connect(ui.actionEditReferenceWindowNew, &QAction::triggered, [this]() {
		std::string Res = "";
		QReferenceEditWindow::NewReference(this, Res);
	});
	connect(ui.actionEditReferenceWindowEdit, &QAction::triggered, [this]() {
		std::string Res = "";
		QReferenceEditWindow::EditReference(this, Res);
	});
	connect(ui.actionImportConsole, &QAction::triggered, [this]() {
		//QImportConsole Con(this);
		//Con.exec();
	});
	
	//ui.widget->setFocus();
	//this->setMouseTracking(true);
}
