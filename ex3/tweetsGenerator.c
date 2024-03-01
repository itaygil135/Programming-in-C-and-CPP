#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BASE                                         10
#define MAX_WORDS_IN_SENTENCE_GENERATION 20
#define MAX_WORD_LENGTH 100
#define MAX_SENTENCE_LENGTH 1000
#define STATUS_OK                                    0
#define STATUS_ERROR                                 1
#define ARGC_WITHOUT_LIMIT                           4
#define ARGC_WITH_LIMIT                              5
#define True 0
#define False 1
#define PERIOD                                      '.'
typedef struct WordProbability {
    struct WordStruct *word_struct_ptr;
    int counter_as_follower;
   	float probability;
} WordProbability;

typedef struct WordStruct {
  char *word;
  struct WordProbability *prob_list;
  int  counter_in_text;
  int  counter_of_followers;
  int  counter_of_different_followers;
} WordStruct;



/************ LINKED LIST ************/
typedef struct Node {
  WordStruct *data;
  struct Node *next;
} Node;

typedef struct LinkList {
  Node *first;
  Node *last;
  int size;
} LinkList;
static WordStruct* add_new_node_to_dict(LinkList *dictionary,char *token);
static WordStruct* find_word_in_dict(LinkList *dictionary,char *token);
static void update_word_struct(WordStruct* p_current);
//static void update_as_follower(LinkList *dictionary,
//                               WordStruct* p_previous_word,
//                               WordStruct* p_current);
static void update_dict_probability(LinkList *dictionary);
static void generate_multiple_sentence(LinkList *dictionary,
                                       int num_of_tweets,
                                       int seed);
static int  verify_args(int argc, char **argv,
                        int *p_seed,
                        int *p_num_of_tweets,
                        char **pp_file_path,
                        int *p_max_words);
static void add_to_dictionary(struct LinkList *dictionary, char *token);
static int find_it_in_int(char* string_of_num);
//static void print_dictionary(struct LinkList *dictionary);

/**
 * Add data to new node at the end of the given link list.
 * @param link_list Link list to add data to
 * @param data pointer to dynamically allocated data
 * @return 0 on success, 1 otherwise
 */
int add(LinkList *link_list, WordStruct *data)
{
  Node *new_node = malloc(sizeof(Node));
  if (new_node == NULL)
  {
      fprintf(stdout,"Allocation failure: failed to allocate new node");
      return STATUS_ERROR;
  }
  *new_node = (Node){data, NULL};

  if (link_list->first == NULL)
  {
	link_list->first = new_node;
	link_list->last = new_node;
  }
  else
  {
	link_list->last->next = new_node;
	link_list->last = new_node;
  }


  link_list->size++;
  return STATUS_OK;
}
/*************************************/

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number
 * @return Random number
 */
int get_random_number(int max_number)
{
 static int i = 0;
 i++;
    int random_number = rand();
    return random_number%max_number;
}

/**
 * Choose randomly the next word from the given dictionary, drawn uniformly.
 * The function won't return a word that end's in full stop '.' (Nekuda).
 * @param dictionary Dictionary to choose a word from
 * @return WordStruct of the chosen word
 */
WordStruct *get_first_random_word(LinkList *dictionary)
{
    int our_choice = get_random_number(dictionary->size);
    int count = 0;
    Node* go_to_first = dictionary->first;
    while (count < our_choice)
    {
        go_to_first = go_to_first->next;
        (count)++;
    }
    if(go_to_first->data->word[strlen(go_to_first->data->word)-1] == PERIOD)
    {
        return NULL;
    }
    return go_to_first->data;

}

/**
 * Choose randomly the next word. Depend on it's occurrence frequency
 * in word_struct_ptr->WordProbability.
 * @param word_struct_ptr WordStruct to choose from
 * @return WordStruct of the chosen word
 */
WordStruct *get_next_random_word(WordStruct *word_struct_ptr)
{
    int current_follower = 0;
    int our_choice = get_random_number(word_struct_ptr->counter_of_followers);
    while (word_struct_ptr->prob_list[current_follower].counter_as_follower
            < our_choice)
    {
        our_choice = our_choice -
             word_struct_ptr->prob_list[current_follower].counter_as_follower;
        current_follower++;
    }
    return word_struct_ptr->prob_list[current_follower].word_struct_ptr;
}

/**
 * Receive dictionary, generate and print to stdout random sentence out of it.
 * The sentence most have at least 2 words in it.
 * @param dictionary Dictionary to use
 * @return Amount of words in printed sentence
 */
int generate_sentence(LinkList *dictionary)
{
    static int tweet_num = 1;
    int words_num_is = 0;
    int word_counter = 0 ;
    WordStruct *current_word = NULL;

    fprintf(stdout,"Tweet %d: ",tweet_num);
    tweet_num++;
    while (current_word == NULL)
    {
        current_word = get_first_random_word(dictionary);
    }
    while (words_num_is < MAX_WORDS_IN_SENTENCE_GENERATION)
    {
        fprintf(stdout,"%s ",current_word->word);
        word_counter++;

        if(current_word->word[strlen(current_word->word)-1] == PERIOD)
        {
            break;
        }
        words_num_is++;
        current_word = get_next_random_word(current_word);
    }
    fprintf(stdout,"\n");
    return word_counter;
}

/**
 * Gets 2 WordStructs. If second_word in first_word's prob_list,
 * update the existing probability value.
 * Otherwise, add the second word to the prob_list of the first word.
 * @param first_word
 * @param second_word
 * @return 0 if already in list, 1 otherwise.
 */
