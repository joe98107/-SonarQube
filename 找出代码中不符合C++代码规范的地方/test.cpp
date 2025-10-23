#include "test.h"

// 8. ��������δʹ��const����
void BadCodeStyle::processData(std::vector<int> data) {
    std::cout << "Processing " << data.size() << " elements." << std::endl;
}

int main() {
    // 9. ʹ��ԭʼָ�����������ָ��
    // ������ڴ����û�б��ͷţ������ڴ�й©��Ӧ��ʹ�� std::unique_ptr �� std::shared_ptr��
    int* numbers = new int[10];
    numbers[0] = 1;

    // 10. ʹ��C��������ת��
    // Ӧ��ʹ�� C++ �� static_cast, dynamic_cast, const_cast, �� reinterpret_cast��
    double value = 99.9;
    int int_value = (int)value;
    std::cout << "C-style casted value: " << int_value << std::endl;

    // 11. ʹ�á�ħ�����֡�
    // ���� 100 Ӧ�ö���Ϊ������ĳ�����
    for (int i = 0; i < 100; ++i) {
        if (i == 99) {
            // ...
        }
    }

    // 12. ʹ�� goto ���
    // goto Ӧ�ñ���ʹ�ã���Ϊ�����ƻ�����Ľṹ�Ϳɶ��ԡ�
    int x = 0;
check_x:
    if (x < 5) {
        x++;
        goto check_x;
    }
    std::cout << "Final x: " << x << std::endl;

    // 13. ������������
    // ���� a, b, c û����ȷ�ĺ��塣
    int a = 5;
    int b = 10;
    int c = a + b;
    std::cout << "Result: " << c << std::endl;

    // ��������ͷ�ļ��ĺ���
    BadCodeStyle bcs;
    bcs.publicMember = 20;
    std::vector<int> my_data = { 1, 2, 3 };
    bcs.processData(my_data);

    // �����ͷ��� new ������ڴ�
    // delete[] numbers; // ���б�ע�͵��ˣ���ģ���ڴ�й©

    return 0; // ��C++�У�main��������ʡ�� return 0;
}