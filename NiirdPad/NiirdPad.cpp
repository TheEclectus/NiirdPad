#include "NiirdPad.h"

#include <filesystem>

#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QThread>

#include "format.h"
//#include "QImportConsole.h"
#include "ImportWorker.h"
#include "QReferenceEditWindow.h"
#include "QScriptEditWindow.h"

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

	std::atomic<bool> bThreadFinished = false;
	QThread *ImportThread = new QThread(this);
	ImportWorker *Worker = new ImportWorker(Dialog.directory().absolutePath().toStdString());
	QProgressDialog *Prg = new QProgressDialog("", "Cancel", 0, 0, this);
	//QMessageBox *Msg = new QMessageBox(this);
	Prg->resize(300, Prg->height());
	Prg->setWindowTitle(fmt::format("Importing '{0}'", Dialog.directory().absolutePath().toStdString()).c_str());
	Prg->setWindowModality(Qt::WindowModality::ApplicationModal);
	Prg->setModal(true);
	Prg->show();

	Worker->moveToThread(ImportThread);

	connect(ImportThread, &QThread::started, Worker, &ImportWorker::Start);
	connect(Worker, &ImportWorker::SendError, [&](const std::string &Error)
	{
		if(Error != "") 
			fmt::print("Encountered an error.\n");
	});
	//connect(Worker, &ImportWorker::SendData, [&](std::vector<std::string> Warnings, RawProjectFile Files)
	//{
	//	if (Warnings.size() > 0)
	//	{
	//		auto Msg = new QMessageBox(this); // this->_importMessageBox;

	//		QString WarningString = fmt::format("== {0} Warnings: ==\n", Warnings.size()).c_str();
	//		for (auto a : Warnings)
	//		{
	//			WarningString.append(a.c_str());
	//			WarningString.append("\n");
	//		}
	//		//QMessageBox *Msg = new QMessageBox(this);
	//		Msg->setWindowTitle("Warnings");
	//		Msg->setText("Warnings were generated during import. Continue?");
	//		Msg->setIcon(QMessageBox::Icon::Warning);
	//		Msg->setText(WarningString);
	//		Msg->setStandardButtons({ QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Cancel });
	//		Msg->exec();
	//	}
	//});
	connect(Worker, &ImportWorker::SendData, this, &NiirdPad::ImportConfirmationMessageBox);
	connect(Worker, &ImportWorker::Finished, [&]()
	{
		//Worker->deleteLater();
		//ImportThread->quit();
	});
	connect(Worker, &ImportWorker::Finished, ImportThread, &QThread::quit);
	connect(Worker, &ImportWorker::Finished, Worker, &ImportWorker::deleteLater);
	connect(Worker, &ImportWorker::Finished, Prg, &QProgressBar::deleteLater);

	connect(Worker, &ImportWorker::SetTotal, Prg, &QProgressDialog::setMaximum);
	connect(Worker, &ImportWorker::SetProgress, Prg, &QProgressDialog::setValue);
	connect(Worker, &ImportWorker::SetMessage, Prg, &QProgressDialog::setLabelText);
	
	// TODO: Cancel button currently doesn't do anything.

	/*connect(Prg, &QProgressDialog::cancel, ImportThread, &QThread::quit);
	connect(Prg, &QProgressDialog::cancel, Worker, &ImportWorker::deleteLater);
	connect(Prg, &QProgressDialog::cancel, Prg, &QProgressBar::deleteLater);*/

	ImportThread->start();

	/*while (!bThreadFinished)
	{

	}

	if (Worker->Error().length() > 0)
	{
		QMessageBox *Msg = new QMessageBox(QMessageBox::Icon::Warning, "Error", "TUScript parser has encountered an error.", QMessageBox::StandardButton::Ok, this);
		Msg->setDetailedText(Worker->Error().c_str());
		Msg->exec();
		return;
	}

	if (Worker->Warnings().size() > 0)
	{
		QMessageBox *Msg = new QMessageBox(QMessageBox::Icon::Warning, "Warning", fmt::format("TUScript parser has encountered {0} warnings. Proceed?", Worker->Warnings().size()).c_str(), { QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::No }, this);
		QString WarningsStr;
		for (const std::string &Warning : Worker->Warnings())
		{
			WarningsStr.append(fmt::format("{0}\n", Warning.c_str()).c_str());
		}
		Msg->setDetailedText(Worker->Error().c_str());
		Msg->exec();
		return;
	}*/
}

void NiirdPad::ImportConfirmationMessageBox(std::vector<std::string> Warnings, RawProjectFile Files)
{
	if (Warnings.size() > 0)
	{
		QMessageBox Msg(this);
		QString WarningString = ""; //fmt::format("== {0} Warnings: ==\n", Warnings.size()).c_str();
		for (auto a : Warnings)
		{
			WarningString.append(a.c_str());
			WarningString.append("\n\n");
		}
		//QMessageBox *Msg = new QMessageBox(this);
		Msg.setWindowTitle(fmt::format("{0} Warnings", Warnings.size()).c_str());
		Msg.setText("Warnings were generated during import. Continue?");
		Msg.setIcon(QMessageBox::Icon::Warning);
		Msg.setDetailedText(WarningString);
		Msg.setStandardButtons({ QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Cancel });
		if (Msg.exec() == QMessageBox::Cancel)
			return;

		// TODO: Do the RawProjectFile -> ProjectFile conversion here
	}

	// Do the actual importing here.
}

NiirdPad::NiirdPad(QWidget *parent) :
	QMainWindow(parent),
	_scriptEngine(),
	_scriptEditWindow(new QScriptEditWindow(this, _scriptEngine, false))
	//_importMessageBox(new QMessageBox(this))
{
	ui.setupUi(this);

	// HACK: hack attack, whack your shmack and clack your knacks :^)
	ui.widget->SetEngine(&_scriptEngine);
	ui.widget->SetNiirdPad(this);

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

	// Neither option actually has a NodeDialogue or NodeOption to edit by design. No fucking around with needing the SDL state.
	connect(ui.actionEditScriptWindowDialogue, &QAction::triggered, [this]() {
		QScriptEditWindow Win(this->window(), this->_scriptEngine, true);
		Win.exec();
	});
	connect(ui.actionEditScriptWindowOption, &QAction::triggered, [this]() {
		QScriptEditWindow Win(this, this->_scriptEngine, false);
		Win.exec();
	});
	
	//ui.widget->setFocus();
	//this->setMouseTracking(true);
}

QScriptEditWindow *NiirdPad::ScriptEditWindow()
{
	return _scriptEditWindow;
}