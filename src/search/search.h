#pragma once
#include <set>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <memory>

// forward decls
class Keyboard;


/*
 * Quite a bit of freedom here, I'll have to try several methods.
 * I will start with pairwise potential method, before worrying about 
 * seperating interface/impl
 *
 * Maintains a bag of threads (ordering is up to implementation) and
 * facilitates scoring
 */

class Searcher {
  protected:
    Keyboard* kbd;

  public:
    Searcher(Keyboard* kbd_) : kbd(kbd_) {} ;

    // Main action function, called by Keyboard
    // It will move any dead threads to dead_threads (die)
    virtual void observe_move(int time) = 0;

    // Evaluate energy, sort results.
    // Any old threads will be completely removed (delete)
    virtual void query(int time) = 0;

    virtual void reset() = 0;
};

