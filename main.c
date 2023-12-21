#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include <time.h>
#define answer_yes 3
#define answer_no 1

void appointments ();
void print_appointments();
void book_appointment ();
int book_appointment_input (char array_input[], int sum_input_1, int sum_input_2);
void your_appointment(int day);

int main() {

    // We declare char arrays for user input and for comparison with user input
    char array_input[99] = "";
    char array_compare_book[] = "book appointment";
    char array_compare_quit[] = "quit";
    // We create variables to store the result of our comparison
    int book, exit;

    // We ask for a specific input to proceed
    printf("Type book appointment to continue or quit to exit: \n");

    while (1)
    {
        // We use fgets to read our input and store it in array_input, we then compare it to the book/quit array, we also remove any \n characters
        fgets(array_input, sizeof(array_input), stdin);
        array_input[strcspn(array_input, "\n")] = '\0';
        exit = strcmp(array_input, array_compare_quit);
        book = strcmp(array_input, array_compare_book);

        // We check to see if our arrays match, if they do we call our function or quit the program, if not we ask for the user to try again
        if (exit == 0)
            break;
        else if (book == 0)
        {
            appointments();
            book_appointment ();
        }
        else if (book == 1)
            printf("Try again\n");
    }
    return 0;
}

// In this function we generate data and call print_appointments to print them
void appointments ()
{
    // We create a pointer to a file called appointments and attempts to open it in read mode
    FILE *appointments;
    appointments = fopen("appointment_list", "r");

    // We use fseek to give us the size of bytes in our file and stores it in file_size
    fseek(appointments, 0, SEEK_END);
    long file_size = ftell(appointments);

    // We check to see if the amount of bytes in file_size == 0, if it is we generate some data and store it in the file
    if (file_size == 0)
    {
        fclose(appointments);
        appointments = fopen("appointment_list", "w");

        // The data that gets generated is a bunch of 0's and 1's based on our random number generator, which gets stored in our int variable random_number
        int random_number;

        time_t value;
        srand((unsigned) time(&value));

        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 13; ++j)
            {
                random_number = rand() % 2;
                fprintf(appointments,"%d", random_number);
            }
        }
        fclose(appointments);

        // We call print_appointments to print our newly generated data
        printf("List of available appointments: \n");
        print_appointments();
    }

    // We call print_appointments to print our already existing data
    else
    {
        print_appointments();
    }
}

// In this function we print our appointments based on the data from the file, we also assign availability based on the data
void print_appointments()
{
    FILE *appointments = fopen("appointment_list", "r");

    int hour = 9, minutes = 0;
    char *array_days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

    for (int j = 0; j < 5; ++j)
    {
        printf("%s:\n", array_days[j]);
        for (int i = 0; i < 13; ++i)
        {
            int read_appointment = fgetc(appointments);

            if (read_appointment == '0')
            {
                printf("%02d:%02d free    ", hour, minutes);
            }
            else if (read_appointment == '1')
            {
                printf("%02d:%02d taken   ", hour, minutes);
            }

            minutes = (minutes + 30) % 60;

            if (minutes == 0)
            {
                hour = (hour + 1) % 24;
            }

            printf("\n");
        }

        printf("\n");

        if (hour == 15 && minutes == 30)
        {
            hour = 9;
            minutes = 0;
        }
    }
    fclose(appointments);
}

// In this function we ask the user a series of questions and give them a score based on their answers
// We then call our function your_appointment based on the result of the score, or suggest e-consultation if the score is too low
void book_appointment ()
{
    int sum = 0;
    char array_input[99] = "";

    printf("To book an appointment, please answer yes/no to the following questions.\n");

    printf("Is your appointment urgent? yes/no\n");
        fgets(array_input, sizeof(array_input), stdin);
        array_input[strcspn(array_input, "\n")] = '\0';
        sum += book_appointment_input(array_input, 1, 0);

    printf("Is your health condition worsening? yes/no\n");
        fgets(array_input, sizeof(array_input), stdin);
        array_input[strcspn(array_input, "\n")] = '\0';
        sum += book_appointment_input(array_input, 0, 0);

    printf("Is your appointment prescription related? yes/no\n");
        fgets(array_input, sizeof(array_input), stdin);
        array_input[strcspn(array_input, "\n")] = '\0';
        sum += book_appointment_input(array_input, -1, 0);

    printf("Could an E-consultation be sufficient? yes/no\n");
        fgets(array_input, sizeof(array_input), stdin);
        array_input[strcspn(array_input, "\n")] = '\0';
        sum += book_appointment_input(array_input, -7, 0);

    int random_number;
    time_t value;
    srand((unsigned) time(&value));

    if (sum >= 8)
    {
        your_appointment(0);
    }

    else if (sum >= 6)
    {
        random_number = rand() % 2 + 1;

            if (random_number == 1)
            {
                your_appointment(1);
            }
            else
            {
                your_appointment(2);
            }
    }

    else if (sum >= 3)
    {
        random_number = rand() % 2 + 3;

            if (random_number == 3)
            {
                your_appointment(3);
            }
            else
            {
                your_appointment(4);
            }

    }

    else if (sum <= 2)
    {
        printf("We suggest you try E-consultation");
    }

}

