#pragma once

class InvalidPath :public std::exception
{
public:
	InvalidPath() {}
	~InvalidPath() {}
	const char* what() const noexcept {
		return "\ninvalid path\n";
	}
};
