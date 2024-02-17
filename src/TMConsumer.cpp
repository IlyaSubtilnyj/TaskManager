#include "TMConsumer.hpp"

Consumer::Consumer(std::shared_ptr<TaskManager> taskManager) 
	: manager(taskManager) 
{}