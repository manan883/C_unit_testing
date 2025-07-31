#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "problems.c"


int test_context_grep(const char *test_name, int argc, char **argv, int context, char *expr, const char *expected_output){
  /*
   * open temp file, save stdout to said file, close file. flush stdout. reopen file in r, read to validate expected output 
  */ 
  const char *tmp_file = "tempFile.txt";
  FILE *fp = freopen(tmp_file, "w+", stdout);
  if (fp == NULL) {
    perror("freopen");
    return 0;
  }
  context_grep(argc,argv,context,expr);
  fflush(stdout);
  fclose(fp);

  fp = fopen(tmp_file, "r");
  if (fp == NULL){
    perror("fopen");
    return 0;
  }
  char buffer[8192];
  size_t read_bytes = fread(buffer, 1, sizeof(buffer) - 1, fp);
  buffer[read_bytes] = '\0';

  fclose(fp);

  int valid = (strcmp(buffer, expected_output) == 0);

  remove(tmp_file);
  // redirect stdout back to stdout. use #if which will run before compilation
  #if defined(_WIN32) || defined(_WIN64)
    freopen("CON", "w", stdout);
  #else
    freopen("/dev/tty", "w", stdout);
  #endif
  
  if (valid){
    printf("[PASS] %s\n", test_name);
  }
  else{
    printf("[FAIL] %s\nExpected:\n%s\nActual:\n%s\n", test_name, expected_output, buffer);
  }

  return valid;


}

int test_reverse_words(const char *test_name, const char *input, char *output, size_t output_size, const char *expected_output, int buffer_too_small) {
  int ret = reverse_words(input, output, output_size);
  
  if (ret) { // buffer large enough, output valid
    if (buffer_too_small) {
      // Fail: expected buffer to be too small, but it was large enough
      printf("[FAIL] %s: Expected buffer to be too small, but was large enough\n", test_name);
      return 0;
    } 
    else {
      if (strcmp(output, expected_output) == 0) {
        printf("[PASS] %s\n", test_name);
        return 1;
      } 
      else {
        printf("[FAIL] %s:Expected:\n%s\nActual:\n%s\n", test_name, expected_output, output);
        return 0;
      }
    }
  } 
  else { // buffer too small, reverse_words returned 0
    if (buffer_too_small) {
      // Pass: expected buffer to be too small
      printf("[PASS] %s\n", test_name);
      return 1;
    } 
    else {
      printf("[FAIL] %s: Expected buffer to be sufficient, but was too small\n", test_name);
      return 0;
    }
  }
}


int main(){
  // test context_grep 
  printf("TESTING CONTEXT GREP\n");
  int total_tests = 0;
  int passed_tests = 0;
  //case 1, given in email
  char *input1[] = {
        "The stars are shining bright",
        "Let's grab some lunch",
        "Astrology fascinates many people",
        "The sky turns orange at dusk",
        "Dinner is almost ready",
        "Night sky is mesmerizing",
        "Midnight strolls are peaceful"
    };
  const char *expected1 =
        "Let's grab some lunch\n"
        "Astrology fascinates many people\n"
        "The sky turns orange at dusk\n"
        "Dinner is almost ready\n"
        "Night sky is mesmerizing\n"
        "Midnight strolls are peaceful\n";
  total_tests++;
  passed_tests += test_context_grep("1 EMAIL TEST", 7, input1, 2, "sky", expected1);
  //case 2, test no matches
  char *input2[] = {
    "test", 
    "test2", 
    "test3"
  };
  const char *expected2 = "";
  total_tests++;
  passed_tests += test_context_grep("2 NO MATCHES", 3, input2, 1, "test4",expected2);

  // case 3: context beyond start end 
    char *input3[] = {
        "first line",
        "matching line",
        "last line"
    };
    const char *expected3 =
        "first line\n"
        "matching line\n"
        "last line\n";

    total_tests++;
    passed_tests += test_context_grep("3 CONTEXT BOUNDARY", 3, input3, 5, "matching", expected3);


  //case 4 overlapping contex, similar to email test 
  char *input4[] = {
        "zero",
        "match one",
        "middle",
        "match two",
        "end"
    };
  const char *expected4 =
        "zero\n"
        "match one\n"
        "middle\n"
        "match two\n"
        "end\n";

  total_tests++;
  passed_tests += test_context_grep("4 OVERLAP CONTEXT", 5, input4, 1, "match", expected4);

  printf("CONTEXT_GREP: Passed %d/%d tests.\n", passed_tests, total_tests);

  printf("TESTING REVERSE WORDS\n");
  total_tests = 0;
  passed_tests = 0;
  // test 1 normal 
  const char *input = "The quick brown fox jumps over the lazy dog";
  char output[512];
  const char *expected_output = "dog lazy the over jumps fox brown quick The";
  total_tests++;
  passed_tests += test_reverse_words("1 NORMAL TEST", input, output, sizeof(output), expected_output, 0);
  
  //test 2 email 
  const char *input1_2 = "I like Thai Food";
  char output1_2[512];
  const char *expected_output1_2 = "Food Thai like I";
  total_tests++;
  passed_tests += test_reverse_words("2 EMAIL TEST", input1_2,output1_2,sizeof(output1_2), expected_output1_2, 0);

  //test 3 buffer too small  
  const char *input2_2 = "I like Thai Food";
  char output2_2[5];
  const char *expected_output2_2 = "Food Thai like I";
  total_tests++;
  passed_tests += test_reverse_words("3 BUFFER TOO SMALL", input2_2,output2_2,sizeof(output2_2), expected_output2_2, 1);

  //test 4 empty string  
  const char *input3_2 = "";
  char output3_2[256];
  const char *expected_output3_2 = "";
  total_tests++;
  passed_tests += test_reverse_words("3 EMPTY STRING", input3_2,output3_2,sizeof(output3_2), expected_output3_2, 0);
  printf("REVERSE WORDS: Passed %d/%d tests.\n", passed_tests, total_tests);






  return (passed_tests == total_tests) ? 0 : 1;
}