int add_word_to_probability_list(WordStruct *first_word,
                                 WordStruct *second_word)
{
    if(first_word->word[strlen(first_word->word)-1] != PERIOD)
    {
        (first_word->counter_of_followers)++;
        if(first_word->prob_list == NULL)
        {
            first_word->prob_list = malloc(sizeof(WordProbability));
            if(first_word->prob_list == NULL)
            {
                fprintf(stdout,"Allocation failure: failed to allocate array");
                exit(EXIT_FAILURE);
            }
            (first_word->counter_of_different_followers)++;
            first_word->prob_list->counter_as_follower = 1;

            first_word->prob_list->word_struct_ptr = second_word;
        }
        else
        {
            int i = 0;
            for(i=0; i<first_word->counter_of_different_followers; i++)
            {
                if(strcmp(first_word->prob_list[i].word_struct_ptr->word,
                          second_word->word) == True)
                {
                    (first_word->prob_list[i].counter_as_follower)++;
                    break;

                }
            }
            if(i == first_word->counter_of_different_followers)
            {
                WordProbability *p =
                        realloc(first_word->prob_list,
                              (first_word->counter_of_different_followers + 1)
                               * sizeof(struct WordProbability));
                if(p == NULL)
                {
                    fprintf(stdout,"Allocation failure: failed to allocate");
                    exit(EXIT_FAILURE);
                }
                first_word->prob_list = p;
                first_word->prob_list[i].word_struct_ptr = second_word;
                first_word->prob_list[i].counter_as_follower = 1;
                (first_word->counter_of_different_followers)++;
            }
        }
    }
}

/**
 * Read word from the given file. Add every unique word to the dictionary.
 * Also, at every iteration, update the prob_list of the previous word with
 * the value of the current word.
 * @param fp File pointer
 * @param words_to_read Number of words to read from file.
 *                      If value is bigger than the file's word count,
 *                      or if words_to_read == -1 than read entire file.
 * @param dictionary Empty dictionary to fill
 */
void fill_dictionary(FILE *fp, int words_to_read, LinkList *dictionary)
{
    // add_word_to_probability_list
    char buffer[MAX_SENTENCE_LENGTH];
    int b_keep_reading = True;
    char  *line = NULL;
    int already_read = 0;
    const char s[] = " \n\r";
    char *token = NULL;
    while(b_keep_reading == True)
    {
        line = fgets(buffer,MAX_SENTENCE_LENGTH,fp);
        if(line == NULL){
            b_keep_reading = False;
            break;
        }
       // buffer[strlen(line)-1]=' ';

        /* get the first token */
        token = strtok(line, s);

        /* walk through other tokens */
        while( token != NULL ) {
             already_read++;
            if((already_read > words_to_read)&&(words_to_read>0)){
                b_keep_reading = False;
                break;
            }

            add_to_dictionary(dictionary,token);
            //printf( " %s\n", token );

            token = strtok(NULL, s);
        }
    }
    update_dict_probability(dictionary);
}

/**
 * Free the given dictionary and all of it's content from memory.
 * @param dictionary Dictionary to free
 */
void free_dictionary(LinkList *dictionary)
{// we want to free al the word stract and all the nodes that there is here
//and all the probability array
    Node * help_to_free = NULL;
    while (dictionary->first != NULL)
    {
        free(dictionary->first->data->prob_list);
        free(dictionary->first->data->word);
        free(dictionary->first->data);
        help_to_free = dictionary->first;
        dictionary->first = dictionary->first->next;
        free(help_to_free);
    }
}

/**
 * @param argc
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 *             3) Path to file
 *             4) Optional - Number of words to read
 */
int main(int argc, char *argv[]) {
    int status = STATUS_OK;
    int words_to_read = 0;
    char *p_file_path = NULL;
    int num_of_tweets = 0;
    int seed = 0;

    struct LinkList dictionary = {0};
    FILE *fp = NULL;

    do {
        status = verify_args(argc,
                             argv,
                             &seed,
                             &num_of_tweets,
                             &p_file_path,
                             &words_to_read);
        if (status != STATUS_OK) {
            break;
        }

        fp = fopen(p_file_path, "r");
        if (fp == NULL) {
            fprintf(stdout, "Error: error reading file");
            status = STATUS_ERROR;
            break;
        }

        fill_dictionary(fp, words_to_read, &dictionary);
        generate_multiple_sentence(&dictionary, num_of_tweets,seed);

    } while (0);

    //if (status == STATUS_OK)
        //print_dictionary(&dictionary);



    free_dictionary(&dictionary);
    if (fp != NULL)
    {
        (void)fclose(fp);
    }


    if (status == STATUS_OK)
    {
        return EXIT_SUCCESS;
    }
    else
    {
        return EXIT_FAILURE;
    }
}

//static void print_dictionary(struct LinkList *dictionary)
//{
//    Node *p_node = dictionary->first;
//    int i = 1;
//    int j=0;
//    while (p_node != NULL)
//    {
//        printf("word # %d word ==%s\n",i,p_node->data->word);
//        printf("word # %d counter_of_different_followers=%d\n",
//               i,
//               p_node->data->counter_of_different_followers);
//        printf("word # %d counter_of_followers=%d\n",
//               i,
//               p_node->data->counter_of_followers);
//        printf("word # %d counter_in_text=%d\n",
//               i,
//               p_node->data->counter_in_text);
//        if (p_node->data->counter_of_different_followers > 1)
//        {
//    printf("");
//        }
//        for (j = 0 ; j < p_node->data->counter_of_different_followers; j++)
//        {
//            printf("word # %d follower word =%s\n",
//                   i,
//                   p_node->data->prob_list[j].word_struct_ptr->word);
//            printf("word # %d counter_as_follower=%d\n",
//                   i, p_node->data->prob_list[j].counter_as_follower);
//            printf("word # %d probability=%f\n", i,
//                   p_node->data->prob_list[j].probability);
//        }
//
//
//
//        p_node = p_node->next;
//        i++;
//    }


