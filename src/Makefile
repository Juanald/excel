FLAGS = -Wall -Wextra -Werror -pedantic -std=c++11

SRCS = excel.cpp
OBJS = excel.o

TARGET = excel

$(TARGET): $(OBJS)
	g++ $(FLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	g++ $(FLAGS) -c $<

clean:
	del -r $(OBJS) $(TARGET)
