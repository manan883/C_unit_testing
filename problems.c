#include <stdio.h>
#include <string.h>


void context_grep(int argc, char **argv, int context, char *expr){
  /*
  * int argc num elements in argv 
  * char **argv char array 
  * int context num lines before and after expr 
  * char *expr expression to be found within the array 
  */ 
  int context_window[argc];
  for (int i = 0; i < argc; i++) {
      context_window[i] = 0;
  }

  for (int i = 0; i < argc; i++) {
    if (strstr(argv[i], expr) != NULL) {
      int start = i - context < 0 ? 0 : i - context;
      int end = i + context >= argc ? argc - 1 : i + context;
      for (int j = start; j <= end; j++) {
        context_window[j] = 1;
      }
    }
  }

  for (int i = 0; i < argc; i++) {
      if (context_window[i]) {
        printf("%s\n", argv[i]);
      }
  }
}


int reverse_words(const char *input, char *output, size_t out_size) {
  if (!input || !output || out_size == 0) return 0;

  char temp[strlen(input) + 1];
  strcpy(temp, input);

  #define MAX_WORDS 100
  char *words[MAX_WORDS];
  int word_count = 0;

  char *token = strtok(temp, " ");
  while (token != NULL && word_count < MAX_WORDS) {
    words[word_count++] = token;
    token = strtok(NULL, " ");
  }

  size_t len = 0;
  output[0] = '\0';

  for (int i = word_count - 1; i >= 0; i--) {
    size_t wlen = strlen(words[i]);
    if (len + wlen + (len > 0 ? 1 : 0) + 1 > out_size) {
        return 0;
    }
    if (len > 0) {
        output[len] = ' ';
        len++;
        output[len] = '\0';
    }
    strcat(output, words[i]);
    len += wlen;
  }

  return 1; 
}


