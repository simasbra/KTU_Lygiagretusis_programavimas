CC = g++
CCXFLAGS = -Wall -Wextra -Werror

SRCDIR = ./src/
SRCS = $(SRCDIR)main.cpp $(SRCDIR)user.cpp $(SRCDIR)usersMonitor.cpp $(SRCDIR)userResult.cpp $(SRCDIR)usersResultMonitor.cpp

TARGET = users

RAPIDJSON_DIR = $(SRCDIR)rapidjson
CRYPTOPP_DIR = $(SRCDIR)cryptopp
INCLUDES = -I$(RAPIDJSON_DIR) -I$(CRYPTOPP_DIR)

CRYPTOPP_LIB = $(CRYPTOPP_DIR)/libcryptopp.a
LIBS = $(CRYPTOPP_LIB)

$(TARGET): $(SRCS)
	$(CC) $(CCXFLAGS) $(INCLUDES) -o $(TARGET) $(SRCS) $(LIBS)

dataGenerator: $(SRCDIR)dataGenerator.cpp
	$(CC) $(CCXFLAGS) -o dataGenerator $(SRCDIR)dataGenerator.cpp

clean:
	rm -f $(TARGET)
