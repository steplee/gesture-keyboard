#pragma once
#include <set>
#include <algorithm>
#include <map>

#include "trie.h"

// Tree search


/*
 * Enapsulates a position the trie and its score
 */
struct Thread {
  using T = float;

  Thread* parent;
  Node<T>* place;
  float score;
};

struct CandidateThread {
  using T = float;

  Thread* thread;
  float score;
  float time;
};


/*
 * Quite a bit of freedom here, I'll have to try several methods.
 * Maintains a bag of threads (ordering is up to implementation) and
 * facilitates scoring
 */
class Searcher {
  public:
    using T = float;

  private:
    std::set<Thread> threads;
    Trie<T>* trie;

    void extend(const Thread& t, const char next_char);
    bool try_score(const Thread& t, char nc, float y,float x, float time);

    std::map<std::string, CandidateThread> cur_cands;

  public:

    Searcher(Trie<T>* trie);

    void observe_move(float y, float x, float t);

};
