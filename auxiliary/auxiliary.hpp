#ifndef AUXILIARY_HPP
# define AUXILIARY_HPP

# include <iostream>
# include <vector>

# define CYAN			"\e[0;36m"
# define L_GRAY			"\e[0;38;5;245m"
# define YELLOW			"\e[0;33m"
# define ORANGE			"\e[0;38;5;166m"
# define RESET			"\e[0m"

void	print_info(std::string info);
void	print_title(std::string title);
void	print_subheading(std::string subheading);
void	print_vector(const std::vector<int>& vec, char sep = ' ');

#include "auxiliary.cpp"
#endif