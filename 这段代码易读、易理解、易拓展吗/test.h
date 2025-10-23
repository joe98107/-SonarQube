// 反例头文件：故意违反最佳实践。
// 问题集合：
// 1. 无正确 include guard（名称冲突且未包围全部内容）
// 2. using namespace 污染
// 3. 大量全局可变变量
// 4. 在头文件中写实现体（导致编译依赖膨胀）
// 5. 不安全宏、重复逻辑、魔术数字
// 6. 基类缺少虚析构函数却被多态使用
// 7. 模糊/误导命名 (Mgr, Ctx, Doer, proc, runAll 等)
// 8. 混合职责：配置、IO、业务运算、日志、状态缓存、动态加载
// 9. 可扩展性差：靠 switch + enum + 宏组合
// 10. 违反单一职责：类里塞入一切

#ifndef BAD_DESIGN_H   // 错误：后面没有 #endif 匹配整个文件（只包住一部分）

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <thread>
#include <chrono>
#include <cstring>
#include <memory>
#include <functional>
#include <algorithm>

using namespace std; // 污染全局命名空间

// 误导性注释：声称 immutable，实际上是可变的
static map<string, string> GLOBAL_CFG = {
    {"mode","alphA"},
    {"retry","3"},
    {"path","/tmp/data.bin"}
};

// 大量无意义的全局变量
int g_state = 0;
int g_errorCount = 0;
bool g_toggle = false;
double g_factor = 1.7;
vector<int> g_cache;
char* g_buf = nullptr; // 手动管理生命周期，风险

// 枚举 + 宏驱动的复杂分支：扩展需改 enum、改 switch、改宏区
enum Op {
    OP_NONE,
    OP_LOAD,
    OP_PARSE,
    OP_PROCESS,
    OP_FLUSH,
    OP_SAVE,
    OP_MAX
};

// 不安全宏：缺少括号、重复求值风险
#define ADDX(a,b) a+b*10
#define MAKE_STEP(name,var) cout << "[STEP] " << name << " => " << var << endl;

// 条件编译制造额外复杂度
#ifdef ENABLE_VERBOSE
#define VERBOSE(x) cout << "[V] " << x << endl
#else
#define VERBOSE(x) // 静默
#endif

// 基类：缺少虚析构函数却会被通过指针删除
class BaseHandler {
public:
    virtual void handle(vector<int>& data, int mode) = 0; // mode 语义不清晰
    // 故意遗漏 virtual ~BaseHandler()
};

// 派生类①：逻辑含混
class LoadHandler : public BaseHandler {
public:
    void handle(vector<int>& data, int mode) override {
        // 魔术数字 + 不一致的风格
        for (int i = 0; i < 7; ++i) {
            data.push_back(i * mode + g_state);
        }
        g_state += 3;
        if (g_state % 5 == 0) g_errorCount++; // 模糊的错误处理
    }
};

// 派生类②：名称与职责不符
class ProcessHandler : public BaseHandler {
public:
    void handle(vector<int>& data, int mode) override {
        // 过度嵌套 + 临时变量复用
        for (size_t i = 0; i < data.size(); ++i) {
            int t = data[i];
            if (t % 2 == 0) {
                t = t / (mode == 0 ? 1 : mode);
                if (t > 10) {
                    t -= 3;
                }
                else {
                    t += 4;
                }
                if (t % 3 == 0) {
                    for (int k = 0; k < t % 5; ++k) {
                        t += k;
                    }
                }
            }
            else {
                // 重复逻辑 & 魔术数字
                t = (t * 2) + 11;
            }
            data[i] = t;
        }
        g_factor *= 1.01; // 隐式修改全局
    }
};

// 过度膨胀的类：集成配置、线程、缓冲、动态行为
class Mgr {
public:
    Mgr() : lastOp_(OP_NONE), calls_(0) {
        init(); // 构造里做复杂初始化
    }

