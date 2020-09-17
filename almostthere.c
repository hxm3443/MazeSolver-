#define MAZE_ALLOC_UNIT 2

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>
#include "queueADT.h"

long int ROWS;
long int COLS;
static char PASS = '.';
static char BLOCKED = '#';

struct neighbor {
    int x;
    int y;
};

struct adjacencyList {
    struct neighbor N;
    struct neighbor prev;
    struct neighbor list_N[4];
    int isVisited;
    int numNeighbor;
};

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

//pretty print
void displayMaze(FILE *fptr, char **maze) {
    for( int i = 0; i < ROWS; i++ ) {
        for( int j = 0; j < COLS; j++ ) {
          
		if( maze[i][j] == '0' ) {
	            fprintf(fptr, " %c", PASS);
                }
		else if( maze[i][j] == 'o' ){
			fprintf(fptr, " +");
		}
                else {
		    fprintf(fptr, " %c", BLOCKED);
	        }		
            
        }
	fprintf(fptr, "\n");
    }
}


void displayShortestSolution( FILE * fptr, int steps ) {
    if( steps > 0 ) 
        fprintf(fptr, "Solution in %d steps.\n", steps);
    else
	fprintf(fptr, "No solution.\n");
}


void modifyMap(int c, char **maze, struct adjacencyList lst[c]){
	struct adjacencyList currentV = lst[c-1];
    while( currentV.prev.x != -1 && currentV.prev.y != -1 ) {
	    maze[currentV.N.x][currentV.N.y] = 'o';
	int prevX = currentV.prev.x;
	int prevY = currentV.prev.y;
	if(currentV.N.x == 0 && currentV.N.y == 0){
		if(prevX == 0 && prevY == 0){
			return;
		}
	}
	for(int i=c-2; i>=0; i--){
		if(lst[i].N.x == prevX && lst[i].N.y == prevY){
			currentV = lst[i];
			break;
		}
	}
		
       // currentV.N.x = currentV.prev.x;
       // currentV.N.y = currentV.prev.y;
    }
    printf("(%d, %d)\n", lst[0].N.x, lst[0].N.y);
	
}


void displayOptimalPath( FILE *output_mode, int steps, char **maze, int c, struct adjacencyList lst[c] ) {
    if( steps <= 0 ) {
        displayMaze( output_mode, maze );
    } 
    else {
    	modifyMap( c, maze, lst);
	displayMaze( output_mode, maze );
    }

}

/**int isSolvable(char maze[ROWS][COLS], int isVisited[ROWS][COLS], struct neighbor N) {
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
f	    if(maze[upX][upY] != '1' && isVisited[upX][upY] != 1 ) {
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
	
}**/

