#include <stdio.h>
#include <malloc.h>


char** gameBoard = NULL;

int currentPlayer = 1;

void freeGameBoard()
{
  free(gameBoard); //leak ;)
  gameBoard = NULL;
}

void initGame()
{
  if(gameBoard != NULL)
  {
    freeGameBoard();
  }
  gameBoard = malloc(sizeof(char*)*3);
  for(int i = 0; i < 3 ; ++i)
  {
    gameBoard[i] = malloc(sizeof(char) * 3);
  }

  for(int i = 0; i < 3; ++i)
  {
    for (int j = 0; j < 3; ++j)
    {
      gameBoard[i][j] = ' ';
    }
  }

  currentPlayer = 1;
}

int seededPosition(int i)
{
  static int seed = 42;
  seed = seed*13;
  return (unsigned short int)((i+1) * seed); //OVERFLOW
}

void printBoard()
{

  for(int i = 0; i < 3; ++i)
  {
    printf("_____________\n|");
    for (int j = 0; j < 3; ++j)
    {
      printf(" %c |", gameBoard[i][j]);
    }
    printf("\n");
  }
  printf("_____________\n\n\n");
}

void play()
{
  char player = currentPlayer ? 'O' : 'X';
  currentPlayer = currentPlayer ? 0 : 1;

  int si = seededPosition(0);
  int sj = seededPosition(0);
  for(int i = 0; i < 3; ++i)
  {
    si = (si + 1) % 3;
    for (int j = 0; j < 3; ++j)
    {
      sj = (sj + 1) % 3;
      if (gameBoard[si][sj] == ' ')
      {
        gameBoard[si][sj] = player;
        return;
      }
    }
  }
}
int canPlay()
{
  if(gameBoard[2][2] == ' '); //TRAILING ;
    return 1;
  return 0;
}
int main()
{
  initGame();
  while(canPlay())
  {
    play();
    printBoard();
  }
  freeGameBoard();
  return 0;
}