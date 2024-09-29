#include <stdbool.h>
#include <conio.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define FLUSH fflush(stdin)
#define DELAY_MS 50

void gotoxy(int x, int y);
void loading();
void minefield_generator( void );
bool check_win();
bool play_game();
void play_minesweeper();
void print_minefield( void );
void print_final_minefield( void );
void win( void );
void boom();
void play_again( void );
void game_over( void );

COORD coord = {0, 0};
int ROWS = 20, COLUMNS = 20;
int user_x,user_y;
int total_mines = 10;
int minefield[20][20];                                      //This 2-D array contains all of the mines, numbers and blank spaces
int blank_minefield[20][20];                                //This contains the minefield full of '|-|' characters
int final_minefield[20][20];
int lives = 3;

int main()
{
    loading();
    minefield_generator();
    play_minesweeper();
    return 0;
}

void gotoxy(int x, int y)
{
    coord.X = x - 1;
    coord.Y = y - 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void loading()
{
    system("color F5");
    char spinner[] = "|/-\\";
    int ctr_3 = 0;
    int ctr_2 = 0;
    int x = 65, y = 15;
    gotoxy(x, y);
    while (ctr_3 <= 100) {
        printf("\r\t\t\t\t\t\t\tLoading: [");
        for (ctr_2 = 0; ctr_2 < ctr_3/5; ctr_2++)
        {
            printf("*");
        }
        for (ctr_2 = ctr_3/5; ctr_2 < 20; ctr_2++)
        {
            printf(" ");
        }
        printf("] %3d%% %c", ctr_3, spinner[ctr_3 % 4]);
        fflush(stdout);
        ctr_3++;
        usleep(DELAY_MS * 1000);
    }
    system("cls");
    // Array of characters for the "Welcome to Minesweeper" message
    char welcome_msg[] = {'W', 'E', 'L', 'C', 'O', 'M', 'E', ' ', 'T', 'O', ' ', 'M', 'I', 'N', 'E', 'S', 'W', 'E', 'E', 'P', 'E', 'R'};
    int welcome_len = sizeof(welcome_msg) / sizeof(char);

    // Print the "Welcome to Minesweeper" message centered on the screen
    gotoxy(x, y);

    for (int ctr_3 = 0; ctr_3 < welcome_len; ctr_3++)
    {
        system("color F5");
        printf("\033[1;35m%c\033[0m", welcome_msg[ctr_3]);
        Sleep(30);
    }

    for (int ctr_3 = 0; ctr_3 < 25; ctr_3++)
    {
        system("color F5");
        gotoxy(x + ctr_3, y);
        printf("\033[1;36m>\033[0m");
        Sleep(60);
    }
}

void minefield_generator( void )
 {
    int ctr_3 = 0, ctr_2 = 0;
    system("cls");
    srand( time( NULL ) );     //Starts the random no. generator

    while( ctr_2 < COLUMNS )
    {                       //Nested loop for making the blank minefield and final minefield
        while( ctr_3 < ROWS)
        {
            minefield[ctr_3][ctr_2] = '-';
            blank_minefield[ctr_3][ctr_2] = minefield[ctr_3][ctr_2];
            final_minefield[ctr_3][ctr_2] = minefield[ctr_3][ctr_2];
            ctr_3++;
        }
        ctr_3 = 0;
        ctr_2++;
    }

    int mines = 0;
    int x_cord = 0, y_cord = 0;

    while( mines < total_mines )
    {                               //Randomly generates the mines into the minefield
        x_cord = rand()%(ROWS);
        y_cord = rand()%(COLUMNS);
        if( minefield[x_cord][y_cord] != '*')   //If statement that checks if there is a mine already there and doesn't place a mine if there is
        {
            minefield[x_cord][y_cord] = '*';
            final_minefield[x_cord][y_cord] = minefield[x_cord][y_cord];
            mines++;
        }
    }
    ctr_3 = 0;
    ctr_2 = 0;
    while( ctr_2 < COLUMNS )        //While loop that generates the numbers for any adjacent mines
    {
        while( ctr_3 < ROWS) {
            if( minefield[ctr_3][ctr_2] != '*')
            {
                minefield[ctr_3][ctr_2] = 0;
            }
            if((minefield[ctr_3-1][ctr_2-1] == '*') && (minefield[ctr_3][ctr_2] != '*'))
            {
                minefield[ctr_3][ctr_2]++;
            }
            if((minefield[ctr_3-1][ctr_2] == '*') && (minefield[ctr_3][ctr_2] != '*'))
            {
                minefield[ctr_3][ctr_2]++;
            }
            if((minefield[ctr_3][ctr_2-1] == '*') && (minefield[ctr_3][ctr_2] != '*'))
            {
                minefield[ctr_3][ctr_2]++;
            }
            if((minefield[ctr_3-1][ctr_2+1] == '*') && (minefield[ctr_3][ctr_2] != '*'))
            {
                minefield[ctr_3][ctr_2]++;
            }
            if((minefield[ctr_3+1][ctr_2-1] == '*') && (minefield[ctr_3][ctr_2] != '*'))
            {
                minefield[ctr_3][ctr_2]++;
            }
            if((minefield[ctr_3+1][ctr_2] == '*') && (minefield[ctr_3][ctr_2] != '*'))
            {
                minefield[ctr_3][ctr_2]++;
            }
            if((minefield[ctr_3][ctr_2+1] == '*') && (minefield[ctr_3][ctr_2] != '*'))
            {
                minefield[ctr_3][ctr_2]++;
            }
            if((minefield[ctr_3+1][ctr_2+1] == '*') && (minefield[ctr_3][ctr_2] != '*'))
            {
                minefield[ctr_3][ctr_2]++;
            }
            ctr_3++;
        }
        ctr_3 = 0;
        ctr_2++;
    }
    ctr_3 = 0;
    ctr_2 = 0;
}

bool check_win() {
    int revealed_cells = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (blank_minefield[i][j] != '-' && blank_minefield[i][j] != '|' && blank_minefield[i][j] != '*') {
                revealed_cells++;
            }
        }
    }
    if (revealed_cells == ROWS * COLUMNS - total_mines && lives > 0) {
        return true;
    }
     return false;
}

