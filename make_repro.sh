#!/bin/bash

echo ""
echo "Building test app (sampleapp.dll)"

pushd sampleapp
dotnet build -c release
dotnet publish -r osx-x64 -c release
popd

echo ""
echo "Building profiler library"

bash build.sh

if [ -d "repro/" ]; then
    echo "deleting repro folder"
    rm -rf repro/
fi

mkdir repro

pushd repro

mkdir runtime

echo ""
echo "Copying repro.cmd to repro folder"
cp ../src/linux/repro.sh .

echo ""
echo "Copying libCorProfiler.dylib to repro folder"
cp ../libCorProfiler.dylib .

echo ""
echo "Copying published files to runtime folder"
cp -R ../sampleapp/bin/release/netcoreapp3.1/osx-x64/* runtime/

echo ""
echo "Copying app files to repro folder"
cp -R ../sampleapp/bin/release/netcoreapp3.1/*.dll .
cp -R ../sampleapp/bin/release/netcoreapp3.1/*.pdb .

if [ -z "$1" ]; then
    echo "Did not pass a path to a coreclr repo, skipping copying private bits"
    exit 1
fi

echo "Copying coreclr from $1*"
cp -R $1/* runtime/

popd