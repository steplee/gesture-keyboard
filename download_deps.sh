#!/bin/zsh

mkdir external

echo " -- cloning JSON library"
git clone --depth 1 -b master https://github.com/nlohmann/json.git external


echo " -- cloning ws library"
git clone --depth 1 -b master https://github.com/zaphoyd/websocket.git external
