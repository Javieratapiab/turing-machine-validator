#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define FILENAME "entrada.in"

bool failed_validation = false;

/* Check if element is numeric only */
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

/* Check if element is alphabetic only */
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

/* Current status and next status */
void validate_first_and_last_elements(char * element, int element_index, int line_counter) {
  if (strcmp(element, "halt") == 0 && element_index == 0) {
    failed_validation = true;
    printf("Estado actual inválido (\"halt\") en línea: %d\n", line_counter);
  } else {
    if (!check_if_numbers_only(element) && !check_if_alphabet_only(element)) {
      if (element_index == 0) {
        failed_validation = true;
        printf("Formato estado actual inválido en línea: %d\n", line_counter);
      } else {
        failed_validation = true;
        printf("Formato estado siguiente inválido en línea: %d\n", line_counter);
      }
    }
  }
}

/* Current symbol and next symbol */
void validate_second_and_third_elements(char * element, int element_index, int line_counter) {
  if (check_if_numbers_only(element) || check_if_alphabet_only(element) || strcmp(element, "*") == 0 || strcmp(element, "_") == 0) { return; }
  /* Símbolo actual */
  if (element_index == 1) {
    failed_validation = true;
    printf("Símbolo actual inválido en línea: %d\n", line_counter);
  /* Nuevo símbolo */
  } else {
    failed_validation = true;
    printf("Nuevo símbolo inválido en línea: %d\n", line_counter);
  }
}

/* Movement */
void validate_fourth_element(char * element, int line_counter) {
  if (strcmp(element, "l") == 0 || strcmp(element, "r") == 0 || strcmp(element, "*") == 0) { return; }
  failed_validation = true;
  printf("Movimiento inválido en línea: %d\n", line_counter);
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
      /* Current status */
      if (words_counter == 0) {
        validate_first_and_last_elements(element, words_counter, line_counter);
      /* Current symbol and next symbol */
      } else if (words_counter == 1 || words_counter == 2) {
        validate_second_and_third_elements(element, words_counter, line_counter);
      /* Movement */
      } else if (words_counter == 3) {
        validate_fourth_element(element, line_counter);
      /* Next status */
      } else if (words_counter == 4) {
        validate_first_and_last_elements(element, words_counter, line_counter);
      }
   
      words_counter++;
      ptr = strtok(NULL, delim);
    }

    /* Instruction (or line) should have 5 elements */
    if (words_counter != 5) {
      failed_validation = true;
      printf("El número de elementos en la línea %d debe ser igual a 5, total elementos: %d\n", line_counter, words_counter);
    }

    /* Entry should have 50 lines maximum */
    if (line_counter > 50) {
      failed_validation = true;
      printf("El número de filas debe ser menor o igual a 50, líneas contadas: %d\n", line_counter);
    }

    line_size = getline(&line_buffer, &line_buffer_size, input_file);
  }

  fclose(input_file);
}

int main() {
  printf("\n*** Starting turing machine validation ***\n");

  start_validation();

  /* Print if entry is valid */
  if(!failed_validation) { printf("Archivo válido\n"); }

  return 0;
}
