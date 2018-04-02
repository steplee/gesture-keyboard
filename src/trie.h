#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <istream>
#include <fstream>


template<class V>
class Node {
  public:
    Node(V* v);

    void insert(std::string& w, V* v);
    V* find(std::string& w);

    Node<V>* getChild(char c);
    char getKey();
    V* getVal();

  private:
    Node<V>* children[26];
    bool terminal;

    char key;
    V* val;

};


template<class V>
class Trie {
  public:
    Trie();

    void create_with_default(std::vector<std::string> words, V* def);
    void create_with_lists(std::vector<std::string> words, std::vector<V*> vals);

    static Trie create_from_file(std::string file_name, V* val);


    V* find(std::string word);

  private:
    Node<V>* root;

};



template class Node<int>;
template class Trie<int>;
template class Node<float>;
template class Trie<float>;
