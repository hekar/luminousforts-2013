#!/bin/sh

sudo apt-get update
sudo apt-get install -y gcc-multilib g++-multilib build-essential rsync

cd "$BUILDPATH/mp/src"
./createallprojects
make -f everything.mak
rsync --progress -v ../game/mod_hl2mp/bin/* "$BUILDPATH"
