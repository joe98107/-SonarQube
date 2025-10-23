#pragma once
// 反例说明：本头文件刻意只实现 R1 的一个子集（添加 + 获取数量），忽略大部分需求。
// 与 requirements.md 存在明显缺口：无删除/更新/搜索/优先级/标签/并发安全/JSON/事件/时间戳等。

#include <string>
#include <vector>

// 简化的 Task 结构：故意缺少 createdAt / updatedAt / description / priority / tags 等字段 (违反 R1, R3, R5)
struct Task {
    std::string id;
    std::string title;
};

class TaskManager {
public:
    // 仅支持最基础的添加；未做：唯一性校验、空字符串校验、异常错误码 (违反 R1, R8)
    void addTask(const std::string& id, const std::string& title);

    // 返回任务总数：仅用于示例 (非文档需求的显式 API)
    size_t count() const noexcept;

    // 演示：获取内部任务快照（暴露内部容器副本，未做性能/并发优化，违反 R7, R9）
    std::vector<Task> listAllUnsafeCopy() const;

    // 占位：序列化功能未实现 (违反 R6)
    std::string toJson() const; // 返回固定占位字符串

private:
    // 简单线性存储 (违反 R9: 未优化搜索；也完全没实现搜索)
    std::vector<Task> tasks_;
};