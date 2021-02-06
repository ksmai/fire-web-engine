#!/usr/bin/env bash
set -eo pipefail

./scripts/build.sh && cp dist/index.js dist/index.wasm demo/
cd demo/
rm resources.zip
zip resources.zip \
  levels.xml \
  music.ogg \
  roguelikeSheet_transparent.png \
  SpriteSheets.xml
cd ..
