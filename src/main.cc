#include <string>
#include <cassert>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"


#include "trie.h"
#include "keyboard.h"

/*
 * This file will set up the server & kbd threads.
 * It also will receive messages from the server and forward them structured to the kbd.
 */

using namespace std;


typedef websocketpp::server<websocketpp::config::asio> server;

//struct Message { string data; };
typedef string Message;

mutex pending_mtx;
condition_variable pending_cv;
vector<Message> pending;

void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
  unique_lock<mutex> lock(pending_mtx);

  //cout << msg->get_payload() << endl;

  string msg_str = msg->get_payload();
  pending.push_back(msg_str);

  lock.unlock();
  pending_cv.notify_one();
}

// Kick off server to listen to UI
void ws_server_thread_target() {
  server print_server;

  print_server.set_message_handler(&on_message);

  print_server.init_asio();
  print_server.listen(9002);
  print_server.start_accept();

  print_server.run();
}

int main() {

  cout << " -- Creating Trie" << endl;
  Trie<int> t;
  int def = 0;
  t.create_with_default({"hello", "apple", "world", "cat", "dog"}, &def);

  Keyboard kbd;

  cout << " -- Launching ws thread" << endl;
  thread ws_thread(ws_server_thread_target);

  cout<<" -- Main thread looping.\n";
  while(true) {

    unique_lock<mutex> lock(pending_mtx);
    pending_cv.wait(lock, []{return not pending.empty();});

    Message msg(pending.back());
    pending.pop_back();

    cout << " -- Read " << msg << endl;
  }

  return 0;
}
