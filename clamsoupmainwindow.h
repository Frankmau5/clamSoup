#ifndef CLAMSOUPMAINWINDOW_H
#define CLAMSOUPMAINWINDOW_H

#include "qvirusscan.h"
#include <QCloseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QStandardPaths>

QT_BEGIN_NAMESPACE
namespace Ui {
class ClamSoupMainWindow;
}
QT_END_NAMESPACE

class ClamSoupMainWindow : public QMainWindow {
  Q_OBJECT

public:
  ClamSoupMainWindow(QWidget *parent = nullptr);
  ~ClamSoupMainWindow();

  long TotalFilesToScan;
  QVirusScan *scanner;
  QString virusData[10000]; // need to set this to an vaild number
  void closeEvent(QCloseEvent *e);

private slots:
  void on_pushButton_5_clicked();
  void on_pushButton_4_clicked();
  void on_pushButton_2_clicked();

  void v_updatedone();

  void fs_scanned(QString file, QString msg);
  void fs_error(QString msg);
  void fs_done();

  void qfs_newFile(QString file);
  void qfs_virusFound(QString file, QString msg);
  void qfs_cc(long i);
  void qfs_file_count_data(QString msg);
  void qfs_total_file_count(long count);

private:
  Ui::ClamSoupMainWindow *ui;
};
#endif // CLAMSOUPMAINWINDOW_H
