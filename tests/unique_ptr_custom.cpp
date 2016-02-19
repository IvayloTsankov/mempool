#include <memory>
#include <utility>
#include <functional>

struct Vector
{
    float x, y, z;
};

struct Del
{
    void del(Vector*)
    {
        printf("Call Del::del\n");
    }
};

int main()
{
    {
        std::unique_ptr<Vector, std::function<void(Vector*)>> v(new Vector{ 1, 1, 1 },
            [](Vector* p) { printf("custom deleter called\n"); });
    }

    {
        Del d;
        std::unique_ptr<Vector, std::function<void(Vector*)>> v(new Vector{ 2, 2, 2 },
            std::bind(&Del::del, d, std::placeholders::_1));
    }
    return 0;
}
