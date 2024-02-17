#include "TM.hpp"

TaskManager::TaskManager(int32_t pool_size) 
    : pool_size(pool_size) 
{
    for (int32_t i = 0; i < pool_size; i++)
        pool.push_back(std::thread(&TaskManager::thread_routine, this));
}

TaskManager::~TaskManager()
{
    TerminateTask term_task = TerminateTask(NULL);
    for (size_t i = 0; i < pool_size; i++)
        routine_parameters_queue.push(std::make_shared<TerminateTask>(term_task));
    for (std::thread& thread : pool)
        thread.join();
}

unsigned long TaskManager::thread_routine(void* lpThreadParameter) {
    TaskManager* manager = static_cast<TaskManager*>(lpThreadParameter);
    unsigned long err{};
    while (true)
    {
        std::shared_ptr<TaskType> operating_task(manager->routine_parameters_queue.pop());
        if (!(err = operating_task->execute()))
            break;
        operating_task->one_thread_method(NULL);
        operating_task->getQueue()->push(operating_task);
    }
    return err;
}