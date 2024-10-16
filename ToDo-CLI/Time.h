#pragma once

#ifndef _20_45_12_10_2024_TIME_H_
#define _20_45_12_10_2024_TIME_H_

#include <chrono>

namespace Time {
	using Date = std::chrono::system_clock::time_point;

	Date parseDate(const std::string& strDate);
}

#endif // !_20_45_12_10_2024_TIME_H_
