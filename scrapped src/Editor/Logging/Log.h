#pragma once


#include <pch.h>

enum class LogColors
{
	black = 30,
	red = 31,
	green = 32,
	yellow = 33,
	blue = 34,
	magenta = 35,
	cyan = 36,
	white = 37,
	Default = 39,
	bright_black = 90,
	bright_red = 91,
	bright_green = 92,
	bright_yellow = 93,
	bright_blue = 94,
	bright_magenta = 95,
	bright_cyan = 96,
	bright_white = 97
};
enum class LogBackgroundColors
{
	black = 30 + 10,
	red = 31 + 10,
	green = 32 + 10,
	yellow = 33 + 10,
	blue = 34 + 10,
	magenta = 35 + 10,
	cyan = 36 + 10,
	white = 37 + 10,
	Default = 38 + 10,
	bright_black = 90 + 10,
	bright_red = 91 + 10,
	bright_green = 92 + 10,
	bright_yellow = 93 + 10,
	bright_blue = 94 + 10,
	bright_magenta = 95 + 10,
	bright_cyan = 96 + 10,
	bright_white = 97 + 10
};



class Log
{
	std::string origin;
	LogColors currentForeColor = LogColors::Default;
	LogBackgroundColors currentBackgroundColor = LogBackgroundColors::Default;
	template <typename T>
	void printThis(const T& t,LogColors fore , LogBackgroundColors back );
public:
	explicit Log(const std::string& o) : origin(o) {};
	//Log(const Log& l);
	
	void Print(const std::string& t);
	void Print( const char* c)
	{
		Print(std::string(c));
	}
	void Print(const std::stringstream& t)
	{
		Print(t.str());
	}


	template <typename T>
	void Debug(const T& t)
	{
		printThis<T>(t,LogColors::Default,LogBackgroundColors::Default);
	}

	template<typename T>
	void Warning(const T& t)
	{
		printThis<T>(t, LogColors::bright_yellow, LogBackgroundColors::Default);
	}
	template<typename  T>
	void Error(const T& t)
	{
		printThis<T>(t, LogColors::bright_red, LogBackgroundColors::Default);
	}
	template<typename  T>
	void Critical(const T& t)
	{
		printThis<T>(t, LogColors::white, LogBackgroundColors::red);
	}
	constexpr void SetColor(LogColors fore,LogBackgroundColors back = LogBackgroundColors::Default)
	{
		currentForeColor = fore;
		currentBackgroundColor = back;
	}
};




template <typename T>
inline  void Log::printThis(const T& t,LogColors fore, LogBackgroundColors back)
{
	const auto _fore = currentForeColor;
	const auto _back = currentBackgroundColor;
	const std::string ANSI_COLOR = "\033[";
	std::string fore_a = ANSI_COLOR+std::to_string((int)fore) + 'm';
	std::string back_a = ANSI_COLOR+std::to_string((int)back) + 'm';

	Print(fore_a+back_a+ t + std::string("\033[0m"));

}
