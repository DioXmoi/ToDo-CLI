#pragma once

#ifndef _19_46_06_10_2024_TASK_H_
#define _19_46_06_10_2024_TASK_H_

#include <chrono>
#include <string>
#include <string_view>

using ID = int;
using Date = std::chrono::system_clock::time_point;

class Task {
public:

	enum Status {
		ToDo,
		InProgress,
		Done,
	};

	Task(ID id, std::string_view description)
		: m_id{ id }
		, m_description{ description }
		, m_status{ ToDo }
		, m_createdAt{ std::chrono::system_clock::now() }
		, m_updateAt{ m_createdAt }
	{

	}

	ID getID() const { return m_id; }

	Date getCreatedAt() const { return m_createdAt; }

	Date getUpdatedAt() const { return m_updateAt; }

	const std::string& getDescription() const { return m_description; }

	void setDescription(std::string_view description) {
		m_updateAt = std::chrono::system_clock::now();
		m_description = description;
	}
	Status getStatus() const { return m_status; }

	void setStatus(Status status) {
		m_updateAt = std::chrono::system_clock::now();
		m_status = status;
	}

private:

	const ID m_id;
	std::string m_description;
	Status m_status;
	const Date m_createdAt;
	Date m_updateAt;

};

#endif //!_19_46_06_10_2024_TASK_H_