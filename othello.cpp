#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <cstring>
#define SIZE 6  //Tamaño del tablero, debe ser par mayor o igual a cuatro para que se de el juego
using namespace std;


/* Function prototypes */
void display(char board[][SIZE]);
int valid_moves(char board[][SIZE], int moves[][SIZE], char player); 
void make_move(char board[][SIZE], int row, int col, char player);  
void computer_move(char board[][SIZE], int moves[][SIZE], char player);  
int get_score(char board[][SIZE], char player);
int min_max(int row_act, int col_act, char board[][SIZE], int moves[][SIZE], char player);
string name_player;

static void help(){
	
	cout
		<< "\n--------------------------------------------------------------------------" << endl
		<< "\n********************************* OTHELLO *********************************\n\n"<<endl
		<< " Ingresa tu nombre: ";
	cin>>name_player;
	cout
		<< " Te dejare iniciar primero tu y luego yo, tomaremos turnos.\n"<<endl
		<< "\tSeras las blancas - (O)\n\tYo las negras (@)\n" << endl
		<< " Seleccionaras la casilla en donde quere tirar poniendo la coordenada \n"<<endl
		<< " digito y  letra sin espacios, ejemplo: 5c\n"<<endl
		<< "--------------------------------------------------------------------------"   << endl<<endl
		<< " Cuando estes listo presiona enter, y BUENA SUERTE \n"
		<< endl;	
}

int main()
{
	char board [SIZE][SIZE] = { 0 };  /* The board           */
	int moves[SIZE][SIZE] = { 0 };    /* Valid moves         */
	int row = 0;                      /* Board row index     */
	int col = 0;                      /* Board column index  */
	int no_of_games = 0;              /* Number of games     */
	int no_of_moves = 0;              /* Count of moves      */
	int invalid_moves = 0;            /* Invalid move count  */
	int comp_score = 0;               /* Computer score      */
	int user_score = 0;               /* Player score        */
	char y = 0;                       /* Column letter       */
	int x = 0;                        /* Row number          */
	char again = 0;                   /* Replay choice input */
	int player = 0;                   /* Player indicator    */
	
	help();
	
	scanf("%c", &again);
	
	do
		{
			/* On even games the player starts; */
			/* on odd games the computer starts */
			player = ++no_of_games % 2; 
			no_of_moves = 4;                /* Starts with four counters */
			
			/* Blank all the board squares */    
			for(row = 0; row < SIZE; row++)
				for(col = 0; col < SIZE; col++)
					board[row][col] = ' ';
			
			/* Place the initial four counters in the center */
			board[SIZE/2 - 1][SIZE/2 - 1] = board[SIZE/2][SIZE/2] = 'O';
			board[SIZE/2 - 1][SIZE/2] = board[SIZE/2][SIZE/2 - 1] = '@';
			
			/* The game play loop */
			do
				{
					display(board);             /* Display the board  */
					if(player++ % 2)
					{ /*   It is the player's turn                    */
						if(valid_moves(board, moves, 'O'))
						{
							/* Read player moves until a valid move is entered */
							for(;;)
							{
								fflush(stdin);              /* Flush the keyboard buffer */
								printf(" Entonces, que vas a mover? (fila columna): "); 
								scanf("%d%c", &x, &y);              /* Read input        */
								y = tolower(y) - 'a';         /* Convert to column index */
								x--;                          /* Convert to row index    */
								if( x>=0 && y>=0 && x<SIZE && y<SIZE && moves[x][y])
								{
									make_move(board, x, y, 'O');
									no_of_moves++;              /* Increment move count */
									break;
								}
								else
									cout<<"\n Lo siento "<<name_player<<", pero esa es una jugada ilegal...\n Trata de nuevo, seguro que puedes hallar una :)\n\n";
							}
						}
						else                              /* No valid moves */
							if(++invalid_moves<2)
							{
								fflush(stdin);
								cout<<"\n Y no tienes ninguna jugada posible "<<name_player<<",\n ni modos presiona enter para que sea mi turno.";
								scanf("%c", &again);
							}
							else
								printf("\n Okey, ninguno puede hacer otra jugada, asi que hasta aqui llega nuestro juego.\n Momento de los resultados:\n");
					}
					else
						{ /* It is the computer's turn                    */
							if(valid_moves(board, moves, '@')) /* Check for valid moves */
							{
								invalid_moves = 0;               /* Reset invalid count   */
								computer_move(board, moves, '@');
								no_of_moves++;                   /* Increment move count  */
							}
							else
								{
									if(++invalid_moves<2)
										cout<<"\n Y parece que no puedo hacer nada...\n Paso, tu turno de nuevo "<<name_player<<".\n"; 
									else
										printf("\n Okey, ninguno puede hacer otra jugada, asi que hasta aqui llega nuestro juego.\n Momento de los resultados:\n");
								}
						}
				}while(no_of_moves < SIZE*SIZE && invalid_moves<2);
			
			/* Game is over */
			display(board);  /* Show final board */
			
			/* Get final scores and display them */
			comp_score = user_score = 0; 
			for(row = 0; row < SIZE; row++)
				for(col = 0; col < SIZE; col++)
				{
					comp_score += board[row][col] == '@';
					user_score += board[row][col] == 'O';
				}
			printf("Y la puntuacion final es: \n");
			cout<<"\tYo "<<comp_score<<" puntos\n";
			cout<<"\t"<<name_player<<" "<<user_score<<" puntos\n";
			
			fflush(stdin);
			
			if(comp_score>user_score){
				cout<<" Jaja, parece que la victoria fue mia esta vez\n\t Quieres la revancha "<<name_player<<"? (y/n): ";}
			if(comp_score==user_score){
				cout<<" Asi que ha sido un empate...\n"<<name_player<< ", no podemos dejarlo asi.\n\t Listo para el desempate? (y/n) :";}
			if(comp_score<user_score){
				cout<<" Has salido victorioso esta vez "<<name_player<<", pero he aprendido de mis errores\n\t Te atreves a comprobarlo con otro juego? (y/n): ";}
			
			scanf("%c", &again);         /* Get y or n             */
		}while(tolower(again) == 'y'); /* Go again on y          */
	
	cout<<"\n Entonces parece que esto a sido todo por hoy "<<name_player<<", juguemos de nuevo en otra ocasion\n"; 
	return 0;
	
}

