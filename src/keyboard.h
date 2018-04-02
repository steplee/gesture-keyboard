#pragma once
#include <utility>
#include "trie.h"
#include "search.h"



class Keyboard {
  private:
    typedef std::pair<float, float> point;
    typedef std::tuple<float, float> timepoint;

    Trie<float> trie;
    Searcher* searcher;


    point key_pos[26];

  public:
    void recv_reset();
    void recv_move(float y, float x, float time);

    Keyboard();
};
