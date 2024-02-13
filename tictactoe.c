/*
 * tictactoe.c
 * 
 * Copyright 2024 Reslashd
 * Update 3.0 based on extensive feedback / improved solution by depperm (thanks!)
 * 
 * A simple program to play Tic Tac Toe versus a friend or a
 * (very basic) computer controlled opponent. * 
 * 
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

bool checkMove(char grid[][3], int move);
bool checkThree(char grid[][3]);
int checkTwo(char grid[][3]);
void clearScreen(void);
int cpuMove(char grid[][3], int turn, int difficulty);
void drawGrid(char grid[][3]);
int gameLoop(char grid[][3], int numPlayers, int difficulty);
int getMove(char grid[][3], char mark, int numPlayers, int turns, int difficulty);
char getStartingMark(int numPlayers);
void makeMove(char grid[][3], int move, char mark);
void pressToContinue(void);
void resetGrid(char grid[][3]);
int setDifficulty(int currentDifficulty);
int showMenu(void);
void showScores(int n_owins, int n_xwins, int n_draws);
char switchMark(char currentMark);
int randMove(char grid[][3]);

int main(void){
  char grid[3][3] = {{'1','2','3'}, {'4','5','6'}, {'7','8','9'}};
  int choice = '0';
  int difficulty = '1'; 
  int n_draws = 0;
  int n_owins = 0;
  int n_xwins = 0; 
  int winner = 0; 
  srand((unsigned int)time(NULL));


  while ((choice = showMenu()) != '5' && choice != EOF){		
    if (choice == '1' || choice == '2'){
      winner = gameLoop(grid, choice, difficulty);
      clearScreen();
      drawGrid(grid);
      if(winner == 1){
        puts("\nThree in a row!");
        puts("O wins!");
        n_owins++;
      }
      else if (winner == 2){
        puts("\nThree in a row!"); 
        puts("X wins!");
        n_xwins++;
      }	
      else{
        puts("\nIts a draw!");
        n_draws++;
      }	
      resetGrid(grid);
      winner = 0;
      pressToContinue();
    } else if (choice == '3') { 
      showScores(n_owins, n_xwins, n_draws);
    } else if (choice == '4') {
      difficulty = setDifficulty(difficulty);		
    }		
  }
  return EXIT_SUCCESS;
}

bool checkMove(char grid[][3], int move){
  /*
   * Check move by checking if 'move' converted to int
   * is equal to grid position.
   * 
   * Example check move '2' which is grid position ([0][1])
   * 
   * Row is 50 - 49 = 1 / 3 = 0.33 
   * Integer division so this is rounded down towards 0. 
   * Column = 50 - 49 = 1 % 3 = 1
   * Result [0][1] which on the grid is '2'.
   * 
   */
  bool move_ok = false;

  if (move >= '1' && move <= '9'){
    int row = (move - 49) / 3; 
    int column = (move - 49) % 3;			
    if(grid[row][column] == move){
      move_ok = true;
    }
  }

  return move_ok;
}

int randMove(char grid[][3]){
  int randMove;
  do{
    randMove = (rand() % 9) + 49;	
  } while(!checkMove(grid, randMove));
  return randMove; // Failsafe return random move.
}

bool checkThree(char grid[][3]){
  /*
   * This functions checks for 'three in a row' by checking the 
   * if the 3 adjacent grid cells have the same value ('X' or 'O')
   * Returns true if a three in a row is found.
   */

  // check diagonal left to right
  if (grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2]) {
    return true;
  }	
  // check diagonal right to left
  if (grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0]) {
    return true;
  }

  for(int i=0;i<3;i++){
    // row check
    if(grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2]){
      return true;
    }
    // col check
    if(grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i]){
      return true;
    }
  }
  return false;
}

int checkTwo(char grid[][3]){
/*	
 *  This function checks if there are 2 grid cells with the same value
 *  in a row, column or one of the diagonals and if a counter or
 *  winning move is possible (there is no X or O in the cell).
 *  It then returns a counter/winning move on the grid to make i.e. '1'  
 *  to make a move for the top left position on the grid.
 */

  char gridPos[3][3] = {{'1','2','3'}, {'4','5','6'}, {'7','8','9'}};
  for(int i=0;i<3;i++){
    // rows check
    if(grid[i][0] == grid[i][1] && grid[i][1] != grid[i][2]){
      if(checkMove(grid, gridPos[i][2])){
        return gridPos[i][2];
      }
    } else if (grid[i][0] != grid[i][1] && grid[i][1] == grid[i][2]) {
      if (checkMove(grid, gridPos[i][0])){
        return gridPos[i][0];
      }
    } else if (grid[i][0] == grid[i][2]) {
      if (checkMove(grid, gridPos[i][1])){
        return gridPos[i][1];
      }
    }
    // columns check
    if(grid[0][i] == grid[1][i] && grid[1][i] != grid[2][i]){
      if(checkMove(grid, gridPos[2][i])){
        return gridPos[2][i];
      }
    } else if (grid[0][i] != grid[1][i] && grid[1][i] == grid[2][i]) {
      if (checkMove(grid, gridPos[0][i])){
        return gridPos[0][i];
      }
    } else if (grid[0][i] == grid[2][i]) {
      if (checkMove(grid, gridPos[1][i])){
        return gridPos[1][i];
      }
    }
  }

  // check diagonal left to right
  if (grid[0][0] == grid[1][1] && grid[1][1] != grid[2][2]) {
    if (checkMove(grid, '9')){
      return '9';
    }
  } else if (grid[0][0] != grid[1][1] && grid[1][1] == grid[2][2]) {
    if (checkMove(grid, '1')){
      return '1';
    }
  } 	else if (grid[0][0] == grid[2][2]) {
    if (checkMove(grid, '5')){
      return '5';
    }
  }

  // check diagonal right to left
  if (grid[0][2] == grid[1][1] && grid[1][1] != grid[2][0]) {
    if (checkMove(grid, '7')){
      return '7';
    }
  } else if (grid[0][2] != grid[1][1] && grid[1][1] == grid[2][0]) {
    if (checkMove(grid, '3')){
      return '3';
    }
  } 	else if (grid[0][2] == grid[2][0]) {
    if (checkMove(grid, '5')){
      return '5';
    }
  }

  return randMove(grid);
}


