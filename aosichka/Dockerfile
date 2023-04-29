FROM gcc:latest

COPY aos.cpp /usr/src/cpp_laba/

WORKDIR /usr/src/cpp_laba/

RUN g++ -o Test aos.cpp

CMD [ "./Test" ]
