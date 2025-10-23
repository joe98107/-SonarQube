#include "task_manager.h"

// 反例：未包含任何时间函数；未生成 ISO8601；无并发控制；无事件回调；未抛出异常。

void TaskManager::addTask(const std::string& id, const std::string& title) {
    // 故意不做：空/重复校验、异常、大小写处理
    tasks_.push_back(Task{id, title});
}

size_t TaskManager::count() const noexcept {
    return tasks_.size();
}

std::vector<Task> TaskManager::listAllUnsafeCopy() const {
    return tasks_; // 直接复制所有内容（性能与并发忽略）
}

std::string TaskManager::toJson() const {
    // 反例：返回固定字符串而不是实际 JSON (违反 R6)
    return "{}"; 
}
