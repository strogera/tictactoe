#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER 1000

void printBoard();
int checkWinCondition();
char board[3][3];

struct move{
	int x;
	int y;
	int value;
};

struct move*  minimaxDecision();
struct move* maxValue();
struct move* minValue();
int checkDraw(){
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

char aisymbol, playersymbol;
int main(){
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
	time_t curTime;
	ctime(&curTime);
	srand(curTime);
	int playerGoesFirst;
	//0 for ai 1 for player
	playerGoesFirst=rand()%2;
	int i,j;
	for(i=0; i<3; i++){
		for(j=0; j<3;j++){
			board[i][j]=' ';
		}
	}
	char * ptr;
	int x,y,countAvailableMoves, move;
	char readBuffer[BUFFER];
	int printOnceFlag=0;
	struct move * currMove=malloc(sizeof(struct move));
	do{

		if(playerGoesFirst==0){
			currMove=minimaxDecision();	
			board[currMove->x][currMove->y]=aisymbol;
			printBoard();
			playerGoesFirst=2;
		}else if(playerGoesFirst==1){
				printBoard();
				playerGoesFirst=2;
		}
		do{
			printf("Your move (x y): ");
			scanf("%*[^\n]%*c\n");
			scanf("%d %d", &x,&y);
			if(x<0||x>2||y<0||y>2){
				printf("Illegal move, out of bounds\n");
			}else if(board[x][y]!=' '){
				printf("Illegal move, already exists\n");
			}
		}while(x<0||x>2||y<0||y>2||board[x][y]!=' ');
		board[x][y]=playersymbol;
		printBoard();
		if(checkWinCondition()){
			printf("Congratz, you won!\n");
			return 0;
		}
		if(checkDraw()){
			printf("Congratz, you managed to draw! That's the best you can get.\n");
			return 0;
		}
			currMove=minimaxDecision();	
			if(currMove==NULL){
				printf("draw?\n");
				return 0;//TODO
			}
			board[currMove->x][currMove->y]=aisymbol;
			printBoard();
			if(checkWinCondition()){
				printf("You lost but dw you can't win the ai!\n");
				return 0;
			}
			if(checkDraw()){
				printf("Congratz, you managed to draw! That's the best you can get.\n");
				return 0;
			}
	}while(1);

}

void printBoard(){
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
	int i,j,flag=1;
	char symbol;
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
	symbol=board[1][1];
	if(symbol!=' '){
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

struct move * minimaxDecision(){
	return maxValue();
}

struct move * maxValue(){
	struct move *currMove, *bestMove=malloc(sizeof(struct move));;
	if(checkWinCondition()){
		bestMove->value=-1;	
		return bestMove;
	}
	int i, j, availableMoves=0;
	if(checkDraw()){
		bestMove->value=0;
		return bestMove;
	}
	int u=-2, tempU;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			if(board[i][j]==' '){
				board[i][j]=aisymbol;
				currMove=minValue();
				if(currMove->value>u){
					u=currMove->value;
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
struct move * minValue(){
	struct move *currMove, *bestMove=malloc(sizeof(struct move));
	if(checkWinCondition()){
		bestMove->value=1;	
		return bestMove;
	}
	int i, j, availableMoves=0;
	if(checkDraw()){
		bestMove->value=0;
		return bestMove;
	}
	int u=2, tempU;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			if(board[i][j]==' '){
				board[i][j]=playersymbol;
				currMove=maxValue();
				if(currMove->value<u){
					u=currMove->value;
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
