#pragma once

#ifndef _16_21_07_10_2024_JSON_H_
#define _16_21_07_10_2024_JSON_H_

#include "Task.h"
#include "Time.h"


#include <string>
#include <vector>


class TaskJsonConverter {

public:

	static std::string serialize(const Task& task);
	static std::string serialize(const std::vector<Task>& tasks);

	static Task parse(const std::string& json);
	static std::vector<Task> parseArray(const std::string& jsonArray);

};

#endif // !_16_21_07_10_2024_JSON_H_