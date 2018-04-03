#pragma once
#include <utility>
#include "trie.h"
#include "search.h"



class Keyboard {
  private:
    // y x
    typedef std::pair<float, float> point;
    // t y x
    typedef std::pair<float, point> timepoint;

    Trie<float> trie;
    Searcher* searcher;

    std::vector<timepoint> path;


    point key_pos[26];

    float distance(const point& a, const point& b) const;

  public:
    void recv_reset();
    void recv_move(float y, float x, float time);

    // Return the compatibility of a path connecting `from` and `to` and
    // the path-index, in between the times given.
    // NOTE: what this does right now is compare the distance from `from` at `min_time`, to `to`
    // at __any possible time__, returning a function of the min distance.
    // The distance is pushed through `1-log((10x^4))`, to convert it to a score (higher is better)
    // :max_time may be -1, in which case it is taken to be T
    std::pair<float, int> score_pairwise_move(int min_time, int max_time, char from, char to) const ;
    float dist_to_score(float dist) const ;

    Keyboard();
};
