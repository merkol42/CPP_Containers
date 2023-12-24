#include "information_printer.hpp"

void print_info(std::string info)
{
	std::cout << YELLOW << info << RESET << std::endl;
}

void print_title(std::string title)
{
	std::cout << CYAN << "\n*******" << title << "*******\n" RESET << std::endl;
}

void print_subheading(std::string subheading)
{
	std::cout << CYAN << "\n---" << subheading << "---\n" RESET << std::endl;
}

void	print_vector(const std::vector<int>& vec, char sep)
{
	print_title("---vector---");
	size_t vecSize = vec.size();
	for (size_t i = 0; i < vecSize; i++)
		std::cout << vec[i] << sep;
	std::cout << std::endl;
}