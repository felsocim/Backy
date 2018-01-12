#include "filer.h"

Filer::Filer() {}

Filer::Filer(QString root) {
  this->root = root;
  this->directoriesToProcess = 0;
  this->filesToProcess = 0;

  QDirIterator i(this->root, QDirIterator::Subdirectories);

  while(i.hasNext()) {
    QFileInfo current(i.next());

    if(current.isDir()) {
      this->directoriesToProcess++;
    } else {
      this->filesToProcess++;
    }
  }
}
