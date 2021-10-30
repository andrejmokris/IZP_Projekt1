/**
 * @name Projekt 1 - Práce s textem
 * @author Andrej Mokris <xmokri01>
 */

#include <stdio.h>

#include <stdlib.h>

#define MAXLEN 102

/**
 * structure holding data necessary for printing statistics
 * array char_count used for keeping track of letters, which already occured
 */
typedef struct {
  // as we only care for char_count (32-126), we don't need to use whole 255 ascii character
  int char_count[94];
  int min_len;
  float n_of_char;
  int n_of_pass;
  int unique;
}
Stats;

int string_length(char password[]) {
  int i;
  for (i = 0; password[i] != '\0' && password[i] != '\n'; ++i);
  if (password[i - 1] == '\n') {
    password[--i] = '\0';
  }
  return i;
}

/**
 * decides, if char is from alphabet
 *
 * @param ch letter to be checked
 * @return 1 if char is from alphabet
 */
int is_alpha(char ch) {
  if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
    return 1;
  }
  return 0;
}

/**
 * returns smaller of two numbers
 *
 * @param a first number
 * @param b second number
 */
int min(int a, int b) {
  return a < b ? a : b;
}

/**
 * generates substring from given string given its starting and finishing position
 *
 * @param from staring index of substring in the given string
 * @param to finishing index of substring in the given string
 * @param target pointer to string, where substring will be generated
 * @return 1 if substring was created succesfully
 */
int get_substring(char * source, char * target, int from, int to, int length) {
  int i = 0, j = 0;
  if (from < 0 || from > length) {
    return 0;
  }
  if (to > length) {
    return 0;
  }
  for (i = from, j = 0; i <= to; i++, j++) {
    target[j] = source[i];
  }
  return 1;
}

/**
 * compare two strings if they are equal
 *
 * @param s1 first string
 * @param s2 second string
 * @return 1 if s1 and s2 are equal, 0 otherwise
 */
int string_compare(char * s1, char * s2, int size) {
  int counter = 0;
  for (int i = 0; i < size; i++) {
    if (s1[i] == s2[i]) {
      counter++;
    }
    if (counter == size) {
      return 1;
    }
  }
  return 0;
}

void initialize_stats(Stats *stats) {
  stats -> min_len = 100;
  stats -> unique = 0;
  stats -> n_of_char = 0;
  stats -> n_of_pass = 0;
  for (int i = 0; i < 94; i++) {
    stats -> char_count[i] = 0;
  }
}

/**
 * update stats after getting new password
 *
 * @param stats pointer to struct containing statistics data
 */
void update_stats(char password[], Stats * stats, int size) {
  stats -> n_of_pass += 1;
  stats -> n_of_char += size;
  stats -> min_len = min(stats -> min_len, size);
  for (int i = 0; i < size; i++) {
    char letter = password[i];
    // change current letter to ascii value
    int ascii = (int) letter;
    ascii -= 32;
    // check if current letter is in range of ascii values (32-126)
    if (letter >= ' ' && letter <= '~') {
      // check if the letter has already been spotted at index of its ascii value
      if (stats -> char_count[ascii] != 1) {
        stats -> unique++;
        stats -> char_count[ascii] = 1;
      }
    }
  }
}

/**
 * print statistics
 *
 * @param stats struct containing statistics data to be printed
 */
void statistics(Stats stats) {
  printf("Statistika:\n");
  printf("Ruznych znaku: %d\n", stats.unique);
  printf("Minimalni delka: %d\n", stats.min_len);
  float average = 0;
  if (stats.n_of_pass != 0) {
    average = stats.n_of_char / stats.n_of_pass;
  }
  printf("Prumerna delka: %.1f\n", average);
}

int level1(char password[], int size) {
  int low_lett = 0, cap_lett = 0;
  for (int i = 0; i < size; i++) {
    char ch = password[i];
    // if current letter is capital (A-Z), increment capital counter
    if (ch >= 'A' && ch <= 'Z') {
      cap_lett++;
      // if current letter is lowercase (a-z), increment lowercase letter counter
    } else if (ch >= 'a' && ch <= 'z') {
      low_lett++;
    }
  }
  if (low_lett > 0 && cap_lett > 0) {
    return 1;
  }
  return 0;
}

