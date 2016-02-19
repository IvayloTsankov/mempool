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

    // disable copy constructor
    MemPool(const MemPool& ) = delete;

    MemPool(MemPool& other)
    {
        this->pool_(std::move(other->pool_));
    }

    template<typename... Args>
    std::shared_ptr<T> create_shared(Args&& ...args)
    {
        printf("creatd_shared called\n");

        void* space = this->alloc();
        std::shared_ptr<T> object(new (space) T(std::forward<Args>(args)...),
                                  std::bind(&MemPool::remove, this, std::placeholders::_1));
        return object;
    }

    template<typename... Argss>
    std::unique_ptr<T, std::function<void(T*)>> create_unique(Argss&& ...args)
    {
        printf("create_unique called\n");

        void* space = this->alloc();
        std::unique_ptr<T, std::function<void(T*)>> object(new (space) T(args...),
            std::bind(&MemPool::remove, this, std::placeholders::_1));

        return object;
    }


    void remove(T* object)
    {
        printf("remove called\n");

        object->~T();
        this->free(object);
    }

    ~MemPool()
    {
        // free all the memory allocated for the pool
        while(!this->pool_.empty())
        {
            ::operator delete(this->pool_.top());
            this->pool_.pop(); 
        }
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
            printf("reuse called\n");
            slot = static_cast<void*>(this->pool_.top());
            this->pool_.pop();
        }
        else
        {
            printf("new called\n");
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

} // end of namespace mp

#endif // MEMPOOL_MEM_POOL_H_
