#!/bin/bash

[ -z "${BuildOS:-}"      ] && export BuildOS=osx-x64
[ -z "${BuildArch:-}"    ] && export BuildArch=x64
[ -z "${BuildType:-}"    ] && export BuildType=Debug

[ -z "${CORECLR_PATH:-}" ] && export CORECLR_PATH=~/work/runtime/src/coreclr
[ -z "${CORECLR_BIN:-}"  ] && export CORECLR_BIN=~/work/runtime/artifacts/bin/coreclr/$BuildOS.$BuildArch.$BuildType

printf '  CORECLR_PATH : %s\n' "$CORECLR_PATH"
printf '  BuildOS      : %s\n' "$BuildOS"
printf '  BuildArch    : %s\n' "$BuildArch"
printf '  BuildType    : %s\n' "$BuildType"

printf '  Building ...'

if [ ! -d "bin/" ]; then
    mkdir bin/
fi

pushd bin

export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
cmake ../ -DCMAKE_BUILD_TYPE=Debug

make -j8

popd

printf '  Copying libCorProfiler.dylib to main directory\n'
cp bin/libCorProfiler.dylib .

printf 'Done.\n'
