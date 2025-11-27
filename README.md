# Capnp_Local

## required

[CMake](https://cmake.org/download/)

[Ninja](https://github.com/ninja-build/ninja/releases)

[Mingw](https://github.com/niXman/mingw-builds-binaries/releases?page=1)

[Python](https://www.python.org/downloads/)

[capnp](https://capnproto.org/install.html) ✓

[zlib-1.3.1](https://github.com/madler/zlib/releases/tag/v1.3.1) ✓

[OpenSSL-Win64](https://slproweb.com/products/Win32OpenSSL.html) ✓

## run

```
# add <project_root>/library/capnproto-tools-win32-1.2.0 to system path

capnp compile -I library/capnproto-c++-1.2.0/src/capnp -oc++ capnp/sample.capnp

cd capnp

python json2dat.py

cd ..

# set project_path in main.cpp:15

mkdir build

cd build

cmake ../ -G Ninja

cmake --build .

./capnp_demo.exe
```