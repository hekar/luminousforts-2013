# LuminousForts 2013

[![CircleCI](https://circleci.com/gh/hekar/luminousforts-2013/tree/master.svg?style=shield&circle-token=30c9378850b61d1265c749fd92e3a0513e7a68ad)](https://circleci.com/gh/hekar/luminousforts-2013/tree/master)

This is the source code repository for LuminousForts. To play the game, you will need the game files in the  [hekar/luminousforts](https://github.com/hekar/luminousforts) repository.

## Getting started

Clone the [LuminousForts](https://github.com/hekar/luminousforts) project into your `sourcemods` folder. Follow the instructions in [the repository for more information](https://github.com/hekar/luminousforts).

### Windows

#### Compile

1. Install [Visual Studio Community 2013](https://visualstudio.microsoft.com/vs/older/downloads/)
1. Run `createallprojects.bat` in `mp\src`
1. Open the solution in Visual Studio 2013
1. Build Solution

#### Debug

Refer to the Valve Source SDK wiki for more information. In particular, the following pages are relevant:

- [Launching from Visual Studio 2013](https://developer.valvesoftware.com/wiki/Launching_from_Visual_Studio_2013)
- [Debugging the Source Code](https://developer.valvesoftware.com/wiki/Installing_and_Debugging_the_Source_Code)

### Linux

The Linux version of the Source SDK requires GCC 4.x due to ABI compatibility with statically linked libraries provided by Valve.

#### Compile

The easiest way to compile is with [docker-compose](https://docs.docker.com/compose/).

```
docker-compose run lf /bin/bash
```

**Note**: Do not execute docker-compose as root. This mounts your `~/.steam` and will mount `/root/.steam` when executed with sudo.

#### Debug

You do not need to debug within the [docker](https://www.docker.com/) container. This can be executed from your host machine.

```
# GDB
GAME_DEBUGGER="gdb" ~/.local/share/Steam/steamapps/common/Source\ SDK\ Base\ 2013\ Multiplayer/hl2.sh -game sourcetest -steam -game ~/.local/share/Steam/steamapps/sourcemods/luminousforts

# Strace
GAME_DEBUGGER="strace -f -o strace.log" ~/.local/share/Steam/steamapps/common/Source\ SDK\ Base\ 2013\ Multiplayer/hl2.sh -game sourcetest -steam -game ~/.local/share/Steam/steamapps/sourcemods/luminousforts
```

## Binary Files

Most binary files are added to [git-lfs](https://git-lfs.github.com). If you see large files directly added to the repo, feel free to add them to [git-lfs](https://git-lfs.github.com).

## CircleCI

Builds for Linux are automatically created and published to [hekar/luminousforts](https://github.com/hekar/luminousforts) on every commit to `master`.

## Valve License

```
SOURCE 1 SDK LICENSE

Source SDK Copyright(c) Valve Corp.

THIS DOCUMENT DESCRIBES A CONTRACT BETWEEN YOU AND VALVE
CORPORATION ("Valve").  PLEASE READ IT BEFORE DOWNLOADING OR USING
THE SOURCE ENGINE SDK ("SDK"). BY DOWNLOADING AND/OR USING THE
SOURCE ENGINE SDK YOU ACCEPT THIS LICENSE. IF YOU DO NOT AGREE TO
THE TERMS OF THIS LICENSE PLEASE DON�T DOWNLOAD OR USE THE SDK.

  You may, free of charge, download and use the SDK to develop a modified Valve game
running on the Source engine.  You may distribute your modified Valve game in source and
object code form, but only for free. Terms of use for Valve games are found in the Steam
Subscriber Agreement located here: http://store.steampowered.com/subscriber_agreement/

  You may copy, modify, and distribute the SDK and any modifications you make to the
SDK in source and object code form, but only for free.  Any distribution of this SDK must
include this LICENSE file and thirdpartylegalnotices.txt.

  Any distribution of the SDK or a substantial portion of the SDK must include the above
copyright notice and the following:

    DISCLAIMER OF WARRANTIES.  THE SOURCE SDK AND ANY
    OTHER MATERIAL DOWNLOADED BY LICENSEE IS PROVIDED
    "AS IS".  VALVE AND ITS SUPPLIERS DISCLAIM ALL
    WARRANTIES WITH RESPECT TO THE SDK, EITHER EXPRESS
    OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED
    WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT,
    TITLE AND FITNESS FOR A PARTICULAR PURPOSE.

    LIMITATION OF LIABILITY.  IN NO EVENT SHALL VALVE OR
    ITS SUPPLIERS BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
    INDIRECT, OR CONSEQUENTIAL DAMAGES WHATSOEVER
    (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF
    BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF
    BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
    ARISING OUT OF THE USE OF OR INABILITY TO USE THE
    ENGINE AND/OR THE SDK, EVEN IF VALVE HAS BEEN
    ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.


If you would like to use the SDK for a commercial purpose, please contact Valve at
sourceengine@valvesoftware.com.
```
