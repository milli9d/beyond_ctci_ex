#include <iostream>
#include <memory>

#include "freelist.hpp"
#include "circ_buf.hpp"
#include "buffer_pool.hpp"
#include "linked_list.hpp"

int main()
{
    // size_t buf_sz = sizeof(FreeList<uint32_t>::node_t) * 10u;
    // uint8_t* buf = (uint8_t*)malloc(buf_sz);

    // FreeList<uint32_t> _list(buf, buf_sz);
    // _list.print();
    // uint32_t* data = _list.list_alloc();

    // *data = 021;
    // _list.print();

    // _list.list_free(data);
    // _list.print();

    // free(buf);

    // RingBuf rb(5u);

    // rb.write(0x01);
    // rb.print();

    // uint8_t vals[] = { 10, 20, 30, 40, 50 };
    // rb.write(vals, sizeof(vals));
    // rb.print();

    // uint8_t read[10u] = {};
    // rb.read(read, 2u);
    // rb.print();

    BufferPool _pool(512u);

    _pool.alloc(32u);
    _pool.alloc(32u);
    void* data = _pool.alloc(64u);
    _pool.print();

    _pool.free(data);
    _pool.print();

    return 0;
}