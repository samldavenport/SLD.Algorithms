#ifndef SLD_ALGORITHMS_HPP
#define SLD_ALGORITHMS_HPP

#include "sld.hpp"

#ifdef SLD_ALGORITHMS_STATIC
#   define SLD_ALGORITHMS_API
#elif defined(SLD_ALGORITHMS_DLL_EXPORT)
#   define SLD_ALGORITHMS_API __declspec(dllexport)
#else
#   define SLD_ALGORITHMS_API __declspec(dllimport)
#endif

namespace sld {

    typedef s32 huffman_result; 

    struct huffman_context;
    struct data_stream {
        byte* data;
        u32   capacity;
        u32   size;
        u32   bits_used;
    };

    enum huffman_result_e {
        huffman_result_e_success         = 0,
        huffman_result_e_failure_general = -1
    };


    SLD_ALGORITHMS_API u32              huffman_context_memory_requirement (const u32 data_size);
    SLD_ALGORITHMS_API huffman_context* huffman_context_create             (const u32 data_size);
    SLD_ALGORITHMS_API huffman_context* huffman_context_memory_init        (const u32 data_size, const u32 mem_size, void* mem_ptr);
    SLD_ALGORITHMS_API huffman_result   huffman_compress                   (huffman_context* hffmn, const u32 data_size, const byte* data_ptr);
    SLD_ALGORITHMS_API u32              huffman_data_stream_memory_size    (const huffman_context* hffmn);
    SLD_ALGORITHMS_API data_stream*     huffman_data_stream_create         (const huffman_context* hffmn);
    SLD_ALGORITHMS_API data_stream*     huffman_data_stream_memory_init    (const u32 data_stream_size, const u32 mem_size, void* mem_ptr);
    SLD_ALGORITHMS_API void             huffman_copy_data_stream           (const huffman_context* hffmn, data_stream* ds);
};

#endif //SLD_ALGORITHMS_HPP