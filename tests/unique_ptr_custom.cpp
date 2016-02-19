#include <memory>
#include <utility>
#include <functional>

struct Vector
{
    float x, y, z;
};

int main()
{
    {
        std::unique_ptr<Vector, std::function<void(Vector*)>> v(new Vector{ 1, 1, 1 },
            [](Vector* p) { printf("custom deleter called\n"); });
    }
    return 0;
}

