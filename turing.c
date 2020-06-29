#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define FILENAME "entry.in"

bool failed_validation = false;

bool check_if_numbers_only(char * element) {
  int i;
  int len = strlen(element);
  bool is_numeric = true;

  for(i = 0; i < len; i++) {
    if (element[i] == '\0' || element[i] == '\n') { continue; }
    if (element[i] < '0' || element[i] > '9') {
      is_numeric = false;
    }
  }

  return is_numeric;
}

bool check_if_alphabet_only(char * element) {
  int i;
  int len = strlen(element);
  bool is_alphabetic = true;

  for(i = 0; i < len; i++) {
    if (element[i] == '\0' || element[i] == '\n') { continue; }
    if (element[i] < 'a' || element[i] > 'z') {
      is_alphabetic = false;
    }
  }

  return is_alphabetic;
}

void validate_current_and_next_status(char * element, int element_index, int line_counter) {
  if (strcmp(element, "halt") == 0 && element_index == 0) {
    failed_validation = true;
    printf("Current status invalid (\"halt\"), line: %d\n", line_counter);
  } else {
    if (check_if_numbers_only(element) || check_if_alphabet_only(element)) { return; }
    if (element_index == 0) {
      failed_validation = true;
      printf("Current status invalid, line: %d\n", line_counter);
    } else {
      failed_validation = true;
      printf("Next status invalid, line: %d\n", line_counter);
    }
  }
}

void validate_current_and_next_symbol(char * element, int element_index, int line_counter) {
  if (check_if_numbers_only(element) || check_if_alphabet_only(element) || strcmp(element, "*") == 0 || strcmp(element, "_") == 0) { return; }
  if (element_index == 1) {
    failed_validation = true;
    printf("Current symbol invalid, line: %d\n", line_counter);
  } else {
    failed_validation = true;
    printf("New symbol invalid, line: %d\n", line_counter);
  }
}

void validate_movement(char * element, int line_counter) {
  if (strcmp(element, "l") == 0 || strcmp(element, "r") == 0 || strcmp(element, "*") == 0) { return; }
  failed_validation = true;
  printf("Invalid movement, line: %d\n", line_counter);
}

void start_validation() {
  FILE * input_file = fopen(FILENAME,"r");
  char * line_buffer = NULL;
  int line_counter = 0;
  size_t line_buffer_size = 0;
  ssize_t line_size;

  line_size = getline(&line_buffer, &line_buffer_size, input_file);

  while (line_size >= 0) {
    line_counter++;
    int words_counter = 0;
    char delim[] = " ";
    char *ptr = strtok(line_buffer, delim);

    while(ptr != NULL)
    {
      char * element = ptr;
      switch (words_counter)
      {
      case 0:
        // Current status
        validate_current_and_next_status(element, words_counter, line_counter);
        break;
      case 1:
        // Current symbol
        validate_current_and_next_symbol(element, words_counter, line_counter);
        break;
      case 2:
        // Next symbol
        validate_current_and_next_symbol(element, words_counter, line_counter);
        break;
      case 3:
        // Movement
        validate_movement(element, line_counter);
        break;
      case 4:
        // Next status
        validate_current_and_next_status(element, words_counter, line_counter);
        break;
      default:
        break;
      }

      words_counter++;
      ptr = strtok(NULL, delim);
    }

    // Instruction must have 5 elements
    if (words_counter != 5) {
      failed_validation = true;
      printf("Number of elements in line %d should be equal to 5, total elements found: %d\n", line_counter, words_counter);
    }

    // Entry must have less or equal to 50
    if (line_counter > 50) {
      failed_validation = true;
      printf("The number of lines should be less or equal to 50, total lines: %d\n", line_counter);
    }

    line_size = getline(&line_buffer, &line_buffer_size, input_file);
  }

  fclose(input_file);
}

int main() {
  printf("\n*** Starting turing machine validator ***\n");

  start_validation();

  /* Print if entry is valid */
  if (!failed_validation) {
    printf("The entry is valid!\n");
  }

  return 0;
}
