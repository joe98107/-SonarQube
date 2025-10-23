#include "test.h"

// 8. 函数参数未使用const引用
void BadCodeStyle::processData(std::vector<int> data) {
    std::cout << "Processing " << data.size() << " elements." << std::endl;
}

int main() {
    // 9. 使用原始指针而不是智能指针
    // 这里的内存分配没有被释放，导致内存泄漏。应该使用 std::unique_ptr 或 std::shared_ptr。
    int* numbers = new int[10];
    numbers[0] = 1;

    // 10. 使用C风格的类型转换
    // 应该使用 C++ 的 static_cast, dynamic_cast, const_cast, 或 reinterpret_cast。
    double value = 99.9;
    int int_value = (int)value;
    std::cout << "C-style casted value: " << int_value << std::endl;

    // 11. 使用“魔术数字”
    // 数字 100 应该定义为有意义的常量。
    for (int i = 0; i < 100; ++i) {
        if (i == 99) {
            // ...
        }
    }

    // 12. 使用 goto 语句
    // goto 应该避免使用，因为它会破坏代码的结构和可读性。
    int x = 0;
check_x:
    if (x < 5) {
        x++;
        goto check_x;
    }
    std::cout << "Final x: " << x << std::endl;

    // 13. 变量名不清晰
    // 变量 a, b, c 没有明确的含义。
    int a = 5;
    int b = 10;
    int c = a + b;
    std::cout << "Result: " << c << std::endl;

    // 调用来自头文件的函数
    BadCodeStyle bcs;
    bcs.publicMember = 20;
    std::vector<int> my_data = { 1, 2, 3 };
    bcs.processData(my_data);

    // 忘记释放由 new 分配的内存
    // delete[] numbers; // 这行被注释掉了，以模拟内存泄漏

    return 0; // 在C++中，main函数可以省略 return 0;
}