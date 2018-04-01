#pragma once
#include "trie.h"


class Keyboard {
  private:
    Trie<float> trie;


  public:
    void recv_reset();
    void recv_move(float y, float x, float time);

    Keyboard();
};
