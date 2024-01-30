/*
 * tictactoe.c
 * 
 * Copyright 2024 Reslashd
 * 
 * A simple program to play Tic Tac Toe versus a friend or a
 * (very basic) computer controlled opponent.
 * 
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

bool checkMove(char grid[][3], int move, bool move_ok);
int checkThree(char grid[][3], int three_row);
int checkTwo(char grid[][3], int move);
void clearScreen(void);
int cpuMove(char grid[][3], int move, int turn, int difficulty);
void drawGrid(char grid[][3]);
int gameLoop(char grid[][3], int choice, int difficulty, int three_row);
int getMove(char grid[][3], int move, char mark, int choice, int turns, int difficulty);
void makeMove(char grid[][3], int move, char mark);
bool needMove(bool need_move, int nX, int nO, int result);
int pickCpuMove(int move, int location);
void pressToContinue(void);
void resetGrid(char grid[][3]);
void resetVars(int *nX_ptr, int *nO_ptr, int *result_ptr);
int setDifficulty(int difficulty);
int showMenu(int choice);
void showScores(int n_owins, int n_xwins, int n_draws);
char startingMark(char mark, int choice);
char switchMark(char mark);

int main(void){
	char grid[3][3] = {{'1','2','3'}, {'4','5','6'}, {'7','8','9'}};
	int choice = 0;
	int difficulty = '1';
	int n_draws = 0;
	int n_owins = 0;
	int n_xwins = 0; 
	int three_row = 0; 
	srand((unsigned int)time(NULL));

	
	while ((choice = showMenu(choice)) != '5'){		
		if (choice == '1' || choice == '2'){
			three_row = gameLoop(grid, choice, difficulty, three_row);
			clearScreen();
			drawGrid(grid);
			if(three_row == 1){
				puts("\nThree in a row!");
				puts("O wins!");
				n_owins++;
			}
			else if (three_row == 2){
				puts("\nThree in a row!"); 
				puts("X wins!");
				n_xwins++;
			}	
			else{
				puts("\nIts a draw!");
				n_draws++;
			}	
			resetGrid(grid);
			three_row = 0;
			pressToContinue();
		} else if (choice == '3') { 
			showScores(n_owins, n_xwins, n_draws);
		} else if (choice == '4') {
			difficulty = setDifficulty(difficulty);		
		}		
	}
	return EXIT_SUCCESS;
}

bool checkMove(char grid[][3], int move, bool move_ok){
	size_t column = 0;
	size_t row = 0;
	
	if (move >= '1' && move <= '9'){			
		for (row = 0 ; row < 3; row++){
				for(column = 0; column < 3 ; column++){
					if (grid[row][column] == move){ 
						move_ok = 1;	
						goto end_checkmove;						
					}	
					else{
						move_ok = 0;
					}
				}				
		}		
	} else {
		move_ok = 0;
	}

	end_checkmove:
	return move_ok;
}

int checkThree(char grid[][3], int three_row){
	/*
	 * This functions checks for 'three in a row' by checking the 
	 * total decimal number in a row, column and diagonal.
	 * 3 times decimal 79 (OOO) = 237
	 * 3 times decimal 88 (XXX) = 264 
	 * Returns 0 = draw, 1 = O wins, 2 = X wins
	 */
	 
	size_t column = 0;
	size_t row = 0;
	int result = 0;	

	// check diagonal left to right
	for (row = 0 ; row < 3; row++){
		result = result + grid[row][row];
		if (result == 237 || result == 264){
			if (result == 237) {
				three_row = 1;				
			} else{
				three_row = 2;
			}			
			goto return_result;
		}	
	}
	result = 0;
	
	// check diagonal right to left
	for (row = 0 ; row < 3; row++){
		result = result + grid[row][3 - row - 1];
		if (result == 237 || result == 264){
			if (result == 237) {
				three_row = 1;				
			} else{
				three_row = 2;
			}			
			goto return_result;
		}	
	}
	result = 0;

	// check rows
	for (row = 0 ; row < 3; row++){
		result = grid[row][0]+grid[row][1]+grid[row][2];	
		if (result == 237 || result == 264){
			if (result == 237) {
				three_row = 1;				
			} else{
				three_row = 2;
			}			
			goto return_result;
		}	
	}
	result = 0;
	
	// check columns
	for (column = 0 ; column < 3; column++){
		result = grid[0][column]+grid[1][column]+grid[2][column];	
		if (result == 237 || result == 264){
			if (result == 237) {
				three_row = 1;
			} else{
				three_row = 2;
			}
		}	
	}	

	return_result:	
	return three_row;
}

