#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define ENCODING_TESTS 10
#define WRONG_CHECKS 6
#define FILE_NAME "cipher.exe"

int test_file_encoding (const char *command, const char *test, const char *key, const char *sol)
{
  char cli_buffer[250], path_buffer[50], save_as[50];
  snprintf (cli_buffer, sizeof (cli_buffer), "./%s %s %s ./test_files/%s.txt ./test_files/output.txt",
            FILE_NAME, command, key, test);
  system (cli_buffer);
  FILE *output = fopen ("./test_files/output.txt", "r");
  snprintf (path_buffer, sizeof (path_buffer), "./test_files/%s.txt", sol);
  FILE *solution = fopen (path_buffer, "r");
  int source_char, sol_char;
  bool success = true;
  while ((source_char = fgetc (output)) != EOF)
    {
      sol_char = fgetc (solution);
      if (source_char != sol_char)
        {
          success = false;
          break;
        }
    }
  if (success && (fgetc (solution) != EOF))
    success = false;
  fclose (output);
  fclose (solution);
  if (!success)
    {
      snprintf (save_as, sizeof (save_as), "./test_files/%s_failed_k=%s.txt", test, key);
      rename ("./test_files/output.txt", save_as);
      fprintf (stderr, "**failed test %s with k = %s, correct solution is %s\n"
                       "**Your result was saved as %s_failed_k%s.txt in test_files folder\n\n",
               test, key, sol, test, key);
      return 1;
    }
  return 0;
}

int test_encoding (bool encode)
{
  int fail_counter = 0;
  char sources[ENCODING_TESTS][40] = {"letters", "capital_letters", "symbols", "empty",
                                      "symbols_with_letters", "test30", "mixed", "mixed", "mixed",
                                      "mixed"};
  char keys[ENCODING_TESTS][10] = {"15", "7", "6", "17", "23", "-1", "-12", "-382", "260000257", "0"};
  char encrypted[ENCODING_TESTS][40] = {"letters_15_enc", "capital_letters_7_enc", "symbols", "empty",
                                        "symbols_with_letters_23_enc", "test30_-1_enc", "mixed_-12_enc",
                                        "mixed_-382_enc",
                                        "mixed_260000257_enc", "mixed"};
  if (encode)
    for (int i = 0; i < ENCODING_TESTS; ++i)
      fail_counter += test_file_encoding ("encode", sources[i], keys[i], encrypted[i]);
  else
    for (int i = 0; i < ENCODING_TESTS; ++i)
      fail_counter += test_file_encoding ("decode", encrypted[i], keys[i], sources[i]);
  return fail_counter;
}

int true_checks (char *source, char *key, char *encrypted)
{
  char correct_buffer[40];
  char cli_buffer[250];
  snprintf (cli_buffer, sizeof (cli_buffer), "./%s %s ./test_files/%s.txt ./test_files/%s.txt",
            FILE_NAME, "check", source, encrypted);
  snprintf (correct_buffer, sizeof (correct_buffer), "Valid encrypting with k = %s", key);
  printf ("next test checking '%s' as the source and '%s' as the encrypted file.\n"
          "Should return\n\"%s\".\n", source, encrypted, correct_buffer);
  system (cli_buffer);
  return 0;
}

int false_checks (char *source, char *encrypted)
{
  char cli_buffer[250];
  snprintf (cli_buffer, sizeof (cli_buffer), "./%s %s ./test_files/%s.txt ./test_files/%s.txt",
            FILE_NAME, "check", source, encrypted);
  printf ("next test checking '%s' as the source and '%s' as the encrypted file."
          "Should return:\n\"Invalid encrypting\".\n", source, encrypted);
  system (cli_buffer);
  return 0;
}

int test_checks ()
{
  int fail_counter = 0, i = 0;
  char sources[
      ENCODING_TESTS + WRONG_CHECKS][40] = {"letters", "capital_letters", "symbols", "empty", "symbols_with_letters",
                                            "test30", "mixed", "mixed", "mixed", "mixed", "mixed", "mixed", "mixed",
                                            "mixed_1_newline", "mixed_32_comma", "mixed_77_space"};
  char keys[ENCODING_TESTS][10] = {"15", "7", "0", "0", "23", "25", "14", "8", "23", "0"};
  char encrypted[ENCODING_TESTS + WRONG_CHECKS][40] = {"letters_15_enc", "capital_letters_7_enc", "symbols", "empty",
                                                       "symbols_with_letters_23_enc", "test30_-1_enc", "mixed_-12_enc",
                                                       "mixed_-382_enc", "mixed_260000257_enc", "mixed",
                                                       "mixed_1_newline", "mixed_32_comma", "mixed_77_space", "mixed",
                                                       "mixed", "mixed"};
  for (; i < ENCODING_TESTS; ++i)
    fail_counter += true_checks (sources[i], keys[i], encrypted[i]);
  for (; i < ENCODING_TESTS + WRONG_CHECKS; ++i)
    fail_counter += false_checks (sources[i], encrypted[i]);
  return fail_counter;
}

int main ()
{
  printf("Running Ori Levi's tests:\n");
  system("py ./tester.py");
  printf("\n\n\n");
  printf("Running Erel Debel's tests:\n");
  int encodings_failed = test_encoding (true);
  int decodings_failed = test_encoding (false);
  test_checks ();
  printf("\n");
  if (encodings_failed)
    printf ("%d encodings failed, %d passed\n", encodings_failed, ENCODING_TESTS - encodings_failed);
  if (decodings_failed)
    printf ("%d decodings failed, %d passed\n", decodings_failed, ENCODING_TESTS - decodings_failed);
  else if (!encodings_failed)
    printf ("ALL ENCODING/DECODING TESTS PASSED\n");
  printf ("Please check above that the output from the check tests results are as they should be as they are not "
          "checked automatically!\n"
          "Good luck!\n\nPress Enter to exit\n");
  getchar();
}