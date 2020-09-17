/**
 * \mopsolver.c
 * \Himani Munshi (hxm3443@rit.edu)
 * \CSCI - 243 - 05
 * \Date: 11/3/2018
**/

#define MAZE_ALLOC_UNIT 10

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>
#include "queueADT.h"

int ROWS;                        //Number of rows in the maze 
int COLS;                       //Number of columns in the maze 
int maxRow;                    //For reallocation and freeing of maze
int maxCol;                   //For reallocation and freeing of maze 
static char PASS = '.';      //Represents an open path
static char BLOCKED = '#';  //Represents a closed path
static char PIPE = '|';    //Represents boundary at the end of each row 
static char DASH = '-';   //Represents boundary at the top and bottom of the maze

//A structure which represents a neighbor with its x- and y- coordinates 
struct neighbor {
    int x;
    int y;
};

/** 
 * A structure which contains for every cell, its x- and y- coordinates,
 * a list of all the neighbors of the cell, 'prev' which keeps track of where it 
 * came from, tells if the cell has been visited or not, and also the number of
 * neigbors of the given cell.
**/
struct adjacencyList {
    struct neighbor N;
    struct neighbor prev;
    struct neighbor list_N[4];
    int isVisited;
    int numNeighbor;
};

/**
 * This function prints out a help message if the user provides '-h' as a 
 * commandline argument. 
**/
void printHelpMessage() {
    printf("USAGE:\n");
    printf("mopsolver [-hdsp] [-i INFILE] [-o OUTFILE]");
    printf("Options:\n");
    printf("\t-h\tPrint this helpful message to stdout and exit.\n");
    printf("\t-d\tPretty print (display) the maze after reading.\n");
    printf("\t-s\tPrint shortest solution steps.\n");
    printf("\t-p\tPrint an optimal path.\n");
    printf("\t-i INFILE\tRead maze from INFILE.  (Default: stdin)\n");
    printf("\t-o OUTFILE\tWrite all output to OUTFILE. (Default: stdout)\n"); 
}

/**
 * This is a helper function which prints out the top and bottom boundaries
 * of the maze.
 *
 * \param *fptr - Represents a file pointer  
**/
void prettyPrintBoundaries( FILE *fptr ) {
    for( int i = 0; i < (COLS*2) + 2; i++ ) {
        if( i == 0 ) {
            fprintf(fptr, "%c", PIPE);
        }
        if( i == (COLS*2) + 1 ) {
            fprintf(fptr, "%c\n", PIPE);
        }
        else {
            fprintf(fptr, "%c", DASH);
        } 
    }
}

/**
 * This function is responsible for pretty-printing the entire maze.
 *
 * \param *fptr - Represents a file pointer 
 * \param **maze - 2-D array representing the maze containing '#', '.', or '+'  
**/
void displayMaze( FILE *fptr, char **maze ) {
    prettyPrintBoundaries( fptr );
    for( int i = 0; i < ROWS; i++ ) {
        for( int j = 0; j < COLS; j++ ) {
            if( j == 0 && i != 0) 
                fprintf(fptr, "%c", PIPE);
	    else if( j == 0 && i == 0 ) 
		fprintf(fptr, " ");	
            if( maze[i][j] == '0' ) {
	        fprintf(fptr, " %c", PASS);
            }
	    else if( maze[i][j] == 'o' ) {
	        fprintf(fptr, " +");
	    }
            else {
		fprintf(fptr, " %c", BLOCKED);
	    }		
        }
	if( i != ROWS-1 ) {
	    fprintf(fptr, " %c", PIPE);
	}
	if( i == ROWS-1 ) 
	    fprintf(fptr, "  ");
	fprintf(fptr, "\n");
    }
    prettyPrintBoundaries( fptr );
}

/**
 * This function prints out the number of steps it took to reach to the last 
 * cell at (ROWS-1, COLS-1)  from the first cell at (0,0).
 *
 * \param *fptr - Represents a file pointer 
 * \param steps - number of steps taken from (0,0) to (ROWS-1, COLS-1)
**/
void displayShortestSolution( FILE *fptr, int steps ) {
    if( steps > 0 ) 
        fprintf(fptr, "Solution in %d steps.\n", steps);
    else
	fprintf(fptr, "No solution.\n");
}

/**
 * This function uses backtracking algorithm and  marks all the cells through 
 * which the last cell was reached in order to keep track of the path. 
 * Marks all those cells as 'o'.
 *
 * \param c - number of elements that exist in the array of structures
 * \param **maze - 2-D array representing the maze containing '#', '.', or '+'
 * \param lst - array of structures containing those structures' information
 *              which contain a '0' in the maze  
**/
void modifyMaze( int c, char **maze, struct adjacencyList lst[c] ) {
    struct adjacencyList currentV = lst[c-1];
    while( currentV.prev.x != -1 && currentV.prev.y != -1 ) {
        maze[currentV.N.x][currentV.N.y] = 'o';
	int prevX = currentV.prev.x;
	int prevY = currentV.prev.y;
	if(currentV.N.x == 0 && currentV.N.y == 0) {
	    if(prevX == 0 && prevY == 0) {
	        return;
	    }
	}
	for(int i = c-2; i >= 0; i--) {
	    if( lst[i].N.x == prevX && lst[i].N.y == prevY ) {
	        currentV = lst[i];
		break;
	    }
	}
    }	
}

