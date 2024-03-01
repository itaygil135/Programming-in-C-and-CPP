#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATUS_OK                                    0
#define STATUS_ERROR_ENCODE_DECODE_INVALID_ARGS      1
#define STATUS_ERROR_CHECK_INVALID_ARGS              2
#define STATUS_ERROR_WRONG_COMMAND                   3
#define STATUS_ERROR_INVALID_FILE                    4

#define DATA_SIZE                               100000

static void change(int , char *, int , int );
static void encode(char *, int );
static void decode(char *, int );
static int valid(char);
static void print_error_message(int status);
static int  verify_args(int argc, char **argv);
static int read_input_file(char *p_input_file_name, char *p_data , int data_size);
static int write_output_file(char *p_output_file_name, char *p_data , int data_size);
static void check(char *p_clear_data,char *p_cipher_data);
int main(int argc, char **argv)
{
    char data[DATA_SIZE] = {0};
    char data2[DATA_SIZE] = {0};


    char *p_command = NULL ;
    char *p_k = NULL ;
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

        if (argc == 5)  // the command is either encode or decode
        {
            p_command = argv[1];
            k = atoi(argv[2]);
            p_input_file_name = argv[3];
            p_output_file_name = argv[4];

            status = read_input_file(p_input_file_name,data , DATA_SIZE);
            if (status != STATUS_OK)
            {
                print_error_message(status);
                break;
            }

            if (strcmp(p_command,"encode") == 0)
            {
                // handle encode
                encode(data,k);
                status = write_output_file(p_output_file_name, data , sizeof(data));
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
                status = write_output_file(p_output_file_name, data , sizeof(data));
                if (status != STATUS_OK)
                {
                    print_error_message(status);
                    break;
                }
            }
        }
        if (argc == 4)
        {
            p_command = argv[1];
            p_input_file_name = argv[2];
            p_output_file_name = argv[3];
            status1 = read_input_file(p_input_file_name,data , DATA_SIZE);
            status2 = read_input_file(p_output_file_name,data2 , DATA_SIZE);
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
                if (   ((p_clear_data[ind] >= 'a') && (p_clear_data[ind] <= 'z'))
                     ||
                       ((p_cipher_data[ind] >= 'A') && (p_cipher_data[ind] <= 'Z')))
                {
                    if (p_clear_data[ind] < p_cipher_data[ind] )
                    {
                        k =p_cipher_data[ind] - p_clear_data[ind];
                    }
                    else
                    {
                        k = 26 - (p_clear_data[ind] - p_cipher_data[ind]);
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
            printf("Valid encrypting with k =%d\n",k);
            return;
        }
    }
}

static int write_output_file(char *p_output_file_name, char *p_data , int data_size)
{
    FILE *p_out_file = NULL ;
    int local_status;
    p_out_file = fopen(p_output_file_name,"w");
    if (p_out_file == NULL)
    {
        return STATUS_ERROR_INVALID_FILE;
    }

    while (*p_data != NULL)
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

static int read_input_file(char *p_input_file_name, char *p_data , int data_size)
{
    FILE *p_in_file = NULL ;
    char in_char;

    p_in_file = fopen(p_input_file_name,"r");

   // p_in_file = fopen("C:\\huji\\c_cpp\\t1\\my_input.txt","r");
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
    int status = STATUS_OK ;
    char *p_command = NULL ;
    char *p_k = NULL ;
    char *p_in_filename = NULL ;
    char *p_out_filename = NULL ;

    if (argc == 1)
    {
        return STATUS_ERROR_WRONG_COMMAND;
    }
    p_command = argv[1];

    if ((strcmp (p_command, "decode") == 0)
        || (strcmp (p_command, "encode") == 0))
    {
        if (argc != 5)
        {
            return STATUS_ERROR_ENCODE_DECODE_INVALID_ARGS;
        }
    }
    else if  (strcmp (p_command, "check") == 0)
    {
        if (argc != 4)
        {
            return STATUS_ERROR_CHECK_INVALID_ARGS;
        }
    }
    else
    {
        return STATUS_ERROR_WRONG_COMMAND;
    }

}

static void decode(char *string, int k) {
  encode (string, -k);
}

static void encode(char *string, int k) {
    int new_k = k % 26;
    for (int i = 0; string[i] != 0; ++i) {
        int direct = valid(string[i]);
        if ((direct == 1) || (new_k == 0)) {}
        else {
            change(i, string, new_k, direct);
        }
    }
}

static void change(int inx, char *kod, int parm, int type) {
    int temp = kod[inx] + parm;
    if (type == 2) {
        if (temp > 'z') {
            kod[inx] = 'a' - 1 + temp - 'z';
        }
        else if (temp < 'a') {
            kod[inx] = 'z' + 1 + temp - 'a';
        }
        else {
            kod[inx] = temp;
        }
    }
    else {
        if (temp > 'Z') {
            kod[inx] = 'A' - 1 + temp - 'Z';
        }
        else if (temp < 'A') {
            kod[inx] = 'Z' + 1 + temp - 'A';
        }
        else {
            kod[inx] = temp;
        }
    }
}


static int valid(char x) {
    if ((x >= 'A') && (x <= 'Z')) {
        return 3;
    }
    else if ((x >= 'a') && (x <= 'z')) {
        return 2;
    }
    else {
        return 1 ;
    }
}


static void print_error_message(int status)
{
    if (status == STATUS_ERROR_ENCODE_DECODE_INVALID_ARGS)
        fprintf(stderr,"Usage: cipher >encode|decode< <k> <source path file> <output path file>\n");
    else if (status == STATUS_ERROR_CHECK_INVALID_ARGS)
        fprintf(stderr,"Usage: cipher <check> <source path file> <output path file>\n" );
    if (status == STATUS_ERROR_WRONG_COMMAND)
        fprintf(stderr,"The given command is invalid\n");
    if (status == STATUS_ERROR_INVALID_FILE)
        fprintf(stderr,"The given file is invalid\n" );
}
