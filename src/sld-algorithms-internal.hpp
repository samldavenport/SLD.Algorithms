#ifndef SLD_ALGORITHMS_INTERNAL_HPP
#define SLD_ALGORITHMS_INTERNAL_HPP

#include "sld-algorithms.hpp"

#define SLD_ALGORITHMS_API_FUNC
#define SLD_ALGORITHMS_INTERNAL static

namespace sld {

    struct huffman_tree;
    struct huffman_node;
    struct huffman_frequency_table;
    struct huffman_heap;

    static constexpr u16 HUFFMAN_INVALID                = 0xFFFF; 
    static constexpr u16 HUFFMAN_MAX_SYMBOLS            = 256;
    static constexpr u16 HUFFMAN_MAX_NODES              = (HUFFMAN_MAX_SYMBOLS * 2) - 1;
    static constexpr u32 HUFFMAN_STRUCT_SIZE_CONTEXT    = sizeof(huffman_context);
    static constexpr u32 HUFFMAN_STRUCT_SIZE_TREE       = sizeof(huffman_tree);
    static constexpr u32 HUFFMAN_STRUCT_SIZE_HEAP       = sizeof(huffman_heap);
    static constexpr u32 HUFFMAN_STRUCT_SIZE_FREQ_TABLE = sizeof(huffman_frequency_table);
    
    struct huffman_context {
        huffman_tree*            tree;
        huffman_heap*            heap;
        huffman_frequency_table* freq_table;
        byte*                    data_ptr;
        u32                      data_size;
    };

    struct huffman_tree {
        u16 node_count;
        u16 root;
        struct {
            u32 frequency  [HUFFMAN_MAX_NODES];
            u16 symbol     [HUFFMAN_MAX_NODES];
            u16 node_left  [HUFFMAN_MAX_NODES];
            u16 node_right [HUFFMAN_MAX_NODES];
        } node_array;
    };

    struct huffman_heap {
        u16 data[HUFFMAN_MAX_SYMBOLS];
        u16 size;
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

    void huffman_frequency_table_build (huffman_frequency_table* table, const byte* data_ptr, const u32 data_size);
   
    void huffman_tree_init             (huffman_tree* tree, huffman_heap* heap, const huffman_frequency_table* table);
    void huffman_tree_merge_nodes      (huffman_tree* tree, huffman_heap* heap);
   
    void huffman_heap_push             (huffman_heap* heap, huffman_tree* tree, const u16 node_index);
    u16  huffman_heap_pop              (huffman_heap* heap, huffman_tree* tree);
};

#endif //SLD_ALGORITHMS_INTERNAL_HPP