/**
 * This function prints the shortest (aka optimal) path taken to get to
 * the last cell from the first cell. If no path exists, then the original
 * maze gets printed.
 *
 * \param *output_mode - Represents a file pointer which writes to a file 
 *                       or to the standard output 
 * \param steps - number of steps taken from (0,0) to (ROWS-1, COLS-1)
 * \param **maze - 2-D array representing the maze containing '#', '.', or '+'
 * \param c - number of elements that exist in the array of structures 
 * \param lst - array of structures containing those structures' information 
 *              which contain a '0' in the maze
**/
void displayOptimalPath( FILE *output_mode, int steps, char **maze, int c, 
		struct adjacencyList lst[c] ) {
    if( steps <= 0 ) {
        displayMaze( output_mode, maze );
    } 
    else {
    	modifyMaze( c, maze, lst );
	displayMaze( output_mode, maze );
    }
}

/**
 * This function is responsible for calculating the neghbors of a given 
 * cell with coordinates x and y. 
 *
 * \param **maze - 2-D array representing the maze containing '#', '.', or '+'
 * \param Nb - list containing all neighbors 
 * \param N - the cell whose neighbors are being calculated
 *
 * \return the number of neighbors 
**/
int calcNeighbors( char **maze, struct neighbor Nb[4], struct neighbor N ) {
     int count = 0;

     int leftX = N.x;
     int leftY = N.y - 1;

     int rightX = N.x;
     int rightY = N.y + 1;

     int upX = N.x - 1;
     int upY = N.y;

     int downX = N.x + 1;
     int downY = N.y;
   
     if( upX >= 0 ) {
         if( maze[upX][upY] != '1' ) {
             Nb[count].x = upX;
             Nb[count].y = upY;
             count += 1;
         }
     }

     if( leftY >= 0 ) {
         if( maze[leftX][leftY] != '1' ) {
             Nb[count].x = leftX;
             Nb[count].y = leftY;

             count += 1;
         }
     }

     if( downX <= ROWS-1 ) {
         if( maze[downX][downY] != '1' ) {
             Nb[count].x = downX;
             Nb[count].y = downY;

             count += 1;
         }
     }

     if( rightY <= COLS-1 ) {
         if(maze[rightX][rightY] != '1' ) {
             Nb[count].x = rightX;
             Nb[count].y = rightY;
             
	     count += 1;
         }
     }

     return count;
}

/**
 * This function uses the backtracking algorithm and counts the number of steps 
 * it took to reach to the last cell as well as keeps track of the path by means
 * of which last cell was reached. 
 *
 * \param c - number of elements that exist in the array of structures 
 * \param lst - array of structures containing those structures' information
 *              which contain a '0' in the maze
 * \return number of steps it took to reach to the last cell of the maze
 *         from the first cell
**/
int backTracking( int c, struct adjacencyList lst[c] ) {
    int steps = 0;
    struct adjacencyList currentV = lst[c-1];
    while( currentV.prev.x != -1 && currentV.prev.y != -1 ) {
        steps++;
	int prevX = currentV.prev.x;
	int prevY = currentV.prev.y;

	if( currentV.N.x == 0 && currentV.N.y == 0 ) {
	    if( prevX == 0 && prevY == 0 ) {
	        //has solution
		return steps;
	    }
	    else {
		//no solution
		return -1;
	    }
	}
	for( int i = c-2; i >= 0; i-- ) {
	    if( lst[i].N.x == prevX && lst[i].N.y == prevY ){
	        currentV = lst[i];
		break;
	    }
	}
    }
    
    return -1;
}

