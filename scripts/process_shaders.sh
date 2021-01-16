#!/usr/bin/env bash

parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

cd "${parent_path}"

for shader in $(find "../src/Graphics/Shaders/" -type f -name '*.glsl')
do
  xxd -i "$shader" > "${shader%.glsl}.h"
done
