#include "clamsoupmainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  // splashe

  QMessageBox msgBox;
  msgBox.setText("Clamsoup is loading");
  msgBox.setInformativeText("Please wait this should not take long");
  msgBox.setStandardButtons(QMessageBox::Abort | QMessageBox::Ok);
  int ret = msgBox.exec();

  if (ret == QMessageBox::Abort) {
    return 1024;
  }

  ClamSoupMainWindow w;
  w.setWindowTitle("Clam Soup");
  w.show();
  return a.exec();
}
