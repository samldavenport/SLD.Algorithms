#ifndef SLD_ALGORITHMS_INTERNAL_HPP
#define SLD_ALGORITHMS_INTERNAL_HPP

#include "sld-algorithms.hpp"

#define SLD_ALGORITHMS_API_FUNC

namespace sld {

    struct huffman_tree;
    struct huffman_node;
    struct huffman_frequency_table;

    static constexpr byte HUFFMAN_INVALID_SYMBOL    = 0xFF; 
    static constexpr u32  HUFFMAN_MAX_SYMBOLS       = 256;
    static constexpr u32  HUFFMAN_MAX_NODES         = (HUFFMAN_MAX_SYMBOLS * 2) - 1;
    static constexpr u32  HUFFMAN_STRUCT_CONTEXT    = sizeof(huffman_context);
    static constexpr u32  HUFFMAN_STRUCT_TREE       = sizeof(huffman_tree);
    static constexpr u32  HUFFMAN_STRUCT_FREQ_TABLE = sizeof(huffman_frequency_table);
    
    struct huffman_context {
        huffman_tree*            tree;
        huffman_frequency_table* freq_table;
        byte*                    data_ptr;
        u32                      data_size;
    };

    struct huffman_tree {
        huffman_nodes node_array[HUFFMAN_MAX_NODES];
        s16           node_count;
        u16           node_root;
    };

    struct huffman_tree_soa {
        u32 node_count;
        struct {
            u32  frequency  [HUFFMAN_MAX_NODES];
            u16  node_left  [HUFFMAN_MAX_NODES];
            u16  node_right [HUFFMAN_MAX_NODES];
            byte symbol     [HUFFMAN_MAX_NODES];
        } node_array;
    };

    struct huffman_node {
        u32 frequency;
        u16 node_left;
        u16 node_right;
        u16 symbol;
    }; 

    struct huffman_frequency_table {
        u32 sparse_array_frequencies[HUFFMAN_MAX_SYMBOLS];
        struct  {
            u32 count;
            u32 frequencies[HUFFMAN_MAX_SYMBOLS];
            u32 symbols    [HUFFMAN_MAX_SYMBOLS];
        } dense_array;
    };

};

#endif //SLD_ALGORITHMS_INTERNAL_HPP