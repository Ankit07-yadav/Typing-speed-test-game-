#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include <conio.h>
#include <ctype.h>


struct Player {
    char name[50];
    double wpm;
    double accuracy;
};


void paragraph(char pgph[]);
void qwicktest(char sentence[]);
double wordpermin(char *typed, double timetaken);
double accuracy(char *original, char *typed);
void leaderboard();
void record(struct Player P);
void starttypingtestgame(char name[]);
void typingWithCorrection(char *original, char *typed, int limit);


int main()
{
    system("chcp 65001 > nul");

    printf("==========================================\n");
    printf("                    UPES                  \n");
    printf("==========================================\n\n");

    printf("PROJECT --- TYPING SPEED TEST GAME \n");
    printf("SUBMITTED BY - ANKIT KUMAR YADAV \n");
    printf("SAP ID --- 590023921 \nBATCH-9\n\n");
    printf("SUBMITTED TO - PROF. PANKAJ BADONI\n");

    // ? DECLARE VARIABLES BEFORE LABEL
    char gch;
    char name[50];
    int choice;

startagain:   // ? LABEL now placed AFTER declarations

    printf("\nDo You Want To Continue \n");
    printf("Y=yes    N=no \n");
    scanf("%c",&gch);
    while(getchar()!='\n');  // clear buffer

    if (gch=='Y' || gch=='y')
    {
        printf("\nWELCOME USER\n");
        printf("Please enter your name:\n");

        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';

        printf("\nHELLO %s; Please enter your choice\n", name);

        printf("1. Start Typing Test Game\n");
        printf("2. Show the Leaderboard\n");
        printf("3. Exit\n");

        scanf("%d",&choice);
        while(getchar()!='\n'); // clear buffer

        switch(choice)
        {
            case 1:
                starttypingtestgame(name);
                break;

            case 2:
                leaderboard();
                break;

            case 3:
                printf("Thank you!! For playing\n");
                return 0;

            default:
                printf("Wrong choice entered\n");
        }

        goto startagain;   // ? works perfectly now
    }
    else
    {
        printf("Thank you!! For playing\n");
        return 0;
    }
}


void starttypingtestgame(char name[])
{
    while (1) {   // <<< LOOP FOR PLAY AGAIN
        int gamechoice=0;
        char text[2500],typed[2500];
        typed[0] = '\0';
        int limit=0;
        time_t start, current;
        struct Player player;

        printf("Select a mode \n");
        printf("1. Qwick test mode (1 minute game) \n");
        printf("2. Paragraph mode (2 minute game) \n");
        scanf("%d", &gamechoice);

        switch (gamechoice)
        {
            case 1:
                qwicktest(text);
                limit = 60;
                break;
            case 2:
                paragraph(text);
                limit = 120;
                break;
            default:
                printf("Wrong choice entered\n");
                continue;   // restart game mode selection
        }

        printf("\nType the following text:\n%s\n", text);
        printf("\nYou have %d seconds. Start typing below:\n", limit);

        time(&start);
        typingWithCorrection(text, typed, limit);
        time(&current);

        double timeTaken = difftime(current, start);
        player.wpm = wordpermin(typed, timeTaken);
        player.accuracy = accuracy(text, typed);
        strcpy(player.name, name);

        printf("\n\n-------- RESULTS --------\n");
        printf("Player: %s\n", player.name);
        printf("Time Taken: %.2f seconds\n", timeTaken);
        printf("Typing Speed: %.2f WPM\n", player.wpm);
        printf("Accuracy: %.2f%%\n", player.accuracy);
        printf("--------------------------\n");

        record(player);

        // ------------------------------
        // ASK USER IF THEY WANT TO PLAY AGAIN
        // ------------------------------
        char again;
        printf("\nDo you want to play again? (Y/N): ");
        while (getchar() != '\n');  // clear buffer
        scanf("%c", &again);

        if (again != 'Y' && again != 'y') {
            printf("\nThank you for playing the Typing Speed Test Game!\n");
            break;   // exit the while loop
        }

        printf("\nRestarting...\n\n");
    }
}



