#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int characters[255];
  int min_len;
  float n_of_char;
  int n_of_pass;
  int unique;
} Stats;

int string_length(char password[]) {
  int i;
  for (i = 0; password[i] != '\0' && password[i] != '\n'; ++i);
  if (password[i - 1] == '\n') {
    password[--i] = '\0';
  }
  return i;
}

int is_alpha(char ch) {
  if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
    return 1;
  }
  return 0;
}

int min(int a, int b) {
  return a < b ? a : b;
}

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

int string_compare(char * string1, char * string2, int size) {
  int counter = 0;
  for (int i = 0; i < size; i++) {
    if (string1[i] == string2[i]) {
      counter++;
    }
    if (counter >= size) {
      return 0;
    }
  }
  return 1;
}

void update_stats(char password[], Stats * stats, int size) {
   stats -> n_of_pass += 1;
   for (int i = 0; i < size; i++) {
    char letter = password[i];
    int ascii = (int) letter; // change char to ascii value
    if (letter >= ' ' && letter <= '~') {
      if (stats -> characters[ascii] != 1) {
        stats -> unique++;
      }
      stats -> characters[ascii] = 1;
    }
  }
}

int level1(char password[], int size) {
  int sm_lett = 0, cap_lett = 0;
  for (int i = 0; i < size; i++) {
    char ch = password[i];
    if (ch >= 'A' && ch <= 'Z') {
      cap_lett++;
    } else if (ch >= 'a' && ch <= 'z') {
      sm_lett++;
    }
  }
  if (sm_lett > 0 && cap_lett > 0) {
    return 1;
  }
  return 0;
}

int level2(char password[], int size, long long param) {
  if (!level1(password, size)) {
    return 0;
  }
  int categories_found = 2;
  int num_found = 0;
  int spec_char_found = 0;
  for (int i = 0; i < size; i++) {
    char letter = password[i];
    if (letter >= '0' && letter <= '9') {
      num_found = 1;
    } else if (!is_alpha(letter)) {
      spec_char_found = 1;
    }
  }
  
  categories_found += num_found + spec_char_found;

  if((categories_found >= param) || (categories_found == 4 && param > 4)) {
    return 1;
  }
  return 0;
}

int level3(char password[], int size, long long param) {
  if (!level2(password, size, param)) {
    return 0;
  }
  int counter = 1;
  char last_char = password[0];
  for (int i = 1; i < size; i++) {
    if (counter >= param) {
      return 0;
    }
    if (password[i] == last_char) {
      counter++;
      if (counter >= param) {
        return 0;
      }
    } else {
      last_char = password[i];
      counter = 1;
    }
  }
  return 1;
}

int level4(char password[], int size, long long param) {
  if (!level3(password, size, param)) {
    return 0;
  }
  // na toto si dat pozor
  char substring1[102] = {};
  char substring2[102] = {};
  for (int i = 0; i < size - param + 1; i++) {
    if (get_substring(password, substring1, i, i + param - 1, size)) {
      for (int j = i + 1; j < size - param + 1; j++) {
        if (get_substring(password, substring2, j, j + param - 1, size)) {
          if (string_compare(substring1, substring2, param) == 0) {
            return 0;
          }
        }
      }
    }
  }
  return 1;
}

void statistics(Stats stats) {
  printf("Statistika:\n");
  printf("Ruznych znaku: %d\n", stats.unique);
  printf("Minimalni delka: %d\n", stats.min_len);
  float average = stats.n_of_char / stats.n_of_pass;
  printf("Prumerna delka: %.1f\n", average);
}

void switch_level(int level, long long param, char password[], int size) {
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

int param_error_handling(int argc, char * argv[], long *level_f, long long *param_f) {
  char *ptr1, *ptr2;
  long level;
  long long param;
  if (argc > 2 && argc < 5) {
    level = strtol(argv[1], & ptr1, 10);
    param = strtoll(argv[2], & ptr2, 10);
    if ((level < 1 || level > 4)) {
      fprintf(stderr, "Err: first argument out of range!\n");
      return 1;
    }
    if (param < 1) {
      fprintf(stderr, "Err: second argument out of range!\n");
      return EXIT_FAILURE;
    }
    if (argc == 4 && (string_length(argv[3]) != 7 || 
      string_compare(argv[3], "--stats", 7) != 0)) {
      fprintf(stderr, "Err: wrong last argument!\n");
      return 1;
    }
    if (ptr1[0] != '\0' || ptr2[0] != '\0') {
      fprintf(stderr, "Err: wrong argument format!\n");
      return 1;
    }
  } else if (argc < 3) {
    fprintf(stderr, "Err: not enough arguments!\n");
    return 1;
  } 
  else {
    fprintf(stderr, "Err: too many arguments!\n");
    return 1;
  }
  *level_f = level;
  *param_f = param;
  return 0;
}

int main(int argc, char * argv[]) {
  long level;
  long long param;
  Stats stats;
  stats.min_len = 100;
  stats.unique = 0;
  stats.n_of_char = 0;
  stats.n_of_pass = 0;
  if(param_error_handling(argc, argv, &level, &param)) {
    return EXIT_FAILURE;
  }
  char password[102];
  while (fgets(password, 102, stdin) != NULL) {
    int size = string_length(password);   
    if (size <= 100) {
      stats.min_len = min(stats.min_len, size);   
      stats.n_of_char += size;
      update_stats(password, &stats, size);
      switch_level(level, param, password, size);
    } 
    else {
      fprintf(stderr, "Err: Password is longer than 100 char!\n");
      return EXIT_FAILURE;
    }
  }
  if (argc == 4 && string_length(argv[3]) == 7 && !string_compare(argv[3], "--stats", 7)) {
    statistics(stats);
  }
  return 0;
}