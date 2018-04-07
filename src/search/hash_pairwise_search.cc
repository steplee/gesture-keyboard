#include <cstring>
#include <utility>
#include <algorithm>
#include <vector>

#include "search/hash_pairwise_search.h"
#include "keyboard.h"

using namespace std;


HashPairwiseSearcher::HashPairwiseSearcher(Keyboard* kbd) : Searcher(kbd) 
{
  populate_words("./words");
};


void HashPairwiseSearcher::observe_move(int time) {
}

void HashPairwiseSearcher::query(int time) {
}

void HashPairwiseSearcher::reset() {
}

void HashPairwiseSearcher::populate_words(string file_name) {
  ifstream file(file_name);
  cout << " --- Populating words from " << file_name << endl;

  string word;

  while (file.good()) {
    getline(file, word);
    if (word.length() > 1 and all_of(word.begin(),word.end(), [](char c){return isalpha(c);}))
      dict.insert(word);
  }


  cout << " --- Done, with " << dict.size() << " words" << endl;
}
