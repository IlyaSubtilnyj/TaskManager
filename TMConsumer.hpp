#pragma once
#include <memory>
#include "TM.hpp"

class Consumer {
public:
    Consumer(std::shared_ptr<TaskManager> taskManager);
protected:
    std::shared_ptr<TaskManager> manager;
    struct concurrent_queue<std::shared_ptr<TaskType>> complitedTasksQueue;
};