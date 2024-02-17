#pragma once
#include <cstdint>
#include <memory>
#include "ConcurrentQueue.hpp"

enum class TASK_TYPE
{
    EXECUTE, TERMINATE, THROW, CHECK, LOG, CLEAR
};

struct TaskType {
public:
    TaskType(concurrent_queue<std::shared_ptr<TaskType>>* queue, enum TASK_TYPE task_type = TASK_TYPE::EXECUTE);
protected:
    enum TASK_TYPE type;
public:
    int32_t execute();
    unsigned long virtual one_thread_method(void*) = 0;
    struct concurrent_queue<std::shared_ptr<TaskType>>* getQueue();
protected:
    struct concurrent_queue<std::shared_ptr<TaskType>>* complited_tasks_queue;
};

struct TerminateTask : public TaskType {
public:
    TerminateTask(concurrent_queue<std::shared_ptr<TaskType>>* queue = NULL);
    unsigned long one_thread_method(void*) override;
};