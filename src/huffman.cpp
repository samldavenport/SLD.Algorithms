#pragma once

#include "sld-algorithms-internal.hpp"

namespace sld {

    //--------------------------------------------------------------------
    // API METHODS
    //--------------------------------------------------------------------

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

        zero_memory(mem_ptr, mem_size);
        
        auto hfmn = (huffman_context*)mem_ptr;
        hfmn->tree       =            (huffman_tree*)(mem_ptr                     + HUFFMAN_STRUCT_SIZE_CONTEXT);
        hfmn->heap       =            (huffman_heap*)(((void*)hfmn->heap)         + HUFFMAN_STRUCT_SIZE_TREE);
        hfmn->freq_table = (huffman_frequency_table*)(((void*)hfmn->tree)         + HUFFMAN_STRUCT_SIZE_HEAP);
        hfmn->data_ptr   =                    (byte*)(((void*)hfmn->symbol_array) + HUFFMAN_STRUCT_SIZE_FREQ_TABLE);
        hfmn->data_size  = data_size;

        return(hfmn);
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

        // build the frequency table
        huffman_frequency_table_build(
            hffmn->freq_table,
            data_size,
            data_ptr
        );

        // initialize the tree
        
    }



    SLD_ALGORITHMS_API_FUNC u32          huffman_data_stream_memory_size    (const huffman_context* hffmn);
    SLD_ALGORITHMS_API_FUNC data_stream* huffman_data_stream_create         (const huffman_context* hffmn);
    SLD_ALGORITHMS_API_FUNC data_stream* huffman_data_stream_memory_init    (const u32 data_stream_size, const u32 mem_size, void* mem_ptr);
    SLD_ALGORITHMS_API_FUNC void         huffman_copy_data_stream           (const huffman_context* hffmn, data_stream* ds);

    //--------------------------------------------------------------------
    // INTERNAL METHODS
    //--------------------------------------------------------------------

    SLD_ALGORITHMS_INTERNAL void
    huffman_frequency_table_build(
        huffman_frequency_table* table,
        const byte*              data_ptr,
        const u32                data_size) {

        assert(
            table     != NULL &&
            data_ptr  != NULL &&
            data_size != 0
        );

        memset((void*)table, 0, HUFFMAN_STRUCT_FREQ_TABLE);

        // count the symbols
        for (
            u32 index = 0;
                index < data_size;
              ++index) {

            const byte symbol = data_ptr[index];
            ++table->sparse_array_frequencies[symbol];
        }

        // add the symbols to the dense array
        for (
            u32 symbol = 0;
                symbol < HUFFMAN_MAX_SYMBOLS;
              ++symbol) {


            const u32 freq = table->sparse_array_frequencies[symbol];
            if (freq > 0) {
                u32& dense_index = table->dense_array.count;
                table->dense_array.frequencies [dense_index] = freq;
                table->dense_array.symbols     [dense_index] = symbol;
                ++dense_index;
            }
        }
    }
    
    SLD_ALGORITHMS_INTERNAL void
    huffman_tree_init(
        huffman_tree*                  tree,
        huffman_heap*                  heap,
        const huffman_frequency_table* table) {

        tree->node_count = 0;

        for (
            u32 index = 0;
                index < table->dense_array.count;
              ++index) {


            u32& node_index = tree->node_count++;
            tree->node_array.frequency  [node_index] = table->dense_array.frequencies [index];
            tree->node_array.symbol     [node_index] = table->dense_array.symbols     [index];
            tree->node_array.node_left  [node_index] = HUFFMAN_INVALID;
            tree->node_array.node_right [node_index] = HUFFMAN_INVALID;

            huffman_heap_push(heap, tree, node_index);
        }
    }

    SLD_ALGORITHMS_INTERNAL void
    huffman_heap_push(
        huffman_heap* heap,
        huffman_tree* tree,
        const u16     node_index) {

        u16 heap_index = heap->size++;
        
        heap->data[heap_index] = node_index;

        //TODO(SAM): need fixed upper bound    
        while (heap_index > 0) {

            u16 push = (heap_index - 1) >> 1;

            const u32* frequency_array = tree->node_array.frequency; 
            u16&       heap_data_p     = heap->data[push];
            u16&       heap_data_i     = heap->data[heap_index];
            
            if (frequency_array[heap_data_p] <= frequency_array[heap_data_i]) {
                break;
            }

            swap_u16(heap_data_p, heap_data_i);
            heap_data_i = heap_data_p;
        }
    }

    SLD_ALGORITHMS_INTERNAL void
    huffman_heap_pop(
        huffman_heap* heap,
        huffman_tree* tree) {

        assert(heap != NULL && tree != NULL);

        u16 min = heap->data[0];

        heap->data[0] = heap->data[--heap->size];

        u16 i = 0;

        while (1) {

            u16 left  = (i * 2) + 1;
            u16 right = (i * 2) + 2;
            u16 s     = i;

            const u32  heap_left       = heap->data [left]; 
            const u32  heap_right      = heap->data [right]; 
            const u32  heap_s          = heap->data [s]; 
            const u32* frequency       = tree->node_array.frequency;
            const u32  frequency_left  = frequency  [heap_left];
            const u32  frequency_right = frequency  [heap_right]; 
            const u32  frequency_s     = frequency  [heap_s]; 

            if (left  < heap->size && frequency_left  < frequency_s) s = left;
            if (right < heap->size && frequency_right < frequency_s) s = right;
            if (s == i)                                              break;

            swap_u16(heap->data[i], heap->data[s]);
            i = s;
        }

        return(min);
    }

};