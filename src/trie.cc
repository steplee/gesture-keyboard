#include "trie.h"
#include <cassert>
#include <cstring>
#include <algorithm>
#include <cctype>

template<class V>
Node<V>::Node(V* v) :
  terminal(false),
  val(v)
{
  //children = new Node<V>[26]();
  memset(children, 0, sizeof(Node<V>*) * 26);
}

template<class V>
void Node<V>::insert(std::string &w, V* v) {
  if (w.size() == 1) {
    // base case
    terminal = true;
    val = v;

  } else {
    // recurse
    char cc = w[0];
    char nc = cc - 'a';

    if (children[nc] == nullptr) {
      children[nc] = new Node(nullptr);
    }

    std::string nw = w.substr(1);
    children[nc]->insert(nw, v);
  }

}

template<class V>
V* Node<V>::find(std::string &w) {
  if (w.size() == 1) {
    if (terminal) return val;
    else return nullptr;
  } else {
    char cc = w[0];
    char nc = cc - 'a';

    if (children[nc]) {
      std::string nw = w.substr(1);
      return children[nc]->find(nw);
    } else
      return nullptr;
  }
}


// ----------------- TRIE -----------------------


template<class V>
Trie<V>::Trie() {}

template<class V>
V* Trie<V>::find(std::string word) {
  return root->find(word);
}


template<class V>
void Trie<V>::create_with_default(std::vector<std::string> words, V* def) {
  //for (int i=0; i<26; i++) {
    //root[i] = new Node<V>(def);
  //}
  root = new Node<V>(def);

  for(auto word : words) {
    root->insert(word, def);
  }
}

/*
template<class V>
void Trie<V>::create_with_lists(std::vector<std::string> words, std::vector<V*> vals) {
  assert(words.size() == vals.size());

  for (int i=0; i<26; i++) {
    root[i] = new Node<V>(nullptr);
  }

  for(int i=0; i<words.size(); i++) {
    std::string word = words[i];
    char cc = word[0];
    char nc = cc - 'a';
    std::string nw = word.substr(1);
    root[nc]->insert(nw, vals[i]);
  }
}
*/

template<class V>
Trie<V> Trie<V>::create_from_file(std::string file_name, V* def) {
  std::ifstream file(file_name);
  std::cout << " --- Creating trie from file " << file_name << std::endl;

  std::string word;
  std::vector<std::string> words;

  while (file.good()) {
    std::getline(file, word);
    // TODO len > 0
    if (word.length() > 1 and all_of(word.begin(),word.end(), isalpha))
      words.push_back(word);
  }

  Trie<V> trie;
  trie.create_with_default(words, def);

  std::cout << " --- Done, with " << words.size() << " words" << std::endl;
  return trie;
}