static void generate_multiple_sentence(LinkList *dictionary,
                                       int num_of_tweets,
                                       int seed)
{
//    int call_to_generate = 0;
//    int num_of_words = 0;
//    for(call_to_generate = 0; call_to_generate < 3;call_to_generate++)
//    {
//        num_of_words = generate_sentence(dictionary);
//
//    }

    srand(seed);
    int i = 0;
    while (i < num_of_tweets)
    {
        generate_sentence(dictionary);
        i++;
    }
}



static void update_dict_probability(LinkList *dictionary)
{
    Node *run_on_nodes = dictionary->first;
    while(run_on_nodes != NULL)
    {
        int i = 0;
        for(i=0; i < run_on_nodes->data->counter_of_different_followers; i++)
        {
            run_on_nodes->data->prob_list[i].probability =
            (float)run_on_nodes->data->prob_list[i].counter_as_follower /
            (float)run_on_nodes->data->counter_of_followers;
        }
        run_on_nodes = run_on_nodes->next;
    }
}
static WordStruct* add_new_node_to_dict(LinkList *dictionary,char *token)
{
    int status = STATUS_OK;
    char * p_word = malloc(strlen(token)+1);
    WordStruct *p_current = malloc(sizeof(WordStruct));
    if ((p_current != NULL) && (p_word != NULL))
    {
        p_current->counter_in_text = 1;
        p_current->counter_of_followers = 0;
        p_current->counter_of_different_followers = 0;
        p_current->prob_list = NULL;
        memcpy(p_word, token, strlen(token)+1);
        p_current->word = p_word;

        status = add(dictionary, p_current);
    }
    else
    {
        fprintf(stdout,"Allocation failure: failed to allocate memory");
        status = STATUS_ERROR;
    }

    if (status == STATUS_ERROR)
    {
        if (p_word != NULL)
        {
            free(p_word);
        }
        if (p_current != NULL)
        {
            free(p_current);
            p_current = NULL;
        }
        free_dictionary(dictionary);
        exit(EXIT_FAILURE);
    }
    return p_current;
}
static WordStruct* find_word_in_dict(LinkList *dictionary,char *token)
{
    struct WordStruct *p_current = NULL;
    Node *p_running_node = dictionary->first;
    while(p_running_node != NULL)
    {
        if(strcmp(p_running_node->data->word,token) == True)
        {
            p_current = p_running_node->data;
            break;
        }
        p_running_node = p_running_node->next;
    }
    return p_current;
}
static void update_word_struct(WordStruct* p_current)
{
    p_current->counter_in_text = p_current->counter_in_text + 1;
}
//static void update_as_follower(LinkList *dictionary,
//                               WordStruct* p_previous_word,
//                               WordStruct* p_current)
//{
//    if(p_previous_word->word[strlen(p_previous_word->word)-1] != PERIOD)
//    {
//        (p_previous_word->counter_of_followers)++;
//        if(p_previous_word->prob_list == NULL)
//        {
//            p_previous_word->prob_list = malloc(sizeof(WordProbability));
//            if(p_previous_word->prob_list == NULL)
//            {
//                free_dictionary(dictionary);
//                exit(EXIT_FAILURE);
//            }
//            (p_previous_word->counter_of_different_followers)++;
//             p_previous_word->prob_list->counter_as_follower = 1;
//
//             p_previous_word->prob_list->word_struct_ptr = p_current;
//        }
//        else
//        {
//            int i = 0;
//            for(i=0; i<p_previous_word->counter_of_different_followers; i++)
//            {
//                if(strcmp(p_previous_word->prob_list[i].word_struct_ptr->word,
//                          p_current->word) == True)
//                {
//                    (p_previous_word->prob_list[i].counter_as_follower)++;
//                    break;
//
//                }
//            }
//            if(i == p_previous_word->counter_of_different_followers)
//            {
//                WordProbability *p =realloc(p_previous_word->prob_list,
//                        (p_previous_word->counter_of_different_followers + 1)
//                        * sizeof(struct WordProbability));
//                if(p == NULL)
//                {
//                    free_dictionary(dictionary);
//                    exit(EXIT_FAILURE);
//                }
//                p_previous_word->prob_list = p;
//                p_previous_word->prob_list[i].word_struct_ptr = p_current;
//                p_previous_word->prob_list[i].counter_as_follower = 1;
//                (p_previous_word->counter_of_different_followers)++;
//            }
//        }
//    }
//}
static void add_to_dictionary(struct LinkList *dictionary, char *token)
{
    static WordStruct *p_previous_word = NULL;
    WordStruct * p_current = NULL;
    if(dictionary->first == NULL)
    {
        p_current =  add_new_node_to_dict(dictionary,token);
    }
    else
    {
        p_current = find_word_in_dict(dictionary,token);
        if(p_current != NULL)
        {
            update_word_struct(p_current);
            add_word_to_probability_list(p_previous_word,p_current);
        }
        else
        {
            p_current = add_new_node_to_dict(dictionary,token);
            if (p_current != NULL)
            {
                add_word_to_probability_list(p_previous_word, p_current);
            }
        }
    }
    p_previous_word = p_current;
}


static int  verify_args(int argc, char **argv,
                        int *p_seed,
                        int *p_num_of_tweets,
                        char **pp_file_path,
                        int *p_max_words)

{

    if((argc != ARGC_WITHOUT_LIMIT) && (argc != ARGC_WITH_LIMIT)){
        fprintf(stdout,"Usage: tweetsGenerator <seed> <tweet_num> <path> "
                       "[num_of_words] \n ");
        return STATUS_ERROR;
    }


    *p_seed = find_it_in_int(argv[1]);
    *p_num_of_tweets = find_it_in_int(argv[2]);
    *pp_file_path = argv[3];
    if(argc ==5){
        *p_max_words = find_it_in_int(argv[4]);
    }
    else{
        *p_max_words = 0;
    }
    return STATUS_OK;
}


static int find_it_in_int(char* string_of_num){
    char * p_start;
    char * p_end;
    p_start = string_of_num;
    p_end = p_start + strlen(string_of_num);
    return (int)strtol(p_start,&p_end,BASE);

}












