# Compilator and flags
CC = g++-14
CCXFLAGS = -Wall -Wextra -Werror

# Directories
SRCDIR = ./src/
RAPIDJSON_DIR = $(SRCDIR)rapidjson
CRYPTOPP_DIR = $(SRCDIR)cryptopp

# Source files
SRCS_USERS = $(SRCDIR)mainA.cpp $(SRCDIR)user.cpp $(SRCDIR)usersMonitor.cpp $(SRCDIR)userResult.cpp $(SRCDIR)usersResultMonitor.cpp
SRCS_OPENMP = $(SRCDIR)mainB.cpp $(SRCDIR)user.cpp $(SRCDIR)userResult.cpp $(SRCDIR)openmpMonitor.cpp
SRCS_TESTS = $(SRCDIR)testsA.cpp $(SRCDIR)user.cpp $(SRCDIR)usersMonitor.cpp $(SRCDIR)userResult.cpp $(SRCDIR)usersResultMonitor.cpp
SRCS_DATA_GENERATOR = $(SRCDIR)dataGenerator/dataGenerator.cpp

# Includes
INCLUDES = -I$(RAPIDJSON_DIR) -I$(CRYPTOPP_DIR)
CRYPTOPP_LIB = $(CRYPTOPP_DIR)/libcryptopp.a
LIBS = $(CRYPTOPP_LIB) -lpthread

# Targets
TARGET_USERS = users
TARGET_OPENMP = openmp
TARGET_TESTS = tests
TARGET_DATA_GENERATOR = dataGenerator

# Rules
$(TARGET_USERS): $(SRCS_USERS)
	$(CC) $(CCXFLAGS) $(INCLUDES) -o $(TARGET_USERS) $(SRCS_USERS) $(LIBS)

$(TARGET_OPENMP): $(SRCS_OPENMP)
	$(CC) $(CCXFLAGS) $(INCLUDES) -o $(TARGET_OPENMP) $(SRCS_OPENMP) $(CRYPTOPP_LIB) -fopenmp
	
$(TARGET_TESTS): $(SRCS_TESTS)
	$(CC) $(CCXFLAGS) $(INCLUDES) -o $(TARGET_TESTS) $(SRCS_TESTS) $(LIBS)

$(TARGET_DATA_GENERATOR): $(SRCS_DATA_GENERATOR)
	$(CC) $(CCXFLAGS) $(INCLUDES) -o $(TARGET_DATA_GENERATOR) $(SRCS_DATA_GENERATOR) $(LIBS)

all: $(TARGET_USERS) $(TARGET_DATA_GENERATOR) $(TARGET_TESTS)

clean:
	rm -f $(TARGET_USERS) $(TARGET_DATA_GENERATOR) $(TARGET_TESTS) $(TARGET_OPENMP)
