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
  bool key_is_close[26];
  timepoint tpt = kbd->getPath(time);
  point pt = tpt.second;
  float ftime = pt.first;

  cout << " obs " << pt.first << " " << pt.second << endl;

  float d_tolerance = .25;


  // If this this the first letter, we add nearest unconditionally.
  if (time == 0 or cands.size() == 0) {
    cout << " - Assuming new word\n";

    for (char c=0; c<26; c++) {
      if ( kbd->dist_to_key(pt, c) < d_tolerance ) {
        cout << (char)('a'+c) << " is close\n";
        Entry new_ent{string(1,(char)(c+'a')), ftime};
        fringe[c].push_back(new_ent);
        bool is_terminal = true;
        if (is_terminal)
          cands.push_back(new_ent);
      }
    }
    cout << " done new\n";
  }


  // Find which keys to check on
  for (char c=0; c<26; c++) {
    key_is_close[c] = kbd->dist_to_key(pt, c) < d_tolerance;
  }

  
  int fringe_sizes[26];
  for (int i=0; i<26; i++) fringe_sizes[i] = fringe[i].size();

  // Loop over close keys, if they have continuations, add them to fringe
  for (char c=0; c<26; c++) if (key_is_close[c]) {
    // Find all `fringe` entries ending in this char.
    // See if we should extend them (if they are young & word exists)

    cout << "check " << (char)(c+'a') << endl;
    cout << "check fringe size " << fringe[c].size() << endl;

    Stopped here: need another inner loop per c2


    for (int i=0; i<fringe_sizes[c]; i++) {
      Entry& ent = fringe[c][i];

      cout << "check ent " << ent.acc << endl;

      // if we encounter an old entry, remove it.
      /*
      if (ent.time_created < ftime+1.f) {
        cout << " - Removing fringe entry " << ent.acc << endl;
        fast_remove(fringe[c], i);
        fringe_sizes[c]--;
        continue;
      }
      */

      // if a word or word continuation exists (TODO use a better datastructure)
      //bool do_extend = dict.find(ent.acc+((char)('a'+c))) != dict.end();
      bool do_extend = true; // TODO

      // TODO make dict store <string,continues,terminal> tuples.
      bool is_terminal = true;

      for (char e='a'; (not do_extend) and e<'z'; e++)
        if (dict.find(ent.acc+c+e) != dict.end()) {
          do_extend = true;
      }

      if (do_extend) {
        Entry new_ent{ent.acc+((char)('a'+c)), ftime};
        cout << " extend -> " << new_ent.acc << "total cands: " << cands.size() << endl;
        fringe[c].push_back(new_ent);

        if (is_terminal)
          cands.push_back(new_ent);
      }
    }
  }
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
