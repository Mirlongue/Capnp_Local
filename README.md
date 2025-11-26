# Capnp_Local

## required

[Python 3.11.5](https://www.python.org/downloads/)

[capnp](https://capnproto.org/install.html) ✓

[zlib-1.3.1](https://github.com/madler/zlib/releases/tag/v1.3.1) ✓

[OpenSSL-Win64](https://slproweb.com/products/Win32OpenSSL.html) ✓

## run

```
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