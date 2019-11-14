FROM ubuntu:latest
MAINTAINER Dmitry Chegolya <blackmesa2018@gmail.com>

RUN  apt-get update && apt-get install -y build-essential

WORKDIR /usr/src
COPY . /usr/src

RUN g++ --version

RUN g++ -o out main.cpp