FROM ubuntu:20.04

ARG uid
ARG gid
ARG user
ARG group

RUN groupadd -g ${gid} ${group} || true
RUN useradd -l -u ${uid} -g ${group} -m ${user} || true

ENV TERM=xterm
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get install -y tzdata
RUN apt-get upgrade -y
RUN apt-get install -y g++ vim cmake make git libgtk2.0-dev pkg-config libopencv-dev
RUN apt-get clean

