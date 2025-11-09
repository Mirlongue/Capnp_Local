@0xdb3844e515d0631b;

using Cxx = import"/c++.capnp";
$Cxx.namespace("capnp");

struct Sample{
    integerSample @0: Int32;
    stSample @1: Text;
    arrSample @2: List(Int32);
    const enumSample : Int32 = 1105;
}
