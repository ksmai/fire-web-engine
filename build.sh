#!/usr/bin/env bash

docker build -t fire-web-engine .
docker run --rm -v $(pwd):/src fire-web-engine "$@"
