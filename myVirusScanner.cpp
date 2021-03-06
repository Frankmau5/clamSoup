#include "myVirusScanner.h"

myVirusScanner::myVirusScanner(QObject *parent) : QObject(parent) {}

void myVirusScanner::Scan_clam(std::string Path, std::string Mode) {
  // TODO: Add error handleing
  int r = cl_init(CL_INIT_DEFAULT);
  if (r == CL_SUCCESS) {
    unsigned int sigs = 0;
    int ret;
    int ret2;

    struct cl_engine *ve = cl_engine_new();
    ret = cl_load(cl_retdbdir(), ve, &sigs, CL_DB_STDOPT);
    ret2 = cl_engine_compile(ve);

    if (ret2 == CL_SUCCESS) {
      if (Mode == "SCAN") {
        FileSystemLoop(Path, ve);
      } else {
        Update();
      }
    }

    cl_engine_free(ve);
  } else {
    qDebug() << "Error with clam init";
  }
}

void myVirusScanner::FileSystemLoop(std::string Path,
                                    struct cl_engine *engine) {
  // loop thought files from Path
  Poco::Path p(Path);
  Poco::SiblingsFirstRecursiveDirectoryIterator rdi(p);
  Poco::SiblingsFirstRecursiveDirectoryIterator end;

  while (rdi != end) {
    try {
      std::string fileName = rdi->path();
      QString f = fileName.c_str();

      if (QFileInfo(f).isFile()) {
        Scan(fileName, engine);
      }

      ++rdi;
    } catch (Poco::FileException &exc) {
      QString err = exc.displayText().c_str();
      qDebug() << err;
      ++rdi;
    }
  }
}

void myVirusScanner::Scan(std::string file, struct cl_engine *engine) {
  const char *f = file.c_str();
  const char *virname_;
  struct cl_scan_options options;
  options.general |= CL_SCAN_GENERAL_ALLMATCHES;
  int ret = cl_scanfile(f, &virname_, NULL, engine, &options);
  if (ret == CL_VIRUS) {
    //"virus found\n";
  }

  if (ret == CL_CLEAN) {
    QString s = f;
    qDebug() << "File:";
    qDebug() << f;
    // emit scanned();
  }
}

void myVirusScanner::Update() {}