int checkTwo(char grid[][3], int move){
/*	
 *  This function checks if there are 2 XX's or OO's in a row,
 *	column or one of the diagonals and if a certain counter move by 
 *  the CPU is needed.  
 * 
 *	If needed it picks (randomly until the empty spot is found) a  
 * 	counter move from a set of possible moves to either prevent a loss   
 * 	or make a move to win the game.
 *  
 */
	size_t column = 0;
	size_t row = 0;
	
	bool need_move = 0;
	int nO = 0;
	int nX = 0;
	int result = 0;
		
	int *nX_ptr = &nX;
	int *nO_ptr = &nO;
	int *result_ptr = &result;	

	// check diagonal left to right 
	for (row = 0 ; row < 3; row++){
		result = result + grid[row][row];
		if(grid[row][row] == 'X'){
			nX++;
		}
		else if (grid[row][row] == 'O'){
			nO++;
		}	
	}	
	need_move = needMove(need_move, nX, nO, result);
	if (need_move == 1){
		move = pickCpuMove(move, 1);		
		goto return_move;
	}	

	resetVars(nX_ptr, nO_ptr, result_ptr);
		
	// check diagonal right to left 
	for (row = 0 ; row < 3; row++){
		result = result + grid[row][3 - row - 1]; 
		if(grid[row][3 - row - 1] == 'X'){
			nX++;
		}
		else if (grid[row][3 - row - 1] == 'O'){
			nO++;
		}
	}
	need_move = needMove(need_move, nX, nO, result);
	if (need_move == 1){
		move = pickCpuMove(move, 2);		
		goto return_move;
	}		 

	resetVars(nX_ptr, nO_ptr, result_ptr); 

	// check rows 
	for (row = 0 ; row < 3; row++){	
		resetVars(nX_ptr, nO_ptr, result_ptr);
		for(column = 0 ; column < 3; column++){
			if(grid[row][column] == 'X'){
				nX++;
			}
			else if(grid[row][column] == 'O'){
				nO++;				
			}
			result = grid[row][0]+grid[row][1]+grid[row][2];	
			need_move = needMove(need_move, nX, nO, result);
			if (need_move == 1){
				move = pickCpuMove(move, (int)row + 3); // location 3, 4, 5
				goto return_move;
			}	
		}	
	}	

	resetVars(nX_ptr, nO_ptr, result_ptr);
	
	// check columns 
	for (column = 0 ; column < 3; column++){	
		resetVars(nX_ptr, nO_ptr, result_ptr);
		for(row = 0 ; row < 3; row++){
			if(grid[row][column] == 'X'){
				nX++;
			}
			else if(grid[row][column] == 'O'){
				nO++;				
			}
			result = grid[0][column]+grid[1][column]+grid[2][column];	
			need_move = needMove(need_move, nX, nO, result);
			if (need_move == 1){
				move = pickCpuMove(move, (int)column + 6); // location 6, 7, 8
				goto return_move;
			}	
		}	
	}		

	move = (rand() % 9) + 49; // Failsafe return random move.
	return_move:	
	return move;	
}

void clearScreen(void){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}

int cpuMove(char grid[][3], int move, int turn, int difficulty){
	char first_turn[5] = {'1','3', '5', '7', '9'};
	char second_turn[4] = {'1','3', '7', '9'};
	int n = 0;	
	
	if(turn == 1){
		// First turn strategy - take center or a corner
			n = rand() % 5;
			move = first_turn[n];	
	} else if (turn == 2){
		// Second turn strategy - take center if possible else a corner
		if(grid[1][1] != 'X' && grid[1][1] != 'O'){
			move = '5';
		}
		else if(grid[1][1] == 'X' || grid[1][1] == 'O'){
			n = rand() % 4;
			move = second_turn[n];
		}	
	} else if (turn == 3){
		// Third turn strategy - take center if possible else random move
		if(grid[1][1] != 'X' && grid[1][1] != 'O'){
			move = '5';
		}
		else {
			move = (rand() % 9) + 49; 
		}
		// Remaining turns
	} else {	
		//easy 
		if (difficulty == '1'){
			move = (rand() % 9) + 49;  // Random move '1' - '9'
		}
		// hard
		else if (difficulty == '2'){
			move = checkTwo(grid, move); // Analyze grid and make move
		}	 		
	}	
	return move;
}

void drawGrid(char grid[][3]){
	printf("\n%c | %c | %c\n", grid[0][0], grid[0][1], grid[0][2]);
	puts("=========");
	printf("%c | %c | %c\n", grid[1][0], grid[1][1], grid[1][2]);
	puts("=========");
	printf("%c | %c | %c\n", grid[2][0], grid[2][1], grid[2][2]);	
}

int gameLoop(char grid[][3], int choice, int difficulty, int three_row){
	int move = 0;
	int turn = 1;
	char mark = 'X';
	
	mark = startingMark(mark, choice);
	clearScreen();	
	while(turn != 10) {
		clearScreen();
		drawGrid(grid);
		move = getMove(grid, move, mark, choice, turn, difficulty);
		makeMove(grid, move, mark);	
		mark = switchMark(mark);
		turn++;
		three_row = checkThree(grid, three_row);
		if(three_row == 1 || three_row == 2){
			break;
		}		
	}
	return three_row;
}

void makeMove(char grid[][3], int move, char mark){	
	size_t column = 0;
	size_t row = 0;
	
	for (row = 0 ; row < 3; row++){
		for(column = 0; column < 3 ; column++){
			if (grid[row][column] == move){ 
				grid[row][column] = mark;						
			}	
		}				
	}
}

