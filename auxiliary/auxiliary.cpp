#include "auxiliary.hpp"

void print_instructions(std::string instruction)
{
	std::cout << L_GRAY << instruction << RESET << std::endl;
}

void print_title(std::string title)
{
	std::cout << CYAN << "\n*******" << title << "*******\n" RESET << std::endl;
}

void print_subheading(std::string subheading)
{
	std::cout << CYAN << "\n\n---" << subheading << "---\n" RESET << std::endl;
}

void	print_vector(const std::vector<int>& vec, char sep)
{
	print_title("---vector---");
	for (size_t i = 0; i < vec.size(); i++)
		std::cout << vec[i] << sep;
	std::cout << std::endl;
}