[kkk]


















typedef void *ExtraDetails;
typedef const void *ConstExtraDetails;

typedef void(*PrintExtraDetailsFunc)(ConstExtraDetails);
typedef void(*FreeExtraDetailsFunc)(ConstExtraDetails);
typedef void(*CopyExtraDetailsFunc)(ExtraDetails dest, ConstExtraDetails src);


void noPrint(ConstExtraDetails a){}
void noFree(ConstExtraDetails a ){}
void noCopy(ExtraDetails dest, ConstExtraDetails src){}

typedef struct Date
{
    unsigned short month: 4;
    unsigned short day: 5;
} Date;

typedef struct Event
{

    char *title;
    ExtraDetails extraDetails; // can be NULL
    Date date;
} Event;

typedef struct Calendar
{
    Event *months[NUM_OF_MONTHS];
    size_t numEvents[NUM_OF_MONTHS]; // number of events in each month
    CopyExtraDetailsFunc copyFunc; // used to copy extraDetails variables
    FreeExtraDetailsFunc freeFunc; // used to free extraDetails variables
    PrintExtraDetailsFunc printFunc; // used to print extraDetails variables
    size_t extraDetailsSize; // the size of an extraDetails variable
} Calendar;

void printCalendar(const Calendar *calendar); // forward declaration - you do not need to use it or implement it in this question!

/***************  Implement from here ***************/


/**
* Allocate a calendar.
* @param extraDetailsSize - actual size of the item the ExtraDetails variables point to
* @param copyFunc - pointer to function that copies one ExtraDetails variable to another.
You can assume this argument is not NULL
* @param freeFunc - pointer to function that frees the variable an ExtraDetails variable points to.
You can assume this argument is not NULL
* @param printFunc - pointer to function that prints the variable an ExtraDetails variable points to.
You can assume this argument is not NULL
* @return allocated calendar, zero-initialized and given fields set
*/
int elemcmp(Event elem1 , Date date ,  const char* title)
{
    if(elem1.date.day == date.day)
    {
        return strcmp(elem1.title, title);
    }
    else
    {
        return elem1.date.day - date.day;
    }
}

Calendar *calendarAlloc(size_t extraDetailsSize, CopyExtraDetailsFunc copyFunc,
                        FreeExtraDetailsFunc freeFunc, PrintExtraDetailsFunc printFunc)
{
    Calendar *cal;
    cal = (Calendar*)calloc(1 ,sizeof(Calendar));
    if(cal != NULL)
    {

        cal->copyFunc = copyFunc;
        cal->freeFunc = freeFunc;
        cal->printFunc = printFunc;
        cal->extraDetailsSize = extraDetailsSize;
    }
    return cal;
}
/**
* Adds a new event to the calendar, in the given date. Duplicates (same title, same date)
* are not allowed.
* @param calendar - calendar to add the event to
* @param title - the title of the new event
* @param date - the date of the new event
* @param extraDetails - the extra details of the new event
* @return 1 if the event was added successfully, 0 otherwise.
*/
int addEvent(Calendar *calendar, const char *title, Date date, ConstExtraDetails extraDetails)
{

    if(calendar->numEvents[date.month] == 0)
    {

        calendar->months[date.month] = (Event*)malloc(sizeof(Event));
        if(calendar->months[date.month] != NULL)
        {
            calendar->months[date.month][0].title = (char *)malloc(strlen(title)+1);
            if (calendar->months[date.month][0].title  == NULL) { return 0;}
            strcpy(calendar->months[date.month][0].title , title);



            calendar->months[date.month][0].extraDetails= malloc(calendar->extraDetailsSize);
            if(calendar->months[date.month][0].extraDetails == NULL){return 0;}
            calendar->copyFunc(calendar->months[date.month][0].extraDetails,extraDetails);
            calendar->months[date.month][0].date = date;
            calendar->numEvents[date.month]++;
            return 1;
        }
        return 0;
    }

    else
    {
        int i = 0;
        for( i = 0; i <calendar->numEvents[date.month];i++)
        {
            int direct = elemcmp(calendar->months[date.month][i],date,title);
            if(direct == 0)
            {
                return 0;
            }
            else if(direct < 0){/*i++;*/}
            else{break;}
        }
        unsigned int num = calendar->numEvents[date.month];
        Event* p;
        p = (Event*)realloc(calendar->months[date.month],(num+1)*sizeof(Event));
        if ( p == NULL) {return 0;}

        calendar->months[date.month] = (Event *)p;
//realloc(calendar->months[date.month],(num+1)*sizeof(Event));
        if(calendar->months[date.month] == NULL){return 0;}

        while(num != i)
        {
//calendar->months[date.month][num] = calendar->months[date.month][num-1];
            calendar->months[date.month][num].title = calendar->months[date.month][num-1].title;
            calendar->months[date.month][num].date = calendar->months[date.month][num-1].date;
            calendar->months[date.month][num].extraDetails = calendar->months[date.month][num-1].extraDetails;

            (num--);
        }

        calendar->months[date.month][i].title = (char *)malloc(strlen(title)+1);
        if (calendar->months[date.month][i].title  == NULL) { return 0;}
        strcpy(calendar->months[date.month][i].title , title);
        calendar->months[date.month][i].extraDetails = malloc(calendar->extraDetailsSize);
        if(calendar->months[date.month][i].extraDetails == NULL){return 0;}
        calendar->copyFunc(calendar->months[date.month][i].extraDetails,extraDetails);
        calendar->months[date.month][i].date = date;
        calendar->numEvents[date.month]++;
        return 1;
    }
}




