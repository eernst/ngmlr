FROM ubuntu

MAINTAINER Philipp Rescheneder <philipp.rescheneder@gmail.com>

ENV NEXTGENMAP_LR_BUILD_ENV 1.0.1
#ENV SLAMDUNK_DOWNLOAD_URL https://github.com/t-neumann/slamdunk.git

# binutils is required to run opencl programs
RUN buildDeps='git wget gcc g++ libc6-dev make cmake zlib1g-dev gdb samtools bedtools vim' \
    && set -x \
    && apt-get update && apt-get install -y $buildDeps $runDeps --no-install-recommends
    