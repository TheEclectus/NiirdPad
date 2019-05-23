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

	connect(Worker, &ImportWorker::SetTotal, Prg, &QProgressDialog::setMaximum);
	connect(Worker, &ImportWorker::SetProgress, Prg, &QProgressDialog::setValue);
	connect(Worker, &ImportWorker::SetMessage, Prg, &QProgressDialog::setLabelText);
	connect(Prg, &QProgressDialog::cancel, Worker, &ImportWorker::Cancel);

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
		QString WarningString = fmt::format("== {0} Warnings: ==\n", Warnings.size()).c_str();
		for (auto a : Warnings)
		{
			WarningString.append(a.c_str());
			WarningString.append("\n");
		}
		//QMessageBox *Msg = new QMessageBox(this);
		Msg.setWindowTitle("Warnings");
		Msg.setText("Warnings were generated during import. Continue?");
		Msg.setIcon(QMessageBox::Icon::Warning);
		Msg.setDetailedText(WarningString);
		Msg.setStandardButtons({ QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Cancel });
		if (Msg.exec() == QMessageBox::Cancel)
			return;
	}

	// Do the actual importing here.
}

NiirdPad::NiirdPad(QWidget *parent) : 
	QMainWindow(parent)
	//_importMessageBox(new QMessageBox(this))
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
