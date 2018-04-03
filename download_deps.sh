#!/bin/zsh

mkdir external

echo " -- cloning JSON library"
git clone --depth 1 -b master https://github.com/nlohmann/json.git external/json


echo " -- cloning ws library"
git clone --depth 1 -b master https://github.com/zaphoyd/websocketpp.git external/websocketpp
