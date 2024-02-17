#pragma once
#include <cstdint>
#include <vector>
#include <thread>
#include "TMJob.hpp"

/**
    @struct TaskManager
    @brief  Manager of task receiving from client threads and execution using its own thread pool.
    @tparam routine           - threads function start address
    @tparam routine_parameter - arguments passed to routine function
**/

struct TaskManager {
public:
    TaskManager(int32_t pool_size);
    ~TaskManager();

    template<typename task_t, typename = std::enable_if_t<std::is_base_of_v<TaskType, task_t>>>
    bool try_add_task(task_t const& task) { return routine_parameters_queue.try_push(std::make_shared<task_t>(task)); }

    template<typename task_t, typename = std::enable_if_t<std::is_base_of_v<TaskType, task_t>>>
    bool add_task(task_t const& task) { return routine_parameters_queue.push(std::make_shared<task_t>(task)); }

public:
    static unsigned long thread_routine(void* lpThreadParameter);
protected:
    std::vector<std::thread> pool;
    int32_t pool_size;
    struct concurrent_queue<std::shared_ptr<TaskType>> routine_parameters_queue;
};