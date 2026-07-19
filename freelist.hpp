// FreeList is a data structure that uses statically allocated nodes
// We allocate n nodes of equal size s.
// We can then get one node of size s from the head of the list.
// Every time we free a node, it gets appended to head.
// Empty list means no more memory
#include <iostream>
#include <memory>
#include <algorithm>
#include <string>
#include <cstring>

template<typename T>
class FreeList
{
  public:
    typedef struct node_t {
      public:
        struct node_t* next;
        T val;
    } node_t;

  private:
    node_t* _head{ nullptr };
    uint8_t* _buf{ nullptr };
    size_t _buf_sz{ 0u };

  public:
    void print()
    {
        node_t* curr = _head;
        int count = 0;
        while (curr) {
            std::cout << "#" << count++ << " Curr : " << curr << " Val: " << curr->val << " Next: " << curr->next
                      << std::endl;

            curr = curr->next;
        }
    }

    T* list_alloc()
    {
        if (!_head) {
            return nullptr;
        }

        node_t* to_give = _head;

        _head = _head->next;
        to_give->next = nullptr;
        return &to_give->val;
    }

    void list_free(T* val)
    {
        if (!val) {
            return;
        }

        // find node
        node_t* curr = (node_t*)((uintptr_t)val - offsetof(FreeList<T>::node_t, val));
        printf("To Free : %p %d\n", (void*)curr, (int)offsetof(FreeList<T>::node_t, val));

        curr->val = {};

        // swap node
        curr->next = _head;
        _head = curr;
    }

    void init_buffer()
    {
        memset(_buf, 0u, _buf_sz);

        // allocate all nodes; sequentially
        for (size_t i = 0u; i < _buf_sz; i += sizeof(node_t)) {
            node_t* curr = (node_t*)&_buf[i];
            // if we are going to run out of memory , then next should be nullptr
            if (i + sizeof(node_t) < _buf_sz) {
                curr->next = (node_t*)(&_buf[i] + sizeof(node_t));
            } else {
                curr->next = nullptr;
            }
        }
        // set head
        _head = (node_t*)_buf;
    }

    FreeList(uint8_t* buf, size_t buf_sz_bytes) : _buf(buf), _buf_sz(buf_sz_bytes)
    {
        if (!_buf) {
            throw std::runtime_error("Invalid buffer pointer.");
        }

        // we can ignore the padding
        if (_buf_sz < sizeof(node_t)) {
            throw std::runtime_error("Invalid buffer size.");
        }

        // init free list
        init_buffer();
    }
};