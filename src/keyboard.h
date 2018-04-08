#pragma once
#include <utility>
#include "trie.h"
#include "search/search.h"

class Searcher;


class Keyboard {
  public:

  private:
    Searcher* searcher;

    std::vector<timepoint> path;


    point key_pos[26];

    float distance(const point& a, const point& b) const;

  public:
    void recv_reset();
    void recv_move(float y, float x, float time);

    float key_dist(char k1, char k2) const;

    // Return the compatibility and time-index of a path connecting `from` and `to`,
    // in between the times given.
    // NOTE: what this does right now is compare the distance from `from` at `min_time`, to `to`
    // at __any possible time__, returning a function of the min distance.
    // The distance is pushed through `1-log((10x^4))`, to convert it to a score (higher is better)
    // :max_time may be -1, in which case it is taken to be T
    // TODO save results, dynamic programming style
    // TODO compare path length with the geodesic connecting them
    std::pair<float, int> score_pairwise_move(int min_time, int max_time, char from, char to) const ;
    float score_start(char ch) const;
    float dist_to_score(float dist) const ;


    // how far is x,y to character c?
    float dist_to_key(point& p, char c);

    timepoint getPath(int time);

    Keyboard();
};
