#ifndef QFILESYSTEMSEARCH_H
#define QFILESYSTEMSEARCH_H

#include "Poco/Exception.h"
#include "Poco/Path.h"
#include "Poco/RecursiveDirectoryIterator.h"
#include <QDebug>
#include <QFileInfo>
#include <QObject>
#include <QProcess>
#include <QThread>
#include <QTimer>

class QFileSystemSearch : public QObject {
  Q_OBJECT
public:
  QFileSystemSearch(std::string ScanPath);
  std::string FILENAME;
  std::string STARTPATH;

  void Init_FileSystemSearch(std::string init_STARTPATH);
  void LoopFileByFile(std::string startPath);
  long CountFiles(std::string startPath);

public slots:
  void process();

signals:
  void cc(long i); // cc = count changed
  void error(QString msg);
  void finished();
  void newFile(QString file);
  void done();

  void file_count_data(QString);
  void totalCount(long);
};

#endif // QFILESYSTEMSEARCH_H