/**
* frees all resources of a calendar.
* @param calendar pointer to a calendar allocated by calendarAlloc.
*/
void freeCalendar(Calendar **calendar)
{

    for(int n = 0; n < NUM_OF_MONTHS; n++)
    {
        for(int e = 0; e < (*calendar)->numEvents[n];e++)
        {
            free((*calendar)->months[n][e].title);
            (*calendar)->months[n][e].title= NULL;
            (*calendar)->freeFunc((*calendar)->months[n][e].extraDetails);
            (*calendar)->months[n][e].extraDetails = NULL;
        }
        free((*calendar)->months[n]);
        (*calendar)->months[n] = NULL;
    }
    free(*calendar);
    (*calendar) = NULL;

}
=====



typedef void *ExtraDetails;
typedef const void *ConstExtraDetails;

typedef void(*PrintExtraDetailsFunc)(ConstExtraDetails);
typedef void(*FreeExtraDetailsFunc)(ConstExtraDetails);
typedef void(*CopyExtraDetailsFunc)(ExtraDetails dest, ConstExtraDetails src);


typedef struct Date
{
    unsigned short month: 4;
    unsigned short day: 5;
} Date;

typedef struct Event
{
    char *title;
    ExtraDetails extraDetails; // can be NULL
    Date date;
} Event;

typedef struct Calendar
{
    Event *months[NUM_OF_MONTHS];
    size_t numEvents[NUM_OF_MONTHS]; // number of events in each month
    CopyExtraDetailsFunc copyFunc; // used to copy extraDetails variables
    FreeExtraDetailsFunc freeFunc; // used to free extraDetails variables
    PrintExtraDetailsFunc printFunc; // used to print extraDetails variables
    size_t extraDetailsSize; // the size of an extraDetails variable
} Calendar;



/***************  Implement from here ***************/


/**
* Searches for events with the given title in the calendar.
* @param calendar - calender to search in
* @param title - the title of the event to search for
* @param numFoundEvents - pointer to variable, that should hold the number of events in the
calendar that match the given title. You cannot assume this variable is
initialized.
* @return dynamically allocated array of Event*, each pointing to an event in the calendar that
* has the same title as the given one. This array must be ordered by date.
*/
Event **findEvent(const Calendar *calendar, const char *title, size_t *numFoundEvents)
{
    Event** array_of_event_ptrs = NULL;


    *numFoundEvents = 0;
    int i = 0;
    int j = 0;
    for(i = 0;i<NUM_OF_MONTHS;i++)
    {
        for(j=0;j<calendar->numEvents[i];j++)
        {
            if(strcmp(title,calendar->months[i][j].title) == 0)
            {
                (*numFoundEvents)++;
                if(array_of_event_ptrs == NULL)
                {array_of_event_ptrs = (Event**)malloc(sizeof(Event *));}
                else{array_of_event_ptrs = (Event**)realloc((array_of_event_ptrs),(*numFoundEvents)*(sizeof(Event *)));}

                array_of_event_ptrs[(*numFoundEvents)-1] = &(calendar->months[i][j]);
            }
        }
    }
    return array_of_event_ptrs;
}


/**
* Clears a date in the calendar. For example, if given {4,27} than all events
* that occur on {4,27} should be removed from the calendar.
*/
void clearDate(Calendar *calendar, Date date)
{
    int amount = calendar->numEvents[date.month];
    if(amount != 0)
    {
        int del = 0;
        int first_ind = -1;
        int i = 0;
        for(i = 0; i < amount;i++)
        {
            if((calendar->months[date.month][i].date.day != date.day) && (first_ind!= -1))
            {
                calendar->months[date.month][first_ind] = calendar->months[date.month][i];
                first_ind++;
            }
            else if(calendar->months[date.month][i].date.day == date.day)
            {
                first_ind = i;

                calendar->freeFunc(calendar->months[date.month][i].extraDetails);
                calendar->months[date.month][i].extraDetails = NULL;
//free (calendar->month[date.month][i].title)
//free(calendar->months[date.month][i]);
//calendar->months[date.month][i] = NULL;
                del++;
                if(i+1 == amount)
                {break;}
            }

        }
        calendar->numEvents[date.month] = calendar->numEvents[date.month] - del;
        calendar->months[date.month] = (Event*)realloc(calendar->months[date.month],(calendar->numEvents[date.month])*sizeof(Event));
        if(calendar->numEvents[date.month] == 0){calendar->months[date.month] = NULL;}

    }
}



/**
* Prints a calendar.
*
* Assume a calendar with 2 events - {"January event",{0,2},NULL},{"March event",{2,27},NULL}
* Printing this calendar should result in the following output
* \n and \t are added to indicate they were used:
Month 0:\n
\t* title: January event, Month: 0, Day: 2, Extra Details:
Month 1:\n
Month 2:\n
\t* title: March event, Month: 2, Day: 27, Extra Details:
Month 3:\n
Month 4:\n
Month 5:\n
Month 6:\n
Month 7:\n
Month 8:\n
Month 9:\n
Month 10:\n
Month 11:\n

* @param calendar the calendar to print
*/
void printCalendar(const Calendar *calendar)
{
    int i = 0;
    for(i=0;i<NUM_OF_MONTHS;i++)
    {
        printf("Month %d:\n",i);
        for(int j=0;j <calendar->numEvents[i];j++)
        {
            printf("\t* title: %s, Month: %d, Day: %d, Extra Details:",
                   calendar->months[i][j].title,calendar->months[i][j].date.month,
                   calendar->months[i][j].date.day);
            calendar->printFunc(calendar->months[i][j].extraDetails);
            printf("\n");

        }
    }
}




pppppppppppppppppp




        ypedef int(*Comparator)(const void*,const void*);

/**
 * Performs a merge sort on the given array
 * @param array - Array to sort
 * @param numElements - number of elements in the array
 * @param elemSize  - size of every element in the array
 * @param comparator - pointer to comparator function, getting two array elements (a,b) and
                       returning a negative number if a<b, 0 if a==b, positive number if a>b
 */

