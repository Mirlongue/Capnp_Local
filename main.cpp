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

const std::string PATH_ROOT = "D:\\Reposity_github\\Capnp_Local\\build\\"; // <project_root>/build

void dump_capnp_for_bit(std::shared_ptr<capnp::MallocMessageBuilder> tmp_mbuilder_ptr, std::string name) {
    std::string path = PATH_ROOT + name;
    std::ofstream ofs(path, std::ios::binary);
    if (ofs) {
        kj::Array<capnp::word> words = capnp::messageToFlatArray(*tmp_mbuilder_ptr);
        kj::ArrayPtr<kj::PropagateConst<capnp::word, capnp::byte>> bytes = words.asBytes();
        const char * addr = reinterpret_cast<const char *>(bytes.begin());
        int size = bytes.size();
        std::copy(addr, addr + size, std::ostream_iterator<char>{ofs});
        ofs.close();
    }
    return;
}

template<typename T>
std::shared_ptr<capnp::MallocMessageBuilder> get_capnp_for_bit(std::string const name) {
    std::string path = PATH_ROOT + name;
    std::ifstream ifs = std::ifstream{path};
    std::shared_ptr<capnp::MallocMessageBuilder> tmp_mbuilder_ptr =
        std::make_shared<capnp::MallocMessageBuilder>();
    if (ifs) {
        std::vector<char> buffer(std::istreambuf_iterator<char>{ifs}, std::istreambuf_iterator<char>{});
        capnp::word * words = reinterpret_cast<capnp::word *>(buffer.data());
        int word_count = buffer.size() / sizeof(capnp::word);
        capnp::FlatArrayMessageReader reader(kj::ArrayPtr<capnp::word>(words, word_count));
        auto tmp_reader = reader.getRoot<T>();
        tmp_mbuilder_ptr->setRoot(tmp_reader);
        ifs.close();
    }
    return tmp_mbuilder_ptr;
}


int main() {
    std::shared_ptr<capnp::MallocMessageBuilder> test0_mbuilder_ptr =
        std::make_shared<capnp::MallocMessageBuilder>();
    sample::Sample::Builder test0_builder =
        test0_mbuilder_ptr->initRoot<sample::Sample>();
    test0_builder.setIntegerSample(999);
    test0_builder.setStSample("hellow world!");    // std::string  or  char st[<length>]    char * st (no safe)
    capnp::List<int, capnp::Kind::PRIMITIVE>::Builder arr_builder = test0_builder.initArrSample(4);
    for(int i = 0; i < 4; i++) {
        arr_builder.set(i, i);    // test0_builder.getArrSample().set(i,i);
    }
    // int arr[4] = {0, 1, 2, 3};
    // test0_builder.setArrSample(arr);
    sample::Sample::Reader test0_reader = test0_builder.asReader();
    // dump_capnp_for_bit(test0_mbuilder_ptr, "test0");

    std::shared_ptr<capnp::MallocMessageBuilder> test1_mbuilder_ptr =
        std::make_shared<capnp::MallocMessageBuilder>();
    test1_mbuilder_ptr->setRoot(test0_reader);
    // std::shared_ptr<capnp::MallocMessageBuilder> test1_mbuilder_ptr =
    //     get_capnp_for_bit<sample::Sample>("test0");
    sample::Sample::Builder test1_builder =
        test1_mbuilder_ptr->getRoot<sample::Sample>();
    sample::Sample::Reader test1_reader = test1_builder.asReader();

    int integer_sample = test1_reader.getIntegerSample();
    std::string st_sample(test1_reader.getStSample().begin(), test1_reader.getStSample().end());
    int arr_sample[4];
    for(int i = 0; i < 4; i++) {
        arr_sample[i] = test1_reader.getArrSample()[i];
    }
    int enum_sample = sample::Sample::ENUM_SAMPLE;
    std::cout << "##########" << std::endl;
    std::cout << "integer_sample: " << integer_sample << std::endl;
    std::cout << "st_sample: " << st_sample << std::endl;
    std::cout << "arr_sample:";
    for(int i = 0; i < 4; i++) {
        std::cout << " " << arr_sample[i];
    }
    std::cout << std::endl;
    std::cout << "enum_sample: " << enum_sample << std::endl;

    std::string sample_flat(test1_reader.toString().flatten().cStr());
    int sample_size(test1_reader.toString().flatten().size());
    std::cout << "##########" << std::endl;
    std::cout << "sample_flat: " << sample_flat << std::endl;
    std::cout << "sample_size: " << sample_size << std::endl;

    return 0;
}