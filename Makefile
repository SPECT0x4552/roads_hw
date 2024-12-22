SRC_DIR = src
INCL_DIR = incl 
BUILD_DIR = build
CC=gcc
CFLAGS=-Wall -Wextra -pedantic -I$(INCL_DIR) -std=c11

SRC = $(wildcard ${SRC_DIR}/*.c)
OBJ = $(patsubst ${SRC_DIR}/%.c, $(BUILD_DIR)/%.o, $(SRC))
EXEC = trails

# cross platform friendly cleanup procedure
ifeq ($(OS), Windows_NT)
	RM = del /Q
	RMDIR = rmdir /S /Q
else
	RM = rm -f
	RMDIR = rm -rf
endif

all: $(EXEC) 

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@
	@echo "[*] Linking $(EXECUTABLE)..."

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "[*] Generating build files for $(EXEC)..."
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


clean: 
	$(RM) $(BUILD_DIR)/*.o
	$(RMDIR) $(BUILD_DIR)
	$(RM) $(EXEC)
	@echo "[+] Object files and build directory removed - cleanup complete."

.PHONY: all clean





