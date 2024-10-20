# Compilator and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic

# Directories
SRC = ./src/
SRC_INCLUDE = ./src/include/

# Source files
SRCS_USERS = $(SRC)user.c $(SRC)userResult.c $(SRC)userDataMonitor.c $(SRC)userResultMonitor.c $(SRC)main.c
SRCS_CJSON = $(SRC_INCLUDE)cjson/cjson.c

# Includes
INCLUDES = -I$(SRC_INCLUDE)


# Targets
TARGET_USERS = users

all: $(TARGET_USERS)

$(TARGET_USERS): $(SRCS_USERS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET_USERS) $(SRCS_USERS) $(SRCS_CJSON)

clean:
	rm -rf $(TARGET_USERS)
