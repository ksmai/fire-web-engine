FROM emscripten/emsdk:2.0.11

RUN apt-get update && apt-get install -y xxd

WORKDIR /src

ENTRYPOINT ["make"]
