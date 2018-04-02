
#include "keyboard.h"
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;

static const string key_layout_file = "./layouts/basic.json";

Keyboard::Keyboard() {
  // ---------------------------------
  // Load key locations
  ifstream layout_stream(key_layout_file);
  json layout;
  layout_stream >> layout;
  json keys(layout["keys"]);

  //for (auto ko : layout["keys"]) {
  for (auto it=keys.begin(); it!=keys.end(); it++)
    key_pos[it.key()[0]-'a'] = {it.value()[0], it.value()[1]};
  // ---------------------------------


  // ---------------------------------
  // create Trie
  float f = 1.0;
  //trie = Trie<float>::create_from_file("/usr/share/dict/cracklib-small", &f);
  trie = Trie<float>::create_from_file("./words", &f);

  // test
  cout << "trie find hello -> " << trie.find("hello") << endl;
  cout << "trie find test -> " << trie.find("test") << endl;
  cout << "trie find apple -> " << trie.find("apple") << endl;
  cout << "trie find zsdjslfje -> " << trie.find("lshflkshdf") << endl;
  // ---------------------------------

  searcher = new Searcher(&trie);
}

static void load_word_file(string& dst, string file_name) {

}

void Keyboard::recv_move(float y, float x, float time) {

}
