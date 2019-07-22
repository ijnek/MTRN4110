NAME := test
GCC	:= -Wall -Werror -std=c++0x

# Requires main.cpp and pid.o
# Final executable is 'test'
main: main.cpp planning.o
	g++ main.cpp planning.o -o $(NAME) $(GCC)

# Requires planning.cpp
planning.o: planning.cpp
	g++ -c planning.cpp -o planning.o

# Cleanup
clean:
	rm $(NAME) *.o