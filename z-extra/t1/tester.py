""""""""""""""""""""""#"""""""""""""""""""""""""
If any bugs are found, send me a mail to 
ori.levi2@mail.huji.ac.il
Happy Debugging!
""""""""""""""""""""""#"""""""""""""""""""""""""
import subprocess
import difflib


def compare_files(test_file, out_file, test_name):
    with open(test_file, 'r') as test:
        with open(out_file, 'r') as output:
            diff = difflib.ndiff(test.readlines(), output.readlines())
            maybe_error = False

            for i, line in enumerate(diff):
                if line.startswith("? "):
                    pass
                elif maybe_error:
                    if line.startswith("+ ERROR: "):
                        maybe_error = False
                    else:
                        print(test_name + "\nTest - FAIL")
                        print("Difference found in '" + out_file + "' at line " + str(i) + "\n")
                        return 1

                elif line.startswith("- "):
                    if line.startswith("- ERROR: "):
                        maybe_error = True
                    else:
                        print(test_name + "\nTest - FAIL")
                        print("Difference found in '" + out_file + "' at line " + str(i) + "\n")
                        return 1

                elif line.startswith("+ "):
                    print(test_name + "\nTest - FAIL")
                    print("Difference found in '" + out_file + "' at line " + str(i) + "\n")
                    return 1
            print(f'"test {test_name} has passed!')


def checking_args():
    no_args = subprocess.run("cipher ", capture_output=True,
                             text=True, shell=True)
    invalid_command = subprocess.run("cipher blabla", capture_output=True,
                                     text=True, shell=True)
    if not (no_args.returncode == 1 and no_args.stderr == "The given command is invalid\n"):
        print("Arg test with no arguments failed, should print invalid command string!")
        return
    if not (invalid_command.returncode == 1 and invalid_command.stderr == "The given command is invalid\n"):
        print("Arg test with no arguments failed, should print invalid command string!")
        return
    two_args = subprocess.run("cipher encode", capture_output=True,
                              text=True, shell=True)
    if not (two_args.returncode == 1 and two_args.stderr == "Usage: cipher <encode|decode> <k> <source path file> <output path "
                                                            "file>\n"):
        print("arg test with 'cipher encode' failed, encode handling no k and file arguments")
        return

    two_args = subprocess.run("cipher decode", capture_output=True,
                              text=True, shell=True)
    if not (two_args.returncode == 1 and two_args.stderr == "Usage: cipher <encode|decode> <k> <source path file> <output path "
                                                            "file>\n"):
        print("arg test with 'cipher decode' failed, decode handling no k and file arguments")
        return

    two_args = subprocess.run("cipher check", capture_output=True,
                              text=True, shell=True)
    if not (two_args.returncode == 1 and two_args.stderr == "Usage: cipher <check> <source path file> <output path "
                                                            "file>\n"):
        print("arg test with 'cipher check' failed, check handling no  file arguments")
        return
    two_args = subprocess.run("cipher encode k in out extra", capture_output=True,
                              text=True, shell=True)
    if not (two_args.returncode == 1 and two_args.stderr == "Usage: cipher <encode|decode> <k> <source path file> <output path "
                                                            "file>\n"):
        print("arg test with 'cipher encode k in out extra' failed, encode handling extra args")
        return

    two_args = subprocess.run("cipher decode k in out extra", capture_output=True,
                              text=True, shell=True)
    if not (two_args.returncode == 1 and two_args.stderr == "Usage: cipher <encode|decode> <k> <source path file> <output path "
                                                            "file>\n"):
        print("arg test with 'cipher decode k in out extra' failed, decode handling extra args")
        return

    two_args = subprocess.run("cipher check in out extra", capture_output=True,
                              text=True, shell=True)
    if not (two_args.returncode == 1 and two_args.stderr == "Usage: cipher <check> <source path file> <output path "
                                                            "file>\n"):
        print("arg test with 'cipher check in out extra' failed, check handling extra args")
        return

    print("args checked successfully!")
    return 1


