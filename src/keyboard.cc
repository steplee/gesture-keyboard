
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
  cout << " -- Trie test...\n";
  cout << "\ttrie find hello -> " << trie.find("hello") << endl;
  cout << "\ttrie find test -> " << trie.find("test") << endl;
  cout << "\ttrie find apple -> " << trie.find("apple") << endl;
  cout << "\ttrie find zsdjslfje -> " << trie.find("lshflkshdf") << endl;
  cout << endl;
  // ---------------------------------

  searcher = new Searcher(&trie, this);
}

static void load_word_file(string& dst, string file_name) {

}

void Keyboard::recv_move(float y, float x, float time) {

}

float Keyboard::distance(const point& a, const point& b) const {
  return sqrt( (a.first-b.first)*(a.first-b.first) + (a.second-b.second)*(a.second-b.second) );
}
float Keyboard::dist_to_score(const float dist) const {
  float s = 1-log(2.71828 * dist*dist);
  return max(0.f,min(1.f,s));
}


std::pair<float,int> Keyboard::score_pairwise_move(int min_time, int max_time, char from, char to) const {
  timepoint ta = path[min_time];
  point &a = ta.second;

  if (max_time < 0) max_time = path.size();

  int argbest = min_time;
  float best = 0.0;

  for (int t=min_time+1; t<max_time; t++) {
    float d = distance(a, path[t].second);
    if (d < best) {
      best = d;
      argbest = t;
    }
  }

  return {dist_to_score(best), argbest};
}
