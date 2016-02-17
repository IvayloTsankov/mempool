#include <cstdint>
#include <cstring>
#include <ctime>
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


void testPool()
{
    SimplePool<Frame> pool;

    Frame* f = pool.create(2, FRAME_TYPE::AUDIO);
    printf("(num_channels: %d) (type: %d)\n", f->num_channels_, (int)f->type_);

    pool.remove(f); 
    printf("After destructor (num_channels: %d) (type: %d)\n", f->num_channels_, (int)f->type_);
}

void usePool()
{
    std::stack<Frame*> s;
    SimplePool<Frame> p;
    for(unsigned i = 0; i < 1000; i++)
    {
        for(unsigned j = 0; j < 1000; j++)
        {
            s.push(p.create(2, FRAME_TYPE::AUDIO));
        }
        while(!s.empty())
        {
            p.remove(s.top());
            s.pop();
        }
    }
}

void noPool()
{
    std::stack<Frame*> s;
    for(unsigned i = 0; i < 1000; i++)
    {
        for(unsigned j = 0; j < 1000; j++)
        {
            s.push(new Frame(2, FRAME_TYPE::AUDIO));
        }
        
        while(!s.empty())
        {
            delete s.top();
            s.pop();
        }
    }
}


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <pool|nopool>\n", argv[0]);
        exit(1);
    }

    char* input = argv[1];
    if (strcmp(input, "pool") == 0)
    {
        usePool();    
    }
    else if (strcmp(input, "nopool") == 0)
    {
        noPool();
    }

    return (0);
}