bool play_game()
{
    int user_x, user_y;
    do {
        system("cls");
        printf("\n\t\t\t\t\033[1;35m#     # ### #     # #######  #####  #     # ####### ####### ######  ####### ######\033[0m");
        printf("\n\t\t\t\t\033[1;35m##   ##  #  ##    # #       #     # #  #  # #       #       #     # #       #     #\033[0m");
        printf("\n\t\t\t\t\033[1;35m# # # #  #  # #   # #       #       #  #  # #       #       #     # #       #     #\033[0m");
        printf("\n\t\t\t\t\033[1;35m#  #  #  #  #  #  # #####    #####  #  #  # #####   #####   ######  #####   ######\033[0m");
        printf("\n\t\t\t\t\033[1;35m#     #  #  #   # # #             # #  #  # #       #       #       #       #   #\033[0m");
        printf("\n\t\t\t\t\033[1;35m#     #  #  #    ## #       #     # #  #  # #       #       #       #       #    #\033[0m");
        printf("\n\t\t\t\t\033[1;35m#     # ### #     # #######  #####   ## ##  ####### ####### #       ####### #     #\033[0m");
        print_minefield();
        printf("\n\nLives remaining: %d\n", lives);
        printf("\nEnter the x value, then a space, then the y value:");
        scanf("%d %d", &user_x, &user_y);
        FLUSH;
        if (user_x >= ROWS || user_x < 0 || user_y < 0 || user_y >= COLUMNS)
        {
            printf("\nPlease enter a valid value for x and y\n");
            system("pause");
        }
        else if (blank_minefield[user_x][user_y] != '-')
        {
            printf("\nPlease enter a valid value for x and y\n");
            system("pause");
        }
        else
        {
            break; // exit the loop when the input is valid
        }
    } while (true);
        if (check_win()) {
            win();
            return true;
        }
        if (lives == 0) {
            boom();
            return true;
        }
    blank_minefield[user_x][user_y] = minefield[user_x][user_y];
    if (minefield[user_x][user_y] == 0)
    {
        // reveal neighboring cells if they are also blank
        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                int x = user_x + dx;
                int y = user_y + dy;
                if (x >= 0 && x < ROWS && y >= 0 && y < COLUMNS && minefield[x][y] == 0 && blank_minefield[x][y] == '-')
                {
                    blank_minefield[x][y] = minefield[x][y];
                }
            }
        }
    }
    else if (minefield[user_x][user_y] == '*')
    {
        // user selected a mine
        printf("\n\033[1;31mYou hit a bomb!\033[0m\n");
        lives--;
        system("pause");
    }
    return false; // game not over
}

