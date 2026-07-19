// A ring buffer is a sequence of memory that loops around at the end
// It can be used as a queue
// RingBuffers support bulk or byte operations
// Some ring buffers provide claim / release mechanisms
// Ideally data structures should be concurrency protected
#include <iostream>
#include <memory>
#include <span>

#include <cstring>
#include <cstdint>

class RingBuf
{
  private:
    size_t _cap{ 0u };

    size_t _sz{ 0u };
    std::shared_ptr<uint8_t[]> _buf{ nullptr };
    size_t _in{ 0u };
    size_t _out{ 0u };

  public:
    void print()
    {
        for (size_t i = 0; i < _cap; ++i) {
            printf("[%zu] : %02x", i, _buf[i]);
            if (i == _in) {
                printf(" < in");
            }
            if (i == _out) {
                printf(" < out");
            }
            printf("\n");
        }
    }

    void write(const uint8_t& val)
    {
        if (_sz + 1 > _cap) {
            printf("ERROR: Out of memory\n");
            return;
        }

        _buf[_in] = val;
        _in = (_in + 1) % _cap;
        _sz++;
    }

    void write(const uint8_t* vals, size_t count)
    {
        for (size_t i = 0u; i < count; i++) {
            _buf[_in] = vals[i];
            _in = (_in + 1) % _cap;
            _sz++;
        }
    }

    uint8_t read()
    {
        if (!_sz) {
            throw std::runtime_error("No memory to read");
        }

        uint8_t val = _buf[_out];
        _out = (_out + 1) % _cap;
        return val;
    }

    void read(uint8_t* vals, size_t count)
    {
        if (_sz < count) {
            return;
        }

        for (size_t i = 0u; i < count; i++) {
            vals[i] = _buf[_out++];
            _sz--;
        }
    }

    RingBuf(size_t sz) : _cap(sz)
    { _buf = std::make_shared<uint8_t[]>(_cap); }
};