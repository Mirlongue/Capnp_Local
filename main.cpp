#include<memory>
#include<iostream>
#include<capnp/serialize.h>
#include<capnp/serialize-packed.h>
#include<capnp/message.h>
#include<kj/array.h>
#include<kj/memory.h>
#include<kj/string.h>
#include<kj/string-tree.h>
#include"capnp/example/test.capnp.h"


using namespace std;

int main(){
    shared_ptr<capnp::MallocMessageBuilder> tmp_mbuilder_ptr = make_shared<capnp::MallocMessageBuilder>();

    test::Test::Builder test_builder = tmp_mbuilder_ptr->initRoot<test::Test>();
    test_builder.setTmpFlag(999);

    cout << "hellow world!" << endl;
    cout << test_builder.getTmpFlag() << endl;
    return 0;
}