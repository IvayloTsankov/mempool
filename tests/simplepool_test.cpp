#include <cstdint>
#include "../src/simplepool.h"


enum class FRAME_TYPE
{
    AUDIO,
    UNKNOWN
};

struct Frame
{
    Frame(int num_ch, FRAME_TYPE type)
    : num_channels_(num_ch),
      type_(type)
    {
    
    }

    ~Frame()
    {
        num_channels_ = 0;
        type_ = FRAME_TYPE::UNKNOWN;
    }

    int num_channels_;
    FRAME_TYPE type_;
    uint16_t buff[3000];
};


int main(int argc, char* argv[])
{
    SimplePool<Frame> pool;

    void* space = pool.alloc(); 
    Frame* f = new(space) Frame(2, FRAME_TYPE::AUDIO);

    printf("(num_channels: %d) (type: %d)\n", f->num_channels_, (int)f->type_);

    f->~Frame();
    printf("After destructor (num_channels: %d) (type: %d)\n", f->num_channels_, (int)f->type_);

    pool.free(f);
}