/***********************************************
* Function to display the board in it's       *
* current state with row numbers and column   *
* letters to identify squares.                *
* Parameter is the board array.               *
***********************************************/
void display(char board[][SIZE])
{
	int row  = 0;          /* Row index      */
	int col = 0;           /* Column index   */
	char col_label = 'a';  /* Column label   */
	
	printf("\n ");         /* Start top line */
	for(col = 0 ; col<SIZE ;col++)
		printf("   %c", col_label+col); /* Display the top line */
	printf("\n");                     /* End the top line     */
	
	/* Display the intermediate rows */  
	for(row = 0; row < SIZE; row++)
	{
		printf("  +");
		for(col = 0; col<SIZE; col++)
			printf("---+");
		printf("\n%2d|",row + 1); 
		
		for(col = 0; col<SIZE; col++)
			printf(" %c |", board[row][col]);  /* Display counters in row */
		printf("\n");    
	}
	
	printf("  +");                  /* Start the bottom line   */
	for(col = 0 ; col<SIZE ;col++)
		printf("---+");               /* Display the bottom line */
	printf("\n");                   /* End the bottom  line    */
}

/***********************************************
/* Calculates which squares are valid moves    *
* for player. Valid moves are recorded in the *
* moves array - 1 indicates a valid move,     *
* 0 indicates an invalid move.                *
* First parameter is the board array          *
* Second parameter is the moves array         *
* Third parameter identifies the player       *
* to make the move.                           *
* Returns valid move count.                   *
***********************************************/
int valid_moves(char board[][SIZE], int moves[][SIZE], char player)
{
	int rowdelta = 0;     /* Row increment around a square    */
	int coldelta = 0;     /* Column increment around a square */
	int row = 0;          /* Row index                        */
	int col = 0;          /* Column index                     */
	int x = 0;            /* Row index when searching         */
	int y = 0;            /* Column index when searching      */
	int no_of_moves = 0;  /* Number of valid moves            */
	
	/* Set the opponent            */
	char opponent = (player == 'O')? '@' : 'O';    
	
	/* Initialize moves array to zero */
	for(row = 0; row < SIZE; row++)
		for(col = 0; col < SIZE; col++)
			moves[row][col] = 0;
	
	/* Find squares for valid moves.                           */
	/* A valid move must be on a blank square and must enclose */
	/* at least one opponent square between two player squares */
	for(row = 0; row < SIZE; row++)
		for(col = 0; col < SIZE; col++)
		{
			if(board[row][col] != ' ')   /* Is it a blank square?  */
				continue;                  /* No - so on to the next */
			
			/* Check all the squares around the blank square  */ 
			/* for the opponents counter                      */
			for(rowdelta = -1; rowdelta <= 1; rowdelta++)
				for(coldelta = -1; coldelta <= 1; coldelta++)
				{ 
					/* Don't check outside the array, or the current square */
					if(row + rowdelta < 0 || row + rowdelta >= SIZE ||
						col + coldelta < 0 || col + coldelta >= SIZE || 
						(rowdelta==0 && coldelta==0))
						continue;
					
					/* Now check the square */
					if(board[row + rowdelta][col + coldelta] == opponent)
					{
						/* If we find the opponent, move in the delta direction  */
						/* over opponent counters searching for a player counter */
						x = row + rowdelta;                /* Move to          */
						y = col + coldelta;                /* opponent square  */
						
						/* Look for a player square in the delta direction */
						for(;;)
						{
							x += rowdelta;                  /* Go to next square */
							y += coldelta;                  /* in delta direction*/
							
							/* If we move outside the array, give up */
							if(x < 0 || x >= SIZE || y < 0 || y >= SIZE)
								break;
							
							/* If we find a blank square, give up */ 
							if(board[x][y] == ' ')
								break;
							/*  If the square has a player counter */
							/*  then we have a valid move          */
							if(board[x][y] == player)
							{
								moves[row][col] = 1;   /* Mark as valid */
								no_of_moves++;         /* Increase valid moves count */
								break;                 /* Go check another square    */
							}
						} 
					} 
				}  
		}
	return no_of_moves; 
}

