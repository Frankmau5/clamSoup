#include "clamsoupmainwindow.h"
#include "qfilesystemsearch.h"
#include "ui_clamsoupmainwindow.h"

ClamSoupMainWindow::ClamSoupMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ClamSoupMainWindow) {
  ui->setupUi(this);
  scanner = new QVirusScan();

  QString welcome = "ClamSoup a clamav based anti-virus "
                    "scanner.\nhttps://www.clamav.net/about#overview for more "
                    "info about clamav";

  ui->textBrowser->setText(welcome);
}

ClamSoupMainWindow::~ClamSoupMainWindow() { delete ui; }

void ClamSoupMainWindow::closeEvent(QCloseEvent *event) {

  QMessageBox::StandardButton resBtn = QMessageBox::question(
      this, "ClamSoup", tr("Are you sure?\n"),
      QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

  if (resBtn != QMessageBox::Yes) {
    event->ignore();
  } else {
    this->scanner->Cleanup();
    this->scanner = nullptr;
    event->accept();
  }
}

void ClamSoupMainWindow::on_pushButton_5_clicked() {
  // About box
  QString title = "About ClamSoup";
  QString msg = "Clamsoup - A QT5 clamav anti-virus\nBy knrf\nVersion 1.0.0\n";
  QMessageBox::about(this, title, msg);
}

void ClamSoupMainWindow::on_pushButton_4_clicked() {
  // custom scan button
  ui->textBrowser->setText("Clamav has been loaded.\n");

  ui->pushButton_2->setDisabled(true);
  ui->pushButton_4->setDisabled(true);
  ui->pushButton_5->setDisabled(true);

  QString path;
  QFileDialog dialog(this);
  dialog.setFileMode(QFileDialog::DirectoryOnly);
  int a = dialog.exec();
  if (a == 1) {
    ui->textBrowser->setText("Clam will start scanning soon\n");
    QUrl durl = dialog.directoryUrl();
    path = durl.path();
    QString s = "Scanning " + path + "\n";

    QThread *thread = new QThread;
    QFileSystemSearch *worker = new QFileSystemSearch(path.toStdString());
    worker->moveToThread(thread);

    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(cc(long)), this, SLOT(qfs_cc(long)));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(newFile(QString)), this, SLOT(qfs_newFile(QString)));
    connect(worker, SIGNAL(error(QString)), this, SLOT(fs_error(QString)));
    connect(worker, SIGNAL(done()), this, SLOT(fs_done()));

    connect(worker, SIGNAL(totalCount(long)), this,
            SLOT(qfs_total_file_count(long)));

    connect(worker, SIGNAL(file_count_data(QString)), this,
            SLOT(qfs_file_count_data(QString)));

    connect(scanner, SIGNAL(scanned(QString, QString)), this,
            SLOT(fs_scanned(QString, QString)));
    connect(scanner, SIGNAL(virusfound(QString, QString)), this,
            SLOT(qfs_virusFound(QString, QString)));
    connect(scanner, SIGNAL(error(QString)), this, SLOT(fs_error(QString)));

    // start thread
    thread->start();
    ui->progressBar->setEnabled(true);
  } else {
    ui->pushButton_2->setDisabled(false);
    ui->pushButton_4->setDisabled(false);
    ui->pushButton_5->setDisabled(false);
  }
}

void ClamSoupMainWindow::on_pushButton_2_clicked() {
  // update button
  ui->textBrowser->setText("Update started. This might take a minute or two");
  ui->pushButton_2->setDisabled(true);
  ui->pushButton_4->setDisabled(true);
  ui->pushButton_5->setDisabled(true);
  connect(this->scanner, SIGNAL(updatedone()), this, SLOT(v_updatedone()));
  this->scanner->Update(); // add to a thread
}

// slots
void ClamSoupMainWindow::fs_scanned(QString file, QString msg) {
  ui->textBrowser->setText("Scanned file " + file + " :  " + msg + "\n");
  ui->textBrowser->update();
}

void ClamSoupMainWindow::fs_done() {
  this->scanner->Cleanup();
  this->scanner = nullptr;

  if (scanner == nullptr) {
    this->scanner = new QVirusScan();
  }

  ui->textBrowser->setText("Scan Done\n");

  for (int var = 0; var < this->virusData->length(); ++var) {
    QString d = ui->textBrowser->toPlainText();
    ui->textBrowser->setText(d + virusData[var]);
  }

  ui->progressBar->setValue(0);
  ui->progressBar->setEnabled(false);
  ui->pushButton_2->setDisabled(false);
  ui->pushButton_4->setDisabled(false);
  ui->pushButton_5->setDisabled(false);
}

void ClamSoupMainWindow::fs_error(QString msg) {
  // put errors in a message box
  QString s = ui->textBrowser->toPlainText();
  ui->textBrowser->setText("Error " + msg + "\n" + s);
}

void ClamSoupMainWindow::qfs_newFile(QString file) {
  scanner->ScanFile(file.toStdString());
}

void ClamSoupMainWindow::qfs_virusFound(QString file, QString msg) {
  QString info = file + " : " + msg + "\n";
  this->virusData->append(info);
}

void ClamSoupMainWindow::qfs_cc(long i) {
  float percentage = (float)i / TotalFilesToScan * 100.0;
  qDebug() << percentage;
  ui->progressBar->setValue(percentage);
}

void ClamSoupMainWindow::qfs_file_count_data(QString msg) {
  ui->textBrowser->setText("Started the counting of files.\n" + msg);
}

void ClamSoupMainWindow::qfs_total_file_count(long count) {
  TotalFilesToScan = count;
}

void ClamSoupMainWindow::v_updatedone() {
  ui->pushButton_2->setDisabled(false);
  ui->pushButton_4->setDisabled(false);
  ui->pushButton_5->setDisabled(false);
  ui->textBrowser->setText("Update Done.");
}
// helper function
// NOTE: need to update this function as well as custonscan
