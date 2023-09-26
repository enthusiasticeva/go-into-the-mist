// name: Eva Jessurun
// unikey: ejes2694
// SID: 500477621

#include <stdio.h>
#include <string.h>

#define BOARD_SIZE 19
#define INSTRUCTION_BUFFER 12 // allows for place a19, which is 10 characters

void make_grid(char grid[BOARD_SIZE][BOARD_SIZE]);
void check_coord(char* coord, int* valid);
char final_char(char string[]);
char second_last_char(char string[]);
int place(char grid[BOARD_SIZE][BOARD_SIZE], char player, char coord[]);
int ascii_to_int(const char s[]);
int get_instruction_col(char instruction[INSTRUCTION_BUFFER]);
int get_instruction_row(char instruction[INSTRUCTION_BUFFER]);
int record(char history[], int* next_history_index, char coord[]);
void change_hole(int hole[], int hole_corner[], char coord[]);
int check_win(char grid[BOARD_SIZE][BOARD_SIZE], char player);
void view_grid(char grid[BOARD_SIZE][BOARD_SIZE], int centre[], int corner[]);
int count_spaces(char string[]);



int main(void) {

    //MAKE THE GRID
    char grid[BOARD_SIZE][BOARD_SIZE];
    make_grid(grid);

    // SETUP HISTORY
    //allows space for all moves to be coords of length 3, plus the \0
    char history[(BOARD_SIZE * BOARD_SIZE*3)+1] = {0};
    int next_history_index = 0; //keeps track of the next free spot in the history

    //SET UP GAME
    char player = '#';

    int win_white = 0;
    int win_black = 0;

    int hole_centre[2] = {10,10};
    int hole_corner[2] = {hole_centre[0]-3, hole_centre[1]-3}; //indicates the top right corner of the hole

    char instruction[INSTRUCTION_BUFFER];

    //keeps track of how many turns, so that if the max amount of turns 
    // has been taken, a draw can be triggered.
    int turns = 0;
    int valid = 1;
    int valid_coord = 1;

    while(fgets(instruction, INSTRUCTION_BUFFER, stdin) != NULL) {
        valid_coord = 1;
        valid = 1;

        if (second_last_char(instruction) == ' ') {
            valid = 0;
        }

        //PLACE <COORD>
        else if (strncmp(instruction, "place ", strlen("place ")) == 0) {
            //seperate the coord from the 'place ' beginning
            char* coord = &instruction[strlen("place ")];
            if (strstr(coord, " ") != NULL) {
                valid = 0;
            }
            int found_spaces = count_spaces(instruction);

            if (found_spaces != 1) {
                valid = 0;
            }
            
            else if ((coord[0] >= 'A' && coord[0] <= 'Z') 
                    || (coord[0] >= 'a' && coord[0] <= 'z')) {
                check_coord(coord, &valid_coord);
                if (valid_coord) {
                    // tries to place the piece
                    int success = place(grid, player, &instruction[strlen("place")+1]);
                    
                    if (success == 0) {
                        record(history, &next_history_index,  coord);
                        change_hole(hole_centre, hole_corner, coord);
                        turns += 1;
                        win_white = check_win(grid, 'o');
                        win_black = check_win(grid, '#');
                    } else if (success == -2){
                        valid_coord = 0;
                    } else {
                        continue;
                    }
                }
            } else {
                valid = 0;
            }
            
            

            

        //TERM
        } else if (strncmp(instruction, "term", strlen("term")) == 0 
                && strlen(instruction) == strlen("term") + 1) {
            return 1;
        

        //WHO
        } else if (strncmp(instruction, "who", strlen("who")) == 0 
                && strlen(instruction) == strlen("who") + 1) {
            if (player == '#') {
                printf("B\n");
            } else {
                printf("W\n");
            }
            continue;

        //VIEW
        } else if (strncmp(instruction, "view", strlen("view")) == 0 
                && strlen(instruction) == strlen("view") + 1) {
            view_grid(grid, hole_centre, hole_corner);
            continue;

        // HISTORY
        } else if (strncmp(instruction, "history", strlen("history")) == 0
                && strlen(instruction) == strlen("history") + 1){
            printf("%s\n", history);
            continue;

        //RESIGN
        } else if (strncmp(instruction, "resign", strlen("resign")) == 0 
                && strlen(instruction) == strlen("resign") + 1){
            if (player == '#') {
                win_black = 0;
                win_white = 1;
            } else {
                win_black = 1;
                win_white = 0;
            }
            
        } else {
            valid = 0;
        }

        // If an input is very long, we only want ot print Invalid once
        // To do this, we only print invalid when we know we are at the 
        // end of the input (because there is a newline). Itherwise, we 
        // keep reading the string in.
        if (valid_coord == 0) {
            printf("Invalid coordinate\n");
            
            //loops through until it finds a \n at the end, 
            //indicating the end of the v long instruction
            while (final_char(instruction) != '\n') {
                fgets(instruction, INSTRUCTION_BUFFER, stdin);
            }

            continue;
        } else if (valid == 0) {
            if (final_char(instruction) == '\n') {
                printf("Invalid!\n");
            }
            continue;
        }

        //CHECK FOR WINS OR DRAWS
        if (turns == 19*19) {
            win_white = 1;
            win_black = 1;
        }

        
        if (win_white == 1 || win_black == 1) {
            if (win_white == 1 && win_black == 1) {
                printf("Wow, a tie!\n");
            } else if (win_white) {
                printf("White wins!\n");
            } else if (win_black) {
                printf("Black wins!\n");
            }
            printf("%s\n",history);
            printf("Thank you for playing!\n");
            return 0;
        }
        

        if (player == '#') {
            player = 'o';
        } else {
            player = '#';
        }


    }    
}