int level2(char password[], int size, long param) {
  // at start of every level (excluding level 1), check if the previous level was completed
  // otherwise return 0, meaning password doesn't meet the security level given
  if (!level1(password, size)) {
    return 0;
  }
  // if the level 1 was completed, lowercase and capital letter must be in the password
  // categories found can therefore be set to 2
  int categories_found = 2;
  int num_found = 0;
  int spec_char_found = 0;
  for (int i = 0; i < size; i++) {
    char letter = password[i];
    // check, if the current letter is number
    if (letter >= '0' && letter <= '9') {
      num_found = 1;
      // if current letter isn't number and alphabetical character, it must be special character
    } else if (!is_alpha(letter)) {
      spec_char_found = 1;
    }
  }

  categories_found += num_found + spec_char_found;
  // check if enough categories to fulfill the given parameter were found
  if ((categories_found >= param) || (categories_found == 4 && param > 4)) {
    return 1;
  }
  return 0;
}

int level3(char password[], int size, long param) {
  if (!level2(password, size, param)) {
    return 0;
  }
  // set counter to 1 and first letter of password as character, which occured last
  int counter = 1;
  char last_char = password[0];
  // iterate through the password and compare current letter with the last_char
  for (int i = 1; i < size; i++) {
    // compare the number of same letters in a row with the parameter given
    if (counter >= param) {
      return 0;
    }
    if (password[i] == last_char) {
      counter++;
      if (counter >= param) {
        return 0;
      }
      // if the current letter does not match with the previous letter, reset the counter
      // set last_char to current letter for further comparison
    } else {
      last_char = password[i];
      counter = 1;
    }
  }
  return 1;
}

int level4(char password[], int size, long param) {
  if (!level3(password, size, param)) {
    return 0;
  }
  // create and initialize char array, which will be used for creating a substring
  char substring1[MAXLEN] = {};
  for (int i = 0; i < size - param + 1; i++) {
    // create substring 1
    if (get_substring(password, substring1, i, i + param - 1, size)) {
      for (int j = i + 1; j < size - param + 1; j++) {
        int index = 0;
        int counter = 0;
        // compare created substring1 with letters in password for match
        for (int k = j; k < j + param; k++) {
          if (substring1[index] != password[k]) {
            break;
          } else if (substring1[index] == password[k]) {
            counter++;
            index++;
          }
          if (index == param) {
            return 0;
          }
        }
      }
    }
  }
  return 1;
}

/**
 * call particular level function according to the level entered
 */
void switch_level(int level, long param, char password[], int size) {
  switch (level) {
  case 1:
    if (level1(password, size)) {
      printf("%s", password);
    }
    break;
  case 2:
    if (level2(password, size, param)) {
      printf("%s", password);
    }
    break;
  case 3:
    if (level3(password, size, param)) {
      printf("%s", password);
    }
    break;
  case 4:
    if (level4(password, size, param)) {
      printf("%s", password);
    }
    break;
  }
}

/**
 * verify, if arguments entered are valid according to the instructions
 */
int param_error_handling(int argc, char * argv[], long * level_f, long * param_f) {
  char * ptr1, * ptr2;
  long level;
  long param;
  // verify the numbers of arguments given
  if (argc > 2 && argc < 5) {
    level = strtol(argv[1], & ptr1, 10);
    param = strtol(argv[2], & ptr2, 10);
    if ((level < 1 || level > 4)) {
      fprintf(stderr, "Err: first argument out of range!\n");
      return 1;
    }
    if (param < 1) {
      fprintf(stderr, "Err: second argument out of range!\n");
      return EXIT_FAILURE;
    }
    // verify, whether "stats" argument is valid
    if (argc == 4 && (string_length(argv[3]) != 7 ||
        !string_compare(argv[3], "--stats", 7))) {
      fprintf(stderr, "Err: wrong last argument!\n");
      return 1;
    }
    // verify, it param and level arguments don't include char_count others than numbers
    if (ptr1[0] != '\0' || ptr2[0] != '\0') {
      fprintf(stderr, "Err: wrong argument format!\n");
      return 1;
    }
  } else if (argc < 3) {
    fprintf(stderr, "Err: not enough arguments!\n");
    return 1;
  } else {
    fprintf(stderr, "Err: too many arguments!\n");
    return 1;
  }
  * level_f = level;
  * param_f = param;
  return 0;
}

int main(int argc, char * argv[]) {
  long level;
  long param;
  // initialize stats struct and its values
  Stats stats;
  initialize_stats(& stats);
  if (param_error_handling(argc, argv, & level, & param)) {
    return EXIT_FAILURE;
  }
  char password[MAXLEN];
  while (fgets(password, MAXLEN, stdin) != NULL) {
    int size = string_length(password);
    if (size <= 100) {
      update_stats(password, & stats, size);
      switch_level(level, param, password, size);
    } else {
      fprintf(stderr, "Err: Password is longer than 100 char!\n");
      return EXIT_FAILURE;
    }
  }
  if (argc == 4) {
    statistics(stats);
  }
  return 0;
}