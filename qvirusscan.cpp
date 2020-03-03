#include "qvirusscan.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

QVirusScan::QVirusScan() { this->InitQVirusScanner(); }

void QVirusScan::InitQVirusScanner() {
  int r = cl_init(CL_INIT_DEFAULT);

  if (r == CL_SUCCESS) {
    unsigned int sigs = 0;
    int ret;
    int ret2;

    this->engine = cl_engine_new();
    ret = cl_load(cl_retdbdir(), this->engine, &sigs, CL_DB_STDOPT);
    if (ret == CL_SUCCESS) {
      ret2 = cl_engine_compile(this->engine);

      if (ret2 == CL_SUCCESS) {
        qDebug() << "Clam av is loaded";
      }
    }
  }
}

void QVirusScan::Update() {

  QNetworkAccessManager *manager = new QNetworkAccessManager();

  connect(manager, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(replyFinished(QNetworkReply *)));

  manager->get(QNetworkRequest(QUrl("http://database.clamav.net/main.cvd")));
  // look at replyFinished
}

void QVirusScan::Cleanup() {
  cl_engine_free(this->engine);
  qDebug() << "clamav has been cleaned up";
}

void QVirusScan::ScanFile(std::string FullFilePath) {
  const char *f = FullFilePath.c_str();
  const char *virname_;

  struct cl_scan_options options;
  options.general |= CL_SCAN_GENERAL_ALLMATCHES;

  int ret = cl_scanfile(f, &virname_, NULL, this->engine, &options);
  qDebug() << f;
  qDebug() << ret;

  if (ret == CL_VIRUS) {
    emit virusfound(f, QString(virname_));
  }

  if (ret == CL_CLEAN) {
    emit scanned(f, QString("Clean"));
  }

  if (ret != CL_CLEAN && ret != CL_VIRUS) {
    emit error(QString(ret));
  }
  emit finishedScan();
}

void QVirusScan::replyFinished(QNetworkReply *reply) {
  if (reply->error()) {
    emit error(reply->errorString());
  } else {
    int httpcode =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    QString path = QDir::homePath();
    path = path + "/Desktop/main.cvd";

    if (httpcode == 200) {

      QFile *file = new QFile(path);
      if (file->open(QFile::Append)) {
        file->write(reply->readAll());
        file->flush();
        file->close();
      }

      QString program = "/usr/bin/gksu";
      QStringList arguments;
      arguments << "-w"
                << "-u"
                << "root"
                << "gksu"
                << "/bin/mv" << path << "/var/lib/clamav/";

      QProcess *myProcess = new QProcess(this);
      myProcess->start(program, arguments);
      myProcess->waitForFinished();
      emit updatedone();

      // note might have to change permison of file to clamav user

      // ALSO need a signal to say that the update is done
    } else {
      QString s = QString(httpcode);
      emit error(s);
    }
  }
}
