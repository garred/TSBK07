#ifndef _interface_hpp_
#define _interface_hpp_

#include "entities.hpp"

#define CAMERA_SLOW 100.0
#define CAMERA_FAST 100.0


class Interface {
public:
  static Node* player;
  static void init_all();
  static void draw_all();
  static void keyboard_update();
  static void mouse_update(int x, int y);
};


#endif
