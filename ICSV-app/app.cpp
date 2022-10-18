#include "app.hpp"

int
main(void) {
  ImguiExample app;
  app.initApp();
  app.getRoot()->startRendering();
  app.closeApp();
  return 0;
}
