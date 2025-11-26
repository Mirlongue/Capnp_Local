# add <project_root>/library/capnproto-tools-win32-1.2.0 to system path
# capnp compile -I <project_root>/library/capnproto-c++-1.2.0/src/capnp -oc++ sample.capnp

@0xdb3844e515d0631b; # capnp id

using Cxx = import"/c++.capnp";
$Cxx.namespace("sample");

struct Sample{
    integerSample @0: Int32;
    stSample @1: Text;
    arrSample @2: List(Int32);
    const enumSample : Int32 = 1105;
}