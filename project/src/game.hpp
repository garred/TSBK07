#ifndef _game_hpp_
#define _game_hpp_


class Game {
public:
  static bool is_running;
  static float time;
  static float delta;

  static void init_all();
  static void update_all(int value);
  static void draw_all();

};


#endif