void clearScreen(void){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}

int cpuMove(char grid[][3], int turn, int difficulty){
  char first_turn[5] = {'1','3', '5', '7', '9'};
  char second_turn[4] = {'1','3', '7', '9'};
  int n = 0;	
  int move = '0';

  if(turn == 1){
    // First turn strategy - take center or a corner
      n = rand() % 5;
      move = first_turn[n];	
  } else if (turn == 2){
    // Second turn strategy - take center if possible else a corner
    if (checkMove(grid, '5')){
      move = '5';
    }
    else {
      n = rand() % 4;
      move = second_turn[n];
    }	
  } else if (turn == 3){
    // Third turn strategy - take center if possible else random move
    if(checkMove(grid, '5')){
      move = '5';
    }
    else {
      move = randMove(grid); 
    }
    // Remaining turns
  } else {	
    //easy 
    if (difficulty == '1'){
      move = randMove(grid);  // Random move '1' - '9'
    }
    // hard
    else if (difficulty == '2'){
      move = checkTwo(grid); // Analyze grid and make move
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

int gameLoop(char grid[][3], int numPlayers, int difficulty){
  int move = '0';
  int turn = 1;
  char mark = getStartingMark(numPlayers);
  clearScreen();	
  while(turn != 10) {
    clearScreen();
    drawGrid(grid);
    move = getMove(grid, mark, numPlayers, turn, difficulty);
    makeMove(grid, move, mark);
    if(checkThree(grid)){
      if(mark == 'O'){
        return 1;
      }
      else {
        return 2;
      }
    }	
    mark = switchMark(mark);
    turn++;	
  }
  return 0;
}

int getMove(char grid[][3], char mark, int numPlayers, int turn, int difficulty){	
  bool move_ok = false;
  int move = '0';

  // get Human player move 
  if(numPlayers == '2' || (numPlayers == '1' && mark == 'X')){	
    while(move_ok != 1){				
      printf("\nIt is %c's turn, place your mark: ", mark);
      while((move = getchar()) == '\n');	

      move_ok = checkMove(grid, move);
      if (!move_ok){
        puts("Invalid move please try again...");
        pressToContinue();
        clearScreen();
        drawGrid(grid);
      } 
    }
  }
  // get CPU move
  else if (numPlayers == '1' && mark == 'O'){		
    move = cpuMove(grid, turn, difficulty);									
  }
  return move;	
}

char getStartingMark(int numPlayers){
  int start_mark = 0;

  puts("\nPlease choose starting player");
  if (numPlayers == '1'){
    puts("1 = O (CPU)");
  } else if (numPlayers == '2'){
    puts("1 = O");
  }
  puts("2 = X");
  getchar();
  start_mark = getchar();
  if (start_mark == '1'){
    return 'O';
  } else {
    return 'X';
  }
}

void makeMove(char grid[][3], int move, char mark){	
  /*
   * Example place mark at grid position '2' ([0][1]):
   * 
   * Row is 50 - 49 = 1 / 3 = 0.33 
   * Integer division so this is rounded down towards 0. 
   * Column = 50 - 49 = 1 % 3 = 1
   * Result [0][1] which on the grid is '2'.
   * 
   */	
  int row = (move - 49) / 3;
  int column = (move - 49) % 3;

  grid[row][column] = mark;						
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

int setDifficulty(int currentDifficulty){
  clearScreen();
  puts("\n***** SET DIFFICULTY *****\n");
  if (currentDifficulty == '1'){
    puts("Current difficulty is EASY\n");
  } else if (currentDifficulty == '2'){
    puts("Current difficulty is HARD\n");
  }
  puts("1 = Easy");
  puts("2 = Hard");
  fputs("\nChoose your difficulty: ", stdout);
  getchar();
  int input = getchar();

  if (input == '1'){				
    puts("Difficulty set to EASY");
    pressToContinue();	
    return '1';
  } else if (input == '2'){
    puts("Difficulty set to HARD");
    pressToContinue();	
    return '2';
  }
  else{
    puts("\nInvalid choice!");
    puts("Difficulty unchanged.");
    pressToContinue();	
    return currentDifficulty;
  }	
}

int showMenu(void){
  clearScreen();
  puts("\nTIC TAC TOE v3.0 by Reslashd");
  puts("\n******* MENU *******\n");
  puts("1 = 1 Player vs CPU");
  puts("2 = 2 Players");
  puts("3 = View scores");
  puts("4 = Set difficulty");	
  puts("5 = Exit");
  
  int choice = getchar();
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

char switchMark(char currentMark){
  if (currentMark == 'X'){
    return 'O';
  } else {
    return 'X';
  }
}
