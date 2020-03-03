#ifndef QVIRUSSCAN_H
#define QVIRUSSCAN_H
extern "C" {
#include "clamav.h"
}

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QObject>
#include <QProcess>
#include <QThread>
#include <QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

class QVirusScan : public QObject {
  Q_OBJECT
public:
  explicit QVirusScan();
  struct cl_engine *engine;

  void InitQVirusScanner();
  void Update();
  void ScanFile(std::string FullFilePath);
  void Cleanup();

private slots:
  void replyFinished(QNetworkReply *reply);

signals:
  void scanned(QString file, QString msg);
  void virusfound(QString file, QString msg);
  void error(QString msg);
  void finishedScan();
  void updatedone();
};

#endif // QVIRUSSCAN_H
