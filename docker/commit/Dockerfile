FROM fedora:28
LABEL Description="LuminousForts CI - commit" Vendor="hekar" Version="1.0"

RUN dnf install -y git curl rsync
RUN curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.rpm.sh | bash
RUN dnf install -y git-lfs

# git clone --depth 1 https://github.com/hekar/luminousforts.git
