#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

SRC_FOLDER=$DIR/mp/src

pushd $SRC_FOLDER
if ! [ -r games.mak ]
then
	./creategameprojects
fi

make -f games.mak
rsync --progress -v ../game/mod_hl2mp/bin/* /game/bin/
popd