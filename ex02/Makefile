# Color codes
GREEN	= \033[0;32m
VIOLET	= \033[0;35m
RESET	= \033[0m

# Variables
NAME = PmergeMe
SRC =	main.cpp \
		PmergeMe.cpp
OBJ = $(SRC:.cpp=.o)
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -Ofast

# Compile the executable
all: $(NAME)

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME) > /dev/null 2>&1
	@echo "$(GREEN)object files created.$(RESET)"
	@echo "$(GREEN)$(NAME) created. $(RESET)"

# Compile object files
%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files
clean:
	@rm -f $(OBJ) > /dev/null 2>&1
	@echo "$(VIOLET)object files deleted.$(RESET)"

# Clean up object files and the executable
fclean: clean
	@rm -f $(NAME) > /dev/null 2>&1
	@echo "$(VIOLET)$(NAME) deleted.$(RESET)"

# Recompile the project
re: fclean all

.PHONY: all clean fclean re
