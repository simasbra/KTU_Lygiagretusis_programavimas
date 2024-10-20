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
USERS_ENTITLEMENTS = users.entitlements

all: $(TARGET_USERS)

$(TARGET_USERS): $(SRCS_USERS) $(USERS_ENTITLEMENTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET_USERS) $(SRCS_USERS) $(SRCS_CJSON) -g
	codesign -s - -f --entitlements $(USERS_ENTITLEMENTS) $(TARGET_USERS) 

users.out: $(SRCS_USERS) $(USERS_ENTITLEMENTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET_USERS) $(SRCS_USERS) $(SRCS_CJSON) -g
	codesign -s - -f --entitlements $(USERS_ENTITLEMENTS) $(TARGET_USERS) 

$(USERS_ENTITLEMENTS):
        /usr/libexec/PlistBuddy -c "Add :com.apple.security.get-task-allow bool true" $(USERS_ENTITLEMENTS)

clean:
	rm -rf $(TARGET_USERS)
	rm -rf $(USERS_ENTITLEMENTS)
