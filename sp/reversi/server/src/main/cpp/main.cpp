#include <iostream>

void increase(int& a){
    a = 10;
}

int main() {
    int a = 0;
    increase(a);
    std::cout << a << std::endl;
    return 0;
}
