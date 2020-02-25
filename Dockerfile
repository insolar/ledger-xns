FROM ubuntu:19.04 as builder

RUN apt-get update
RUN apt-get install -y build-essential  python3-pip git

RUN apt-get install -y cmake libusb-1.0.0-dev libudev-dev

RUN pip3 install conan ledgerblue

ENV BOLOS_SDK=/app/nanos-secure-sdk/

COPY . /app
WORKDIR /app

RUN conan profile new default --detect
RUN conan profile update settings.compiler.libcxx=libstdc++11 default

RUN cmake . && make -j5
RUN make test