//
// HELPER FUNCTIONS
//

// Takes a 2D array of size BOARD_SIZE, and populates it with '.', indicating an empty game board
void make_grid(char grid[BOARD_SIZE][BOARD_SIZE]) {
    int row = 0;
    while (row < BOARD_SIZE) {
        int col = 0;
        while (col < BOARD_SIZE) {
            grid[row][col] = '.';
            col ++;
        }
        row ++;
    }
}

//Checks that the coord entered is made up of only capital letters and numbers
void check_coord(char* coord, int* valid) {
    if (strlen(coord) < 3) {
        *valid = 0;
    }
    int i = 0;
    while (i < strlen(coord)) {
        char c = coord[i];
        if (!((c >= 'A'  && c <= 'Z') 
            || (c >= 'a'  && c <= 'z') 
            || (c >= '0'  && c <= '9') 
            || c == '\n')) {
            
            *valid = 0;
        }
        i ++;
    }

    if (coord[1] == '0') {
        *valid = 0;
    }
    
}

//returns the final character of a string (whatever is immediately before the '\0')
char final_char(char string[]) {
    int i = 0;
    while (string[i] != '\0') {
        i ++;
    }
    return string[i-1];
}

//returns the final character of a string (whatever is immediately before the '\0')
char second_last_char(char string[]) {
    int i = 0;
    while (string[i] != '\0') {
        i ++;
    }
    if (i>1) {
        return string[i-2];
    }
    return ' ';
}

//takes in an instruction, converts it to indexes, and places a piece appropriately
//returns 0 if success, otherwise returns -1
//returns -2 for invalid coord
//returns -3 for occupied coord
int place(char grid[BOARD_SIZE][BOARD_SIZE], char player, char coord[]) {
    //TODO CHECK IF NEEDED
    // if (strlen(coord) <2) {
    //     return -1;
    // }

    int instruction_col = get_instruction_col(coord);
        
    int instruction_row = get_instruction_row(coord);

    if (instruction_col == -1 || instruction_row == -1) {
        // printf("Invalid coordinate\n");
        return -2;
    }

    if (instruction_col > BOARD_SIZE || instruction_col < 0 
        || instruction_row > BOARD_SIZE || instruction_row < 0) {
        // printf("Invalid coordinate\n");
        return -2;
    }

    if (grid[instruction_row][instruction_col] == '#' 
        || grid[instruction_row][instruction_col] == 'o') {
        printf("Occupied coordinate\n");
        return -3;
    }

    grid[instruction_row][instruction_col] = player;
    return 0;
    
}
//Takes the letter form A-S and converts it into an index
int get_instruction_col(char instruction[INSTRUCTION_BUFFER]) {
    
    if (instruction[0] < 'A' || instruction[0] > 'Z') {
        return -1;
    }
    return instruction[0]-'A';
}

//Takes the string number, and converts it into an index
//1000 means that the number was out of range
// note that this function assumes a valid coordinate format, (<letter><number>[number])
//so checking must be done before passing to it.
int get_instruction_row(char instruction[INSTRUCTION_BUFFER]) {
    int instruction_row = -1;
        
    if (strlen(instruction) == 3) {
        char clipped_row[2] = {instruction[1], '\0'};
        instruction_row = ascii_to_int(clipped_row);

    } else if (strlen(instruction) == 4) {
        char clipped_row[3] = {instruction[1], instruction[2], '\0'};
        instruction_row = ascii_to_int(clipped_row);
    } else {
        return -1;
    }

    if (instruction_row < 1 || instruction_row > 19) {
        return -1;
    }

    return instruction_row-1;
}

