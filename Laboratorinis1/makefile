CC = g++
CCXFLAGS = -Wall -Wextra -Werror -pedantic
SRCDIR = ./src/
SRCS = $(SRCDIR)main.cpp $(SRCDIR)user.cpp
TARGET = users

$(TARGET): $(SRCS)
	$(CC) $(CCXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
