#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

cd ./mp/src
make -f games.mak
rsync --progress -v ../game/mod_hl2mp/bin/* /game/bin/
