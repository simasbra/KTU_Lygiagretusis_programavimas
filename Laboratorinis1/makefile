CC = g++
CCXFLAGS = -Wall -Wextra -Werror -pedantic
SRCDIR = ./src/
SRCS = $(SRCDIR)main.cpp $(SRCDIR)user.cpp
TARGET = users

$(TARGET): $(SRCS)
	$(CC) $(CCXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

dataGenerator: $(SRCDIR)dataGenerator.c $(SRCDIR)cjson/cJSON.c
	gcc -o dataGenerator $(SRCDIR)dataGenerator.c $(SRCDIR)cjson/cJSON.c -I$(SRCDIR)cjson -Wall -Wextra -Werror -pedantic

clean_all: clean
	rm -f $(SRCDIR)cjson/*.o
	rm -f $(SRCDIR)cjson/*.so
	rm -f $(SRCDIR)cjson/*.a
