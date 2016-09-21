
/*mastermind.c by Josue Martinez 02/21/2016*/
#include <stdio.h>
#include <stdlib.h>


void userIn(int**,int**,int,int);
int** buildDArray(int, int);
int* buildSArray(int );
void getFreq(int*, int*,int, int);
int getTotalPegs(int *,int *,int);
int getBPeg(int*,int*,int);
void getColorPegs(int* combo,int*guess,int*,int*,int,int ,int*,int*);
int** freeDArray(int**,int);
int combos(int**, int*,int** ,int**,int*, int,int,int,int);
int movement(int*,int,int,int);

int main(){
	int cases,colors,cols,moves,match,i,j;
	int **combo,*guess,**pegC, **freqC,*freqG;
	scanf("%d",&cases);
	for(i=0;i<cases;i++){
		int count=0;
		
		scanf("%d",&cols);
		scanf("%d",&colors);
		scanf("%d",&moves);
		
		combo=buildDArray(moves,cols);
		
		pegC=buildDArray(moves,2);
		/*plus one added for the colunm memory allocation because moment function will move out of bounds of the allocated memmory*/
		
		guess=buildSArray(cols+1);
		
		freqC=buildDArray(moves,colors+1);
		
		freqG=buildSArray(colors);
		/*in case of an allocation error*/
		if(combo == NULL || pegC == NULL || guess == NULL || freqC == NULL || freqG == NULL){
		
			printf("memory allocation error");
		
			return 0;
		}
		
		userIn(combo,pegC,cols,moves);
		
		for(j = 0;j < moves; j++)
		/*gets the freqency table for the given combinations*/
			getFreq(freqC[j],combo[j],colors,cols);
		
		printf("\n%d\n",combos(combo ,guess ,pegC ,freqC ,freqG ,cols ,colors ,moves ,count ));
		
		freeDArray(combo, moves);
		freeDArray(pegC, moves);
		freeDArray(freqC, moves);
		free(freqG);
		free(guess);
	}
	
	return 0;
}

void userIn(int**combo,int** peg,int n, int m){
	int i,j;
	
	for(i = 0; i < m; i++){
		/*n+2 for the extra two inputs of the black and white pegs*/
		for(j = 0; j < n+2; j++)
			if(j < n)
				scanf("%d",&combo[i][j]);
			else if(j >= n)
				scanf("%d",&peg[i][j-n]);
	}
}

int** buildDArray(int r,int c){
	int **x,i;
	
	x = (int**)calloc(r,sizeof(int*));
	for(i = 0; i < r; i++)
		x[i]=(int*)calloc(c,sizeof(int));
	
	return x;
}


int* buildSArray(int r){
	
	int *x = (int*)calloc(r,sizeof(int));
	
	return x;
}

void getFreq(int* freqC,int* combo,int colors, int cols){
	int i,j;
	
		for(i = 0; i < colors; i++)
			for(j = 0; j < cols; j++){
				/*will increment the element of the array which corrisponds to the color in combo*/
				if(combo[j] == i)
					freqC[i]++;
			}		
}

int getTotalPegs(int* freqG,int* freqC,int colors){
	int i,j,match=0;
	
	/*compares both freqency tables and gets matches of the two*/
		for(i = 0; i < colors; i++){
			if(freqG[i] >= freqC[i])
				for(j = 0; j < freqC[i]; j++)
						match++;
				else if(freqG[i] <= freqC[i])
					for(j = 0; j < freqG[i]; j++)
						match++;
		}
		
	return match;
}

int getBPeg(int* combo,int* guess,int cols){

	int i,bPeg=0;
	
	for(i = 0; i < cols; i++){	
	/*when an element in combo and guess are equal it is 1 black peg and incements bPeg*/
			if(combo[i] == guess[i]){
				bPeg++;	
			}
		}
		
	return bPeg;
}

int combos(int** combo,int* guess,int** pegC,int**freqC,int* freqG,int cols,int colors,int m,int count){
	int bPeg=0, wPeg=0, match, i, j, move, val=0;
	do{
		bPeg=0,wPeg=0;
		
		for(i = 0; i < colors; i++)
			freqG[i] = 0;
		getFreq(freqG, guess, colors, cols);
		
		move = 0;
		
		for(i=0;i<m;i++){
			/*will send combo and freqC one row at a time to getColorPeg*/
			getColorPegs(combo[i],guess,freqG,freqC[i],colors,cols,&bPeg,&wPeg);
			
			if(bPeg == pegC[i][0] && wPeg == pegC[i][1]){
				val = 1;
				continue;
			}	
			/*if the pegs dont match up with each peg from each row of pegC will break from the loop and check the next combination*/		
			val = 0;
			break;
		}
		move = movement(guess,move,cols,colors);
		/*when all pegs match up with pegs from pegC will break from the loop*/
		if(val)
			break;
	}while(move < cols);
	/*recursion ifs  with 2 different base cases for when val is true*/
	if(move == cols && val){
		return 1 + count;
		}
	else if(move == cols)
		return 0 + count;
	else if(val)
		return combos(combo, guess, pegC, freqC, freqG, cols, colors, m, count+1);
}

void getColorPegs(int* combo,int*guess,int*freqG,int*freqC,int colors,int cols,int*bPeg,int*wPeg){
	int match = 0;
	/*bPeg and wPeg passed mby reference so value changes in the void function are also made out of the void function*/
	match = getTotalPegs(freqG, freqC, colors);
		*bPeg = getBPeg(combo, guess, cols);
		*wPeg = match - *bPeg;
}

int** freeDArray(int**x,int r){
	int i;
	
	for(i = 0; i < r; i++)
		free(x[i]);
	free(x);
	return x;
}

int movement(int*guess,int move,int cols,int colors){
	move = 0;
	
	guess[move]++;
	
	/*movement moves out of bounds of guess and guess was inirialized with one extra element to fix any out of bound errors*/
	while(guess[move] == colors && move < cols){
		if(move >= cols)
			break;
		guess[move] = 0;
		
		move++;
		
		guess[move]++;
	}
	
	return move;
}