/**
 * This function uses the algorithm of breadth-first search to find if there
 * exists a shortest path. If a solution exits, then it makes a call to the 
 * backTracking function to get the number of steps it took to reach the last cell. 
 * It then accordingly makes a call to the rest of the functions to display 
 * shortest solution and optimal path.
 *
 * \param maze - 2-D array representing the maze containing '#', '.', or '+'
 * \param isS - a boolean which checks if displayShortestSolution() needs to 
 *              be called
 * \param isP - a boolean which checks if displayOptimalPath() needs to be 
 *              called 
 * \param output_mode - Represents a file pointer which writes to a file
 *                      or to the standard output 
**/
void BFS( char **maze, int isS, int isP, FILE *output_mode ) {
    int c = 0;
    for( int i = 0; i < ROWS; i++ ) {
        for( int j = 0; j < COLS; j++ ) {
	    if( maze[i][j] == '0' )
	        c++;
	}
    }

    struct adjacencyList lst[c];
    int count = 0, numNeighbors = 0;

    for( int i = 0; i < ROWS; i++ ) {
        for( int j = 0; j < COLS; j++ ) {
	    if( maze[i][j] == '0' ) {
                lst[count].N.x = i;
		lst[count].N.y = j;
		numNeighbors = calcNeighbors( maze, lst[count].list_N, lst[count].N );
		lst[count].numNeighbor = numNeighbors;
		lst[count].prev.x = -1;
		lst[count].prev.y = -1;
		lst[count].isVisited = 0;
		count++;
	    }
	}
    }

    if( maze[0][0] != '1' ) {
       lst[0].prev.x = 0;
       lst[0].prev.y = 0;
    }

    Queue queue = NULL;
    queue = createQueue(100);
    
    lst[0].isVisited = 1; 

    enqueue(queue, (void *)&lst[0]);

    while( !isEmpty(queue) ) {
        struct adjacencyList val = *(struct adjacencyList *) dequeue(queue);
        for( int i = 0; i < val.numNeighbor; i++ ) {
	    int x_Neigh = val.list_N[i].x;
	    int y_Neigh = val.list_N[i].y;

	    for( int j = 0; j < c; j++ ) {
	        if( lst[j].N.x == x_Neigh && lst[j].N.y == y_Neigh ) {
		    if( lst[j].isVisited == 0 ) {
			lst[j].isVisited = 1;
			lst[j].prev.x = val.N.x;
			lst[j].prev.y = val.N.y; 
			enqueue( queue, (void *) &lst[j] );
			break;
		    }
		}
	    } 
	}
    }
     
    int steps, flag = 0;
    if ( lst[count-1].isVisited == 1 ) {
        steps = backTracking( c, lst );
	flag = 1;
    }
    if( isS == 1 ) {
	if( flag == 1 ) {
            displayShortestSolution( output_mode, steps );
	}
	else {
	    displayShortestSolution( output_mode, 0 );
	}
    }

    if( isP == 1 ) {
	if( flag == 1 ) {
            displayOptimalPath( output_mode,  steps, maze, c, lst );
	}
	else {
	    displayOptimalPath( output_mode,  0, maze, c, lst );
	}
    }
    destroyQueue( queue );
}

/**
 * This function reads an input file (or standard input) and builds the entire maze
 * and stores in a dynamically allocated 2-D array. 
 *
 * \return maze  
**/
char **readMaze( FILE *fptr ) {
    int row = 0, col = 0;
    maxRow = MAZE_ALLOC_UNIT;
    maxCol = MAZE_ALLOC_UNIT;
    char c;
    char **maze;

    maze = calloc( maxRow , sizeof(char*) );
    for( int i = 0; i < MAZE_ALLOC_UNIT; i++ ) {
        maze[i] = calloc( maxCol, sizeof(char));
    }
 
    while( (c = fgetc(fptr)) != EOF ) {
        if( c == '\n' ) {
	    row++;
            ROWS = row;
	    COLS = col;
	    col = 0;

	    if(row == maxRow) {
                maxRow *= 2;
                maze = realloc( maze, sizeof(char *)*maxRow);
                for(int i = row; i < maxRow; i++) {
                    maze[i] = calloc( maxCol, sizeof(char));
	        }
            }
	}
        if( isdigit(c) ) {
            maze[row][col] = c;
            col++;

            if(col == maxCol) {
                maxCol *= 2;
                for( int i = 0; i < maxRow; i++ ) {
                    maze[i] = realloc(maze[i], sizeof(char)*maxCol);
                }
            }
	}
    }
   
    return maze;
}

/**
 * This function is responsible for deallocating (or freeing) the entire maze.
 *
 * \param maze - 2-D array representing the maze containing '#', '.', or '+'
**/
void freeMaze( char **maze ) {
    for(int i = 0; i < maxRow; i++) {
        free( maze[i] );
    }
    free(maze);
}

/**
 * The main() function reads all the commandline arguments. Makes use of a 
 * switch case to keep track of which functions need to be called. Reads the entire 
 * maze and then accordingly makes calls to functions. Later, frees the maze created 
 * and also closes all file pointers opened, if any.  
**/
int main( int argc, char *argv[] ) {
    FILE *input_mode = stdin;
    FILE *output_mode = stdout;
    
    int opt;
    int isH = 0, isD = 0, isS = 0, isP = 0; 
    while( (opt = getopt( argc, argv, "hdspi:o:" )) != -1 ) {
        switch( opt ) {
	    case 'h':
	        isH = 1;
		break;
	    case 'd': 
		isD = 1;
                break;
            case 's':
		isS = 1;
                break;
	    case 'p':
		isP = 1;
 	        break;
	    case 'i':
                input_mode = fopen( optarg, "r");
		if( input_mode == NULL ) {
                    perror( optarg );
                    exit(EXIT_FAILURE);
                }
                break;
            case 'o':
		output_mode = fopen( optarg, "w" );
		break;
            default:
                printf("Unorganized option '%c'.\n", opt);
        }
    }
    
    if( isH == 1 ) {
        printHelpMessage();
        exit(EXIT_FAILURE);
    }
   
    char **maze;
    maze = readMaze(input_mode);
       
    if( isD == 1 ) {
        displayMaze(output_mode, maze);
    }

    BFS( maze, isS, isP, output_mode );   
    
    freeMaze( maze );

    fclose( input_mode );
    fclose( output_mode );
    
    return 0;
}
