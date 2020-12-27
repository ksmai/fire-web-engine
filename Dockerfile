FROM emscripten/emsdk:2.0.11

WORKDIR /src

ENTRYPOINT ["make"]
