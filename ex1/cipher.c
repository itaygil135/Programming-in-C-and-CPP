#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATUS_OK                                    0
#define STATUS_ERROR_ENCODE_DECODE_INVALID_ARGS      1
#define STATUS_ERROR_CHECK_INVALID_ARGS              2
#define STATUS_ERROR_WRONG_COMMAND                   3
#define STATUS_ERROR_INVALID_FILE                    4

#define DATA_SIZE                               100000
#define ENCODE_DECODE_ARGC                           5
#define CHECK_ARGC                                   4
#define NUM_OF_STEPS                                26


static void change(int inx, char *p_string, int parm, int type);
static void encode(char *p_string, int k);
static void decode(char *p_string, int k);
static int valid(char x);
static void print_error_message(int status);
static int  verify_args(int argc, char **argv);
static int read_input_file(char *p_input_file_name, char *p_data );
static int write_output_file(char *p_output_file_name, char *p_data );
static void check(char *p_clear_data,char *p_cipher_data);

int main(int argc, char **argv)
{
    char data[DATA_SIZE] = {0};
    char data2[DATA_SIZE] = {0};


    char *p_command = NULL ;
    char *p_input_file_name = NULL ;
    char *p_output_file_name = NULL ;

    int k ;
    int status = STATUS_OK;
    int status1 = STATUS_OK;
    int status2 = STATUS_OK;


    do {
        status = verify_args(argc, argv);
        if (status != STATUS_OK)
        {
            print_error_message(status);
            break;
        }

        if (argc == ENCODE_DECODE_ARGC)  // the command is either encode or decode
        {
            p_command = argv[1];
            k = atoi(argv[2]);
            p_input_file_name = argv[3];
            p_output_file_name = argv[4];

            status = read_input_file(p_input_file_name,data );
            if (status != STATUS_OK)
            {
                print_error_message(status);
                break;
            }

            if (strcmp(p_command,"encode") == 0)
            {
                // handle encode
                encode(data,k);
                status = write_output_file(p_output_file_name, data );
                if (status != STATUS_OK)
                {
                    print_error_message(status);
                    break;
                }
            }
            else if (strcmp (p_command, "decode") == 0)
            {
                // handle decode
                decode(data,k);
                status = write_output_file(p_output_file_name, data);
                if (status != STATUS_OK)
                {
                    print_error_message(status);
                    break;
                }
            }
        }
        if (argc == CHECK_ARGC)
        {
            p_command = argv[1];
            p_input_file_name = argv[2];
            p_output_file_name = argv[3];
            status1 = read_input_file(p_input_file_name, data);
            status2 = read_input_file(p_output_file_name, data2);
            if ((status1 != STATUS_OK) || (status2 != STATUS_OK))
            {
                print_error_message(STATUS_ERROR_INVALID_FILE);
                break;
            }


            check(data,data2);

        }

    } while (0);


    if (status == STATUS_OK)
    {
        return EXIT_SUCCESS;
    }
    else
    {
        return EXIT_FAILURE;
    }
}



static void check(char *p_clear_data,char *p_cipher_data)
{
     int k = 0;
    int len = strlen(p_clear_data);
    if (strlen(p_clear_data) != strlen(p_cipher_data))
    {
        printf("Invalid encrypting\n");
        return;
    }
    else
    {
        int ind = 0 ;

        while (ind < len)
        {
            if (p_clear_data[ind] != p_cipher_data[ind])
            {
                if (   ((p_clear_data[ind] >= 'a')
                && (p_clear_data[ind] <= 'z'))
                     ||
                       ((p_cipher_data[ind] >= 'A')
                       && (p_cipher_data[ind] <= 'Z')))
                {
                    if (p_clear_data[ind] < p_cipher_data[ind] )
                    {
                        k =p_cipher_data[ind] - p_clear_data[ind];
                    }
                    else
                    {
                        k = NUM_OF_STEPS - (p_clear_data[ind] - p_cipher_data[ind]);
                    }
                    encode (p_clear_data, k);
                }
                else
                {
                    printf("Invalid encrypting\n");
                    return;
                }
                break ;
            }
            ind++ ;
        }

        if (strcmp(p_clear_data,p_cipher_data) != 0)
        {
            printf("Invalid encrypting\n");
            return ;
        }
        else
        {
            printf("Valid encrypting with k = %d\n",k);
            return;
        }
    }
}

