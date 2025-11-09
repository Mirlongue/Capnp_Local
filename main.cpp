#include<memory>
#include<iostream>
#include<fstream>
#include<iterator>
#include<vector>
#include<capnp/serialize.h>
#include<capnp/serialize-packed.h>
#include<capnp/message.h>
#include<kj/array.h>
#include<kj/memory.h>
#include<kj/string.h>
#include<kj/string-tree.h>
#include"capnp/sample.capnp.h"

const std::string PATH_ROOT = "D:\\Reposity_github\\Capnp_Local\\build\\";

void dump_sample_for_bit(std::shared_ptr<capnp::MallocMessageBuilder> sample_mbuilder_ptr, std::string name){
    std::string path = PATH_ROOT + name;
    std::ofstream ofs(path, std::ios::binary);
    if (ofs) {
        kj::Array<capnp::word> words = capnp::messageToFlatArray(*sample_mbuilder_ptr);
        kj::ArrayPtr<kj::PropagateConst<capnp::word, capnp::byte>> bytes = words.asBytes();
        auto addr = reinterpret_cast<const char*>(bytes.begin());
        auto size = bytes.size();
        std::copy(addr,addr + size, std::ostream_iterator<char>{ofs});
        ofs.close();
    }
    return;
}

template<typename T>
std::shared_ptr<capnp::MallocMessageBuilder> get_sample_for_bit(std::string const name){
    std::string path = PATH_ROOT + name;
    std::shared_ptr<capnp::MallocMessageBuilder> sample_mbuilder_ptr = std::make_shared<capnp::MallocMessageBuilder>();
    std::ifstream ifs = std::ifstream{path};
    if (ifs) {
        std::vector<char> buffer(std::istreambuf_iterator<char>{ifs}, std::istreambuf_iterator<char>{});
        auto words = reinterpret_cast<capnp::word*>(buffer.data());
        int wordCount = buffer.size()/ sizeof(capnp::word);
        capnp::FlatArrayMessageReader reader(kj::ArrayPtr<capnp::word>(words, wordCount));
        auto tmp = reader.getRoot<T>();
        sample_mbuilder_ptr->setRoot(tmp);
        ifs.close();
    }
    return sample_mbuilder_ptr;

} 


int main(){
    std::shared_ptr<capnp::MallocMessageBuilder> test0_mbuilder_ptr = std::make_shared<capnp::MallocMessageBuilder>();
    sample::Sample::Builder test0_builder = test0_mbuilder_ptr->initRoot<sample::Sample>();
    test0_builder.setIntegerSample(999);
    test0_builder.setStSample("hellow world!");    // std::string    char st[<length>]    char * st (no safe)
    int arr[4] = {0, 1, 2, 3};
    test0_builder.setArrSample(arr);

    sample::Sample::Reader test0_reader = test0_builder.asReader();
    dump_sample_for_bit(test0_mbuilder_ptr,"test0");

    std::shared_ptr<capnp::MallocMessageBuilder> test1_mbuilder_ptr = std::make_shared<capnp::MallocMessageBuilder>();
    // test1_mbuilder_ptr = get_sample_for_bit<sample::Sample>("test0");
    test1_mbuilder_ptr->setRoot(test0_reader);

    sample::Sample::Builder test1_builder = test1_mbuilder_ptr->getRoot<sample::Sample>();
    capnp::List<int, capnp::Kind::PRIMITIVE>::Builder arr_builder = test1_builder.getArrSample();    // test1_builder.initArrSample(4)
    for(int i = 0; i < 4; i++) {
        arr_builder.set(i, 4 - i);    // test1_builder.getArrSample().set(i, 4 - i);
    }

    sample::Sample::Reader test1_reader = test1_builder.asReader();

    int integer_sample = test1_reader.getIntegerSample();
    std::string st_sample(test1_reader.getStSample().begin(), test1_reader.getStSample().end());
    int arr_sample[4];
    for(int i = 0; i < 4; i++){
        arr_sample[i] = test1_reader.getArrSample()[i];
    }

    std::string sample_flat(test1_reader.toString().flatten().cStr());
    int sample_size(test1_reader.toString().flatten().size());

    std::cout << "##########" << std::endl;
    std::cout << "sample_flat: " << sample_flat << std::endl;
    std::cout << "sample_size: " << sample_size << std::endl;

    std::cout << "##########" << std::endl;
    std::cout << "integer_sample: " << integer_sample << std::endl;
    std::cout << "st_sample: " << st_sample << std::endl;
    std::cout << "arr_sample: ";
    for(int i = 0; i < 4; i++)
        std::cout << arr_sample[i] << " ";
    std::cout << std::endl;
    std::cout << "enum_sample: " << sample::Sample::ENUM_SAMPLE << std::endl;


    return 0;
}