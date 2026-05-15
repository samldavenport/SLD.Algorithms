#pragma once

#include "sld-algorithms-internal.hpp"

namespace sld {

    SLD_ALGORITHMS_API_FUNC u32
    huffman_context_memory_requirement(
        const u32 data_size) {

        assert(data_size != 0);

        const u32 total_size = (
            data_size              +
            HUFFMAN_STRUCT_CONTEXT +
            HUFFMAN_STRUCT_TREE    +
            HUFFMAN_STRUCT_SYMBOL_TABLE
        );

        return(total_size);
    }

    SLD_ALGORITHMS_API_FUNC huffman_context*
    huffman_context_create(
        const u32 data_size) {

        assert(data_size != 0);

        const u32 mem_size = huffman_context_memory_requirement(data_size);
        void*     mem_ptr  = malloc(mem_size);
        assert(mem_ptr);

        huffman_context* hfmn = huffman_context_memory_init(
            data_size,
            mem_size,
            mem_ptr
        );
        assert(hfmn);        
    }
    
    SLD_ALGORITHMS_API_FUNC huffman_context*
    huffman_context_memory_init(
        const u32 data_size,
        const u32 mem_size,
        void*     mem_ptr) {

        assert(
            data_size != 0 &&
            mem_size  != 0 &&
            mem_ptr   != NULL
        );

        memset(mem_ptr, 0, mem_size);
        auto hfmn = (huffman_context*)mem_ptr;
        hfmn->tree       =            (huffman_tree*)(mem_ptr + HUFFMAN_STRUCT_CONTEXT);
        hfmn->freq_table = (huffman_frequency_table*)(((void*)hfmn->tree)         + HUFFMAN_STRUCT_TREE);
        hfmn->data_ptr   =                    (byte*)(((void*)hfmn->symbol_array) + HUFFMAN_STRUCT_SYMBOL_TABLE);
        hfmn->data_size  = data_size;

        return(huffman_context);
    }

    huffman_result
    huffman_compress(
        huffman_context* hffmn,
        const u32        data_size,
        const byte*      data_ptr) {

        assert(
            hffmn     != NULL &&
            data_size != 0    &&
            data_ptr  != NULL
        );

        // reset the symbol table and tree
        memset((void*)hffmn->freq_table, 0,   HUFFMAN_STRUCT_FREQ_TABLE);
        memset((void*)hffmn->tree,         0, HUFFMAN_STRUCT_TREE);

        // count the symbols
        huffman_frequency_table* freq_table = hfmn->freq_table;
        for (
            u32 index = 0;
                index < data_size;
              ++index) {

            const byte symbol = data_ptr[index];
            ++freq_table->sparse_array_frequencies[symbol];
        }

        // add the symbols to the dense array
        for (
            byte symbol = 0;
                 symbol < HUFFMAN_MAX_SYMBOLS;
               ++symbol) {

            const u32 freq = hffmn->freq_table->sparse_array_frequencies[symbol]; 
            if (freq != 0) {
                
                u32& dense_index = freq_table->dense_array.count;
                freq_table->dense_array.frequencies [dense_index] = freq;
                freq_table->dense_array.symbols     [dense_index] = symbol;
                ++dense_index
            }
        }
    }

    SLD_ALGORITHMS_API_FUNC u32                  huffman_data_stream_memory_size    (const huffman_context* hffmn);
    SLD_ALGORITHMS_API_FUNC data_stream*         huffman_data_stream_create         (const huffman_context* hffmn);
    SLD_ALGORITHMS_API_FUNC data_stream*         huffman_data_stream_memory_init    (const u32 data_stream_size, const u32 mem_size, void* mem_ptr);
    SLD_ALGORITHMS_API_FUNC void                 huffman_copy_data_stream           (const huffman_context* hffmn, data_stream* ds);


};