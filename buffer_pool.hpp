#include <iostream>
#include <memory>
#include <mutex>

#include <cstdint>
#include <cstring>
#include <cmath>

// statically allocated buffer pool
// uses a linked list to manage nodes
// stores some metadata
class BufferPool
{
  public:
#pragma pack(push, 1)
    typedef struct BufferNode_t {
        struct BufferNode_t* next;
        size_t sz_n_valid;
        uint8_t data;

        void in_use(bool using_it)
        {
            if (using_it) {
                sz_n_valid |= (1 << 31u);
            } else {
                sz_n_valid &= ~(1 << 31u);
            }
        }

        bool in_use()
        { return sz_n_valid >> 31u; }

        size_t size()
        { return sz_n_valid & ~(1 << 31u); }

        void size(size_t sz)
        {
            size_t mask = (1u << 31u);
            sz_n_valid &= mask;
            sz_n_valid |= sz;
        }

    } BufferNode_t;
#pragma pack(pop)

  private:
    std::shared_ptr<uint8_t[]> _mem{ nullptr };
    std::mutex _mtx{};
    BufferNode_t* _head{ nullptr };
    size_t _sz{ 0u };

  public:
    void print()
    {
        std::unique_lock<std::mutex> _lk(_mtx);
        BufferNode_t* curr = _head;
        while (curr) {
            printf("Node: %p, Size: %zu, In Use: %s Next: %p\n", (void*)curr, curr->size(),
                   curr->in_use() ? "true" : "false", (void*)curr->next);
            for (size_t i = 0u; i < curr->size(); i++) {
                if (i && i % 16u == 0) {
                    printf("\n");
                }
                printf("%02X ", *(&curr->data + i));
            }
            printf("\n");
            curr = curr->next;
        }
        printf("\n");
    }

    // Allocate a buffer of size sz
    void* alloc(size_t sz)
    {
        std::unique_lock<std::mutex> _lk(_mtx);

        // find a node with enough space
        BufferNode_t* curr = _head;
        while (curr) {
            if (curr->size() > (sz + offsetof(BufferNode_t, data)) && !curr->in_use()) {
                break;
            }
            curr = curr->next;
        }
        if (!curr) {
            printf("Error: Out of memory!\n");
            return nullptr;
        }

        // split it;
        // take node + memory and have enough space for another node
        size_t og_size = curr->size();
        BufferNode_t* og_next = curr->next;
        curr->in_use(true);

        if ((og_size - sz) > sizeof(BufferNode_t)) {
            // make new node
            BufferNode_t* n_node = (BufferNode_t*)(&curr->data + sz);
            curr->size(sz);
            curr->next = n_node;
            n_node->in_use(false);
            n_node->size(og_size - offsetof(BufferNode_t, data) - sz);
            n_node->next = og_next;
        }

        // return data
        return (void*)&curr->data;
    }

    void free(void* data)
    {
        std::unique_lock<std::mutex> _lk(_mtx);

        BufferNode_t* curr = reinterpret_cast<BufferNode_t*>((uint8_t*)data - offsetof(BufferNode_t, data));
        curr->in_use(false);

        size_t sz = 0;
        BufferNode_t* ptr = curr;
        while (ptr && !ptr->in_use()) {
            sz += ptr->size();
            ptr = ptr->next;
        }
        curr->size(sz);
        curr->next = ptr;
        memset(&curr->data, 0u, curr->size());
    }

    void reset(void)
    {
        std::unique_lock<std::mutex> _lk(_mtx);

        // clear all memory
        memset(_mem.get(), 0u, _sz);

        // allocate a single node that occupies entire memory space
        _head = (BufferNode_t*)_mem.get();
        _head->next = nullptr;
        _head->size(_sz - offsetof(BufferNode_t, data));
        _head->in_use(false);
    }

    BufferPool& operator=(const BufferPool& buf)
    {
        this->_head = buf._head;
        this->_sz = buf._sz;
        this->_mem = buf._mem;
        return *this;
    }

    BufferPool(const BufferPool& buf)
    {
        this->_head = buf._head;
        this->_sz = buf._sz;
        this->_mem = buf._mem;
    };

    BufferPool(size_t sz) : _sz(sz)
    {
        if (_sz < sizeof(BufferNode_t)) {
            throw std::runtime_error("Invalid Size Request");
        }
        _mem = std::make_shared<uint8_t[]>(_sz);

        reset();
    }
};