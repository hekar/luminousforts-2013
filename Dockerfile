FROM ubuntu:14.04

RUN apt-get update && apt-get install -y gcc-multilib g++-multilib build-essential rsync

CMD ["/lf/build.sh"]
