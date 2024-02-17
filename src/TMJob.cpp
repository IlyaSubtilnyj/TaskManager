#include "TMJob.hpp"

TaskType::TaskType(concurrent_queue<std::shared_ptr<TaskType>>* queue, enum TASK_TYPE task_type) 
    : complited_tasks_queue(queue), type(task_type)
{}

int32_t TaskType::execute() {
    if (type == TASK_TYPE::TERMINATE) 
        return 0; 
    else return 1; 
}

struct concurrent_queue<std::shared_ptr<TaskType>>* TaskType::getQueue() { 
    return complited_tasks_queue; 
}

TerminateTask::TerminateTask(concurrent_queue<std::shared_ptr<TaskType>>* queue)
    : TaskType(queue, TASK_TYPE::TERMINATE)
{}

unsigned long TerminateTask::one_thread_method(void*) { 
    return 0; 
}