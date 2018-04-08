#include "keyboard.h"
#include <nlohmann/json.hpp>


//#include "search/trie_search.h"
#include "search/hash_pairwise_search.h"


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


  searcher = new HashPairwiseSearcher(this);
  //searcher = new TrieSearcher(this);
}

void Keyboard::recv_move(float y, float x, float time) {
  point p{y,x};
  path.push_back({time,p});
  cout << "move " << y << " " << x << " at " << time << " path len " << path.size() << endl;

  searcher->observe_move(path.size()-1);
}

void Keyboard::recv_reset() {
  searcher->reset();
}

float Keyboard::distance(const point& a, const point& b) const {
  float d = sqrt( (a.first-b.first)*(a.first-b.first) + (a.second-b.second)*(a.second-b.second) );
  return d;
}
float Keyboard::key_dist(char k1, char k2) const {
  return distance(key_pos[k1], key_pos[k2]);
}
float Keyboard::dist_to_score(const float dist) const {
  //float s = 1-log(2.71828 * dist*dist);
  float s;
  if (dist == 0)
    s = 1.00001;
  else
    s = 1. - log(2.71828 * dist * 2.0);


  return max(0.f,min(1.f,s));
}

float Keyboard::score_start(char ch) const {
  // take path[0] and compare it to the key.
  float d = distance(key_pos[ch], path[0].second);
  return dist_to_score(d);
}

std::pair<float,int> Keyboard::score_pairwise_move(int min_time, int max_time, char from, char to) const {
  timepoint ta = path[min_time];
  point &a = ta.second;

  if (max_time < 0) max_time = path.size();

  int argbest = min_time;
  float bestdist = 99.0;

  /*
  bestdist = distance(path[min_time].second, path[max_time].second);
  return {max(0.f, .6f-bestdist),0};
  */

  for (int t=min_time+1; t<max_time; t++) {
    float d = distance(a, path[t].second);
    if (d < bestdist) {
      bestdist = d;
      argbest = t;
    }
  }


  float s = dist_to_score(bestdist); 
  return {s, argbest};
}

float Keyboard::dist_to_key(point& p, char ch) {
  return distance(p, key_pos[ch]);
}

timepoint Keyboard::getPath(int time) {
  return path[time];
}
