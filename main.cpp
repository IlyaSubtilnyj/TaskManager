#include "TMConsumer.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <utility>

class MyData : public TaskType {
public:
    MyData(std::string&& string, int32_t index, int32_t size, std::vector<double>& array, concurrent_queue<std::shared_ptr<TaskType>>* queue) 
        : TaskType(queue), _string(std::move(string)), _index(index), _size(size), _array(array)
    {}
    unsigned long virtual one_thread_method(void*) override {
        std::cout << _string << std::endl;

        for (size_t i = _index; i < _index + _size; i++)
        {
            _array.at(i) *= 2;
        }

        _result = "I finished";
        return 0;
    }
public:
    std::string getResult() {
        return _result;
    }
private:
    int32_t _index;
    int32_t _size;
    std::vector<double>& _array;
    std::string _string;
    std::string _result;
};

std::vector<std::pair<int, int>> sliceVector(int size, int n) {
    std::vector<std::pair<int, int>> result;

    if (size < n) {
        throw std::runtime_error("The input vector is too small to be sliced into n parts.");
    }

    int size_per_part = size / n;
    int remaining_size = size % n;

    int end_index = 0;
    int start_index = 0;
    for (int i = 0; i < n; ++i) {
        start_index = end_index;
        end_index = start_index + size_per_part;

        if (i < remaining_size) {
            end_index += 1; // Include the remaining elements if there are any
        }

        result.push_back(std::make_pair(start_index, end_index - start_index));
    }

    return result;
}

class CustomConsumer : public Consumer {
public:
    CustomConsumer(std::shared_ptr<TaskManager> manager, std::string&& name, std::vector<double>& darray)
        : Consumer(manager), _name(name), _darray(darray)
    {}
public:
    void run(int32_t thread_count) {
        std::cout << _name << std::endl;

        auto sliced = sliceVector(_darray.size(), thread_count);

        /*Create tasks*/
        std::vector<MyData> tasks;
        for (size_t i = 0; i < thread_count; i++)
        {
            tasks.emplace_back(MyData("Processing data", sliced.at(i).first, sliced.at(i).second, _darray, &complitedTasksQueue));
        }

        /*Insert tasks into manager*/
        for (size_t i = 0; i < thread_count; i++)
        {
            manager->try_add_task(tasks.at(i));
        }

        /*Gather results*/
        std::vector<MyData> results;
        for (size_t i = 0; i < thread_count; i++)
        {
            results.push_back(*static_cast<MyData*>(complitedTasksQueue.pop().get()));
        }

        /*Use results*/
        for (auto&& res: results)
        {
            std::cout << res.getResult() << std::endl;
        }

    }
private:
    std::string _name;
    std::vector<double>& _darray;
};

std::vector<double> _doubleArray;

int main() {

    for (size_t i = 0; i < 11; i++)
    {
        _doubleArray.push_back(i);
    }

    int32_t thread_count = 5;
    std::shared_ptr<TaskManager> sharedTaskManager = std::make_shared<TaskManager>(thread_count);
    CustomConsumer consumer = CustomConsumer(sharedTaskManager, "Custom consumer started", _doubleArray);
    consumer.run(thread_count);

    for (size_t i = 0; i < 11; i++)
    {
        std::cout << _doubleArray.at(i) << std::endl;
    }
}