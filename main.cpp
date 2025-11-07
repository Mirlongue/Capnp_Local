#include<memory>
#include<iostream>
#include<capnp/serialize.h>
#include<capnp/serialize-packed.h>
#include<capnp/message.h>
#include<kj/array.h>
#include<kj/memory.h>
#include<kj/string.h>
#include<kj/string-tree.h>
#include"capnp/example/example.capnp.h"

using namespace std;

int main(){
    shared_ptr<capnp::MallocMessageBuilder> test0_mbuilder_ptr = make_shared<capnp::MallocMessageBuilder>();
    capnp::Sample::Builder test0_builder = test0_mbuilder_ptr->initRoot<capnp::Sample>();
    test0_builder.setIntegerSample(999);
    test0_builder.setStSample("hellow world!");    // std::string    char st[<length>]    char * st (no safe)
    int arr[4] = {0, 1, 2, 3};
    test0_builder.setArrSample(arr);

    capnp::Sample::Reader test0_reader = test0_builder.asReader();

    shared_ptr<capnp::MallocMessageBuilder> test1_mbuilder_ptr = make_shared<capnp::MallocMessageBuilder>();
    test1_mbuilder_ptr->setRoot(test0_reader);
    capnp::Sample::Builder test1_builder = test1_mbuilder_ptr->getRoot<capnp::Sample>();
    capnp::List<int, capnp::Kind::PRIMITIVE>::Builder arr_builder = test1_builder.getArrSample();    // test1_builder.initArrSample(4)
    for(int i = 0; i < 4; i++) {
        arr_builder.set(i, 4 - i);    // test1_builder.getArrSample().set(i, 4 - i);
    }

    capnp::Sample::Reader test1_reader = test1_builder.asReader();

    int integer_sample = test1_reader.getIntegerSample();
    string st_sample(test1_reader.getStSample().begin(), test1_reader.getStSample().end());
    int arr_sample[4];
    for(int i = 0; i < 4; i++){
        arr_sample[i] = test1_reader.getArrSample()[i];
    }

    string sample_flat(test1_reader.toString().flatten().cStr());
    int sample_size(test1_reader.toString().flatten().size());

    cout << "##########" << endl;
    cout << "sample_flat: " << sample_flat << endl;
    cout << "sample_size: " << sample_size << endl;

    cout << "##########" << endl;
    cout << "integer_sample: " << integer_sample << endl;
    cout << "st_sample: " << st_sample << endl;
    cout << "arr_sample: ";
    for(int i = 0; i < 4; i++)
        cout << arr_sample[i] << " ";
    cout << endl;
    cout << "enum_sample: " << capnp::Sample::ENUM_SAMPLE << endl;


    return 0;
}