// In this function we receive a user input in form of a string and 2 integers from the previous function
// We then calculate and add to our sum value based on the answers given, and then we return our sum value to book_appointment
int book_appointment_input (char array_input[], int sum_input_1, int sum_input_2)
{
    int yes, no;
    int sum = 0;
    char array_yes [] = "yes";
    char array_no [] = "no";

    while (1)
    {
    yes = strcmp(array_input, array_yes);
    no = strcmp(array_input, array_no);

        if (yes == 0)
        {
            sum += answer_yes + sum_input_1;
            break;
        }
        else if (no == 0)
        {
            sum += answer_no + sum_input_2;
            break;
        }
        else
        {
            printf("Try again\n");
            fgets(array_input, sizeof(array_input), stdin);
            array_input[strcspn(array_input, "\n")] = '\0';
        }
    }
    return sum;
}

// In this function we receive an integer value from the previous function book_appointment
// We also assign the user with an appointment and updates the file data changing the data (appointment) from free to taken
void your_appointment(int day)
{
    // We open our file in r+ mode meaning both read and write
    FILE *appointments = fopen("appointment_list", "r+");

    // We create two variables and set their values, we also create an array to represent the days of a normal workweek
    int available_appointment = 0;
    int hour = 9, minutes = 0;
    char *array_days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

    // We use fseek to make sure our file pointer is in the right place when its about to read from our file
    // We do this to insure the data isn't being misread
    // The offset for our pointer is based on the day given * 13 * the sizeof char (1 byte)
    // This way we skip the data for Monday, if the day given is 1 (Tuesday), 2 (Wednesday) etc.
    fseek(appointments, 13 * day * sizeof (char), SEEK_CUR);

    for (int j = 0; j < 5; ++j)
    {
        for (int i = 0; i < 13; ++i)
        {
            // We create an integer to store the read character we get from fgetc
            // fgetc moves the file position indicator one step to the right after reading a character
            // essentially searching the file for 0's = a free appointment
            int read_appointment = fgetc(appointments);

            // If the read character == 0 we print the day and time of the appointment
            if (read_appointment == '0')
            {
                printf("Your appointment: %s at %02d:%02d.\n", array_days[day], hour, minutes);
                // We also set the value of available_appointment to 1 as a way to stop our outer loop from looping
                available_appointment = 1;

                // We then move the pointer one position backwards to where the 0 was found and change the value to 1
                // Marking the appointment as taken. We then break out of the loop
                fseek(appointments, - 1, SEEK_CUR);
                fputc('1', appointments);
                break;
            }

            // We update our minutes variable by +30 and %60 to represent 30 minutes passed on the clock
            minutes = (minutes + 30) % 60;

            // We update our hour variable by +1 each time our minutes variable reaches 0
            if (minutes == 0)
            {
                hour = (hour + 1) % 24;
            }

            // We set our values back to 9 and 0 when they reach 15 and 30
            if (hour == 15 && minutes == 30)
            {
                hour = 9;
                minutes = 0;
            }
        }

        // When available_appointment is set to 1 which means an available appointment has been found we break out of the outer loop
        if (available_appointment == 1)
        {
            break;
        }

        // We increase the day variable by +1 if no available appointment has been found
        // We do this to check if the next day has any available appointments
        day += 1;
    }

    // If available_appointment remains == 0 we tell the user that there are no appointments available
    if (available_appointment == 0)
    {
        printf("No available time found\n");
    }

    fclose(appointments);
}