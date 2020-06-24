#pragma once

#include <string>
#include <vector>

namespace Library
{
	class Utility
	{
	public:
		Utility() = delete;
		Utility(const Utility&) = delete;
		Utility& operator=(const Utility&) = delete;
		Utility(Utility&&) = delete;
		Utility& operator=(Utility&&) = delete;
		~Utility() = default;

		static void LoadBinaryFile(const std::string& filename, std::vector<char>& data);
	};
}