void mergeArrays(void* array,size_t elemSize,Comparator comparator,size_t start1,size_t end1, size_t start2, size_t end2)
{
    char* arr = (char*)array;
    size_t arraySize = (end1 - start1 + end2 - start2);
    char* sorted_array = (char*)malloc(arraySize*elemSize);

    char* s_1 = arr+start1*elemSize;
    char* e_1 = arr + end1*elemSize;
    char* s_2 = arr + start2*elemSize;
    char* e_2 = arr + end2*elemSize;
    if(sorted_array == NULL){return;}
    size_t curIndex = 0;
    while((s_1 != e_1)&&(s_2!=e_2))
    {
        if(comparator(s_1,s_2) < 0)
        {
            memcpy(sorted_array+ (curIndex * elemSize),s_1,elemSize);
            s_1 = s_1 + elemSize;
            curIndex++;
        }
        else
        {
            memcpy(sorted_array+ (curIndex * elemSize),s_2,elemSize);
            s_2 = s_2 + elemSize;
            curIndex++;
        }
    }
    if(s_1 == e_1)
    {
        while((s_2!=e_2))
        {
            memcpy(sorted_array+ (curIndex * elemSize),s_2,elemSize);
            s_2 = s_2 + elemSize;
            curIndex++;
        }
    }
    if(s_2 == e_2)
    {
        while((s_1!=e_1))
        {
            memcpy(sorted_array+ (curIndex * elemSize),s_1,elemSize);
            s_1 = s_1 + elemSize;
            curIndex++;
        }
    }
    memcpy(arr +start1*elemSize, sorted_array, arraySize*elemSize);
    free(sorted_array);

}
void mergeSortHelper(void *array, size_t left, size_t right,
                     size_t elemSize, Comparator comparator) {
    if (left == right-1) {
        return;
    }
    size_t middle = (left + right) / 2;
    mergeSortHelper(array, left, middle, elemSize, comparator);
    mergeSortHelper(array, middle, right, elemSize, comparator);
    mergeArrays(array, elemSize, comparator, left, middle, middle, right);
}
void mergeSort(void *array, size_t numElements, size_t elemSize, Comparator comparator) {
    if (!array || !comparator || elemSize == 0) {
        return;
    }
    mergeSortHelper(array, 0, numElements / 2, elemSize, comparator);
    mergeSortHelper(array, numElements / 2, numElements, elemSize, comparator);
    mergeArrays(array, elemSize, comparator, 0, numElements / 2, numElements / 2, numElements);


}


-------------------




/**########################**/
typedef struct TouristAttraction TouristAttraction;
typedef void* ExtraDetails;
typedef int (*CompareFunc)(const void* attraction1, const void* attraction2);
typedef void (*ForEachFunc)(const TouristAttraction* attraction, void* extra_data);
typedef int (*CopyFunc)(ExtraDetails dest,const ExtraDetails src);
typedef void (*FreeFunc)(ExtraDetails to_free);

/**#########################**/
/**######## strcuts ########**/
/**#########################**/

typedef struct TouristAttraction{
    double cost;
    double novelty;
    char* name; //must be changeable
    ExtraDetails extra_details;
}TouristAttraction;


typedef struct Trip{
    size_t capacity; //the actual number of cells allocated for attractions
    size_t size; //number of attractions added
    size_t extra_details_size; //the size of an ExtraDetails variable in bytes
    TouristAttraction* attractions; // array of attractions
    CompareFunc compare_func; // compare function to order attractions by
    CopyFunc copy_func; // copy function, to copy ExtraDetails
    FreeFunc free_func; // free function, to free ExtraDetails internal data
} Trip;


/**************************************************/
/**************************************************/
/************* Your code from here ****************/
/**************************************************/
/**************************************************/

/**
 * Allocates a New Trip.
 * you should set the given functions and the given details size and allocate
 * the trip. All other fields should be zero initialized.
 *
 * @param: extra_details_size - size of an ExtraDetails variable in bytes - can be 0!
 * @param: compare_func - pointer to a function that compares TouristAttractions
 * @param: copy_func - pointer to a function that copies one ExtraDetails to another (does not allocated the ExtraDetails)
 * @param: free_func - pointer to a function that frees an ExtraDetails variable (does not free the given pointer - only inner fileds if necessary)
 * @return: A dynamically allocated Trip, null initialized except the given arguments.
 */
Trip* NewTrip(size_t extra_details_size, CompareFunc compare_func, CopyFunc copy_func, FreeFunc free_func)
{
    if((copy_func == NULL)||(copy_func == NULL)||(free_func == NULL))
    {return NULL;}
    Trip* my_trip = calloc(1,sizeof(Trip));
    if(my_trip == NULL){return NULL;}
    my_trip->extra_details_size = extra_details_size;
    my_trip->compare_func = compare_func;
    my_trip->copy_func = copy_func;
    my_trip->free_func = free_func;
    return my_trip;

}

bool find(Trip* trip, const char* name)
{
    for(size_t i = 0; i<trip->size;i++)
    {
        if(strcmp(name,trip->attraction[i].name) == 0){return true;}
    }
    return false;
}

void free_almost_all(Trip* trip)
{
    for (int i =0;i<trip->size;i++)
    {
        trip->free_func(trip->attraction[i].extra_details);
        free(trip->attraction[i].name);
    }
}

/**
 * Adds a new attraction to the given trip
 *
 * @param: trip - the trip to add the attraction to
 * @param: name - the name of the attraction
 * @param: cost - the cost of the attraction
 * @param: novelty - the novelty of the attraction
 * @param: extra_details - the extra details to save a copy of in the attraction. Can be NULL!
 * @return: 0 for fail (allocation fail, duplicates, invalid arguments)
 */
