
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
            {return 0;}
            else if(direct < 0){i++;}
            else{break;}
        }
        int num = calendar->numEvents[date.month];
        char *p = realloc(calendar->months[date.month],(num+1)*sizeof(Event));
        if ( p == NULL) {return 0;}

        calendar->months[date.month] = (Event *)p;
        //realloc(calendar->months[date.month],(num+1)*sizeof(Event));
        if(calendar->months[date.month] == NULL){return 0;}

        while(num != i)
        {
            calendar->months[date.month][num] = calendar->months[date.month][num-1];
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

int main()
{

    Calendar *calendar = calendarAlloc(0, noCopy, noFree, noPrint);
    PRINT_BOOL(addEvent(calendar, "Jan event", (Date) {JAN, 27}, NULL));
    PRINT_BOOL(addEvent(calendar, "Jan2 event", (Date) {JAN, 2}, NULL));
    PRINT_BOOL(addEvent(calendar, "Mar event", (Date) {MAR, 23}, NULL));
    PRINT_BOOL(addEvent(calendar, "Mar event", (Date) {MAR, 27}, NULL));
    PRINT_BOOL(addEvent(calendar, "Mar event", (Date) {MAR, 27}, NULL));
    freeCalendar(&calendar);
}




#if 0
typedef struct Date
{
    unsigned short month: 4;
    unsigned short day: 5;
} Date;

int addEvent(Calendar *calendar, const char *title, Date date, ConstExtraDetails extraDetails)
{
    int x:;
    if(calendar->numEvents[date.month:] == 0)
    {
        calendar->months[date.month:] = (Event*)calloc(date.day:,sizeof(Event));
        calendar->months[date.month:][date.day:]->title = title;
        calendar->copyFunc(calendar->months[date.month:][date.day:]->extraDetails,extraDetails);
        calendar->months[date.month:][date.day:]->date = (date*)malloc(sizeof(date));
        calendar->months[date.month:][date.day:]->date = date;
        calendar->numEvents[date.month:] = date.day:;
    }
    else
    {
        if(date.day: > calendar->numEvents[date.month:])
        {
            calendar->months[date.month:] =
            realloc(calendar->months[date.month:], date.day * sizeof(Event));
            calendar->months[date.month:][date.day:]->title = title;
            calendar->copyFunc(calendar->months[date.month:][date.day:]->extraDetails,extraDetails);
            calendar->months[date.month:][date.day:]->date = (date*)malloc(sizeof(date));
            calendar->months[date.month:][date.day:]->date = date;
            calendar->numEvents[date.month:] = date.day:;
        }
        else if(date.day: < calendar->numEvents[date.month:])
        {
            if(calendar->months[date.month:][date.day:] == NULL)
            {
                calendar->months[date.month:][date.day:]->title = title;
                calendar->copyFunc(calendar->months[date.month:][date.day:]->extraDetails,extraDetails);
                calendar->months[date.month:][date.day:]->date = (date*)malloc(sizeof(date));
                calendar->months[date.month:][date.day:]->date = date;
                calendar->numEvents[date.month:] = date.day: ;
            }
            else if(calendar->months[date.month:][date.day:]->date->day == date->day)
            {
                if(strcmp(calendar->months[date.month:][date.day:]->title,title) > 0)
                {}


            }
        }
    }















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
    int elemcmp(Event* elem1 , Date* date , const char* title)
    {
        if(elem1->date->day == date->day)
        {
            return strcmp(elem1->title, title);
        }
        else
        {
            return elem1->date->day - date->day;
        }
    }

    Calendar *calendarAlloc(size_t extraDetailsSize = 0, CopyExtraDetailsFunc copyFunc,
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

        if(calendar->numEvents[date.month:] == 0)
        {

            calendar->months[date.month:] = (Event*)malloc(sizeof(Event));
            if(calendar->months[date.month:] != NULL)
            {
                calendar->months[date.month:][0]->title = title;
                calendar->copyFunc(calendar->months[date.month:][0]->extraDetails,extraDetails);
                calendar->months[date.month:][0]->date = date;
                calendar->numEvents[date.month:]++;
                return 1;
            }
            return 0;
        }

        else
        {
            for(int i = 0; i <calendar->numEvents[date.month:];i++)
            {
                int direct = elemcmp(calendar->months[date.month:][i],date,title)
                if(direct == 0)
                {return 0;}
                else if(direct < 0){i++;}
                else{break;}
            }
            int num = calendar->numEvents[date.month:];
            calendar->months[date.month:] = realloc(calendar->months[date.month:],(num+1)*sizeof(Event));
            if(calendar->months[date.month:] == NULL){return 0;}
            while(num != i)
            {
                calendar->months[date.month:][num]->title = calendar->months[date.month:][num-1]->title;
                calendar->copyFunc(calendar->months[date.month:][num]->extraDetails,
                        calendar->months[date.month:][num-1]->extraDetails);
                calendar->months[date.month:][num]->date = calendar->months[date.month:][num-1]->date;
                (num--);
            }
            calendar->months[date.month:][i]->title = title;
            calendar->copyFunc(calendar->months[date.month:][i]->extraDetails,extraDetails);
            calendar->months[date.month:][i]->date = date;
            calendar->numEvents[date.month:]++;
            return 1;
        }

#endif