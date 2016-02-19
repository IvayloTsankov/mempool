#include <cstdint>
#include <vector>
#include <cstring>
#include <ctime>
#include "../src/mempool.h"
using namespace mp;


enum class FRAME_TYPE
{
    AUDIO,
    UNKNOWN
};

class Frame
{
public:
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
    uint16_t buff[32000];
};

void usePool()
{
    std::stack<Frame*> s;
    MemPool<Frame> p;
    for(unsigned i = 0; i < 1000; i++)
    {
        for(unsigned j = 0; j < 1000; j++)
        {
            auto frame = p.create_raw(2, FRAME_TYPE::AUDIO);
            s.push(frame);
        }
        while(!s.empty())
        {
            auto frame = s.top();
            p.remove(frame);
            s.pop();
        }
    }
}

void usePoolShared()
{
    MemPool<Frame> p;
    std::stack<std::shared_ptr<Frame>> s;
    for(unsigned i = 0; i < 1000; i++)
    {
        for(unsigned j = 0; j < 1000; j++)
        {
            auto frame = p.create_shared(2, FRAME_TYPE::AUDIO);
            s.push(frame);
        }

        while(!s.empty())
        {
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
        printf("Usage: %s <pool|nopool|poolshared>\n", argv[0]);
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
    else if (strcmp(input, "poolshared") == 0)
    {
        usePoolShared();
    }
    else
    {
        printf("not implemented test for %s\n", input);
    }

    return (0);
}