bool needMove(bool need_move, int nX, int nO, int result){
	if (nX == 2 || nO == 2){
		if (result < 237){
			need_move = 1;
		}		 
	} 
	else{
		need_move = 0;
	}
	return need_move;
}

int pickCpuMove(int move, int location){
/*	
 *  Return a (random until empty spot is found) move from a 
 *  selection of moves based on where the sequence XX or OO was 
 *  found on the grid.
*/
	int n = 0;
	char diag_LR[3] = {'1', '5', '9'}; // location 1 
	char diag_RL[3] = {'3', '5', '7'}; // location 2
	char row_One[3] = {'1', '2', '3'}; // location 3
	char row_Two[3] = {'4', '5', '6'}; // location 4
	char row_Three[3] = {'7', '8', '9'}; // location 5
	char col_One[3] = {'1', '4', '7'}; // location 6
	char col_Two[3] = {'2', '5', '8'}; // location 7
	char col_Three[3] = {'3', '6', '9'}; // location 8
	n = rand() % 3;
	
	if(location == 1){
		move = diag_LR[n];
	} else if(location == 2){
		move = diag_RL[n];
	} else if(location == 3){
		move = row_One[n];
	} else if (location == 4){
		move = row_Two[n];
	} else if (location == 5){
		move = row_Three[n];
	} else if(location == 6){
		move = col_One[n];
	} else if (location == 7){
		move = col_Two[n];
	} else if (location == 8){
		move = col_Three[n];
	}
	return move;
}

void pressToContinue(void){
	puts("\nPress ENTER to continue");
	getchar();
	getchar();	
}

void resetGrid(char grid[][3]){
	size_t column = 0;
	size_t row = 0;	
	char first = '1';
	
	for (row = 0 ; row < 3; row++){
		for(column = 0; column < 3 ; column++){
			grid[row][column] = first;
			first++;		
		}				
	}	
	
}
void resetVars(int *nX_ptr, int *nO_ptr, int *result_ptr){
	*nX_ptr = 0;
	*nO_ptr = 0;
	*result_ptr = 0;	
}

int setDifficulty(int difficulty){
	int input = 0;
	clearScreen();
	puts("\n***** SET DIFFICULTY *****\n");
	if (difficulty == '1'){
		puts("Current difficulty is EASY\n");
	} else if (difficulty == '2'){
		puts("Current difficulty is HARD\n");
	}
	puts("1 = Easy");
	puts("2 = Hard");
	fputs("\nChoose your difficulty: ", stdout);
	getchar();
	input = getchar();
	
	if (input == '1'){				
		difficulty = '1';				
		puts("Difficulty set to EASY");
	} else if (input == '2'){
		difficulty = '2';	
		puts("Difficulty set to HARD");
	}
	else{
		puts("\nInvalid choice!");
	}	
	pressToContinue();	
	return difficulty;
}

int showMenu(int choice){
	clearScreen();
	puts("\nTIC TAC TOE v1.0 by Reslashd");
	puts("\n******* MENU *******\n");
	puts("1 = 1 Player vs CPU");
	puts("2 = 2 Players");
	puts("3 = View scores");
	puts("4 = Set difficulty");	
	puts("5 = Exit");
	choice = getchar();
	return choice;
}

void showScores(int n_owins, int n_xwins, int n_draws){
	clearScreen();
	puts("\n***** SCORES *****");
	printf("\nO has %d wins", n_owins);
	printf("\nX has %d wins", n_xwins);	
	printf("\nThere are %d draws\n", n_draws);
	pressToContinue();
}

char startingMark(char mark, int choice){
	int start_mark = 0;
		
	puts("\nPlease choose starting player");
	if (choice == '1'){
		puts("1 = O (CPU)");
	} else if (choice == '2'){
		puts("1 = O");
	}
	puts("2 = X");
	getchar();
	start_mark = getchar();
	if (start_mark == '1'){
		mark = 'O';
	} else if (start_mark == '2'){
		mark = 'X';
	}
	return mark;
}

char switchMark(char mark){
	if (mark == 'X'){
		mark = 'O';
	} else if (mark == 'O') {
		mark = 'X';
	}	
	return mark;
}

int getMove(char grid[][3], int move, char mark, int choice, int turn, int difficulty){	
	bool move_ok = 0;
	
	// get Human player move 
	if(choice == '2' || (choice == '1' && mark == 'X')){	
		while(move_ok != 1){				
			printf("\nIt is %c's turn, place your mark: ", mark);
			while((move = getchar()) == '\n');	
		
			move_ok = checkMove(grid, move, move_ok);
			if (move_ok != 1){
				puts("Invalid move please try again...");
				pressToContinue();
				clearScreen();
				drawGrid(grid);
			} 
		}
	}
	// get CPU move
	else if (choice == '1' && mark == 'O'){		
		while (move_ok != 1){	
			move = cpuMove(grid, move, turn, difficulty);
			move_ok = checkMove(grid, move, move_ok);
		}										
	}
	return move;	
}
