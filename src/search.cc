#include "search.h"

using T = float;
using namespace std;


// ---------- THREAD ------------------

static string thread_to_string(Thread* t) {
  string s;

  while (t) {
    s += t->place->getKey();
    t = t->parent;
  }
}



// ---------- SEARCHER ------------------

Searcher::Searcher(Trie<T>* trie_):
  trie(trie_)
{}

void Searcher::observe_move(float y, float x, float time) {
  for (const Thread &thread : threads) {
    for (char nc=0; nc<26; nc++) {
      if ( try_score(thread, nc, y,x,time) ) {
        extend(thread, nc);
      }
    }
  }
}

bool Searcher::try_score(const Thread& t, char nc, float y, float x, float time) {
  return true;
}

void Searcher::extend(const Thread &t, const char nc) {
  Node<T> *next_node = t.place->getChild(nc);

  if (next_node) {
    Thread nt;
    nt.score = t.score+.1;
  } else {
    // report that this thread is a possible final thread
    // TODO keep first reported and newest cands
    //cur_cands[
  }
}