int AddAttraction(Trip* trip, const char* name, double cost, double novelty, ExtraDetails extra_details)
{
    if((trip == NULL)||(name == NULL)){return 0;}
    {
        if(trip->size == trip->capacity)
        {
            if(trip->size == 0)
            {
                trip->attractions = (TouristAttraction*)calloc(2,sizeof(TouristAttraction));
                if(trip->attractions == NULL){return 0;}
                if(extra_details != NULL)
                {
                    trip->attractions[trip->size].extra_details = malloc(trip->extra_details_size);
                    if(trip->attractions[trip->size].extra_details == NULL)
                    {
                        free_almost_all(trip);
                        free(trip->attractions);
                        return 0;
                    }
                }
                trip->copy_func(trip->attractions[trip->size].extra_details,extra_details);
                trip->attractions[trip->size].name = (char*)malloc(strlen(name)+1);
                if(trip->attractions[trip->size].name == NULL)
                {
                    trip->free_func(trip->attractions[trip->size].extra_details);
                    free_almost_all(trip);
                    free->attractions;
                    return 0;
                }
                strcpy(trip->attractions[trip->size].name,name);
                trip->attractions[trip->size].novelty = novelty;
                trip->attractions[trip->size].cost = cost;
                trip->capacity =2;
                trip->size++;
                return 1;
            }

            else
            {
                if(find(Trip* trip, const char* name)){return 0;}
                trip->attractions = realloc(trip->attractions,2*trip->capacity*sizeof(TouristAttraction));
                if(trip->attractions == NULL){return 0;}
                if(extra_details != NULL)
                {
                    trip->attractions[trip->size].extra_details = malloc(trip->extra_details_size);
                    if(trip->attractions[trip->size].extra_details == NULL)
                    {
                        free_almost_all(trip);
                        free(trip->attractions);
                        return 0;
                    }
                }
                trip->copy_func(trip->attractions[trip->size].extra_details,extra_details);
                trip->attractions[trip->size].name = (char*)malloc(strlen(name)+1);
                if(trip->attractions[trip->size].name == NULL)
                {
                    trip->free_func(trip->attractions[trip->size].extra_details);
                    free_almost_all(trip);
                    free->attractions;
                    return 0;
                }
                strcpy(trip->attractions[trip->size].name,name);
                trip->attractions[trip->size].novelty = novelty;
                trip->attractions[trip->size].cost = cost;
                trip->capacity = 2*trip->capacity;
                trip->size++;
                return 1;
            }

        }
        if(find(Trip* trip, const char* name)){return 0;}
        if(extra_details != NULL)
        {
            trip->attractions[trip->size].extra_details = malloc(trip->extra_details_size);
            if(trip->attractions[trip->size].extra_details == NULL)
            {

                free_almost_all(trip);
                free->attractions;
                return 0;
            }
        }
        trip->copy_func(trip->attractions[trip->size].extra_details,extra_details);
        trip->attractions[trip->size].name = (char*)malloc(strlen(name)+1);
        if(trip->attractions[trip->size].name == NULL)
        {
            trip->free_func(trip->attractions[trip->size].extra_details);
            free_almost_all(trip);
            free->attractions;
            return 0;
        }
        strcpy(trip->attractions[trip->size].name,name);
        trip->attractions[trip->size].novelty = novelty;
        trip->attractions[trip->size].cost = cost;
        trip->capacity = 2*trip->capacity;
        trip->size++;
        return 1;
    }


}

/**
 * Changes the compare function of the given trip and reorders the attractions
 * based on the new compare function
 *
 * @param: trip - the trip to reorder
 * @param: compare - the new compare function for the trip
 */
void SetSortingOrder(Trip* trip,CompareFunc compare)
{
    NewTrip(size_t extra_details_size, CompareFunc compare_func, trip->copy_func,trip->free_func)

}

/**
 * Calls the given function on every attraction in the trip
 *
 * @param: trip - the trip to perform the function on
 * @param: func - the function to call on the given trips' attractions
 * @param: extra_data - required data for the given func. Optional - could be NULL
 */
void ForEachAttraction(Trip* trip,ForEachFunc func,void* extra_data){

}

/**
 * Frees all resources of the given pointer to Trip*.
 * Assume the Teip* was allocated by NewTrip
 *
 * @param: trip - the trip to free all resources of
 */
void FreeTrip(Trip** trip){

}
// ******  typedefs: ******
// ****
#include <stdlib.h>
#include <assert.h>
// We want to run with NDEBUG defined - so that asserts will not run
#define NDEBUG
#define NO_RETURN_VALUE ((void)0) // this can be used as a return value in a void function
// This macro validates the input
#define VALIDATE_INPUT(e, return_value) if (!(e)){ \
return return_value;                               \
}

/**
 * dynamically allocates a new BST.
 * @param elem_size Size of elem_t in bytes.
 * @param free_func pointer to function that frees the resources in an element (not the element itself)
 * @param cpy_func pointer to a function that copies one element into another (works like memcpy)
 * @param compare_func pointer to function that compares elements
 * @return the new BST, NULL on allocation fail
 */
BST *NewBST(size_t elem_size, FreeFunc free_func, CpyFunc cpy_func, CompareFunc compare_func) {
    VALIDATE_INPUT(free_func && cpy_func && compare_func,NULL)
    BST *tree = malloc(sizeof(BST));
    assert(tree);
    if (tree) {
        tree->elem_size_ = elem_size;
        tree->head_ = NULL;
        tree->free_func_ = free_func;
        tree->cpy_func_ = cpy_func;
        tree->compare_func_ = compare_func;
    }
    return tree;
}

/**
 * Frees a given BST.
 * @param p_bst A pointer to a BST* allocated by NewBST
 */
void FreeBST(BST**p_bst) {
    VALIDATE_INPUT(p_bst&&(*p_bst), NO_RETURN_VALUE)
    FreeHelper((*p_bst)->head_, (*p_bst)->free_func_);
    free(*p_bst);
    *p_bst = NULL;
}

