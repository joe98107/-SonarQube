// 反例实现文件：继续添加不良实践。
// 问题：
// 1. 再次使用 using namespace
// 2. 重复逻辑与过度注释噪音
// 3. 无单元测试却伪装“测试”输出
// 4. main 做过多工作
// 5. 手动错误的内存释放方式
// 6. 不一致的格式与缩进

#include "test.h"
#include <cstdlib>
#include <fstream>
#include <iomanip>

using namespace std;

// 误导性函数：名称 vs 行为
int helperCalc(vector<int>& v, int s) {
    // 重复统计逻辑
    int sum = 0;
    for (auto x : v) sum += (x % 2 == 0 ? x / 2 : x);
    if (sum > 500) {
        for (int i = 0; i < (int)v.size(); ++i) {
            v[i] = v[i] % 17;
        }
    }
    return sum + s;
}

// 大量全局变量再次在此使用
static int LOCAL_TRACK = 0;

void pseudoTestSequence(Mgr& m) {
    for (int i = 0; i < 4; ++i) {
        m.legacyDispatch(i);
        LOCAL_TRACK += i;
        if (LOCAL_TRACK % 2 == 1) {
            cout << "[track]" << LOCAL_TRACK << endl;
        }
    }
}

// main 函数承担脚本执行、状态展示、伪测试、资源管理、配置变更等多重职责
int main() {
    Mgr* mgr = new Mgr(); // 动态分配却晚释放
    mgr->runAll("aa,bb,cc,dd,ee,ff", OP_LOAD, true, 2);

    vector<int> localVec = { 1,2,3,4,5,6,7,8,9 };
    int res = helperCalc(localVec, 11);
    cout << "helperCalc res=" << res << endl;

    pseudoTestSequence(*mgr);

    // 再次直接操纵全局缓冲
    char* buf = mgr->internalBuffer();
    strcat(buf, "_TAIL"); // 未检查越界

    // 错误的释放方式：malloc 分配的内存用 delete
    // 故意制造风险
    if (g_buf) {
        delete g_buf; // 未使用 delete[] / free
        g_buf = nullptr;
    }

    // 忘记释放 handlers（泄漏在 runAll 内创建的对象）
    // 忘记释放 mgr 本身（泄漏）
    // delete mgr; // 故意注释掉

    cout << "FINAL g_state=" << g_state
        << " errors=" << g_errorCount
        << " factor=" << g_factor
        << endl;
    return 0;
}