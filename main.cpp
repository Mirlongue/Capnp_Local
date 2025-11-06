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
    shared_ptr<capnp::MallocMessageBuilder> test0_mbuilder_ptr = make_shared<capnp::MallocMessageBuilder>();
    test::Test::Builder test0_builder = test0_mbuilder_ptr->initRoot<test::Test>();
    test0_builder.setTmpFlag(999);
    test::Test::Reader test0_reader = test0_builder.asReader();

    shared_ptr<capnp::MallocMessageBuilder> test1_mbuilder_ptr = make_shared<capnp::MallocMessageBuilder>();
    test1_mbuilder_ptr->setRoot(test0_reader);
    test::Test::Builder test1_builder = test1_mbuilder_ptr->getRoot<test::Test>();
    test::Test::Reader test1_reader = test1_builder.asReader();

    cout << test1_builder.getTmpFlag() << endl;
    return 0;
}