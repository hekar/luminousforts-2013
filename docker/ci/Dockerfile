FROM ubuntu:14.04
LABEL Description="Image LuminousForts Linux CI builds" Vendor="hekar" Version="1.0"

RUN apt-get update
RUN apt-get install -y gcc-multilib g++-multilib build-essential rsync

# CMD ./createallprojects && make -f everything.mak && rsync --progress -v ../game/mod_hl2mp/bin/* /lf/.steam/steam/steamapps/sourcemods/luminousforts/bin/

