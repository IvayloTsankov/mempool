#include "../src/mempool.h"
#include <cstdio>
using namespace mp;


class Vector
{
public:
    Vector(float x, float y, float z): x_(x), y_(y), z_(z)
    {
        printf("Vector ctor\n");
    }

    virtual ~Vector()
    {
        x_ = 0;
        y_ = 0;
        z_ = 0;
        printf("~Vector\n");
    }

    float x_, y_, z_;
};


void TestUsage()
{
    printf("test mempool\n");
    MemPool<Vector> p;
    auto v = p.create(1, 1, 1);
    printf("v(%f, %f, %f)\n", v->x_, v->y_, v->z_);
}

void TestPointerAlloc()
{
    printf("2 pointers from mempool\n");
    MemPool<Vector> p;
    auto v = p.create(1, 1, 1);
    printf("v(%f, %f, %f)\n", v->x_, v->y_, v->z_);

    auto vv = p.create(1, 1, 1);
    printf("v(%f, %f, %f)\n", vv->x_, vv->y_, vv->z_);
}

void TestMemoryReuse()
{
    printf("reuse memory from mempool\n");
    MemPool<Vector> p;

    {
        auto v = p.create(1, 1, 1);
        printf("v(%f, %f, %f)\n", v->x_, v->y_, v->z_);
    }

    auto vv = p.create(1, 1, 1);
    printf("v(%f, %f, %f)\n", vv->x_, vv->y_, vv->z_);
}

int main()
{
    TestUsage();
    TestPointerAlloc();
    TestMemoryReuse();

    return (0);
}
