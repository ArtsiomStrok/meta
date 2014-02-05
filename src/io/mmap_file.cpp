/**
 * @file mmap_file.cpp
 */

#include "io/mmap_file.h"
#include "util/common.h"

namespace meta {
namespace io {

mmap_file::mmap_file(const std::string & path):
    _path{path},
    _start{nullptr},
    _size{common::file_size(path)}
{
     _file_descriptor = open(_path.c_str(), O_RDONLY);
    if(_file_descriptor < 0)
        throw mmap_file_exception{"error obtaining file descriptor for "
            + _path};

    _start = (char*) mmap(nullptr, _size, PROT_READ, MAP_SHARED,
                          _file_descriptor, 0);
    if(_start == nullptr)
    {
        close(_file_descriptor);
        throw mmap_file_exception("error memory-mapping " + _path);
    }
}

uint64_t mmap_file::size() const
{
    return _size;
}

std::string mmap_file::path() const
{
    return _path;
}

char* mmap_file::start() const
{
    return _start;
}

mmap_file::~mmap_file()
{
    if(_start != nullptr)
    {
        munmap(_start, _size);
        close(_file_descriptor);
    }
}

}
}