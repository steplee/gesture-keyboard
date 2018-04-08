#pragma once
#include <set>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string>

#include "search/search.h"


// forward decls
class Keyboard;


struct Entry {
  std::string acc;

  float time_created;

  float potential;
};

class HashPairwiseSearcher : public Searcher {
  public:
    HashPairwiseSearcher(Keyboard* kbd);

    virtual void observe_move(int time);
    virtual void query(int time);
    virtual void reset();

  private:

    // An entry is added to fringe when it is a possible extension.
    // Note: it is not yet activated, so it is not currently included in query
    std::vector<Entry> fringe [26]; // possible follow-ups, check every movement

    std::vector<Entry> cands;

    // TODO make dict store <string,continues,terminal> tuples.
    std::unordered_set<std::string> dict;

    //std::unordered_map<std::string, float> alive;
    void populate_words(std::string filename);

};
