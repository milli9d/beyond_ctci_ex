#include <iostream>
#include <memory>

template<typename T>
class LinkedList
{
  public:
    typedef struct node_t {
        struct node_t* next;
        T val;
    } node_t;

  private:
    std::shared_ptr<node_t> _head{ nullptr };
    std::shared_ptr<node_t> _tail{ nullptr };

  public:
    // print

    void print() {}

    // push_front
    // push_back
    // pop_front
    // pop_back

    //
};