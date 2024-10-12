#pragma once

#ifndef _16_21_07_10_2024_JSON_H_
#define _16_21_07_10_2024_JSON_H_

#include "Task.h"

#include <array>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>




Date stringToTimePoint(const std::wstring& strDate) {
	using namespace std::chrono;

	std::tm t{ };
	std::wistringstream stream{ strDate };
	stream >> std::get_time(&t, L"%Y-%m-%d %H:%M:%S");

	return sys_days{ year{ t.tm_year + 1900 } / (t.tm_mon + 1) / t.tm_mday } +
		hours{ t.tm_hour } + minutes{ t.tm_min } + seconds{ t.tm_sec };
}

class TaskJsonConverter {

public:
	static std::wstring to(const Task& task) {
		std::wostringstream stream;
		stream << L"{\n" <<
			L"\t\"id\": " << std::to_wstring(task.getID()) << L",\n" <<
			L"\t\"description\": \"" << task.getDescription() << L"\",\n" <<
			L"\t\"status\": \"" << nameStatusTask[task.getStatus()] << L"\",\n" <<
			L"\t\"createdAt\": \"" << task.getCreatedAt() << L"\",\n" <<
			L"\t\"updateAt\": \"" << task.getUpdatedAt() << L"\"" << "\n" <<
			L"}";

		return stream.str();
	}

	static Task parse(const std::wstring& json) {
		ID id{};
		std::wstring description{};
		Task::Status status{};
		Date createdAt{};
		Date updateAt{};

		wchar_t ch;
		std::wistringstream stream(json);
		while (stream >> ch) {
			if (ch == L'\"') {
				std::wstring key;
				std::getline(stream, key, L'\"');
				stream >> ch;
				stream >> ch;

				std::wstring value;
				if (ch == L'\"') {
					std::getline(stream, value, L'\"');
				}
				else {
					stream.putback(ch);
					stream >> value;
				}

				if (key == L"id") {
					id = std::stoi(value);
				}
				else if (key == L"description") {
					description = value;
				}
				else if (key == L"status") {
					for (std::size_t i{ 0 }; i < nameStatusTask.size(); ++i) {
						if (value == nameStatusTask[i]) {
							status = static_cast<Task::Status>(i);
							break;
						}
					}
				}
				else if (key == L"createdAt") {
					createdAt = stringToTimePoint(value);
				}
				else if (key == L"updateAt") {
					updateAt = stringToTimePoint(value);
				}
			}
		}

		return Task{ id, description, status, createdAt, updateAt };
	}

private:
	static constexpr std::array nameStatusTask{ L"ToDo", L"InProgress", L"Done" };

};

#endif // !_16_21_07_10_2024_JSON_H_