/*  2048 game project for BBG
 *	Started 21/12/2018
 *	17011903 Nihad Guluzade
 *	=========================
 *  Windows version, final
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SIZE 15
#define SPACE 0

typedef enum {
	ERR_INPUT_VAL,
	ERR_GRID,
	ERR_KEY
} error_t;

// Prototypes
void printm(int matrix[][MAX_SIZE], int size);
void printerr(error_t error_code);
int move(int matrix[][MAX_SIZE], int size, char key);
void copy(int m1[][MAX_SIZE], int m2[][MAX_SIZE], int size);
int wander(int matrix[][MAX_SIZE], int size);

int elements[2] = { 2, 4 };

int main(void) {
	system("COLOR");
	system("CHCP 65001"); // Changes cmd format to UTF-16 to correctly display table
	system("cls"); // Hides previous command message
	printf("%*c------\n",32,' ');
	printf("   =========================== | 2048 | ===========================\n");
	printf("%*c------\n",32,' ');
	printf("\n\t     --------- Welcome to the 2048 game ---------\n");
	printf("\t\t ----- \033[1;35mPlease choose grid type\033[0m -----\n\n");
	printf("\t\t    (1) 3x3    (2) 4x4    (3) 6x6\n\n > ");

	int i, j;
	int N, grid, score = 0, game = 0;
	int main[MAX_SIZE][MAX_SIZE];
	error_t error_code;

	srand(time(NULL));

	// User enters game mode
	scanf("%d",&grid);

	// Game grid sizes
	switch (grid) {
		case 1: N = 3; break;
		case 2: N = 4; break;
		case 3: N = 6; break;
		default:
			printerr(ERR_GRID);
			return 0;
	}

	// Clears the matrix
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			main[i][j] = SPACE; // This value is random. Space will be printed
		}
	}

	printf("\n \033[1;35mChosen game mode: \033[0m%dx%d\n", N, N);
	printf(" ---------------------------------------------\n");
	printf("  (W) upward\n  (S) down\n  (A) left\n  (D) right\n  (X) Reset the game\n  (R) Restore the previous state of game\n  (E) End the game\n");
	printf(" ---------------------------------------------\n\n");
	printf(" If you are ready press anything to start the game..");
	getch();
	system("cls");

	// Create first elements of game
	main[rand() % N][rand() % N] = elements[rand() % 2];
	main[rand() % N][rand() % N] = elements[rand() % 2];
	printf("\n\t\033[1;35m Score: \033[0m%d\n", score);
	printm(main, N);

	while (score >= 0 && game == 0)
	{
		printf("\n Please, press the direction key...\n");
		score = move(main, N, getch());
		if (score >= 0)
			printf("\t\033[1;35m Highest Score: \033[0m%d\n", score);
		printm(main, N);
		game = wander(main, N);
		if (game == 2)
			printf("\t%*c\033[1;32mYOU WON!!! \\\\\\CONGRATULATIONS///\033[0m", 9, ' ');
		else if (game == 1)
			printf("\n\t%*cGame Over! You lost. Haha.\n", 12, ' ');
	}

	printf("\n\t     --------- Thank you for playing ---------\n");
	if (game == 0)
		score = score * (-1) - 1;
	printf("\t%*c\033[1;35mHighest score: \033[0m%d\n\n", 17, ' ', score);

	return 0;
}

// Displays the matrix on screen
void printm(int matrix[][MAX_SIZE], int size) {

	/*	As 32 is Space in ASCII Table, program
	 *	must control for other value. If element
	 *	equals to that value then it prints space.
	*/

	// Counts game state
	static int state = 0;
	state++;
	//printf("%d.", state);

	int cnt = 0, ch; // Counts characters in matrix and stores it in ch var
	int count = 0; // Counts number of digits of max number in matrix
	int max = matrix[0][0]; // Used for finding max element in matrix

	// Finds max number in matrix
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (matrix[i][j] != SPACE && matrix[i][j] > max)
				max = matrix[i][j];

	// Counts digit number of max number
	while (max != 0) {
		max = max / 10;
		count++;
	}
	max = count;
	printf("\n\t");

	for (int i = 0; i < size; i++) {
		printf(" |");
		cnt++; // Counts '|' character
		for (int j = 0; j < size; j++) {

			if ( matrix[i][j] != SPACE ) { // If number

				// Draw upper lines till the number (ex: ‾‾2)
				for (int k = 0; k < count; k++) {
					printf("\xE2\x80\xBE"); // "‾" (U+203E)
					cnt++;
				}

				// Controls if number is newly generated
				if (matrix[i][j] < 0) {
					matrix[i][j] *= (-1);
					printf("%d*", matrix[i][j]);
					count--;
					cnt += 2;
				} else {
					printf("%d", matrix[i][j]);
					int tmp, digit = 0;
					tmp = matrix[i][j];
					while (tmp > 0) {
						tmp = tmp / 10;
						digit++;
					}
					// count the number of bottom lines to be written
					for (int i = 0; i < digit; i++) {cnt++;}
				}

				/* Calculates the number of "‾" will be writter
				*  based on the number of digits of number */
				if (matrix[i][j] / 10 < 1)
					count -= 0;
				else if (matrix[i][j] / 100 < 1)
					count -= 1;
				else if (matrix[i][j] / 1000 < 1)
					count -= 2;
				else
					count -= 3;

				// Draw upper lines after number (ex: 2‾‾)
				for (int k = 0; k < count; k++) {
					printf("\xE2\x80\xBE"); // "‾" (U+203E)
					cnt++;
				}
				printf("|"); // right column
				cnt++;
			}
			else { // If space

				for (int k = 0; k < count * 2 + 1; k++) {
					printf("\xE2\x80\xBE"); // "‾" (U+203E)
					cnt++;
				}
				printf("|"); // right column


				cnt++;

			}

			/* Display controls info,
			 * Will be hidden if table is 6x6 and
			 * the digit number is more than 3
			*/

			if (size != 6 && max < 3 && j == size - 1) {
				switch (i) {
					case 0:
						printf("  | (w) up  (d) right  (e) end |"); // length = 30
						break;
					case 1:
						printf("  | (s) down  (x) reset%*c", 9, '|');
						break;
					case 2:
						printf("  | (d) left  (r) restore%*c", 7, '|');
						break;
					default:
						break;
				}
			}

			// Resotres old value of count
			count = max;
		}
		ch = cnt;
		cnt = 0;
		printf("\n\t");
	}

	printf("  "); // Puts space to display last row correctly

	// Bottom line of table
	for (int i = 0; i < ch - 2; i++) {
		printf("\xE2\x80\xBE"); // Unicode Character "‾" (U+203E)
	}
	printf("\n");
}