void play_minesweeper()
{
    while (!play_game())
    {
        // continue playing
    }
}

void print_minefield(void)
{
    int ctr_3 = 0, ctr_2 = 0, ctr_1 = 0;
    while (ctr_1 < ROWS)
    {
        if (ctr_1 == 0)
        {
            gotoxy(6, 10);
        }
        printf("   \033[1;36m%2d\033[0m  ", ctr_1 );
        ctr_1++;
    }
    printf("\n\n");

    while (ctr_2 < COLUMNS)
    {

        printf("\033[1;36m> %2d\033[0m  ", ctr_2 );
        while (ctr_3 < ROWS)
        {
            if (blank_minefield[ctr_3][ctr_2] == '-')
            {
                printf("  \033[1;35m[%c]\033[0m  ", blank_minefield[ctr_3][ctr_2]);
            } else if (minefield[ctr_3][ctr_2] == 0)
            {
                blank_minefield[ctr_3][ctr_2] = '0';
                printf("  \033[1;34m[%c]\033[0m  ", blank_minefield[ctr_3][ctr_2]);
            }
            else if (blank_minefield[ctr_3][ctr_2] == '*')
            {
                blank_minefield[ctr_3][ctr_2] = 'X';
                printf("  \033[1;31m[%c]\033[0m  ", blank_minefield[ctr_3][ctr_2]);
            }
            else if (blank_minefield[ctr_3][ctr_2] == 'X')
            {
                printf("  \033[1;31m[%c]\033[0m  ", blank_minefield[ctr_3][ctr_2]);
            }
            else
            {
                printf("  \033[1;32m[%d]\033[0m  ", blank_minefield[ctr_3][ctr_2]);
            }
            ctr_3++;
        }
        printf("\n");
        ctr_3 = 0;
        ctr_2++;
    }
}

void print_final_minefield( void )                      // Prints the minefield, showing where all of the mines are placed
{
    system("cls");
    int ctr_3 = 0, ctr_2 = 0, ctr_1 = 0;
    while (ctr_1 < ROWS)
    {
        if (ctr_1 == 0)
        {
            gotoxy(6, 3);
        }
        printf("   \033[1;36m%2d\033[0m  ", ctr_1 );
        ctr_1++;
    }
    printf("\n\n");

    while (ctr_2 < COLUMNS)
    {

        printf("\033[1;36m> %2d\033[0m  ", ctr_2 );
         while( ctr_3 < ROWS)
        {
            printf("  \033[1;33m[%c]\033[0m  ", final_minefield[ctr_3][ctr_2]);
            ctr_3++;
        }
        printf("\n");
        ctr_3 = 0;
        ctr_2++;
    }
}

void win( void )                                        // Runs the play_again function
{
    printf("\n\n\n\n\t\t\033[1;32mYOU WIN!!!!!\033\n\n");
    system("pause");
    play_again();
}

void boom( void )                                       // Runs the print_final_minefield function, then the play_again function
{
    print_final_minefield();
    printf("\n\n\n\n\t\t\033[1;31mYOU LOSE!!!!\033\n\n");
    system("pause");
    play_again();
}

void play_again( void )                                 // Gives the user the option to play again
{
    system("cls");
    char option;
    printf("\n\t\tWould you like to play again(Y/N)?: ");
    scanf("%c", &option);
    if((option == 'Y') || (option == 'y'))        // Restarts the program from after the welcome message
    {
        lives=3;
        main();
    }
    else if( (option == 'N') || (option == 'n'))
    {
        game_over();
    }
    else
    {
        printf("Please enter either Y or N:");
        system("pause");
        play_again();
    }
}

void game_over( void )                                  // Ends the program
{
    printf("\n\n\n\n\t\t\033[1;31mGame Over\033\n\n");
    system("pause");
    exit(1);
}