/**
 * Inserts a copy of the given element to the tree
 * @param tree The tree to insert to
 * @param data The data to insert
 * @return 1 if the insertion was successful (not a duplicate, no allocation fail), 0 otherwise
 */
int Insert(BST *tree, const_elem_t data) {
    VALIDATE_INPUT(tree && data, 0)
    Node *cur_node = tree->head_;
    Node *parent_node = NULL;
    int compare_result;
    if (!FindParentNode(tree, data, cur_node, &parent_node, &compare_result)) {
        return 0;
    }
    if (!AddDataToTree(tree, data, parent_node, compare_result)) {
        return 0;
    }
    return 1;
}

/**
 * Finds a given element in the tree
 * @param tree the tree to search in
 * @param data the element to search for
 * @return pointer to the element in the tree if found, NULL otherwise
 */
elem_t *Find(BST *tree, const_elem_t data) {
    VALIDATE_INPUT(tree && data, NULL)
    Node *cur_node = tree->head_;
    Node *parent_node = NULL;
    int compare_result;
    assert(FindParentNode(tree, data, cur_node, &parent_node, &compare_result));
    return compare_result > 0 ? parent_node->right_->data_ : parent_node->left_->data_;
}

/**
 * inserts the elements of a tree to a sorted array (small to large)
 * @param tree tree to insert elements of to array
 * @param array_size pointer to size_t vairable that will hold the number of elements in the
 * array at the end of the function
 * @return A dynamically allocated array. The User should not free the elements in this array,
 * only the array itself
 */
elem_t *ToArray(BST *tree, size_t *array_size) {
    VALIDATE_INPUT(tree && array_size, NULL)
    *array_size = 0;
    elem_t *final_array = NULL;
    if (!ToArrayHelper(tree->head_, &final_array, array_size)) {
        return NULL;
    }
    *array_size = sizeof(final_array) / sizeof(final_array[0]);
    return final_array;
}

/**********************************************
 *************  Private functions  ************
 **********************************************/
/**
 * Dynamically allocates a new Node
 * @param parent the Node* that is the parent of the node
 * @param data the data of the node
 * @return the node, NULL on allocation fail
 */
static struct Node *NewNode(Node *parent, elem_t data) {
    Node *node = calloc(1, sizeof(Node));
    assert(node);
    if (node) {
        node->parent_ = parent;
        node->data_ = data;
    }
    return node;
}


/**
 * Helper function for free. Frees the left and right children, then the node
 */
static void FreeHelper(Node *node, FreeFunc free_func) {
    if (node) {
        FreeHelper(node->left_, free_func);
        FreeHelper(node->right_, free_func);
        free_func(node->data_);
        free(node);
        node = NULL;
    }
}

/**
 * Finds the Node that should be the parent of the given data.
 * @param tree the tree to search in
 * @param data the data to search parent for
 * @param cur_node the node to start the search from (pointer)
 * @param parent_node pointer to a parent node (pointer to pointer)
 * @param compare_result pointer to an int that will be used to save the comparison results. Will
 * allow the calling scope to know if data should be inserted as a right/left child of the parent
 * node
 * @return 1 if a parent node was found, 0 otherwise (allocation fail, duplicates)
 */
static int FindParentNode(const BST *tree, const_elem_t data, Node *cur_node, Node **parent_node,
                          int *compare_result) {
    while (cur_node) {
        (*parent_node) = cur_node;
        (*compare_result) = tree->compare_func_(data, cur_node->data_);
        if (!(*compare_result)) {
            return 0;
        }
        cur_node = (*compare_result) > 0 ? cur_node->right_ : cur_node->left_;
    }
    return 1;
}

/**
 * Adds the given data to the tree as a child of the given parent node, based on the comparison
 * result
 * @param tree the tree to add the data in
 * @param data the data to add to the tree
 * @param parent_node the node to add data as a child of
 * @param compare_result <0 if data < parent_node->data, >0 data > parent_node->data
 * @return 1 on success, 0 on fail (allocation fail)
 */
static int AddDataToTree(BST *tree, const void *data, Node *parent_node, int compare_result) {
    // copy the data
    elem_t new_data = malloc(tree->elem_size_);
    if(new_data == NULL){return 0;}
    tree->cpy_func_(new_data, data);
    // add to the tree by creating a new node and connecting to the parent
    Node *to_add = NULL;
    if (!tree->head_) {
        tree->head_ =to_add;
    } else {
        if(compare_result > 0){parent_node->right_ =to_add;}
        else{ parent_node->left_ = to_add;}

    }
    to_add = NewNode(parent_node, new_data);
    if (!to_add) {
        free(new_data);
        return 0;
    }
    return 1;
}

/**
 * Helper function for the ToArray function. Creates the array by running AddNodeDataToArray
 * function in-order on the tree elements
 * @param node The current node
 * @param p_array pointer to dynamically allocated array of elem_t variables
 * @param array_size the current size of p_array. array_size=4 means 4*size(elem_t) bytes are
 * allocated for the array
 * @return 1 on success, 0 on fail
 */
static int ToArrayHelper(Node *node, elem_t **p_array, size_t *array_size) {
    if (node) {
        if (!ToArrayHelper(node->left_, p_array, array_size)) {
            return 0;
        }
        if (!AddNodeDataToArray(node, p_array, array_size)) {
            return 0;
        }
        if (!ToArrayHelper(node->right_, p_array, array_size)) {
            return 0;
        }
    }
    return 1;
}

/**
 * Reallocates the array and adds the data of the node to it, update array_size as needed. Free the
 * array in case of fail.
 * @return 1 on success, 0 on fail
 */
static int AddNodeDataToArray(const Node *node, elem_t **p_array, size_t *array_size) {

    elem_t * tmp = realloc((*p_array), (1 + (*array_size)) * sizeof(node->data_));
    if (!tmp) {
        free(*p_array);
        *p_array = NULL;
        return 0;
    }