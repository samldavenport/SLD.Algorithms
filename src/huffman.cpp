#pragma once

#include "sld-algorithms.hpp"

namespace sld {

    static constexpr u32 HUFFMAN_MAX_SYMBOLS = 256;
    static constexpr u32 HUFFMAN_MAX_NODES   = (HUFFMAN_MAX_SYMBOLS * 2) - 1;

    struct huffman_tree;
    struct huffman_node;
    struct huffman_symbols;

    struct huffman_context {
        huffman_tree*    tree;
        huffman_symbols* symbol_array;
        byte*            data_ptr;
        u32              data_size;
    };

    struct huffman_tree {
        huffman_nodes node_array[HUFFMAN_MAX_NODES];
        s16           node_count;
        u16           node_root;
    };

    struct huffman_node {
        u32 frequency;
        u16 node_left;
        u16 node_right;
        u16 symbol;
    };

    struct huffman_symbol_table {
        u32  count;
        byte array_symbols   [HUFFMAN_MAX_SYMBOLS];
        u32  array_frequency [HUFFMAN_MAX_SYMBOLS];
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

    }

};