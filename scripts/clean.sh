#!/usr/bin/env bash

docker build -t fire-web-engine .
docker run \
  --rm \
  -v $(pwd):/src \
  -v emscriptencache:/emsdk/upstream/emscripten/cache \
  -u $(id -u):$(id -g) \
  fire-web-engine clean