def basic_tests():
    print("checking basic encoding with key = 0...")
    basic_test_encryption = subprocess.run("cipher encode 0 encode_hello.txt encode_hello_done.txt", capture_output=True,
                                           text=True, shell=True)
    if basic_test_encryption.returncode:
        print("Basic test did not run successfully, please check if argument handling if good, or if the files exist in place")
        return
    if compare_files("encode_hello.txt", "encode_hello_done.txt", "encode k=0"):
        return
    print("Testing check of files, should print key = 0")
    basic_test_check = subprocess.run("cipher check encode_hello.txt encode_hello_done.txt", capture_output=True,
                                      text=True, shell=True)
    if basic_test_check.returncode:
        print("Test did not run successfully, check that the files exist, or that your argument handling is correct, "
              "or that your checking function is functioning  correctly\n")
        return
    else:
        print("Test check with k = 0 passed!")
    print("Testing Decode with k = 0")
    basic_test_decode = subprocess.run("cipher decode 0 encode_hello.txt encode_hello_done.txt", capture_output=True,
                                       text=True, shell=True)
    if basic_test_decode.returncode:
        print("Basic test did not run successfully, please check if argument handling if good, or if the files exist in place")
        return
    if compare_files("encode_hello.txt", "encode_hello_done.txt", "encode k=0"):
        return
    return 1


def handle_files():
    print("testing files")
    no_file = subprocess.run("cipher decode 0 nofile.txt encode_hello_done.txt", capture_output=True,
                             text=True, shell=True)
    if not no_file.returncode:
        print("encode - Handling of non-existent file is incorrect, make sure to check if pointer is null")
        return
    no_file = subprocess.run("cipher check encode_hello_done.txt nofile.txt", capture_output=True,
                             text=True, shell=True)

    if not no_file.returncode:
        print("check - Handling of non-existent file is incorrect, make sure to check if pointer is null")
        return

    no_file = subprocess.run("cipher decode nofile.txt encode_hello_done.txt", capture_output=True,
                             text=True, shell=True)
    if not no_file.returncode:
        print("decode - Handling of non-existent file is incorrect, make sure to check if pointer is null")
        return
    print("handling files succeeded, please check that each file you opened was closed")
    return 1


def test_advance():
    print("Starting advanced testings:")
    invalid = "Invalid encrypting\n"
    valid = "Valid encrypting with k = {k}\n"
    tests = [["encode k = 26", "encode", "26", "encode_hello.txt", "decode_26.txt", "encode_hello.txt"],
             ["decode k = 26", "decode", "-26", "decode_26.txt", "decode26_check.txt", "decode_26.txt"],
             ["encode k = 27", "encode", "27", "encode_hello.txt", "encode_27.txt", "27_cmp.txt"],
             ["decode k = -1", "decode", "-1", "decode_26.txt", "encode_27.txt", "encode_27_check.txt"],
             ["decode k = 27", "decode", "27", "encode_27.txt", "decode_27.txt", "encode_hello.txt"],
             ["check k = 1 ", "check", "encode_hello.txt", "encode_27.txt", valid.format(k="1")],
             ["check k = 25", "check", "encode_27.txt", "encode_hello.txt", valid.format(k="25")],
             ["check 'check' command error handling", "check", "encode_27.txt", "check_27.txt", invalid],
             ["check 'check' command error handling 2", "check", "encode_27.txt", "check_28.txt", invalid],
             ["encode with k = -3", "encode", "-3", "encode_hello.txt", "encoded_-3.txt", "-3.txt"],
             ["check encoding of -3 to be 23", "check", "encode_hello.txt", "encoded_-3.txt", valid.format(k="23")]]
    for test in tests:
        print("test case - ", test[0])
        if test[1] == "check":
            checking = subprocess.run("cipher " + test[1] + " " + test[2] + " " + test[3], capture_output=True,
                                      text=True, shell=True)
            if checking.returncode:
                print("test run failed, please check for file existence/handling")
                return
            if checking.stdout != test[4]:
                print("check failed, please recheck your code")
                print(checking.stderr, checking.stdout)
                return
        else:
            cipher = subprocess.run("cipher " + test[1] + " " + test[2] + " " + test[3] + " " + test[4], capture_output=True,
                                    text=True, shell=True)
            if cipher.returncode:
                print("test ran into problem, please check file existence", test[3])
                return
            if compare_files(test[4], test[5], test[0]):
                return
            print("test passed")
    return 1


if __name__ == '__main__':
    print("Compiling with -Wextra, -Wallm -Wvla on std=c99 calling excecutable \'cipher\' ")
    compiled = subprocess.run('gcc -Wextra -Wall -Wvla -std=c99 -lm cipher.c -o cipher', capture_output=True, text=True,
                              shell=True)
    if not compiled.check_returncode():
        print("compiled successfully!")
    else:

        print("compilation failed! check stderr below:\n", compiled.stderr)
        print(compiled.stderr, compiled.stdout)
        print(compiled.returncode)
        exit(1)

    if not checking_args():
        exit(1)
    if not basic_tests():
        exit(1)
    if not handle_files():
        exit(1)
    if not test_advance():
        exit(1)
    print("passed all tests!")
