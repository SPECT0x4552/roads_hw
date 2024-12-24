SRC_DIR = src
INCL_DIR = incl 
BUILD_DIR = build
TEST_DIR = tests
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -I$(INCL_DIR) -std=c11

SRC = $(wildcard $(SRC_DIR)/*.c)
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))
TEST_OBJ = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_SRC))
EXEC = trails

ifeq ($(OS), Windows_NT)
    RM = del /Q
    RMDIR = rmdir /S /Q
    MKDIR = if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
    KERNEL_NULL = nul
    TEST_SUPPORTED = false
else
    RM = rm -f
    RMDIR = rm -rf
    MKDIR = mkdir -p $(BUILD_DIR)
    KERNEL_NULL = /dev/null
    TEST_SUPPORTED = true
endif

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@
	@echo "[*] Linking $(EXEC)..."

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "[*] Compiling $<..."
	@$(MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(BUILD_DIR)/*.o > $(KERNEL_NULL) 2>&1
	$(RMDIR) $(BUILD_DIR) > $(KERNEL_NULL) 2>&1
	$(RM) $(EXEC) > $(KERNEL_NULL) 2>&1
	$(RM) test_runner > $(KERNEL_NULL) 2>&1
	@echo "[+] Cleanup complete."

tests: $(TEST_OBJ) $(filter-out $(BUILD_DIR)/main.o, $(OBJ))
ifeq ($(TEST_SUPPORTED), true)
	@echo "[*] Building test runner..."
	$(CC) $(CFLAGS) -o test_runner $^
	@echo "[+] Test runner built: test_runner"
else
	@echo "[-] Tests are only supported on Unix-like environments."
	@exit 1
endif

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	@echo "[*] Compiling test file $<..."
	@$(MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean tests
