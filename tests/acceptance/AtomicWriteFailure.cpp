#include "application/document/DocumentCoordinator.h"
#include <iostream>
int main(int argc, char** argv) {
  if (argc != 2)
    return 2;
  xfmd::DocumentSession session;
  xfmd::LocalFileStore files;
  xfmd::DocumentCoordinator documents(session, files);
  if (!documents.requestOpen(argv[1]))
    return 3;
  session.applyEdit({0, 0, "edited"});
  bool failed = !documents.save();
  return failed && session.dirty() && files.read(argv[1]).text == "original" ? 0 : 1;
}
