
#include "keyboard.h"


static void load_word_file(std::string& dst, std::string file_name) {

}

Keyboard::Keyboard()
{
  // create Trie
  float f = 1.0;
  //trie = Trie<float>::create_from_file("/usr/share/dict/cracklib-small", &f);
  trie = Trie<float>::create_from_file("./words", &f);
}


void Keyboard::recv_move(float y, float x, float time) {

}
