#include <cstring>
#include <utility>
#include <algorithm>
#include <vector>

#include "search/trie_search.h"
#include "trie.h"
#include "keyboard.h"

using T = float;
using namespace std;


// ---------- THREAD ------------------

Thread::Thread(Thread::ptr p, Node<T>* n, char ch, int time) :
  parent(p),
  place(n),
  last_char(ch),
  time_created(time),
  just_created(true),
  alive_children(0)
{
  if (p != nullptr)
    potential = p->potential,
    acc = p->acc + last_char;
  else
    potential = 1.0,
    acc = "_"+last_char;
  memset(children, 0, sizeof(Thread::ptr) * 26);
}

// Utility to unwind a path to a string TODO pass by ref
static string thread_to_string(Thread::ptr t) {
  string s;

  while (t) {
    s += t->last_char;
    //t = t->parent;
  }

  reverse(s.begin(),s.end());
  return s;
}

Thread::~Thread() {
}


// ---------- SEARCHER ------------------


TrieSearcher::TrieSearcher(Keyboard* kbd_) :
  Searcher(kbd_)
{
  // ---------------------------------
  // create Trie
  float f = 1.0;
  //trie = Trie<float>::create_from_file("/usr/share/dict/cracklib-small", &f);
  trie = Trie<float>::create_from_file("./words", &f);

  // test
  cout << " -- Trie test...\n";
  cout << "\ttrie find hello -> " << trie->find("hello") << endl;
  cout << "\ttrie find test -> " << trie->find("test") << endl;
  cout << "\ttrie find apple -> " << trie->find("apple") << endl;
  cout << "\ttrie find zsdjslfje -> " << trie->find("lshflkshdf") << endl;
  cout << endl;
  // ---------------------------------
}

template <class C>
inline void fast_remove(C& t, int i) {
  iter_swap(t.begin()+i, t.begin()+t.size()-1);
  t.pop_back();
}

void TrieSearcher::observe_move(int time) {
  // first movement
  if (threads.size() == 0) {
    cout << " Begin. " << endl;
    // create a thread for surrounding keys.
    vector<pair<float, char>> cands;
    for (char nc=0; nc<26; nc++) {
      float score = kbd->score_start(nc);
      cands.push_back({score,nc});
    }
    sort(cands.begin(),cands.end(), [](const auto& a, const auto& b){return a.first>b.first;}); // >, reversed

    // consider nearest __4__ keys
    for (int i=0; i<4; i++) {
      //cout << " next includes " << (int)cands[i].second << endl;
      Node<T> *next_node = trie->get_first_level(cands[i].second);

      if (next_node)
        threads.push_back( make_shared<Thread>(nullptr, next_node, cands[i].second, time) );
    }

  }

  // not first movment
  else {
    int sz = threads.size();
    for (int i=0; i<sz; i++) {
      // TODO analyze die condition
      if (threads[i]->alive_children == 0 and threads[i]->time_created - time > 20) {
        dead_threads.push_back(threads[i]);
        fast_remove(threads, i);
        i--; // repeat index
      } else {

        // TODO don't check all keys, condition on quadrant
        for (char nc=0; nc<26; nc++) {
          if (threads[i]->children[nc] == nullptr)
            maybe_extend(threads[i], nc);
        }
      }
    }
  }
}

void TrieSearcher::query(int time) {
  // Evaluate score, higher is better

  // Build result array
  vector<pair<float, Thread::ptr>> st;

  for (int i=0; i<threads.size(); i++) {
    float score = threads[i]->potential;
    score *= min(1.0, ((float)threads[i]->time_created+.1)/((float)time));

    st.push_back({score, threads[i]});
  }
  for (int i=0; i<dead_threads.size(); i++) {
    // TODO better delete condition
    if (threads[i]->time_created <= time/2 and threads[i]->alive_children == 0) {
      // This __will__ delete the shared_ptr since alive_children is zero
      fast_remove(dead_threads, i); // TODO does it?
    } else {
      float score = threads[i]->potential;
      score *= min(1.0, ((float)threads[i]->time_created+.1)/((float)time));

      st.push_back({score, threads[i]});
    }
  }

  // Sort
  sort(st.begin(), st.end(), [](auto& a, auto& b){return a.first>b.first;}); // >, reverse

  // Debug print
  cout << " - Query at time " << time << ", top results"<<endl;
  for (int i=0; i<min(5,(int)st.size()); i++) {
    std::string str = thread_to_string(st[i].second);
    cout << "\t"<<i<<") "<<str<<"\n";
  }
  cout << endl;

}


bool TrieSearcher::maybe_extend(const Thread::ptr t, char nc) {
  pair<float,int> ts;
  float score;
  int time;
  tie(score, time) = kbd->score_pairwise_move(t->time_created, -1, t->last_char, nc);

  // TODO: if nc is last_char, check that we did a loop-like movement

  bool do_extend = score > .3;

  if (do_extend) {
    Node<T> *next_node = t->place->getChild(nc);

    if (next_node != nullptr) {
      //printf(" - extend %c %c (%f) [%d]\n", (char)(t->last_char+'a'),(char)(nc+'a'), score, threads.size());
      printf(" - extend %s %c (%f) [%d]\n", t->acc.c_str(),(char)(nc+'a'), score, threads.size());
      //cout << " - extend " << t->acc << (char)(nc+'a') << " " << score << endl;
      auto nt = make_shared<Thread>(t, next_node, nc, time);
      threads.push_back( nt );
      //cout << " extended " << nt->acc << endl;
    }
  }

  return do_extend;
}


/*
 * This /should/ work  :|
 *
 * If I remove all references, shared_ptrs should be cleaned up.
 * TODO test with a global counter and call ->reset()
 */
void TrieSearcher::reset() {
  for (auto& t : threads) {
    t->parent.reset();
    for (auto &tt : t->children)
      if (tt)
        tt.reset();
  }
  for (auto& t : dead_threads) {
    t->parent.reset();
    for (auto &tt : t->children)
      if (tt)
        tt.reset();
  }

  threads.clear();
  dead_threads.clear();
}
