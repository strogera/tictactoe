#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char board[3][3];
char aisymbol, playersymbol;

struct move{
	int x;
	int y;
	int value;
};

void printBoard();
int checkWinCondition();
int checkDraw();
struct move *minimaxDecision();
struct move *maxValue();
struct move *minValue();


int main(){
	//get symbol preference from the player
	do{
		printf("Choose your prefered symbol(0 for O, 1 for X): ");
		playersymbol=getchar();
	}while(playersymbol!='1'&&playersymbol!='0');
	if(playersymbol=='1'){
		aisymbol='O';
		playersymbol='X';
	}else{
		aisymbol='X';
		playersymbol='O';
	}

	//pick randomly who goes first
	time_t curTime; //pseudorandom seed
	ctime(&curTime);
	srand(curTime);

	int playerGoesFirst;
	playerGoesFirst=rand()%2; //0 for ai 1 for player
	
	//initialize the board
	int i,j;
	for(i=0; i<3; i++){
		for(j=0; j<3;j++){
			board[i][j]=' ';
		}
	}

	char c;
	int x,y, printOnceFlag=0;
	struct move * currMove=malloc(sizeof(struct move));

	//actual game
	do{
		if(playerGoesFirst==0){
			//ai moves
			currMove=minimaxDecision();	
			board[currMove->x][currMove->y]=aisymbol;
			printBoard();
			playerGoesFirst=2;
		}else if(playerGoesFirst==1){
				//print the empty board and then continue
				printBoard();
				playerGoesFirst=2; //set to 2 so it gets called once only when player goes first
		}
		do{
			//get move until legal move
			printf("Your move (x y): ");
			while((c=getchar())!='\n' && c!=EOF); //clear the input buffer
			scanf("%d %d", &x,&y);
			if(x<0||x>2||y<0||y>2){
				printf("Illegal move, out of bounds\n");
			}else if(board[x][y]!=' '){
				printf("Illegal move, already exists\n");
			}
		}while(x<0||x>2||y<0||y>2||board[x][y]!=' ');

		board[x][y]=playersymbol; //make the move and then print the board
		printBoard();

		if(checkWinCondition()){
			printf("Congratz, you won!\n");
			break;
		}
		if(checkDraw()){
			printf("Congratz, you managed to draw! That's the best you can get.\n");
			break;
		}

		//ai's turn
		currMove=minimaxDecision(); //get move
		board[currMove->x][currMove->y]=aisymbol; //make the move and print the board
		printBoard();

		if(checkWinCondition()){
			printf("You lost but dw you can't win the ai!\n");
			break;
		}
		if(checkDraw()){
			printf("Congratz, you managed to draw! That's the best you can get.\n");
			break;
		}
	}while(1);
	free(currMove);

}

void printBoard(){
	//prints the board and the coordinates required to make moves
	int i, j;
	printf("     y  \n");
	printf("   0 1 2\n\n");
	for(i=0; i<3; i++){
		if(i==1){
			printf("x%d ", i);
		}else{
			printf(" %d ", i);
		}
		for(j=0; j<3;j++){
			if(j!=0){
				printf("|");
			}
			printf("%c", board[i][j]);
		}
		if(i!=2){
			printf("\n   - - -\n");
		}else{
			printf("\n");
		}
	}
	printf("\n");

}


int checkWinCondition(){
	//checks if there the current board contains a winning condition for either player
	//returns 1 someone wins, 0 if not
	int i,j,flag=1;
	char symbol;

	//checks rows
	for(i=0; i<3; i++){
		flag=1;
		symbol=board[i][0];
		if(symbol==' '){
			continue;
		}
		for(j=0; j<3;j++){
			if(board[i][j]!=symbol){
				flag=0;
				break;
			}
		}
		if(flag==1){
			return 1;
		}
	}

	//checks columns
	for(j=0; j<3; j++){
		flag=1;
		symbol=board[0][j];
		if(symbol==' '){
			continue;
		}
		for(i=0; i<3;i++){
			if(board[i][j]!=symbol){
				flag=0;
				break;
			}
		}
		if(flag==1){
			return 1;
		}
	}

	//checks diagonals
	symbol=board[1][1]; //if a winning condition exist in the diagonal, the 1,1 cell needs to be occupied with the winning symbol
	if(symbol!=' '){
		//check the four corners antidametric
		if(board[0][0]==symbol){
			if(board[2][2]==symbol){
				return 1;
			}
		}
		if(board[0][2]==symbol){
			if(board[2][0]==symbol){
				return 1;
			}
		}

	}
	return 0;

}

int checkDraw(){
	//checks if the current board is a draw
	//in order to draw all cells needs to be occupied
	//returns 1 if draw, 0 if not
	int i,j;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			if(board[i][j]==' '){
				return 0;
			}
		}
	}
	return 1;
}

struct move * minimaxDecision(){
	//recursive minimax algorithm
	//minimax has two problems
	//a)it assumes that the player plays optimaly
	//b)is quite time consuminig
	//in this case, a) it will not lose either way and b) its not so bad for this one
	/////
	//values are 1 for ai wins, 0 for draw, -1 for player wins
	//max=ai to move, min=player to move
	return maxValue();
}

struct move * maxValue(){
	struct move *currMove, *bestMove=malloc(sizeof(struct move));;
	//check if we are in a terminal state
	if(checkWinCondition()){
		bestMove->value=-1; //player wins because the current board is after player's move so -1	
		return bestMove;
	}
	if(checkDraw()){
		bestMove->value=0;
		return bestMove;
	}

	int i, j, u=-2;
	//get max value of min values of the succesors
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			if(board[i][j]==' '){ //get all moves for the current state of board
				board[i][j]=aisymbol; //make the move
				currMove=minValue();
				if(currMove->value>u){ //get max value of the minValues of the succesors
					u=currMove->value; //u holds the current maximum value
					bestMove->value=u;
					bestMove->x=i;
					bestMove->y=j;
				}
				free(currMove); //each time min/maxValue() is called a malloc of size struct move happens
						//so after each call after we update our current ones we can free it
				board[i][j]=' '; //undo move in order to check the rest of available moves
			}
		}

	}
	return bestMove;
}
struct move * minValue(){
	struct move *currMove, *bestMove=malloc(sizeof(struct move));
	//check if we are in a terminal state
	if(checkWinCondition()){
		bestMove->value=1; //ai wins so 1	
		return bestMove;
	}
	if(checkDraw()){
		bestMove->value=0;
		return bestMove;
	}

	//get max value of min values of the succesors
	int i, j, u=2;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			if(board[i][j]==' '){ //see maxValue() for explanation
				board[i][j]=playersymbol;
				currMove=maxValue(); //now we need the maxValue of the succesors
				if(currMove->value<u){ //and the move with the minimum value 
					u=currMove->value; //u holds the current minimum value
					bestMove->value=u;
					bestMove->x=i;
					bestMove->y=j;
				}
				free(currMove);
				board[i][j]=' ';
			}
		}

	}
	return bestMove;

}