void qwicktest(char sentence[])
{
    FILE *qtg = fopen("qwicktest.txt", "r");
    if (!qtg)
    {
        printf("ERROR: qwicktest.txt not found!\n");
        exit(1);
    }

    char lines[50][300];
    int next = 0;

    while (fgets(lines[next], sizeof(lines[next]), qtg))
    {
        lines[next][strcspn(lines[next], "\n")] = '\0';
        next++;
    }

    fclose(qtg);

    srand(time(0) ^ clock() ^ rand());
    int randomIndex = rand() % next;
    strcpy(sentence, lines[randomIndex]);
}


void paragraph(char pgph[])
{
    FILE *para = fopen("paragraphs.txt", "r");
    if (para == NULL)
    {
        printf("ERROR: paragraph file not found!\n");
        exit(1);
    }

    char paras[50][3000];
    int count = 0;

    while (fgets(paras[count], sizeof(paras[count]), para))
    {
        paras[count][strcspn(paras[count], "\n")] = '\0';
        count++;
    }

    fclose(para);

    srand(time(0) ^ clock() ^ rand());
    int randomIndex = rand() % count;
    strcpy(pgph, paras[randomIndex]);
}


void typingWithCorrection(char *original, char *typed, int limit) {
    int len = strlen(original);
    int i = 0;
    char c;
    time_t start, current;
    time(&start);

    while (1) {
        time(&current);

        // Stop immediately when time is over
        if (difftime(current, start) >= limit) {
            printf("\n\nTime's up!\n");
            break;
        }

        // No key pressed ? continue loop
        if (!_kbhit()) {
            continue;
        }

        c = getch();

        // Stop when Enter pressed
        if (c == '\r') break;

        // Handle Backspace
        if (c == '\b') {
            if (i > 0) {
                printf("\b \b");
                i--;
            }
            continue;
        }

        // Correct character
        if (tolower(c) == tolower(original[i])) {
            printf("\033[32m%c\033[0m", c);
            typed[i++] = c;
        }
        else {
            // Wrong character
            printf("\033[31m%c\033[0m", c);

            while (1) {
                if (_kbhit()) {
                    char fix = getch();
                    if (fix == '\b') {
                        printf("\b \b");
                        break;
                    }
                }
                // check time even during correction
                time(&current);
                if (difftime(current, start) >= limit) {
                    printf("\n\nTime's up!\n");
                    goto END;
                }
            }
        }

        if (i >= len) break;
    }

END:
    typed[i] = '\0';
}

double accuracy(char *original, char *typed) {
    int correct = 0;
    int total = strlen(original);
    int minLen = strlen(typed) < total ? strlen(typed) : total;

    for (int i = 0; i < minLen; i++) {
        if (tolower(original[i]) == tolower(typed[i]))
            correct++;
    }
    return ((double)correct / total) * 100;
}


double wordpermin(char *typed, double timeTaken) {
    int words = 0;
    for (int i = 0; typed[i]; i++)
        if (typed[i] == ' ' || typed[i] == '\n')
            words++;

    double minutes = timeTaken / 60.0;
    if (minutes == 0) minutes = 1;
    return words / minutes;
}


void record(struct Player P) {
    FILE *fp = fopen("results.txt", "a");
    if (!fp) {
        printf("Error: Could not open results.txt\n");
        return;
    }
    fprintf(fp, "%s\t%.2f WPM\t%.2f%% Accuracy\n", P.name, P.wpm, P.accuracy);
    fclose(fp);
    printf("\nResult saved to results.txt \n");
}


void leaderboard() {
    FILE *fp = fopen("results.txt", "r");
    if (!fp) {
        printf("\nNo previous records found.\n");
        return;
    }

    char line[200];
    printf("\n=========== LEADERBOARD ===========\n");
    while (fgets(line, sizeof(line), fp))
        printf("%s", line);
    printf("===================================\n");
    fclose(fp);
}