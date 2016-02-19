#include <memory>
#include <stack>
#include <cstdio>


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


class Deleter
{
public:
    void operator() (Vector* v)
    {
    }

private:

};

class Pool
{
public:
    std::shared_ptr<Vector> create(float a, float b, float c)
    {
        printf("create object\n");
        Vector* v = NULL;

        if (!this->pool.empty())
        {
            printf("reuse memory\n");
            v = this->pool.top();
            this->pool.pop();
            v->x_ = a;
            v->y_ = b;
            v->z_ = c;
        }
        else
        {
            v = new Vector(1, 1, 1);
        }

        return std::shared_ptr<Vector>(v, std::bind(&Pool::free, this, std::placeholders::_1));
    }

    void free(Vector* v)
    {
        v->~Vector();
        printf("call free\n");
        this->pool.push(v);
    } 

private:
    std::stack<Vector*> pool;
};


int main()
{
    {
        printf("create normal shared_ptr\n");
        std::shared_ptr<Vector> v(new Vector(1, 1, 1));
    }

    {
        printf("create shared_ptr with pool deleter\n");
        Pool p;
        std::shared_ptr<Vector> v(new Vector(1, 1, 1), std::bind(&Pool::free, p, std::placeholders::_1));
    }

    Pool p;
    {
        printf("get shared_ptr from pool\n");
        std::shared_ptr<Vector> v = p.create(1, 1, 1);
    }

    {
        printf("reuse shared_ptr from pool\n");
        std::shared_ptr<Vector> v = p.create(1, 1, 1);
    }

    return (0);
}
