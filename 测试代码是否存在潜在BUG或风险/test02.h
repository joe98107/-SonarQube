#ifndef RISKY_CODE_H
#define RISKY_CODE_H

#include <string>
#include <thread>

class RiskAnalyzer {
public:
    RiskAnalyzer(const std::string& baseDir);


    bool processFile(const std::string& userInput);

    void processUserData(const char* data, size_t size);

    void launchBackgroundJob();

private:
    std::string baseUploadDir;

    // 辅助函数，可能抛出异常
    void internalProcessing(const char* buffer);
};

#endif // RISKY_CODE_H