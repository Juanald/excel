FLAGS = -Wall -Wextra -Werror -pedantic -std=c++11

excel: excel.o
	g++ $(FLAGS) -o excel excel.o

excel.o: excel.cpp
	g++ $(FLAGS) -c excel.cpp

clean:
	del -r excel.exe excel.o