static int write_output_file(char *p_output_file_name, char *p_data)
{
    FILE *p_out_file = NULL ;
    int local_status;
    p_out_file = fopen(p_output_file_name,"w");
    if (p_out_file == NULL)
    {
        return STATUS_ERROR_INVALID_FILE;
    }

    while (*p_data != 0)
    {

        local_status= fputc((int)(*p_data), p_out_file );
        if (local_status == EOF)
        {
            fclose(p_out_file);
            return STATUS_ERROR_INVALID_FILE;
        }
        p_data++;
    }

    fclose(p_out_file);
    return STATUS_OK;
}

static int read_input_file(char *p_input_file_name, char *p_data)
{
    FILE *p_in_file = NULL ;
    char in_char;

    p_in_file = fopen(p_input_file_name,"r");

      if (p_in_file == NULL)
    {
        return STATUS_ERROR_INVALID_FILE;
    }

    while ((in_char = (char)fgetc(p_in_file)) != EOF)
    {
        *p_data = in_char;
        p_data++;
    }
    *p_data = 0; // adding the NULL to the end of the array
    fclose(p_in_file);

    return STATUS_OK;
}


static int  verify_args(int argc, char **argv)
{
    char *p_command = NULL ;

    if (argc == 1)
    {
        return STATUS_ERROR_WRONG_COMMAND;
    }
    p_command = argv[1];

    if ((strcmp (p_command, "decode") == 0)
        || (strcmp (p_command, "encode") == 0))
    {
        if (argc != ENCODE_DECODE_ARGC)
        {
            return STATUS_ERROR_ENCODE_DECODE_INVALID_ARGS;
        }
    }
    else if  (strcmp (p_command, "check") == 0)
    {
        if (argc != CHECK_ARGC)
        {
            return STATUS_ERROR_CHECK_INVALID_ARGS;
        }
    }
    else
    {
        return STATUS_ERROR_WRONG_COMMAND;
    }

    return STATUS_OK;
}

static void decode(char *p_string, int k)
{
  encode (p_string, -k);
}

static void encode(char *p_string, int k)
{
    int new_k = k % NUM_OF_STEPS;
    for (int i = 0; p_string[i] != 0; ++i)
    {
        int direct = valid(p_string[i]);
        if ((direct == 1) || (new_k == 0)) {}
        else
        {
            change(i, p_string, new_k, direct);
        }
    }
}

static void change(int inx, char *p_string, int parm, int type)
{
    int temp = p_string[inx] + parm;
    if (type == 2)
    {
        if (temp > 'z')
        {
            p_string[inx] = 'a' - 1 + temp - 'z';
        }
        else if (temp < 'a')
        {
            p_string[inx] = 'z' + 1 + temp - 'a';
        }
        else
        {
            p_string[inx] = temp;
        }
    }
    else {
        if (temp > 'Z')
        {
            p_string[inx] = 'A' - 1 + temp - 'Z';
        }
        else if (temp < 'A')
        {
            p_string[inx] = 'Z' + 1 + temp - 'A';
        }
        else
        {
            p_string[inx] = temp;
        }
    }
}


static int valid(char x)
{
    if ((x >= 'A') && (x <= 'Z'))
    {
        return 3;
    }
    else if ((x >= 'a') && (x <= 'z'))
    {
        return 2;
    }
    else
    {
        return 1 ;
    }
}


static void print_error_message(int status)
{
    if (status == STATUS_ERROR_ENCODE_DECODE_INVALID_ARGS)
        fprintf(stderr,
"Usage: cipher >encode|decode< <k> <source path file> <output path file>\n");
    else if (status == STATUS_ERROR_CHECK_INVALID_ARGS)
        fprintf(stderr,
         "Usage: cipher <check> <source path file> <output path file>\n");
    if (status == STATUS_ERROR_WRONG_COMMAND)
        fprintf(stderr,"The given command is invalid\n");
    if (status == STATUS_ERROR_INVALID_FILE)
        fprintf(stderr,"The given file is invalid\n" );
}
