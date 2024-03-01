#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATUS_OK                                    0
#define STATUS_ERROR                                 1
#define STATUS_ERROR_WRONG_ARGC                      2
#define STATUS_ERROR_WRONG_COMMAND                   3

#define CMD_BEST "best"
#define CMD_BUBBLE "bubble"
#define CMD_QUICK "quick"

#define VALID_ARGC                                   2
#define ARGV_CMD_INDEX                               1

typedef struct Student
{
    long id;
    int grade, age;
} Student;

static int  verify_args(int argc, char **argv);
static int read_num_of_students(int *p_n);
static int read_user_data(Student *student_list, int num_of_students);
static void  best_student(Student *start, Student *end);
static  int validate_age(char *p_start, char *p_end, int *p_student_age);
static  int validate_grade(char *p_start, char *p_end, int *p_student_grade);
static  int validate_id(char *p_start, char *p_end, long int *p_student_id);
static void swap(Student  *p_a, Student *p_b);
static void quick_sort_helper(Student *start, Student *end);
static void print_students_list(Student *p_start, Student *p_end);
void bubble_sort(Student *start, Student *end);
void quick_sort(Student *start, Student *end);
Student *partition(Student *start, Student *end);

int main(int argc, char **argv)
{
    char *p_command = NULL ;
    int n = 0 ;
    int status = STATUS_OK;

    do {
        status = verify_args(argc, argv);
        if (status != STATUS_OK)
        {
            fprintf(stdout,"USAGE: manageStudent <best|bubble|quick>\n");
            break;
        }

        p_command = argv[ARGV_CMD_INDEX];

        // read the number of the students
        status = read_num_of_students(&n);
        if (status != STATUS_OK)
        {
            break;
        }

        //  allocate the array of the students
        Student *student_list = (Student*) malloc(sizeof(Student)*n);
        if (student_list == NULL)
        {
            status = STATUS_ERROR;
            break;
        }
        // read the students info from the user
        status = read_user_data(student_list, n);
        if (status != STATUS_OK)
        {
            break;
        }

        if (strcmp(p_command,CMD_BEST) == 0)
        {
            best_student(student_list, student_list + n);

        }
        else if (strcmp (p_command, CMD_BUBBLE) == 0)
        {
            bubble_sort(student_list, student_list + n);
        }
        else if (strcmp (p_command, CMD_QUICK) == 0)
        {
            quick_sort(student_list, student_list + n);
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

static void  best_student(Student *start, Student *end)
{
    Student *p_best = start;
    while (start < end){
        if ((start->grade/start->age) > (p_best->grade/p_best->age))
        {
            p_best = start;
        }

        start++;
    }
    printf("best student info is: %ld,%d,%d", p_best->id,p_best->grade,p_best->age);
}
static int read_num_of_students(int *p_n)
{

    char buffer[20];

    while (1)
    {
        printf("Enter number of students. Then enter \n");
        if (fgets(buffer, 20, stdin) == NULL)
        {
            return STATUS_ERROR;
        }
        int flag = STATUS_OK;
        int i = 0;
        while ( buffer[i] != '\n')
        {
            if ((buffer[i] < 48) || (buffer[i] > 57))
            {
                flag = STATUS_ERROR;
                printf("ERROR: wrong input, please type again \n");
                break;
            }
            i++;
        }
        if (flag == STATUS_OK)
        {
            int num_of_numbers = sscanf(buffer, "%d", p_n);
            if (num_of_numbers == 0) {
                return STATUS_ERROR;
            }
            if (*p_n > 0)
            {
                return STATUS_OK;
            }
            else
            {
                printf("ERROR: wrong input, please type again \n");
            }
        }
    }
    return STATUS_OK;
}


static int get_num_from_buffer(char *p_start, char *p_end, long int *p_number)
{
    char * p_digit = p_start;
    char *p = NULL;
    while ( p_digit < p_end)
    {
        if ((*p_digit < 48) || (*p_digit > 57))
        {
            return STATUS_ERROR;
        }
        p_digit++;
    }

    *p_number = strtol(p_start, &p,10);

    return STATUS_OK;
}
static int read_user_data(Student *student_list, int num_of_students)
{
    char line[60];

    char *p_end = NULL;
    char * p_start = NULL;
    Student student;
    int valid_num_of_students = 0;
    while (valid_num_of_students < num_of_students)
    {
        printf("Enter student info. Then enter\n");
        if (fgets(line, 60, stdin) == NULL)
        {
            return STATUS_ERROR;
        }
        p_start = line ;
        p_end = strchr(p_start,',');
        if (validate_id(p_start,p_end, &student.id) != STATUS_OK)
        {
            continue;
        }
        p_start = p_end + 1;
        p_end = strchr(p_start,',');
        if (validate_grade(p_start, p_end, &student.grade) != STATUS_OK)
        {
            continue;
        }
        p_start = p_end + 1;
        p_end = strchr(p_start,'\n');
        if (validate_age(p_start,p_end, &student.age) != STATUS_OK)
        {
            continue;
        }

        student_list[valid_num_of_students].age = student.age;
        student_list[valid_num_of_students].id = student.id;
        student_list[valid_num_of_students].grade = student.grade;
        valid_num_of_students++;
    }

    return STATUS_OK;
}

static  int validate_age(char *p_start, char *p_end, int *p_student_age)
{
    long int age ;
    if (get_num_from_buffer(p_start, p_end, &age) != STATUS_OK)
    {
        printf("ERROR: Age should be an integer between 18 and 120 (includes)\n");
        return STATUS_ERROR;
    }
    if ((age < 18) || (age > 120))
    {
        printf("ERROR: Age should be an integer between 18 and 120 (includes).\n");
        return STATUS_ERROR;
    }
    *p_student_age = (int)age;

    return STATUS_OK;
}
static void swap(Student  *p_a, Student *p_b)
{
    Student cache = *p_a;
    *p_a = *p_b;
    *p_b = cache;
}
void bubble_sort(Student *start, Student *end)
{
    unsigned long  n, counter, moving;
    n = (unsigned long)(end - start);

    for (counter = 0 ; counter < ( n - 1 ); counter++) {
        for (moving = 0 ; moving < n - counter - 1; moving++) {
            if ((start + moving)->grade > (start + moving + 1)->grade) {
                swap((start + moving), (start + moving + 1));
            }
        }
    }
    print_students_list(start, end);
}

Student *partition(Student *start, Student *end){
    Student *p_pivot = end - 1 ;
    Student *p_low = start - 1 ;
    Student *p_moving = start ;
    while (p_moving < p_pivot){
        if (p_moving->age <= p_pivot->age){
            p_low++;
            swap(p_low,p_moving);
        }
        p_moving++;
    }
    swap(p_low+1,p_pivot);
    return p_low+1;
}

void quick_sort(Student *start, Student *end){
        quick_sort_helper(start,end);
        print_students_list(start,end);
}

static void quick_sort_helper(Student *start, Student *end){
    if(((int)(end-start)) > 1){
        Student *pivot = partition(start,end);
        quick_sort_helper(start,pivot);
        quick_sort_helper(pivot+1,end);
    }
}


static  int validate_grade(char *p_start, char *p_end, int *p_student_grade)
{
    long int grade ;
    if (get_num_from_buffer(p_start, p_end, &grade) != STATUS_OK)
    {
        printf("ERROR: Grade should be an integer between 0 and 100 (includes).\n");
        return STATUS_ERROR;
    }
    if ((grade <= 100) && (grade >= 0))
    {
        *p_student_grade = (int)grade;
        return STATUS_OK;
    }
    printf("ERROR: Grade should be an integer between 0 and 100 (includes).\n");
    return STATUS_ERROR;
    }
static  int validate_id(char *p_start, char *p_end, long int *p_student_id)
{
    long int id ;
    if (get_num_from_buffer(p_start, p_end, &id) != STATUS_OK)
    {
        printf("ERROR: Id must have 10 digits and can not start with 0.\n");
        return STATUS_ERROR;
    }


    if ((*p_start == '0') || ((int)(p_end - p_start) != 10))
    {
        printf("ERROR: Id must have 10 digits and can not start with 0.\n");
        return STATUS_ERROR;
    }
    *p_student_id = (int)id;

    return STATUS_OK;
}

static int  verify_args(int argc, char **argv)
{
    char *p_command = NULL ;

    if (argc != VALID_ARGC)
    {
        return STATUS_ERROR_WRONG_ARGC;
    }
    p_command = argv[ARGV_CMD_INDEX];

    if ((strcmp (p_command, "best") != 0)
        && (strcmp (p_command, "bubble") != 0)
           && (strcmp (p_command, "quick") != 0))
    {
        return STATUS_ERROR_WRONG_COMMAND;
    }

    return STATUS_OK;
}
static void print_students_list(Student *p_start, Student *p_end)
{
    Student *p_current = p_start;
    while (p_current < p_end){
        printf("%ld,%d,%d\n",p_current->id,p_current->grade,p_current->age);
        p_current++;
    }
}


