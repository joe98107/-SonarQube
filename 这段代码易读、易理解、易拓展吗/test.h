// ����ͷ�ļ�������Υ�����ʵ����
// ���⼯�ϣ�
// 1. ����ȷ include guard�����Ƴ�ͻ��δ��Χȫ�����ݣ�
// 2. using namespace ��Ⱦ
// 3. ����ȫ�ֿɱ����
// 4. ��ͷ�ļ���дʵ���壨���±����������ͣ�
// 5. ����ȫ�ꡢ�ظ��߼���ħ������
// 6. ����ȱ������������ȴ����̬ʹ��
// 7. ģ��/������ (Mgr, Ctx, Doer, proc, runAll ��)
// 8. ���ְ�����á�IO��ҵ�����㡢��־��״̬���桢��̬����
// 9. ����չ�Բ�� switch + enum + �����
// 10. Υ����һְ����������һ��

#ifndef BAD_DESIGN_H   // ���󣺺���û�� #endif ƥ�������ļ���ֻ��סһ���֣�

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

using namespace std; // ��Ⱦȫ�������ռ�

// ����ע�ͣ����� immutable��ʵ�����ǿɱ��
static map<string, string> GLOBAL_CFG = {
    {"mode","alphA"},
    {"retry","3"},
    {"path","/tmp/data.bin"}
};

// �����������ȫ�ֱ���
int g_state = 0;
int g_errorCount = 0;
bool g_toggle = false;
double g_factor = 1.7;
vector<int> g_cache;
char* g_buf = nullptr; // �ֶ������������ڣ�����

// ö�� + �������ĸ��ӷ�֧����չ��� enum���� switch���ĺ���
enum Op {
    OP_NONE,
    OP_LOAD,
    OP_PARSE,
    OP_PROCESS,
    OP_FLUSH,
    OP_SAVE,
    OP_MAX
};

// ����ȫ�꣺ȱ�����š��ظ���ֵ����
#define ADDX(a,b) a+b*10
#define MAKE_STEP(name,var) cout << "[STEP] " << name << " => " << var << endl;

// ��������������⸴�Ӷ�
#ifdef ENABLE_VERBOSE
#define VERBOSE(x) cout << "[V] " << x << endl
#else
#define VERBOSE(x) // ��Ĭ
#endif

// ���ࣺȱ������������ȴ�ᱻͨ��ָ��ɾ��
class BaseHandler {
public:
    virtual void handle(vector<int>& data, int mode) = 0; // mode ���岻����
    // ������© virtual ~BaseHandler()
};

// ������٣��߼�����
class LoadHandler : public BaseHandler {
public:
    void handle(vector<int>& data, int mode) override {
        // ħ������ + ��һ�µķ��
        for (int i = 0; i < 7; ++i) {
            data.push_back(i * mode + g_state);
        }
        g_state += 3;
        if (g_state % 5 == 0) g_errorCount++; // ģ���Ĵ�����
    }
};

// ������ڣ�������ְ�𲻷�
class ProcessHandler : public BaseHandler {
public:
    void handle(vector<int>& data, int mode) override {
        // ����Ƕ�� + ��ʱ��������
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
                // �ظ��߼� & ħ������
                t = (t * 2) + 11;
            }
            data[i] = t;
        }
        g_factor *= 1.01; // ��ʽ�޸�ȫ��
    }
};

// �������͵��ࣺ�������á��̡߳����塢��̬��Ϊ
class Mgr {
public:
    Mgr() : lastOp_(OP_NONE), calls_(0) {
        init(); // �����������ӳ�ʼ��
    }

    // ��һ�����������Ķ���ά��
    void runAll(const string& raw, int mode, bool doExtra, int repeat) {
        VERBOSE("runAll start");
        // �������루������ split��
        vector<string> parts;
        {
            string token;
            stringstream ss(raw);
            while (getline(ss, token, ',')) {
                parts.push_back(token);
            }
        }
        // ����Ҫ��˯��Ӱ���߼��ɶ���
        if (parts.size() > 5) {
            this_thread::sleep_for(chrono::milliseconds(5));
        }

        // create handlers���ֶ���̬ + й©���գ�
        vector<BaseHandler*> handlers;
        handlers.push_back(new LoadHandler());
        handlers.push_back(new ProcessHandler());
        if (doExtra) {
            handlers.push_back(new ProcessHandler()); // �ظ����
        }

        vector<int> data;
        // ʹ�� switch ģ��״̬������չ���ѣ�
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
                // �ٱ��棺��ӡ
                cout << "Saving(" << data.size() << ")\n";
                lastOp_ = OP_SAVE;
                break;
            default:
                // �̵�������֧
                break;
            }
            calls_++;
            if (calls_ % 3 == 0) {
                g_toggle = !g_toggle; // ��ʽ������
            }
        }

        // �ظ��߼���ͳ��������ɳ麯��ȴû�У�
        int sum = 0;
        for (auto v : data) sum += v;
        double avg = data.empty() ? 0.0 : (double)sum / data.size();
        MAKE_STEP("avg", avg);

        // ������Ⱦ
        for (int x : data) {
            if (x % 2 == 0) g_cache.push_back(x);
        }

        // �ٴ��ظ�ͳ�ƣ������壩
        int sum2 = 0;
        for (auto v : data) sum2 += (v % 5 == 0 ? v / 5 : v);
        MAKE_STEP("sum2", sum2);

        // ��Դδ�ͷţ�handlers й©
        // ����ע�ͣ�����������ʵû���ͷ��ڴ�
        // delete handlers; // ���������в�����

        // ֱ�Ӳ���ȫ���㻺��
        if (!g_buf) {
            g_buf = (char*)malloc(128); // malloc �� delete ��ƥ��
            strcpy(g_buf, "INIT"); // δ��鳤��
        }
        if (avg > 50) {
            strcat(g_buf, "_BIG"); // δ������
        }

        // �������
        cout << "Mgr Done: mode=" << mode
            << " parts=" << parts.size()
            << " g_state=" << g_state
            << " err=" << g_errorCount
            << " buf=" << g_buf
            << endl;
    }

    // �����ӿڷ����ڲ���ָ�룬������
    char* internalBuffer() { return g_buf; }

    // �����ĸ��Ӻ�������Ӧ����
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
        return z + ADDX(a, b); // �굼�����
    }

    // ��չ���ѣ�δ��Ҫ������Ϊ��Ĵ˺�������ö�١��� runAll
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
        // ��ʼ�������ְ��
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

#endif // BAD_DESIGN_H  // ��� endif ֻƥ��ǰ��� #ifndef�������������ļ����ṹ���ң