/************
* Finds the best move for the computer. This is the move for      *
* which the opponent's best possible move score is a minimum.     *
* First parameter is the board array.                             *
* Second parameter is the moves array containing valid moves.     *
* Third parameter identifies the computer.                        *
************/
void computer_move(char board[][SIZE], int moves[][SIZE], char player)
{
	int row = 0;                          /* Row index               */
	int col = 0;                          /* Column index            */
	int best_row = 0;                     /* Best row index          */
	int best_col = 0;                     /* Best column index       */
	int i = 0;                            /* Loop index              */
	int j = 0;                            /* Loop index              */
	int new_score = 0;                    /* Score for current move  */
	int score = 100;                      /* Minimum opponent score  */
	char temp_board[SIZE][SIZE];          /* Local copy of board     */
	int temp_moves[SIZE][SIZE];           /* Local valid moves array */
	char opponent = (player == 'O')? '@' : 'O'; /* Identify opponent */
	
	//evalua a traves de todos los movimientos posibles
	for(row = 0; row < SIZE; row++)
		for(col = 0; col < SIZE; col++)
		{
			if(moves[row][col] == 0)
				continue;
			
			new_score = min_max(row, col, board, temp_moves, player);
			
			/* Now find the score for the opponents best move */
			//new_score = min_max(temp_board, temp_moves, opponent);
			
			if(new_score<score)    /* Is it worse?           */
			{                      /* Yes, so save this move */
				score = new_score;   /* Record new lowest opponent score */
				best_row = row;  /* Record best move row             */
				best_col = col;  /* and column                       */
			}
		}
	
	/* Make the best move */
	make_move(board, best_row, best_col, player); 
}

