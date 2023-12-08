#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <locale>
#include <codecvt>

// String to Wstring
inline std::wstring to_wide_string(const std::string& input)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(input);
}



