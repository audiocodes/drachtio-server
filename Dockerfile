FROM ubuntu:24.10 AS build
RUN apt-get update \
  && apt-get install -y --no-install-recommends \
    autoconf \
    automake \
    build-essential \
    ca-certificates \
    cmake \
    curl \
    g++ \
    gcc \
    git \
    jq \
    libboost-filesystem-dev \
    libboost-log-dev \
    libboost-system-dev \
    libboost-thread-dev \
    libcurl4-openssl-dev \
    libgoogle-perftools-dev \
    libhiredis-dev \
    libssl-dev \
    libtool \
    libtool-bin \
    make \
    zlib1g-dev

WORKDIR /usr/local/src/drachtio-server
COPY . .
RUN ./bootstrap.sh
WORKDIR /usr/local/src/drachtio-server/build
ARG MYVERSION=1.0.0
RUN ../configure --enable-tcmalloc=yes CPPFLAGS='-DNDEBUG' CXXFLAGS='-O2'
RUN make -j$(nproc) MYVERSION=${MYVERSION}

FROM ubuntu:24.10
RUN apt-get update \
  && apt-get install -y --no-install-recommends \
    ca-certificates \
    curl \
    jq \
    less \
    libboost-filesystem1.83.0 \
    libboost-log1.83.0 \
    libboost-system1.83.0 \
    libboost-thread1.83.0 \
    libgoogle-perftools4 \
    libhiredis1.1.0 \
    net-tools \
    procps \
    sudo \
  && rm -rf /var/lib/apt/lists/* /var/cache/apt/archives/*

COPY --from=build /usr/local/src/drachtio-server/build/drachtio /usr/local/bin/
COPY docker.drachtio.conf.xml /etc/drachtio.conf.xml
COPY ./entrypoint.sh /

VOLUME ["/config"]

ENTRYPOINT ["/entrypoint.sh"]

RUN echo '%sudo   ALL=(ALL:ALL) NOPASSWD: ALL' > /etc/sudoers.d/nopasswd && \
  usermod -md /home/drachtio -l drachtio ubuntu && \
  groupmod -n drachtio ubuntu
USER drachtio
WORKDIR /home/drachtio

CMD ["drachtio"]
