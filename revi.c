#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>
#include "queueADT.h"

long int ROWS;
long int COLS;

void printHelpMessage() {
    printf("USAGE:\n");
    printf("mopsolver [-hdsp] [-i INFILE] [-o OUTFILE]");
    printf("Options:\n");
    printf("        -h      Print this helpful message to stdout and exit.\n");
    printf("        -d      Pretty print (display) the maze after reading.   (Default: off)\n");
    printf("        -s      Print shortest solution steps.                   (Default: off)\n");
    printf("        -p      Print an optimal path.                           (Default: off)\n");
    printf("        -i INFILE       Read maze from INFILE.                   (Default: stdin)\n");
    printf("        -o OUTFILE      Write all output to OUTFILE.             (Default: stdout)\n"); 
}

void displayMaze(FILE *fptr, char maze[ROWS][COLS]) {
    for( int i = 0; i < ROWS; i++ ) {
        for( int j = 0; j < COLS; j++ ) {
            if( j == COLS-1 ) {
                fprintf(fptr, "%c", maze[i][j]);
	    }
	    else {
	        fprintf(fptr, "%c ", maze[i][j]);	    
            }
        }
	fprintf(fptr, "\n");
    }
}

struct neighbor {
    int x;
    int y;
};

int isSolvable(char maze[ROWS][COLS], int isVisited[ROWS][COLS], struct neighbor N) {
	if( maze[0][0] == '1' ) {
	    return 0;
	}

        isVisited[N.x][N.y] = 1;
       
        struct neighbor Nb[4];
	int flag = 0, count = 0;
         
	int leftX = N.x;
	int leftY = N.y - 1;

	int rightX = N.x;
	int rightY = N.y + 1;

	int upX = N.x - 1;
	int upY = N.y;

	int downX = N.x + 1;
	int downY = N.y;
        
        if( leftY >= 0 ) {
	    if(maze[leftX][leftY] != '1' && isVisited[leftX][leftY] != 1 ) {
                Nb[count].x = leftX;
		Nb[count].y = leftY;
		flag = 1;  
		count += 1;
	    }	   
        } 
	
	if( rightY <= COLS-1 ) {
	     if(maze[rightX][rightY] != '1' && isVisited[rightX][rightY] != 1 ) {
                 Nb[count].x = rightX;
		 Nb[count].y = rightY;
		 flag = 1;
		 count += 1;
	     } 
	}
	
	if( upX >= 0 ) {
	    if(maze[upX][upY] != '1' && isVisited[upX][upY] != 1 ) {
                Nb[count].x = upX;
	        Nb[count].y = upY;
	        flag = 1;
	        count += 1;	
            }
	}

	if( downX <= ROWS-1 ) {
            if(maze[downX][downY] != '1' && isVisited[downX][downY] != 1 ){
                Nb[count].x = downX;
		Nb[count].y = downY;
		flag = 1;
		count += 1;
	    }
	}

	if( flag == 0 ) {
		return 99;
	}
	
       for( int i = 0; i < count; i++ ) {
	        if( !isVisited[Nb[i].x][Nb[i].y] ) {	
	            int isValid = isSolvable(maze, isVisited, Nb[i]);
		    if( isValid == 1 ) {
		        return 1;
	            }  
		    else if( isValid == 99 ) {
		        continue;
		    }
		    if( isVisited[ROWS-1][COLS-1] == 1 ) {
			return 1;
		    }
	        }
       }

       return 0;
	
}

void displayShortestSolution();
void displayOptimalPath();

void getRowCol(FILE *fptr) {
    int line_length = 0;
    long int size;
    char c;

    while( ((c = getc(fptr)) != '\n') && c != EOF ) {
        line_length++;    
    }
    COLS = ( (int) line_length/2 ) + 1;
    fseek( fptr, 0L, SEEK_END );
    size = ftell(fptr);
    ROWS = size/line_length;
    fseek( fptr, 0L, SEEK_SET );
}   

void readMaze(FILE *fptr, char maze[ROWS][COLS]) {
    int i = 0, j = 0;
    char c;

    while( ((c = getc(fptr)) != EOF) ) {
	    if( c == '\n' ) {
                i++;
	        j = 0;
	    }
            if( isdigit(c) ) {		    
	        maze[i][j] = c;
		j++;
	    }
    }
}

void writeMaze();
   
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
    getRowCol(input_mode);
    char maze[ROWS][COLS];
    int isVisited[ROWS][COLS];

    for( int i = 0; i < ROWS; i++ ) {
        for( int j = 0; j < COLS; j++ ) {
	   isVisited[i][j] = 0;
        }
    }
        
    //int distances[ROWS][COLS];
    readMaze(input_mode, maze);
    struct neighbor N; 
    N.x = 0;
    N.y = 0;
    int check = isSolvable(maze, isVisited, N);
    printf("%d\n", check);
    /**for( int i = 0; i < ROWS; i++ ) {
        for( int j = 0; j < COLS; j++ ) {
	    printf("%d ", isVisited[i][j]);
        }
        printf("\n");
    }**/	
    if( isD == 1 ) {
        displayMaze(output_mode, maze);
    }	
    
   /** if( isS == 1 ) {
        displayShortestSolution();
    }
    
    if( isP == 1 ) {
	displayOptimalPath();
    }   
    
    writeMaze(output_mode, maze);**/

    fclose( input_mode );
    fclose( output_mode );

    return 0;
}
