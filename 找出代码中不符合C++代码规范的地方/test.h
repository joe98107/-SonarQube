// 1. 缺失头文件保护宏 (Include Guards)
// 应该有 #ifndef test, #define test, #endif

#include <iostream>
#include <string>
#include <vector>

// 2. 在头文件中使用 using namespace std;
// 这会将所有std命名空间成员引入包含此头文件的所有文件中，可能导致命名冲突。
using namespace std;

// 3. 在头文件中定义全局变量
// 如果多个.cpp文件包含此头文件，会导致链接错误（重复定义）。
int global_var = 42;

// 4. 使用宏定义一个类似函数的表达式
// 这可能导致意想不到的副作用，应该优先使用内联函数。
#define SQUARE(x) (x * x)

class BadCodeStyle {
public:
    // 5. 公开的成员变量
    // 成员变量通常应该是私有的，通过公有方法访问，以实现封装。
    int publicMember;

    // 6. 函数实现在头文件中（非模板或非内联）
    // 大型函数的实现应该放在.cpp文件中，以减少编译时间和依赖。
    void printInfo() {
        std::cout << "This is a class with bad coding style." << std::endl;
    }

    // 7. 函数参数未使用const引用
    // 对于大的对象，应该使用const引用来避免不必要的拷贝。
    void processData(std::vector<int> data);
};