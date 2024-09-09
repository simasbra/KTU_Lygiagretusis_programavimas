CC = g++
CCXFLAGS = -Wall -Wextra -Werror -pedantic
SRCDIR = ./src/
SRCS = $(SRCDIR)main.cpp $(SRCDIR)user.cpp
TARGET = users
RAPIDJSON_DIR = $(SRCDIR)rapidjson
INCLUDES = -I$(RAPIDJSON_DIR)

$(TARGET): $(SRCS)
	$(CC) $(CCXFLAGS) $(INCLUDES) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
