#pragma once
#include <set>
#include <algorithm>
#include <map>
#include <memory>

// forward decls
class Keyboard;
template <class T> class Node;
template <class T> class Trie;


// A key param.
// Defines how good a thread must be to be extended.
constexpr float EXTEND_THRESH = .3;

// Another key param.
// Defines how old a thread can be before it can extend anymore.
//constexpr float EXTEND_LIFE = 1.0; // 1.0 => one swipe across screen

// Tree search


/*
 * Enapsulates a position the trie and its score
 * Threads will have children, but pointers only go up.
 * Each chain is an auxiliary structure on-top of the trie (i.e. their may be loops, etc.)
 */
struct Thread {
  using T = float;
  using ptr = std::shared_ptr<Thread>;

  Thread(Thread::ptr p, Node<T>* n, char ch, int time);
  ~Thread();

  Thread::ptr parent;
  Node<T>* place;

  Thread::ptr children[26];
  float potential;

  // Number of children that are alive.
  // When it reaches 0 and just_created is false, we kill self and decrement parent.
  int alive_children;
  int time_created;
  bool just_created; // TODO implement

  // TODO just for debugging.
  std::string acc;

  char last_char;
};

/*
 * Quite a bit of freedom here, I'll have to try several methods.
 * I will start with pairwise potential method, before worrying about 
 * seperating interface/impl
 *
 * Maintains a bag of threads (ordering is up to implementation) and
 * facilitates scoring
 */

class Searcher {
  public:
    using T = float;

  private:
    //std::set<Thread> threads;
    const Trie<T>* trie;
    const Keyboard* kbd;

    //void extend(const Thread& t, const char next_char);
    bool maybe_extend(const Thread::ptr t, char nc);

    // My plan for the algo for these is to loop over each, each action
    // When they are old, move living threads to `dead_threads`.
    // Note these are completely unordered
    //
    // Each query, we check both living and dead_threads. Remove if age expired.
    std::vector<Thread::ptr> threads;
    std::vector<Thread::ptr> dead_threads;


  public:
    Searcher(Trie<T>* trie, Keyboard* kbd);

    // Main action function, called by Keyboard
    // It will move any dead threads to dead_threads (die)
    void observe_move(int time);

    // Evaluate energy, sort results.
    // Any old threads will be completely removed (delete)
    void query(int time);

    void reset();
};
