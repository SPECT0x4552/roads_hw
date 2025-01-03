#include "fileHandler.h"
#include "throwError.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ_BUFFER_INIT 512

// f_descriptor backup and restoring utility functions
void backupFile(const char *filePath, const char *backupPath) {
  FILE *original = fopen(filePath, "r");
  FILE *backup = fopen(backupPath, "w");
  assert(original != NULL && backup != NULL);

  char buffer[512];
  while (fgets(buffer, sizeof(buffer), original)) {
    fputs(buffer, backup);
  }

  fclose(original);
  fclose(backup);
}

void restoreFile(const char *backupPath, const char *originalPath) {
  FILE *backup = fopen(backupPath, "r");
  FILE *original = fopen(originalPath, "w");
  assert(backup != NULL && original != NULL);

  char buffer[512];
  while (fgets(buffer, sizeof(buffer), backup)) {
    fputs(buffer, original);
  }

  fclose(backup);
  fclose(original);
  unlink(backupPath);
}

// Test validate_TrailData with mismatched count
void test_validateTrailData_mismatchCount() {
  const char *filePath = "tests/mismatched_count.txt";

  FILE *f_descriptor = fopen(filePath, "r");
  assert(f_descriptor != NULL);

  int invalidCount = 0;
  int readResult = fscanf(f_descriptor, "%d", &invalidCount);
  fclose(f_descriptor);

  assert(readResult == 1);
  OK("Read invalid declared count: %d", invalidCount);

  // just a backup file descriptor to restore the original file descriptor after
  // the tests have passed
  const char *backupPath = "tests/mismatched_count_backup.txt";
  backupFile(filePath, backupPath);

  f_descriptor = fopen(filePath, "r+");
  assert(f_descriptor != NULL);

  int correctedCount = validate_TrailData(&f_descriptor, filePath);
  fclose(f_descriptor);

  OK("Corrected count after validation: %d", correctedCount);

  // ensure that the count was corrected after detection
  assert(correctedCount > invalidCount);
  OK("validate_TrailData corrected mismatched count as expected.");

  restoreFile(backupPath, filePath);

  // verify that the restored f_descriptor has the invalid count
  f_descriptor = fopen(filePath, "r");
  assert(f_descriptor != NULL);

  int restoredCount = 0;
  readResult = fscanf(f_descriptor, "%d", &restoredCount);
  fclose(f_descriptor);

  assert(readResult == 1);
  assert(restoredCount == invalidCount);
  OK("File restored to original invalid count: %d", restoredCount);

  unlink(backupPath);

  OK("validate_TrailData (mismatched count) test passed.");
}

// test validate_TrailData with valid input
void test_validateTrailData_validInput() {
  FILE *f_descriptor = fopen("tests/valid_input.txt", "r");
  assert(f_descriptor != NULL);

  int trailCount = validate_TrailData(&f_descriptor, "tests/valid_input.txt");
  fclose(f_descriptor);

  // ensure no correction was needed and the count is valid
  assert(trailCount == 15);

  OK("validate_TrailData (valid input) passed.");
}

void test_validateTrailData_invalidInput() {
  const char *filePath = "tests/invalid_input.txt";

  pid_t pid = fork();
  assert(pid >= 0); // Ensure fork was successful

  if (pid == 0) {
    // Child process: Run the function that triggers the error
    FILE *f_descriptor = fopen(filePath, "r");
    assert(f_descriptor != NULL);

    validate_TrailData(&f_descriptor, filePath);

    fclose(f_descriptor);
    exit(EXIT_SUCCESS); // Should not reach here if error is thrown
  } else {
    // Parent process: Wait for the child to determine if it exited as expected
    int status;
    wait(&status);

    // Check if the child process exited due to `throwOnError`
    if (WIFEXITED(status)) {
      assert(WEXITSTATUS(status) == EXIT_FAILURE);
      OK("validate_TrailData (invalid input) exited as expected.");
    } else {
      assert(0 && "Child process did not exit as expected");
    }
  }
}

void test_validateTrailData_wrongStructureInput() {
  const char *filePath = "tests/wrong_input.txt";

  pid_t pid = fork(); // create a new process
  assert(pid >= 0);   // check for fork's success

  if (pid == 0) {
    // child process runs the functions to ensure that it fails
    FILE *f_descriptor = fopen(filePath, "r");
    assert(f_descriptor != NULL);

    // attempt to validate the trail data; it should exit on error
    validate_TrailData(&f_descriptor, filePath);

    fclose(f_descriptor);
    exit(EXIT_SUCCESS); // if it reaches this, the test failed
  } else {
    // parent process waits for the child and checks its status
    int status;
    wait(&status);

    // check  if child exited
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) !=
           EXIT_SUCCESS); // ensure that it exited with error
  }

  OK("validate_TrailData (wrong structure input) passed.");
}

int main() {
  test_validateTrailData_validInput();
  test_validateTrailData_mismatchCount();
  test_validateTrailData_invalidInput();
  test_validateTrailData_wrongStructureInput();

  OK("All tests passed successfully.");
  return 0;
}
