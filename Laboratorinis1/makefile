# Compilator and flags
CC = g++
CCXFLAGS = -Wall -Wextra -Werror

# Directories
SRCDIR = ./src/
RAPIDJSON_DIR = $(SRCDIR)rapidjson
CRYPTOPP_DIR = $(SRCDIR)cryptopp

# Source files
SRCS_USERS = $(SRCDIR)main.cpp $(SRCDIR)user.cpp $(SRCDIR)usersMonitor.cpp $(SRCDIR)userResult.cpp $(SRCDIR)usersResultMonitor.cpp
SRCS_TESTS = $(SRCDIR)tests.cpp $(SRCDIR)user.cpp $(SRCDIR)usersMonitor.cpp $(SRCDIR)userResult.cpp $(SRCDIR)usersResultMonitor.cpp
SRCS_DATA_GENERATOR = $(SRCDIR)dataGenerator/dataGenerator.cpp

# Includes
INCLUDES = -I$(RAPIDJSON_DIR) -I$(CRYPTOPP_DIR)
CRYPTOPP_LIB = $(CRYPTOPP_DIR)/libcryptopp.a
LIBS = $(CRYPTOPP_LIB) -lpthread

# Targets
TARGET_USERS = users
TARGET_TESTS = tests
TARGET_DATA_GENERATOR = dataGenerator

# Rules
$(TARGET_USERS): $(SRCS_USERS)
	$(CC) $(CCXFLAGS) $(INCLUDES) -o $(TARGET_USERS) $(SRCS_USERS) $(LIBS)

$(TARGET_TESTS): $(SRCS_TESTS)
	$(CC) $(CCXFLAGS) $(INCLUDES) -o $(TARGET_TESTS) $(SRCS_TESTS) $(LIBS)

$(TARGET_DATA_GENERATOR): $(SRCS_DATA_GENERATOR)
	$(CC) $(CCXFLAGS) $(INCLUDES) -o $(TARGET_DATA_GENERATOR) $(SRCS_DATA_GENERATOR) $(LIBS)

all: $(TARGET_USERS) $(TARGET_DATA_GENERATOR) $(TARGET_TESTS)

clean:
	rm -f $(TARGET_USERS) $(TARGET_DATA_GENERATOR) $(TARGET_TESTS)
