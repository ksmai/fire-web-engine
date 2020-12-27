#!/usr/bin/env bash

docker build -t fire-web-engine .
docker run --rm -v $(pwd):/src -u $(id -u):$(id -g) fire-web-engine "$@"
