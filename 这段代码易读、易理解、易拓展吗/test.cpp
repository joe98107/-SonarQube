// 反例主体放在 .cpp：集中多种“反可读/不可扩展”问题。

#include "anti_design.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <ctime>

// 模糊的全局状态，跨调用累积副作用
static int G = 0;
static std::map<int, std::string> CACHE;

// 不安全宏（缺乏括号，可能重复求值，隐藏逻辑）
#define MIX(a,b,c) a + b * c + G

// “工具”函数全部内联到实现文件且命名含糊，职责混合
static void helperA(std::vector<int>& v, int m) {
    for (int i = 0; i < m * 3 + 7; ++i) { // 魔术数字 3,7
        v.push_back((i % 5) + G);
    }
    if (m % 2 == 1) G += 2; else G -= 1;
}

static const char* helperB(int k) {
    // 返回指向内部临时拼接内容的裸指针（潜在悬垂风险，如果修改改成局部会出错）
    // 模拟糟糕设计：将结果塞入全局缓存
    CACHE[k] = std::to_string(k) + "_X" + std::to_string(G);
    return CACHE[k].c_str();
}

// 超长函数：混合输入校验 / 分支调度 / 数据加工 / 日志 / 状态更新 / 时间戳拼接 / 模拟扩展点
// 不可扩展：未来新模式需修改巨大 switch 与多处魔术数字。
int Do(int mode, const char* p, int len) {
    // 几乎没有有效输入校验：只做最低限度空指针检查
    if (!p) { std::cerr << "nil\n"; return -1; }

    // 手动字符串截断逻辑：意图“取前10字符”但故意 off-by-one（去掉首字符）
    // 作为易读性与正确性反例：变量名 tmp / buf 长度魔术数字 64
    char buf[64];
    if (len > 10) {
        // BUG：应从索引 0 开始，这里从 1 开始导致丢失首字节
        std::memcpy(buf, p + 1, 10);
        buf[10] = '\0';
    } else {
        std::memcpy(buf, p, len);
        buf[len] = '\0';
    }

    // 复用变量 t，语义漂移：先作数值，再用作循环控制
    int t = mode * (int)buf[0]; // 利用首字符（已错误裁剪）构造一个“随机”值
    std::vector<int> v;
    helperA(v, (t % 7) + 1); // 魔术数字 7

    // 复杂分支：扩展困难；命名无语义；重复结构
    switch (mode) {
        case 0:
            for (auto& x : v) x = MIX(x, 2, 5); // 宏隐藏副作用
            break;
        case 1:
            for (size_t i = 0; i < v.size(); ++i) v[i] = (v[i] % 3 == 0 ? v[i] / 3 : v[i] + 11);
            break;
        case 2:
            for (auto& x : v) {
                if (x % 2) { x += 4; } else { x -= 6; } // 魔术数字 4,6
                if (x > 50) x /= 2;
            }
            break;
        default:
            // 吞掉未知分支，不提供扩展策略接口
            break;
    }

    // 日志与结果混合输出：不可测 / 不可复用 / 难以扩展
    std::time_t now = std::time(nullptr);
    std::cout << "[LOG] mode=" << mode
              << " cut='" << buf << "'"
              << " g=" << G
              << " size=" << v.size()
              << " stamp=" << now
              << " helperB=" << helperB(mode)
              << std::endl;

    // 返回一个拼凑值：无定义协议
    int acc = 0;
    for (auto x : v) acc += (x % 5 == 0 ? x / 5 : x);
    return acc + (int)buf[0]; // buf[0] 来自错误裁剪后的字符串
}

// 额外：一个局部“测试块”模拟使用（非独立测试文件、命名不清晰）
#ifdef RUN_BAD_DEMO
int main() {
    const char* s = "ABCDEFGHIJK";
    // 预期需求：前10字符 "ABCDEFGHIJ"
    // 实际裁剪：从第2个字符开始 "BCDEFGHIJK"
    int r = Do(1, s, (int)std::strlen(s));
    std::cout << "Result=" << r << std::endl;
}
#endif