//takes in a number in character form and converts it to decimal
int ascii_to_int(const char s[]) { 
    //will store our result
    int full_num = 0;

    // will go from the last character back
    int index = strlen(s)-1;

    // keeps track of our place value
    int place_val = 1;

    //checks whether we're negative or not
    int neg = 1;


    while (index >= 0) {
        //check if its negative
        if (s[index] == '-') {
            neg = -1;
            break;
        }

        // get the digit as a number, times by the place value
        int num = (s[index]-'0') * place_val;

        // add to the result
        full_num = full_num + num;

        // increase the place value and decrease the index
        place_val *= 10;
        index --;
    }

    // make neg/pos
    full_num = full_num * neg;
    return full_num;
}

//Takes the coordinate and adds it to the history
int record(char history[], int* next_history_index, char coord[]) {
    for (int i = 0; i < strlen(coord)-1; i++) {
        history[*next_history_index+i] = coord[i];
    }
    *next_history_index += strlen(coord)-1;
    return 0;
}

//Takes the current mist hole location, and updates it (stored both as centre and top right corner)
void change_hole(int hole[], int hole_corner[], char coord[]) {
    int instruction_col = get_instruction_col(coord)+1;
    int instruction_row = get_instruction_row(coord)+1;

    int x_hole = 1 + ((5 * (instruction_col * instruction_col) + 3 * (instruction_col) + 4) % 19);
    int y_hole = 1 + ((4 * (instruction_row * instruction_row) + 2 * (instruction_row) - 4) % 19);

    hole[0] = x_hole;
    hole[1] = y_hole;

    hole_corner[0] = x_hole-3;
    hole_corner[1] = y_hole-3;
}

//takes in the board and cheacks for a win, checkinng for lines of 5 in horizontal, vertical, forward and backward diagonals
//0 - no win
//1 - win detected
int check_win(char grid[BOARD_SIZE][BOARD_SIZE], char player) {
    //horizontal wins
    int row = 0;
    while (row < BOARD_SIZE) {
        int col = 0;

        //loop through and check each line of 5
        while (col < BOARD_SIZE-3) {
            if (grid[row][col] == player 
                && grid[row][col+1] == player 
                && grid[row][col+2] == player 
                && grid[row][col+3] == player 
                && grid[row][col+4] == player) {

                return 1;
            }
            col ++;
        }
        row ++;
    }

    //verical wins
    int col = 0;
    while (col < BOARD_SIZE) {
        row = 0;

        //loop through and check each line of 5
        while (row < BOARD_SIZE-3) {
            if (grid[row][col] == player 
                && grid[row+1][col] == player 
                && grid[row+2][col] == player 
                && grid[row+3][col] == player 
                && grid[row+4][col] == player) {

                return 1;
            }
            row ++;
        }
        col ++;
    }

    //diagonal wins
    row = 0;
    //loop through and check each forward diagonal of 5
    while (row < BOARD_SIZE-3) {
        col = 0;
        while (col < BOARD_SIZE-3) {
            if (grid[row][col] == player 
                && grid[row+1][col+1] == player 
                && grid[row+2][col+2] == player 
                && grid[row+3][col+3] == player 
                && grid[row+4][col+4] == player) {

                return 1;
            }
            col ++;
        }
        row++;
    }

    //diagonal wins
    row = 4;
    //loop through and check each forward diagonal of 5
    while (row < BOARD_SIZE) {
        col = 0;
        while (col < BOARD_SIZE-3) {
            if (grid[row][col] == player 
                && grid[row-1][col+1] == player 
                && grid[row-2][col+2] == player 
                && grid[row-3][col+3] == player 
                && grid[row-4][col+4] == player) {

                return 1;
            }
            col ++;
        }
        row++;
    }



    return 0;

}

//creates the appropriate outpur for view, the centre coord + the states of the 7x7 square
void view_grid(char grid[BOARD_SIZE][BOARD_SIZE], int centre[], int corner[]) {

    printf("%c%d,", 'A'+centre[0]-1, centre[1]);

    //loops through a 7x7 square, converts this square's indexes to the 
    // actual grids, and checks if they are/arent actually on the game board
    int col_index = 6;
    while (col_index >= 0) {
        int row_index = 0;
        while (row_index < 7) {

            int real_row = corner[0] + row_index-1;
            int real_col = corner[1] + col_index-1;

            if (real_row < BOARD_SIZE && real_row >= 0 
                && real_col  < BOARD_SIZE && real_col >= 0) {
                //exists on board
                printf("%c", grid[real_row][real_col] );
            } else {
                //off board
                printf("x");
            }
            row_index ++;

        }
        col_index --;
    }
    printf("\n");

}

//Given a string, counts the amount fo spaces in it
int count_spaces(char string[]) {
    int i = 0;
    int count = 0;
    while (i < strlen(string)) {
        if (string[i] == ' ') {
            count ++;
        }
        i ++;
    }
    return count;
}