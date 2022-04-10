#pragma once

class FileError :public std::exception
{
public:
	FileError() {}
	~FileError() {}
	/*const char* what() const noexcept {
		return "\nError at line: ";
	}*/
};


class InvalidPath :public std::exception
{
public:
	InvalidPath() {}
	~InvalidPath() {}
	const char* what() const noexcept {
		return "\ninvalid path\n";
	}
};
