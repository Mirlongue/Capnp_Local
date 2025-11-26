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

const std::string PATH_ROOT = "D:/Reposity_github/Capnp_Local";

void capnp_dump(std::shared_ptr<capnp::MallocMessageBuilder> tmp_mbuilder_ptr, std::string name) {
    std::string path = PATH_ROOT + "/build/" + name;
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
std::shared_ptr<capnp::MallocMessageBuilder> capnp_load_from_dump(std::string const name) {
    std::string path = PATH_ROOT +  "/build/" + name;
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

template<typename T>
std::shared_ptr<capnp::MallocMessageBuilder> capnp_load_from_text(std::string const name) {
    std::string path = PATH_ROOT + "/capnp/" + name;
    std::ifstream ifs = std::ifstream{path};
    std::shared_ptr<capnp::MallocMessageBuilder> tmp_mbuilder_ptr =
        std::make_shared<capnp::MallocMessageBuilder>();
    if (ifs) {
        std::vector<char> buffer(std::istreambuf_iterator<char>{ifs}, std::istreambuf_iterator<char>{});
        capnp::word * words = reinterpret_cast<capnp::word *>(buffer.data());
        int word_count = buffer.size() / sizeof(capnp::word);
        const kj::ArrayPtr<const capnp::word> segment(words, word_count);
        kj::ArrayPtr<const kj::ArrayPtr<const capnp::word>> segments(&segment, 1);
        capnp::SegmentArrayMessageReader reader(segments);
        auto tmp_reader = reader.getRoot<T>();
        tmp_mbuilder_ptr->setRoot(tmp_reader);
        ifs.close();
    }
    return tmp_mbuilder_ptr;
}

int main() {

    std::shared_ptr<capnp::MallocMessageBuilder> test0_mbuilder_ptr = std::make_shared<capnp::MallocMessageBuilder>();
    sample::Sample::Builder test0_builder = test0_mbuilder_ptr->initRoot<sample::Sample>();

    test0_builder.setIntegerSample(999);
    test0_builder.setStSample("hellow world!");    // std::string  or  char st[<length>]    char * st (no safe)
    int arr[4] = {0, 1, 2, 3};
    test0_builder.setArrSample(arr);
    capnp::List<int, capnp::Kind::PRIMITIVE>::Builder arr_builder = test0_builder.initArrSample(4);
    for(int i = 0; i < 4; i++) { arr_builder.set(i, i); }    // test0_builder.getArrSample().set(i,i);

    capnp_dump(test0_mbuilder_ptr, "test0.dat");

    std::shared_ptr<capnp::MallocMessageBuilder> test1_mbuilder_ptr = std::make_shared<capnp::MallocMessageBuilder>();
    test1_mbuilder_ptr->setRoot(test0_builder.asReader());
    sample::Sample::Builder test1_builder = test1_mbuilder_ptr->getRoot<sample::Sample>();
    sample::Sample::Reader test1_reader = test1_builder.asReader();

    int integer_sample = test1_reader.getIntegerSample();
    std::string st_sample(test1_reader.getStSample().begin(), test1_reader.getStSample().end());
    int arr_sample[4];
    for(int i = 0; i < 4; i++) { arr_sample[i] = test1_reader.getArrSample()[i]; }
    int enum_sample = sample::Sample::ENUM_SAMPLE;

    std::cout << "#### print test1 ####" << std::endl;
    std::cout << "integer_sample: " << integer_sample << std::endl;
    std::cout << "st_sample: " << st_sample << std::endl;
    std::cout << "arr_sample:";
    for(int i = 0; i < 4; i++) { std::cout << " " << arr_sample[i]; }
    std::cout << std::endl;
    std::cout << "enum_sample: " << enum_sample << std::endl << std::endl;

    std::shared_ptr<capnp::MallocMessageBuilder> test2_mbuilder_ptr = capnp_load_from_dump<sample::Sample>("test0.dat");
    sample::Sample::Builder test2_builder = test2_mbuilder_ptr->getRoot<sample::Sample>();
    std::string test2_flat(test2_builder.asReader().toString().flatten().cStr());
    int test2_size(test2_builder.asReader().toString().flatten().size());
    std::cout << "#### capnp load from dump ####" << std::endl;
    std::cout << "test2_flat: " << test2_flat << std::endl;
    std::cout << "test2_size: " << test2_size << std::endl << std::endl;

    std::shared_ptr<capnp::MallocMessageBuilder> test3_mbuilder_ptr = capnp_load_from_text<sample::Sample>("sample.dat");
    sample::Sample::Builder test3_builder = test3_mbuilder_ptr->getRoot<sample::Sample>();
    std::string test3_flat(test3_builder.asReader().toString().flatten().cStr());
    int test3_size(test3_builder.asReader().toString().flatten().size());
    std::cout << "#### capnp load from text ####" << std::endl;
    std::cout << "test3_flat: " << test3_flat << std::endl;
    std::cout << "test3_size: " << test3_size << std::endl;

    return 0;
}