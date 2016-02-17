#ifndef MEMPOOL_SIMPLE_POOL_H_
#define MEMPOOL_SIMPLE_POOL_H_

#include <memory>
#include <stack>


template <typename T>
class SimplePool
{
public:
    SimplePool() {}

    // disable copy constructor
    SimplePool(const SimplePool& ) = delete;

    SimplePool(SimplePool& other)
    {
        this->pool_(std::move(other->pool_));
    }

    template<typename... Args>
    T* create(Args&& ...args)
    {
        void* space = this->alloc();
        T* object = new (space) T(std::forward<Args>(args)...);
        return object;
    }

    void remove(T* object)
    {
        object->~T();
        this->free(object);
    }

    ~SimplePool()
    {
        // free all the memory allocated for the pool
        while(!this->pool_.empty())
        {
            ::operator delete(this->pool_.top());
            this->pool_.pop(); 
        }
    }

protected:
    // first check for empty slot in pool
    // if we have return it
    // if we don't have create new slot and return it
    void* alloc()
    {
        void* slot;
        if (!this->pool_.empty())
        {
            slot = static_cast<void*>(this->pool_.top());
            this->pool_.pop();
        }
        else
        {
            slot = ::operator new (sizeof(T));
        }

        return slot;
    }

    // add free memory to pool
    void free(void* mem)
    {
        this->pool_.push(static_cast<T*>(mem));
    }


private:
    std::stack<T*> pool_;
};

#endif // MEMPOOL_SIMPLE_POOL_H_
