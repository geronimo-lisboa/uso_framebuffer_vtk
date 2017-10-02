#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <cctype>
#include <locale>
#include <regex>
#include <array>
#include <ctime>
#include <chrono>

// trim from start
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}

// trim from start
static inline std::wstring &wltrim(std::wstring &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::wstring &wrtrim(std::wstring &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::wstring &wtrim(std::wstring &s) {
	return wltrim(wrtrim(s));
}



static inline  std::vector<std::string> SplitString(const std::string& input, const std::string& regex) {
	// passing -1 as the submatch index parameter performs splitting
	std::regex re(regex);
	std::sregex_token_iterator
		first{ input.begin(), input.end(), re, -1 },
		last;
	return{ first, last };
}

//Current time as string
static inline std::string GetDateAsString()
{
	std::chrono::time_point<std::chrono::system_clock> hora = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(hora);
	std::string t(std::ctime(&end_time));
	t = trim(t);
	return t;
}