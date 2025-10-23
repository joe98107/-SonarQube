#include "risky_code.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono> // For std::this_thread::sleep_for

RiskAnalyzer::RiskAnalyzer(const std::string& baseDir) : baseUploadDir(baseDir) {}

bool RiskAnalyzer::processFile(const std::string& userInput) {
    // ��ȫ���գ�·��ƴ��ǰδ�� userInput �����κξ�������֤
    // ����һ�����͵�·������©�� (Path Traversal)
    std::string fullPath = baseUploadDir + "/" + userInput;
    std::cout << "Attempting to process file: " << fullPath << std::endl;

    std::ifstream file(fullPath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << fullPath << std::endl;
        return false;
    }

    // ��ȡ�ļ�����...
    std::cout << "Successfully opened file." << std::endl;
    // ...
    file.close();
    return true;
}

void RiskAnalyzer::internalProcessing(const char* buffer) {
    if (buffer == nullptr) {
        // ģ��һ������ʧ�ܵĳ���
        throw std::invalid_argument("Received null buffer");
    }
    std::cout << "Internal processing successful." << std::endl;
}

void RiskAnalyzer::processUserData(const char* data, size_t size) {
    // Ǳ���ڴ�й©���� internalProcessing �׳��쳣ʱ��delete[] �����ᱻִ��
    char* buffer = new char[size];
    std::copy(data, data + size, buffer);

    try {
        internalProcessing(buffer);
        // ֻ���� internalProcessing �ɹ�ʱ���ڴ�Żᱻ�ͷ�
        std::cout << "Releasing memory for buffer." << std::endl;
        delete[] buffer;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Caught exception: " << e.what() << ". Memory was not released!" << std::endl;
        // �쳣�����֧�������ͷ��ڴ�
    }
}

void RiskAnalyzer::launchBackgroundJob() {
    // Ǳ��BUG���������̶߳����ں�������ʱ�����١�
    // ������֮ǰ���̱߳��뱻 join() (�ȴ������) �� detach() (�������������)��
    // ������߶�û�У������� std::thread �����������л���� std::terminate()�����³��������
    std::thread worker([]() {
        std::cout << "Background job started..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Background job finished." << std::endl;
        });

    std::cout << "launchBackgroundJob function is returning." << std::endl;
    // �˴� worker ���󼴽��뿪�����򲢱����٣��Ӷ���������
}


int main() {
    RiskAnalyzer analyzer("uploads");
    
    // 1. ��ʾ�ļ���ȫ����
    std::cout << "--- 1. Testing File Path Traversal ---" << std::endl;
    // ���Է���һ���Ϸ��ļ�
    analyzer.processFile("safe_document.txt");
    // ���Խ���·���������� (��Linux/macOS��)
    analyzer.processFile("../../../../../etc/hosts");
    std::cout << std::endl;


    // 2. ��ʾ�ڴ�й©����
    std::cout << "--- 2. Testing Memory Leak on Exception ---" << std::endl;
    const char* valid_data = "some data";
    analyzer.processUserData(valid_data, 9); // ����·������й©
    analyzer.processUserData(nullptr, 0);      // �쳣·��������й©
    std::cout << "Continuing after potential memory leak..." << std::endl;
    std::cout << std::endl;


    // 3. ��ʾ�̹߳��������µı��� (Ϊ��ֹ����ֱ���˳������˲���ע��)
    std::cout << "--- 3. Testing Thread Management (CRASH) ---" << std::endl;
    // std::cout << "The next call will likely crash the program." << std::endl;
    // analyzer.launchBackgroundJob();
    std::cout << "If this line is printed, the thread was managed correctly (e.g., by joining)." << std::endl;

    return 0;
}