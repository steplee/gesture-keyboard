#include <cstring>
#include <utility>
#include <algorithm>
#include <vector>

#include "search.h"
#include "trie.h"
#include "keyboard.h"

using T = float;
using namespace std;


// ---------- THREAD ------------------

Thread::Thread(const Thread* p, Node<T>* n, char ch) :
  parent(p),
  place(n),
  last_char(ch),
  just_created(true),
  alive_children(0)
{
  potential = p->potential;
  memset(children, 0, sizeof(Thread*) * 26);
}

// Utility to unwind a path to a string
static string thread_to_string(const Thread* t) {
  string s;

  while (t) {
    s += t->last_char;
    t = t->parent;
  }

  reverse(s.begin(),s.end());
  return s;
}


// ---------- SEARCHER ------------------


Searcher::Searcher(Trie<T>* trie_, Keyboard* kbd_) :
  trie(trie_),
  kbd(kbd_)
{}

void Searcher::observe_move(int time) {
  for (const Thread &thread : threads) {
    // TODO analyze die condition
    if (thread.alive_children == 0 and thread.time_created - time > 10) {
      // move to dead_threads
      // TODO
    }

    for (char nc=0; nc<26; nc++) {
      maybe_extend(thread, nc);
    }
  }
}

void Searcher::query(int time) {
  // Evaluate `energy`, lower is better
  vector<pair<float, const Thread*>> st;
  for (int i=0; i<threads.size(); i++) {
    // TODO check if delete.
    

    float score = threads[i].potential;

    st.push_back({score, &thread});
  }
  for (int i=0; i<dead_threads.size(); i++) {
    // TODO
  }

  // Sort
  sort(st.begin(), st.end(), [](auto& a, auto& b){return a.first < b.first;});

  // Debug print
  cout << " - Query at time " << time << ", top results\n";
  for (int i=0; i<min(5,(int)st.size()); i++) {
    std::string str = thread_to_string(st[i].second);
    cout << "\t"<<i<<") "<<str<<"\n";
  }
  cout << endl;

}

void Searcher::fast_remove(std::vector<Thread>& t, int i) {
  iter_swap(t.begin()+i, t.begin()+t.size()-1);
  t.pop_back();
}

bool Searcher::maybe_extend(const Thread& t, char nc) {
  pair<float,int> ts;
  float score;
  int time;
  std::tie(score, time) = kbd->score_pairwise_move(t.time_created, -1, t.last_char, nc);

  // TODO: if nc is last_char, check that we did a loop-like movement

  bool do_extend = score > EXTEND_THRESH;

  if (do_extend) {
    Node<T> *next_node = t.place->getChild(nc);

    if (next_node != nullptr) {
      std::cout << " - extending " << t.last_char << " " << nc << "\n";
      Thread nt(&t, next_node, nc);
    }
  }

  return do_extend;
}

