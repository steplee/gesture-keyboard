#include "trie.h"
#include <cassert>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <iostream>

using namespace std;

template<class V>
Node<V>::Node(V* v) :
  terminal(false),
  val(v)
{
  //children = new Node<V>[26]();
  memset(children, 0, sizeof(Node<V>*) * 26);
}

template<class V>
Node<V>* Node<V>::getChild(char nc) {
  if (children[nc] != 0)
    return children[nc];
  return nullptr;
}

template<class V>
char Node<V>::getKey() {
  return key;
}


template<class V>
void Node<V>::insert(string &w, V* v) {
  if (w.size() == 1) {
    // base case
    terminal = true;
    val = v;
    key = w[0];
  } else {
    // recurse
    char cc = w[0];
    char nc = cc - 'a';

    if (children[nc] == nullptr) {
      children[nc] = new Node(nullptr);
    }

    string nw = w.substr(1);
    children[nc]->insert(nw, v);
  }

}

template<class V>
V* Node<V>::find(string &w) {
  if (w.size() == 1) {
    if (terminal) return val;
    else return nullptr;
  } else {
    char cc = w[0];
    char nc = cc - 'a';

    if (children[nc]) {
      string nw = w.substr(1);
      return children[nc]->find(nw);
    } else
      return nullptr;
  }
}


// ----------------- TRIE -----------------------


template<class V>
Trie<V>::Trie() {}

template<class V>
V* Trie<V>::find(string word) {
  return root->find(word);
}

template<class V>
Node<V>* Trie<V>::get_first_level(char nc) const {
  assert( root );
  assert(root->getChild(nc));
  return root->getChild(nc);
}


template<class V>
void Trie<V>::create_with_default(vector<string> words, V* def) {
  //for (int i=0; i<26; i++) {
    //root[i] = new Node<V>(def);
  //}
  root = new Node<V>(def);

  for(auto word : words) {
    root->insert(word, def);
  }
}

template<class V>
Trie<V> *Trie<V>::create_from_file(string file_name, V* def) {
  ifstream file(file_name);
  cout << " --- Creating trie from file " << file_name << endl;

  string word;
  vector<string> words;

  while (file.good()) {
    getline(file, word);
    // TODO len > 0
    // TODO using namespace std; overloads space, must wrap isalpha. Is it slower?
    if (word.length() > 1 and all_of(word.begin(),word.end(), [](char c){return isalpha(c);}))
      words.push_back(word);
  }

  Trie<V> *trie = new Trie<V>();
  trie->create_with_default(words, def);

  cout << " --- Done, with " << words.size() << " words" << endl;
  return trie;
}


/*
template<class V>
void Trie<V>::create_with_lists(vector<string> words, vector<V*> vals) {
  assert(words.size() == vals.size());

  for (int i=0; i<26; i++) {
    root[i] = new Node<V>(nullptr);
  }

  for(int i=0; i<words.size(); i++) {
    string word = words[i];
    char cc = word[0];
    char nc = cc - 'a';
    string nw = word.substr(1);
    root[nc]->insert(nw, vals[i]);
  }
}
*/
