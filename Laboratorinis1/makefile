CC = g++
CCXFLAGS = -Wall -Wextra -Werror

SRCDIR = ./src/
SRCS_USERS = $(SRCDIR)main.cpp $(SRCDIR)user.cpp $(SRCDIR)usersMonitor.cpp $(SRCDIR)userResult.cpp $(SRCDIR)usersResultMonitor.cpp
SRCS_TESTS = $(SRCDIR)tests.cpp $(SRCDIR)user.cpp $(SRCDIR)usersMonitor.cpp $(SRCDIR)userResult.cpp $(SRCDIR)usersResultMonitor.cpp
SRCS_DATA_GENERATOR = $(SRCDIR)dataGenerator/dataGenerator.cpp

RAPIDJSON_DIR = $(SRCDIR)rapidjson
CRYPTOPP_DIR = $(SRCDIR)cryptopp
INCLUDES = -I$(RAPIDJSON_DIR) -I$(CRYPTOPP_DIR)

CRYPTOPP_LIB = $(CRYPTOPP_DIR)/libcryptopp.a
LIBS = $(CRYPTOPP_LIB) -lpthread

TARGET_USERS = users
TARGET_TESTS = tests
TARGET_DATA_GENERATOR = dataGenerator

$(TARGET_USERS): $(SRCS_USERS)
	$(CC) $(CCXFLAGS) $(INCLUDES) -o $(TARGET_USERS) $(SRCS_USERS) $(LIBS)

$(TARGET_TESTS): $(SRCS_TESTS)
	$(CC) $(CCXFLAGS) $(INCLUDES) -o $(TARGET_TESTS) $(SRCS_TESTS) $(LIBS)

$(TARGET_DATA_GENERATOR): $(SRCS_DATA_GENERATOR)
	$(CC) $(CCXFLAGS) $(INCLUDES) -o $(TARGET_DATA_GENERATOR) $(SRCS_DATA_GENERATOR) $(LIBS)

clean:
	rm -f $(TARGET) dataGenerator tests
