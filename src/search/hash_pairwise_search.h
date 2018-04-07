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

class HashPairwiseSearcher : public Searcher {
  public:
    HashPairwiseSearcher(Keyboard* kbd);

    virtual void observe_move(int time);
    virtual void query(int time);
    virtual void reset();

  private:

    std::unordered_set<std::string> dict;

    std::unordered_map<std::string, float> alive;
    void populate_words(std::string filename);

};
