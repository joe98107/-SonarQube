// 1. ȱʧͷ�ļ������� (Include Guards)
// Ӧ���� #ifndef test, #define test, #endif

#include <iostream>
#include <string>
#include <vector>

// 2. ��ͷ�ļ���ʹ�� using namespace std;
// ��Ὣ����std�����ռ��Ա���������ͷ�ļ��������ļ��У����ܵ���������ͻ��
using namespace std;

// 3. ��ͷ�ļ��ж���ȫ�ֱ���
// ������.cpp�ļ�������ͷ�ļ����ᵼ�����Ӵ����ظ����壩��
int global_var = 42;

// 4. ʹ�ú궨��һ�����ƺ����ı��ʽ
// ����ܵ������벻���ĸ����ã�Ӧ������ʹ������������
#define SQUARE(x) (x * x)

class BadCodeStyle {
public:
    // 5. �����ĳ�Ա����
    // ��Ա����ͨ��Ӧ����˽�еģ�ͨ�����з������ʣ���ʵ�ַ�װ��
    int publicMember;

    // 6. ����ʵ����ͷ�ļ��У���ģ����������
    // ���ͺ�����ʵ��Ӧ�÷���.cpp�ļ��У��Լ��ٱ���ʱ���������
    void printInfo() {
        std::cout << "This is a class with bad coding style." << std::endl;
    }

    // 7. ��������δʹ��const����
    // ���ڴ�Ķ���Ӧ��ʹ��const���������ⲻ��Ҫ�Ŀ�����
    void processData(std::vector<int> data);
};