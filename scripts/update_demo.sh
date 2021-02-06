#!/usr/bin/env bash
set -eo pipefail

./scripts/build.sh && \
cp dist/index.js dist/index.wasm demo/ && \
cd demo/ && make && cd ..
