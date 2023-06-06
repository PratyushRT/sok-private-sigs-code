FROM alpine:latest

RUN apk --no-cache add git cmake clang clang-dev make gcc g++ libc-dev linux-headers libressl-dev

COPY . /libgroupsig

RUN cd libgroupsig && \
    rm -rf build && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make

ENV JAVA_HOME=/usr/lib/jvm/java-11-openjdk

RUN apk --no-cache add openjdk11 maven && \
    cd /libgroupsig/src/wrappers/java/jgroupsig && \
    mvn clean package

ENTRYPOINT [ "/bin/sh" ]