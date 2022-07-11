CXX = clang++

#Compilation flag
CPPFLAGS = #-Wall -Wextra -Werror -std=c++98

INCLUDES = includes

SRCS = srcs/ServerApi.cpp

SELECT_SRCS = srcs/ServerSelect.cpp

POLL_SRCS = srcs/ServerPoll.cpp

MAIN_TEST = srcs/MainServerApi.cpp

all:
	@echo "Makefile All"


select:
	$(CXX) $(CPPFLAGS) -I$(INCLUDES) $(SRCS) $(SELECT_SRCS) $(MAIN_TEST)
poll:
	$(CXX) $(CPPFLAGS) -I$(INCLUDES) $(SRCS) $(POLL_SRCS) $(MAIN_TEST)