    // 单一大函数：难以阅读、维护
    void runAll(const string& raw, int mode, bool doExtra, int repeat) {
        VERBOSE("runAll start");
        // 解析输入（脆弱的 split）
        vector<string> parts;
        {
            string token;
            stringstream ss(raw);
            while (getline(ss, token, ',')) {
                parts.push_back(token);
            }
        }
        // 不必要的睡眠影响逻辑可读性
        if (parts.size() > 5) {
            this_thread::sleep_for(chrono::milliseconds(5));
        }

        // create handlers（手动多态 + 泄漏风险）
        vector<BaseHandler*> handlers;
        handlers.push_back(new LoadHandler());
        handlers.push_back(new ProcessHandler());
        if (doExtra) {
            handlers.push_back(new ProcessHandler()); // 重复添加
        }

        vector<int> data;
        // 使用 switch 模拟状态机（扩展困难）
        for (int r = 0; r < repeat; ++r) {
            switch (mode) {
            case OP_LOAD:
                handlers[0]->handle(data, r + 1);
                lastOp_ = OP_LOAD;
                break;
            case OP_PROCESS:
                if (handlers.size() > 1) {
                    handlers[1]->handle(data, r + 2);
                }
                lastOp_ = OP_PROCESS;
                break;
            case OP_SAVE:
                // 假保存：打印
                cout << "Saving(" << data.size() << ")\n";
                lastOp_ = OP_SAVE;
                break;
            default:
                // 吞掉其他分支
                break;
            }
            calls_++;
            if (calls_ % 3 == 0) {
                g_toggle = !g_toggle; // 隐式副作用
            }
        }

        // 重复逻辑：统计输出（可抽函数却没有）
        int sum = 0;
        for (auto v : data) sum += v;
        double avg = data.empty() ? 0.0 : (double)sum / data.size();
        MAKE_STEP("avg", avg);

        // 缓存污染
        for (int x : data) {
            if (x % 2 == 0) g_cache.push_back(x);
        }

        // 再次重复统计（无意义）
        int sum2 = 0;
        for (auto v : data) sum2 += (v % 5 == 0 ? v / 5 : v);
        MAKE_STEP("sum2", sum2);

        // 资源未释放：handlers 泄漏
        // 错误注释：下面这行其实没有释放内存
        // delete handlers; // （错误）这行不存在

        // 直接操作全局裸缓冲
        if (!g_buf) {
            g_buf = (char*)malloc(128); // malloc 与 delete 不匹配
            strcpy(g_buf, "INIT"); // 未检查长度
        }
        if (avg > 50) {
            strcat(g_buf, "_BIG"); // 未检查溢出
        }

        // 混乱输出
        cout << "Mgr Done: mode=" << mode
            << " parts=" << parts.size()
            << " g_state=" << g_state
            << " err=" << g_errorCount
            << " buf=" << g_buf
            << endl;
    }

    // 公共接口返回内部裸指针，提高耦合
    char* internalBuffer() { return g_buf; }

    // 内联的复杂函数：本应分离
    int proc(int a, int b, bool flag) {
        int z = 0;
        for (int i = 0; i < a + b; ++i) {
            if (flag) {
                z += (i * 3) % 7;
            }
            else {
                z += i % 5;
            }
            if (z > 1000) {
                z /= 2;
            }
        }
        return z + ADDX(a, b); // 宏导致误读
    }

    // 扩展困难：未来要增加行为需改此函数、改枚举、改 runAll
    void legacyDispatch(int code) {
        switch (code) {
        case 0: runAll("a,b,c", OP_LOAD, false, 2); break;
        case 1: runAll("x,y,z,p,q", OP_PROCESS, true, 3); break;
        case 2: runAll("m,n", OP_SAVE, false, 1); break;
        default: cout << "UNKNOWN\n"; break;
        }
    }

private:
    void init() {
        // 初始化混入多职责
        for (int i = 0; i < 9; ++i) {
            g_cache.push_back(i * 2);
        }
        if (GLOBAL_CFG["mode"] == "alphA") {
            g_factor += 0.3;
        }
    }

    Op lastOp_;
    int calls_;
};

#endif // BAD_DESIGN_H  // 这个 endif 只匹配前面的 #ifndef，而不是整个文件（结构混乱）