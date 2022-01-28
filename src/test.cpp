#include <iostream>
#include <vector>
int main()
{
    std::vector<int*> vec;
    for (unsigned int i = 0; i < 8000000; ++i)
    {
        vec.push_back(new int(1));
    }
    std::cin.get();
    return 0;
}