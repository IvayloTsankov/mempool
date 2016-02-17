#include <cstdio>
#include <memory>


struct Vector
{
    Vector(float x, float y, float z): x_(x), y_(y), z_(z) {}
    ~Vector() { x_ = 0; y_ = 0; z_ = 0; }

    float x_, y_, z_;
};


int main(int argc, char* argv[])
{
    void* free_space = ::operator new(sizeof(Vector) * 2);
    Vector* v = new (free_space) Vector(5, 5, 5);

    printf("%f, %f, %f\n", v->x_, v->y_, v->z_);

    v->~Vector();

    ::operator delete(free_space);
   
    return (0);
}
