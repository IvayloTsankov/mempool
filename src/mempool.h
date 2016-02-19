#ifndef MEMPOOL_MEM_POOL_H_
#define MEMPOOL_MEM_POOL_H_

#include <memory>
#include <stack>
#include <utility>


namespace mp
{

template <typename T>
class MemPool
{
public:
    MemPool() {}

    template<typename... Args>
    explicit MemPool(size_t size, Args&& ...args)
    {
        for(size_t i = 0; i < size; ++i)
        {
            this->pool_.push(static_cast<T*>(::operator new (sizeof(T))));
        }

#if defined (MPDEBUG)
            printf("Memory pool preallocated size: %zu\n", this->pool_.size());
#endif // (MPDEBUG)
    }

    // disable copy constructor
    MemPool(const MemPool& ) = delete;

    MemPool(MemPool& other)
    {
        this->pool_(std::move(other->pool_));
    }

    template<typename... Args>
    std::shared_ptr<T>
    create_shared(Args&& ...args)
    {
        void* space = this->alloc();
        std::shared_ptr<T> object(new (space) T(std::forward<Args>(args)...),
                                  std::bind(&MemPool::remove, this, std::placeholders::_1));
        return object;
    }

    template<typename... Argss>
    std::unique_ptr<T, std::function<void(T*)>>
    create_unique(Argss&& ...args)
    {
        void* space = this->alloc();
        std::unique_ptr<T, std::function<void(T*)>> object(new (space) T(args...),
            std::bind(&MemPool::remove, this, std::placeholders::_1));

        return object;
    }

    template<typename... Argss>
    T* create_raw(Argss&& ...args)
    {
        void* space = this->alloc();
        return new (space) T(args...);
    }

    void remove(T* object)
    {
        object->~T();
        this->free(object);
    }

    size_t free_size(void) const
    {
        return this->pool_.size();
    }

    void clear(void)
    {
        // free all the memory allocated for the pool
        while(!this->pool_.empty())
        {
            ::operator delete(this->pool_.top());
            this->pool_.pop(); 
        }
    }

    ~MemPool()
    {
        this->clear();
    }

public:
    // first check for empty slot in pool
    // if we have return it
    // if we don't have create new slot and return it
    void* alloc()
    {
        void* slot;
        if (!this->pool_.empty())
        {
#if defined (MPDEBUG)
            printf("reuse memory\n");
#endif // (MPDEBUG)

            slot = static_cast<void*>(this->pool_.top());
            this->pool_.pop();
        }
        else
        {
#if defined (MPDEBUG)
            printf("new memory\n");
#endif // (MPDEBUG)

            slot = ::operator new (sizeof(T));
        }

        return slot;
    }

    // add free memory to pool
    void free(void* mem)
    {
#if defined (MPDEBUG)
        printf("release memory to pool\n");
#endif // (MPDEBUG)

        this->pool_.push(static_cast<T*>(mem));
    }

private:
    std::stack<T*> pool_;
};

} // end of namespace mp

#endif // MEMPOOL_MEM_POOL_H_