/************
* Calculates the score for the current board position for the     *
* player. player counters score +1, opponent counters score -1    *
* First parameter is the board array                              *
* Second parameter identifies the player                          *
* Return value is the score.                                      *
************/
int get_score(char board[][SIZE], char player)
{
	int score = 0;      /* Score for current position */
	int row = 0;        /* Row index                  */    
	int col = 0;        /* Column index               */
	char opponent = player == 'O' ? '@' : 'O';  /* Identify opponent */
	
	/* Check all board squares */
	for(row = 0; row < SIZE; row++)
		for(col = 0; col < SIZE; col++)
		{ 
			score -= board[row][col] == opponent; /* Decrement for opponent */
			score += board[row][col] == player;   /* Increment for player   */
		}
	return score;     
}

int min_max(int row_act, int col_act, char board[][SIZE], int moves[][SIZE], char player){
	
	int row = 0, col = 0, i = 0, j = 0, score=0, new_score=0;//variables
	char opponent = player=='O'?'@':'O';
	char temp_board[SIZE][SIZE], new_board[SIZE][SIZE] = { 0 };
	
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			temp_board[i][j] = board[i][j];
	

	make_move(temp_board, row_act, col_act, player); 
	valid_moves(temp_board, moves, opponent);
		
	for(row = 0 ; row<SIZE ; row++){
		for(col = 0 ; col<SIZE ; col++)
		{
			if(!moves[row][col]) //si no es valido, busca el siguiente
				continue;                   

			for(i = 0 ; i<SIZE ; i++)
				for(j = 0 ; j<SIZE ; j++)
					new_board[i][j] = board[i][j];
			
			make_move(new_board, row, col, player);  
			
			new_score = get_score(new_board, player);  
			
			if(score<new_score) //si es un mejor movimiento, entonces lo guarda
				score = new_score;
		}
	}
	
	return score;    
}

/*************
* Makes a move. This places the counter on a square,and reverses   *
* all the opponent's counters affected by the move.                *
* First parameter is the board array.                              *
* Second and third parameters are the row and column indices.      *
* Fourth parameter identifies the player.                          *
*************/
void make_move(char board[][SIZE], int row, int col, char player)
{
	int rowdelta = 0;                   /* Row increment              */
	int coldelta = 0;                   /* Column increment           */
	int x = 0;                          /* Row index for searching    */
	int y = 0;                          /* Column index for searching */
	char opponent = (player == 'O')? '@' : 'O';  /* Identify opponent */
	
	board[row][col] = player;           /* Place the player counter   */
	
	/* Check all the squares around this square */
	/* for the opponents counter                */
	for(rowdelta = -1; rowdelta <= 1; rowdelta++)
		for(coldelta = -1; coldelta <= 1; coldelta++)
		{ 
			/* Don't check off the board, or the current square */
			if(row + rowdelta < 0 || row + rowdelta >= SIZE ||
				col + coldelta < 0 || col + coldelta >= SIZE || 
				(rowdelta==0 && coldelta== 0))
				continue;
			
			/* Now check the square */
			if(board[row + rowdelta][col + coldelta] == opponent)
			{
				/* If we find the opponent, search in the same direction */
				/* for a player counter                                  */
				x = row + rowdelta;        /* Move to opponent */
				y = col + coldelta;        /* square           */
				
				for(;;)
				{
					x += rowdelta;           /* Move to the      */
					y += coldelta;           /* next square      */ 
					
					/* If we are off the board give up */
					if(x < 0 || x >= SIZE || y < 0 || y >= SIZE)
						break;
					
					/* If the square is blank give up */
					if(board[x][y] == ' ')
						break;
					
					/* If we find the player counter, go backwards from here */
					/* changing all the opponents counters to player         */
					if(board[x][y] == player)
					{
						while(board[x-=rowdelta][y-=coldelta]==opponent) /* Opponent? */
							board[x][y] = player;    /* Yes, change it */
						break;                     /* We are done    */
					} 
				}
			}
		}
}
