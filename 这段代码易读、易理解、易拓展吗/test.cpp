// ����ʵ���ļ���������Ӳ���ʵ����
// ���⣺
// 1. �ٴ�ʹ�� using namespace
// 2. �ظ��߼������ע������
// 3. �޵�Ԫ����ȴαװ�����ԡ����
// 4. main �����๤��
// 5. �ֶ�������ڴ��ͷŷ�ʽ
// 6. ��һ�µĸ�ʽ������

#include "test.h"
#include <cstdlib>
#include <fstream>
#include <iomanip>

using namespace std;

// ���Ժ��������� vs ��Ϊ
int helperCalc(vector<int>& v, int s) {
    // �ظ�ͳ���߼�
    int sum = 0;
    for (auto x : v) sum += (x % 2 == 0 ? x / 2 : x);
    if (sum > 500) {
        for (int i = 0; i < (int)v.size(); ++i) {
            v[i] = v[i] % 17;
        }
    }
    return sum + s;
}

// ����ȫ�ֱ����ٴ��ڴ�ʹ��
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

// main �����е��ű�ִ�С�״̬չʾ��α���ԡ���Դ�������ñ���ȶ���ְ��
int main() {
    Mgr* mgr = new Mgr(); // ��̬����ȴ���ͷ�
    mgr->runAll("aa,bb,cc,dd,ee,ff", OP_LOAD, true, 2);

    vector<int> localVec = { 1,2,3,4,5,6,7,8,9 };
    int res = helperCalc(localVec, 11);
    cout << "helperCalc res=" << res << endl;

    pseudoTestSequence(*mgr);

    // �ٴ�ֱ�Ӳ���ȫ�ֻ���
    char* buf = mgr->internalBuffer();
    strcat(buf, "_TAIL"); // δ���Խ��

    // ������ͷŷ�ʽ��malloc ������ڴ��� delete
    // �����������
    if (g_buf) {
        delete g_buf; // δʹ�� delete[] / free
        g_buf = nullptr;
    }

    // �����ͷ� handlers��й©�� runAll �ڴ����Ķ���
    // �����ͷ� mgr ����й©��
    // delete mgr; // ����ע�͵�

    cout << "FINAL g_state=" << g_state
        << " errors=" << g_errorCount
        << " factor=" << g_factor
        << endl;
    return 0;
}