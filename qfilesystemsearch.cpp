#include "qfilesystemsearch.h"

QFileSystemSearch::QFileSystemSearch(std::string ScanPath) {
  this->Init_FileSystemSearch(ScanPath);
}

void QFileSystemSearch::Init_FileSystemSearch(std::string init_STARTPATH) {
  this->STARTPATH = init_STARTPATH;
}
void QFileSystemSearch::LoopFileByFile(std::string startPath) {

  long count = 0;
  Poco::Path p(startPath);
  Poco::SiblingsFirstRecursiveDirectoryIterator rdi(p);
  Poco::SiblingsFirstRecursiveDirectoryIterator end;

  while (rdi != end) {
    try {

      std::string fileName = rdi->path();
      QString f = fileName.c_str();

      if (QFileInfo(f).isFile()) {
        emit newFile(f);
        QThread::msleep(200);
        count++;
        emit cc(count);
        QThread::msleep(200);

      } else {
        QThread::msleep(100);
      }

      ++rdi;

    } catch (Poco::FileException &exc) {
      QString err = exc.displayText().c_str();
      qDebug() << "Error ";
      qDebug() << err;

      emit error(err);
    }
  }
}

void QFileSystemSearch::process() {
  long r = this->CountFiles(this->STARTPATH);
  emit totalCount(r);
  qDebug() << "total";
  qDebug() << r;
  this->LoopFileByFile(this->STARTPATH);
  emit done();
};

long QFileSystemSearch::CountFiles(std::string startPath) {
  long ret_count = 0;
  Poco::Path p(startPath);
  Poco::SiblingsFirstRecursiveDirectoryIterator rdi(p);
  Poco::SiblingsFirstRecursiveDirectoryIterator end;

  while (rdi != end) {
    try {

      std::string fileName = rdi->path();
      QString f = fileName.c_str();

      if (QFileInfo(f).isFile()) {
        emit file_count_data(f);
        QString msg = "Counting: " + f;
        qDebug() << msg;
        ret_count++;
      }
      ++rdi;

    } catch (Poco::FileException &exc) {
    }
  }

  return ret_count;
}
