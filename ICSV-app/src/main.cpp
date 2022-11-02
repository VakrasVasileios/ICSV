#include "app.hpp"

int
main(void) {
  ICSVapp::ICSVapp app;
  app.initApp();
  app.getRoot()->startRendering();
  app.closeApp();
  return 0;
}
