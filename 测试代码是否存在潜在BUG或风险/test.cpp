#include "risky_code.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono> // For std::this_thread::sleep_for

RiskAnalyzer::RiskAnalyzer(const std::string& baseDir) : baseUploadDir(baseDir) {}

bool RiskAnalyzer::processFile(const std::string& userInput) {
    // 安全风险：路径拼接前未对 userInput 进行任何净化或验证
    // 这是一个典型的路径遍历漏洞 (Path Traversal)
    std::string fullPath = baseUploadDir + "/" + userInput;
    std::cout << "Attempting to process file: " << fullPath << std::endl;

    std::ifstream file(fullPath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << fullPath << std::endl;
        return false;
    }

    // 读取文件内容...
    std::cout << "Successfully opened file." << std::endl;
    // ...
    file.close();
    return true;
}

void RiskAnalyzer::internalProcessing(const char* buffer) {
    if (buffer == nullptr) {
        // 模拟一个处理失败的场景
        throw std::invalid_argument("Received null buffer");
    }
    std::cout << "Internal processing successful." << std::endl;
}

void RiskAnalyzer::processUserData(const char* data, size_t size) {
    // 潜在内存泄漏：当 internalProcessing 抛出异常时，delete[] 将不会被执行
    char* buffer = new char[size];
    std::copy(data, data + size, buffer);

    try {
        internalProcessing(buffer);
        // 只有在 internalProcessing 成功时，内存才会被释放
        std::cout << "Releasing memory for buffer." << std::endl;
        delete[] buffer;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Caught exception: " << e.what() << ". Memory was not released!" << std::endl;
        // 异常处理分支中忘记释放内存
    }
}

void RiskAnalyzer::launchBackgroundJob() {
    // 潜在BUG：创建的线程对象在函数结束时被销毁。
    // 在销毁之前，线程必须被 join() (等待其完成) 或 detach() (允许其独立运行)。
    // 如果两者都没有，程序在 std::thread 的析构函数中会调用 std::terminate()，导致程序崩溃。
    std::thread worker([]() {
        std::cout << "Background job started..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Background job finished." << std::endl;
        });

    std::cout << "launchBackgroundJob function is returning." << std::endl;
    // 此处 worker 对象即将离开作用域并被销毁，从而触发崩溃
}


int main() {
    RiskAnalyzer analyzer("uploads");
    
    // 1. 演示文件安全风险
    std::cout << "--- 1. Testing File Path Traversal ---" << std::endl;
    // 尝试访问一个合法文件
    analyzer.processFile("safe_document.txt");
    // 尝试进行路径遍历攻击 (在Linux/macOS上)
    analyzer.processFile("../../../../../etc/hosts");
    std::cout << std::endl;


    // 2. 演示内存泄漏风险
    std::cout << "--- 2. Testing Memory Leak on Exception ---" << std::endl;
    const char* valid_data = "some data";
    analyzer.processUserData(valid_data, 9); // 正常路径，无泄漏
    analyzer.processUserData(nullptr, 0);      // 异常路径，导致泄漏
    std::cout << "Continuing after potential memory leak..." << std::endl;
    std::cout << std::endl;


    // 3. 演示线程管理不当导致的崩溃 (为防止程序直接退出，将此部分注释)
    std::cout << "--- 3. Testing Thread Management (CRASH) ---" << std::endl;
    // std::cout << "The next call will likely crash the program." << std::endl;
    // analyzer.launchBackgroundJob();
    std::cout << "If this line is printed, the thread was managed correctly (e.g., by joining)." << std::endl;

    return 0;
}