// Prints error messages based on codes
void printerr(error_t error_code) {
	printf("\033[1;31m error: \033[1;0m");
	switch (error_code) {
		case ERR_INPUT_VAL:
			printf("illegal input value. \n");
			break;
		case ERR_GRID:
			printf("illegal grid type. \n");
			break;
		case ERR_KEY:
			printf("invalid key. Please, try again. \n");
			break;
		default:
			printf("unknown error code. \n");
			break;
	}
}

// Moves elements of game
int move(int matrix[][MAX_SIZE], int size, char key) {

	static int points = 0; // Counts game score, so no additional function parameter is needed
	static int prev_point = 0; // Stores previous state of points
	static int highest_p = 0; // Keeps the highest score
	int changes = 0; // Display messages based on values of this variable
	int done = 0; // Prevents from doing 2 steps
	int max = 0; // Defines max size of controlling array
	int index[MAX_SIZE], value[MAX_SIZE], previous[MAX_SIZE][MAX_SIZE];
	static int reserve[MAX_SIZE][MAX_SIZE]; // Keeps record of previous[][]
	int rI, rJ; // Stores random numbers
	int tmp; // To make sure assigned element doesn't lose its value

	// Counts game state
	static int state = 0;
	state++;

	// If user clicks R on the first step of game
	if (state == 1) {
		copy(previous, matrix, size);
		copy(reserve, previous, size);
	}

	if (key == 'r' || key == 'R') {
		// Displays previous state of board
		copy(matrix, previous, size);
		// Keeps highest score collected from all game
		if (points >= highest_p) {
			highest_p = points;
		}
		points = prev_point;
		changes = 2;
	}
	else if (key == 'e' || key == 'E') {
		changes = 3;
	}
	else if (key == 'x' || key == 'X') {
		// Resets the game
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				matrix[i][j] = SPACE;
			}
		}
		// Creates first elements
		matrix[rand() % size][rand() % size] = elements[rand() % 2];
		matrix[rand() % size][rand() % size] = elements[rand() % 2];
		// Resets previous matrixes
		copy(reserve, previous, size);
		copy(previous, matrix, size);
		// Resets all variables
		points = 0;
		prev_point = 0;
		highest_p = 0;
		changes = 4;
	}
	else {
		// Keeps records of previous[][] matrix
		copy(reserve, previous, size);
		copy(previous, matrix, size);
		prev_point = points;
		// Performs moving operations
   		if (key == 'w' || key == 'W') {
			for (int i = 1; i < size; i++) {
				for (int j = 0; j < size; j++) {
					if (matrix[i][j] != SPACE) {
						tmp = i; // We don't want to manipulate i in any case
						// Puts elements up
						while (( tmp > 0 ) && ( matrix[tmp - 1][j] == SPACE )) {
							changes = 1;
							matrix[tmp - 1][j] = matrix[tmp][j];
							matrix[tmp][j] = SPACE;
							tmp--;
						}
						// Prevents summing of 3 same consecutive elements
						for (int k = 0; k < max; k++) {
							if (j == index[k] && matrix[tmp][j] == value[k])
								done = 1;
						}
						// If value with top element is same, sum them
						if (( done == 0 ) && (( tmp > 0 ) && ( matrix[tmp - 1][j] == matrix[tmp][j] ))) {
							changes = 1;
							matrix[tmp - 1][j] += matrix[tmp][j];
							points += matrix[tmp - 1][j];
							index[max] = j;
							value[max] = matrix[tmp - 1][j];
							matrix[tmp][j] = SPACE;
							max++;
						}
						done = 0;
					}
				}
			}
		}
		else if (key == 's' || key == 'S') {
			for (int i = size - 2; i >= 0; i--) {
				for (int j = size - 1; j >= 0; j--) {
					if (matrix[i][j] != SPACE) {
						tmp = i; // We don't want to manipulate i in any case
						// Puts element down
						while (( tmp < size - 1 ) && ( matrix[tmp + 1][j] == SPACE )) {
							changes = 1;
							matrix[tmp + 1][j] = matrix[tmp][j];
							matrix[tmp][j] = SPACE;
							tmp++;
						}
						// Prevents summing of 3 same consecutive elements
						for (int k = 0; k < max; k++) {
							if (j == index[k] && matrix[tmp][j] == value[k])
								done = 1;
						}
						// If value with bottom element is same, sum them
						if (( done == 0 ) && (( tmp < size - 1 ) && ( matrix[tmp][j] == matrix[tmp + 1][j] ))) {
							changes = 1;
							matrix[tmp + 1][j] += matrix[tmp][j];
							points += matrix[tmp + 1][j];
							index[max] = j;
							value[max] = matrix[tmp + 1][j];
							matrix[tmp][j] = SPACE;
							max++;
						}
						done = 0;
					}
				}
			}
		}
		else if (key == 'a' || key == 'A') {
			for (int i = 0; i < size; i++) {
				for (int j = 1; j < size; j++) {
					if (matrix[i][j] != SPACE) {
						tmp = j; // We don't want to manipulate i in any case
						// Puts element to the left
						while (( tmp > 0 ) && ( matrix[i][tmp - 1] == SPACE )) {
							changes = 1;
							matrix[i][tmp - 1] = matrix[i][tmp];
							matrix[i][tmp] = SPACE;
							tmp--;
						}
						// Prevents summing of 3 same consecutive elements
						for (int k = 0; k < max; k++) {
							if (i == index[k] && matrix[i][tmp] == value[k])
								done = 1;
						}
						// If value with left element is same, sum them
						if (( done == 0 ) && (( tmp > 0 ) && ( matrix[i][tmp] == matrix[i][tmp - 1] ))) {
							changes = 1;
							matrix[i][tmp - 1] += matrix[i][tmp];
							points += matrix[i][tmp - 1];
							index[max] = i;
							value[max] = matrix[i][tmp - 1];
							matrix[i][tmp] = SPACE;
							max++;
						}
						done = 0;
					}
				}
			}
		}
		else if (key == 'd' || key == 'D') {
			for (int i = 0; i < size; i++) {
				for (int j = size - 2; j >= 0; j--) {
					if (matrix[i][j] != SPACE) {
						tmp = j; // We don't want to manipulate i in any case
						// Puts element to the right
						while ( ( tmp < size - 1 ) && ( matrix[i][tmp + 1] == SPACE )) {
							changes = 1;
							matrix[i][tmp + 1] = matrix[i][tmp];
							matrix[i][tmp] = SPACE;
							tmp++;
						}
						// Prevents summing of 3 same consecutive elements
						for (int k = 0; k < max; k++) {
							if (i == index[k] && matrix[i][tmp] == value[k])
								done = 1;
						}
						// If value with right element is same, sum them
						if (( done == 0 ) && (( tmp < size - 1 ) && ( matrix[i][tmp] == matrix[i][tmp + 1] ))) {
							changes = 1;
							matrix[i][tmp + 1] += matrix[i][tmp];
							points += matrix[i][tmp + 1];
							index[max] = i;
							value[max] = matrix[i][tmp + 1];
							matrix[i][tmp] = SPACE;
							max++;
						}
						done = 0;
					}
				}
			}
		}
		else {
			// If unknown key is pressed
			printerr(ERR_KEY);
		}
	}

	// Controls highest score
	if (points > highest_p) {
		highest_p = points;
	}


	switch (changes)
	{
		case (1):
			// Generate two random numbers
			rI = rand() % size;
			rJ = rand() % size;
			// If spot is full, generates new values
			while ( matrix[rI][rJ] != SPACE ) {
				rI = rand() % size;
				rJ = rand() % size;
			}
			// Inserts random number into random available spot
			matrix[rI][rJ] = elements[rand() % 2];
			matrix[rI][rJ] *= (-1);
			break;
		case (2):
			printf("\n\t\033[1;33m(^) Previous State\033[0m\n");
			break;
		case (3):
			printf("\n\t\033[1;33m(!) Game stopped\033[0m\n");
			// Forces to exit from loop in main(); last (-1) in equation is for 0 case
			highest_p = highest_p * (-1) - 1;
			break;
		case (4):
			printf("\n\t\033[1;33m(0) Game has been reset\033[0m\n");
			break;
		default:
			printf("\n\t\033[1;33m(!) Nothing changed\033[0m\n");
			copy(previous, reserve, size);
	}

	printf("\n\t\033[1;35m Score: \033[0m%d\n", points);
	return highest_p;
}

// Copies one matrix into another
void copy(int m1[][MAX_SIZE], int m2[][MAX_SIZE], int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			m1[i][j] = m2[i][j];
		}
	}
}

// Looks if matrix whether is full, or 2048 is present
int wander(int matrix[][MAX_SIZE], int size) {
	int g = 0, count = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (matrix[i][j] == 2048)
				g = 2;

			if (matrix[i][j] != SPACE) {
				// Doesn't control last element of matrix, so control will be till N*N-1
				if ((i < size - 1 && j < size - 1) && ((matrix[i][j] != matrix[i][j+1]) && (matrix[i][j] != matrix[i+1][j])))
					count++;
				else if ((i == size - 1 && j < size - 1) && (matrix[i][j] != matrix[i][j+1]))
					count++;
				else if ((i < size - 1 && j == size - 1) && (matrix[i][j] != matrix[i+1][j]))
					count++;
			}
		}
	}
	// If there are no same elements next to each other
	if ((count == size*size-1))
		g = 1;
	return g;
}