int calcNeighbors( char **maze, struct neighbor Nb[4] , struct neighbor N ) {
     int count = 0;

     int leftX = N.x;
     int leftY = N.y - 1;

     int rightX = N.x;
     int rightY = N.y + 1;

     int upX = N.x - 1;
     int upY = N.y;

     int downX = N.x + 1;
     int downY = N.y;

     if( leftY >= 0 ) {
            if(maze[leftX][leftY] != '1' ) {
                Nb[count].x = leftX;
                Nb[count].y = leftY;
            
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

        if( upX >= 0 ) {
            if(maze[upX][upY] != '1' ) {
                Nb[count].x = upX;
                Nb[count].y = upY;
                count += 1;
            }
        }

        if( downX <= ROWS-1 ) {
            if(maze[downX][downY] != '1' ){
                Nb[count].x = downX;
                Nb[count].y = downY;
           
                count += 1;
            }
        }

	return count;
}



int backTracking( int c, struct adjacencyList lst[c] ) {
    int steps = 0;
	struct adjacencyList currentV = lst[c-1];
    while( currentV.prev.x != -1 && currentV.prev.y != -1 ) {
        printf("(%d, %d)\n", currentV.N.x, currentV.N.y);
	steps++;
	int prevX = currentV.prev.x;
	int prevY = currentV.prev.y;
	if(currentV.N.x == 0 && currentV.N.y == 0){
		if(prevX == 0 && prevY == 0){
			//has solution
			return steps;
		}
		else{
			//no solution
			return -1;
		}
	}
	for(int i=c-2; i>=0; i--){
		if(lst[i].N.x == prevX && lst[i].N.y == prevY){
			currentV = lst[i];
			break;
		}
	}
		
       // currentV.N.x = currentV.prev.x;
       // currentV.N.y = currentV.prev.y;
    }
    //printf("(%d, %d)\n", lst[0].N.x, lst[0].N.y);

    return -1;
}

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
		//printf("numNeighbor: %d\n", numNeighbors);
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

    //printf("c = %d\n", c); 
    /**for( int k = 0; k < c; k++ ) {
	    printf("(%d, %d)\n", lst[k].prev.x, lst[k].prev.y);
    } **/

    Queue queue = NULL;
    queue = createQueue(100);
    
    lst[0].isVisited = 1; 

    enqueue(queue, (void *)&lst[0]);

    while( !isEmpty(queue) ) {
        struct adjacencyList val = *(struct adjacencyList *) dequeue(queue);
	//printf("\ncurrent: (%d, %d)\n", val.N.x, val.N.y);
	//printf("val numNeighbors: %d\n", val.numNeighbor);
        for( int i = 0; i < val.numNeighbor; i++ ) {
	    int x_Neigh = val.list_N[i].x;
	    int y_Neigh = val.list_N[i].y;
		//printf("checking neighbor: (%d, %d)\n", x_Neigh, y_Neigh);

	    for( int j = 0; j < c; j++ ) {
	        if( lst[j].N.x == x_Neigh && lst[j].N.y == y_Neigh ) {
		    if( lst[j].isVisited == 0 ) {
	                //printf("visiting: (%d, %d)\n", lst[j].N.x, lst[j].N.y);
			lst[j].isVisited = 1;
			lst[j].prev.x = val.N.x;
			lst[j].prev.y = val.N.y;
			//printf("prev is: (%d, %d)\n\n", lst[j].prev.x, lst[j].prev.y); 
			enqueue( queue, (void *) &lst[j] );
			break;
		    }
		}
	    } 
	}
    }
  
   destroyQueue( queue );   
   int steps = backTracking( c, lst );
   if( isS == 1 ) {
        displayShortestSolution( output_mode, steps );
    }

    if( isP == 1 ) {
        displayOptimalPath( output_mode,  steps, maze, c, lst );
    }
}
   

char **readMaze(FILE *fptr) {
    int row = 0, col = 0;
    int maxRow = MAZE_ALLOC_UNIT;
    int maxCol = MAZE_ALLOC_UNIT;
    char c;
    char **maze = calloc( maxRow , sizeof(char*));
    for( int i = 0; i < MAZE_ALLOC_UNIT; i++ ) {
	    maze[i] = calloc( maxCol, sizeof(char));
    }

    while( ((c = fgetc(fptr)) != EOF) ) {
	    if( c == '\n' ) {
                row++;
	        col = 0;
		if(row == maxRow){
		    maxRow *= 2;
	            maze = realloc( maze, sizeof(char *)*maxRow);
                    for(int i = row; i < maxRow; i++) {
 			maze[i] = calloc( maxCol, sizeof(char));
		    }
		}
	    }
            if( isdigit(c) ) {
		printf("max col = %d, col =%d\n", maxCol, col);		    
	        maze[row][col] = c;
		col++;
		if(col == maxCol) {
		    maxCol *= 2;
		    for( int i = 0; i < maxRow; i++ ) { 
		        maze[i] = realloc(maze[i], sizeof(char*)*maxCol);
		    }
		}
	    }
    }

    ROWS = row;
    COLS = col;
    return maze;
}

void freeMaze( char **maze ) {
    for(int i = 0; i < ROWS; i++) {
        free( maze[i] );
    }
    free(maze);
}

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
   
        
    char **maze =  readMaze(input_mode);
       
    if( isD == 1 ) {
        displayMaze(output_mode, maze);
    }	
    BFS( maze, isS, isP, output_mode );   
    
    freeMaze( maze );

    fclose( input_mode );
    fclose( output_mode );

    return 0;
}
