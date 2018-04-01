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


typedef websocketpp::server<websocketpp::config::asio> server;

//struct Message { std::string data; };
typedef std::string Message;

std::mutex pending_mtx;
std::condition_variable pending_cv;
std::vector<Message> pending;

void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
  std::unique_lock<std::mutex> lock(pending_mtx);

  //std::cout << msg->get_payload() << std::endl;

  std::string msg_str = msg->get_payload();
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

  std::cout << " -- Creating Trie" << std::endl;
  Trie<int> t;
  int def = 0;
  t.create_with_default({"hello", "apple", "world", "cat", "dog"}, &def);

  Keyboard kbd;

  std::cout << " -- Launching ws thread" << std::endl;
  std::thread ws_thread(ws_server_thread_target);

  std::cout<<" -- Main thread looping.\n";
  while(true) {

    std::unique_lock<std::mutex> lock(pending_mtx);
    pending_cv.wait(lock, []{return not pending.empty();});

    Message msg(pending.back());
    pending.pop_back();

    std::cout << " -- Read " << msg << std::endl;
  }

  return 0;
}
