#ifndef QVIRUSSCANNER_H
#define QVIRUSSCANNER_H

extern "C" {
#include "clamav.h"
}
#include "Poco/Exception.h"
#include "Poco/Path.h"
#include "Poco/RecursiveDirectoryIterator.h"
#include <QDebug>
#include <QFileInfo>
#include <QObject>
#include <QThread>
#include <QTimer>

class QVirusScanner : public QObject {
  Q_OBJECT
public:
  explicit QVirusScanner(QObject *parent = nullptr);

  void Scan_clam(std::string Path, std::string Mode);
  void FileSystemLoop(std::string Path, struct cl_engine *engine);
  void Scan(std::string file, struct cl_engine *engine);
  void Update();

public slots:
  void process();
signals:
  void scanned(QString file, QString msg);
  void error(QString msg);
  void finished();
  void done();
};

#endif // QVIRUSSCANNER_H
