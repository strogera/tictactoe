#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER 1000

void printBoard();
int checkWinCondition();
char board[3][3];
char *openSquares[9];

void getOpenSquares();
void initOpenSquares(){
	//not important delete later
	int i;
	for(i=0;i<9;i++){
		openSquares[i]=NULL;
	}
}
int getCountOfAvailableMoves();
int main(){
	char aisymbol, playersymbol;
	do{
		printf("Choose your prefered symbol(0 for O, 1 for X): ");
		playersymbol=getchar();
	}while(playersymbol!='1'&&playersymbol!='0');
	if(playersymbol=='1'){
		aisymbol='0';
	}else{
		aisymbol='1';
	}
	time_t curTime;
	ctime(&curTime);
	srand(curTime);
	int goesFirst;
	//0 for pc 1 for player
	goesFirst=rand()%2;
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
	do{

		if(goesFirst==0){
			getOpenSquares();
			countAvailableMoves=getCountOfAvailableMoves();
			move=rand()%countAvailableMoves;
			*openSquares[move]=aisymbol;
			printBoard();
			goesFirst=1;
		}else{
			if(printOnceFlag==0){
				printBoard();
				printOnceFlag=1;
			}
		}
		do{
			printf("Your move (x y): ");
			scanf("%*[^\n]%*c\n");
			scanf("%d %d", &x,&y);
			if(x<0||x>2||y<0||y>2){
				printf("Illegal move\n");
			}
		}while(x<0||x>2||y<0||y>2);
		board[x][y]=playersymbol;
		printBoard();
		if(checkWinCondition()){
			printf("Congratz, you won!\n");
			break;
		}
		getOpenSquares();
		countAvailableMoves=getCountOfAvailableMoves();
		move=rand()%countAvailableMoves;
		printf("%d\n", move);
		*openSquares[move]=aisymbol;
		printBoard();
		if(checkWinCondition()){
			printf("You suck, you lost to random moves!\n");
			break;
		}
		getOpenSquares();
	}while(getCountOfAvailableMoves!=0);

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

void getOpenSquares(){
	int i, j, count=0;
	for(i=0; i<3; i++){
		for(j=0; j<3;j++){
			if(board[i][j]==' '){
				openSquares[count]=&board[i][j];
				count++;
			}
		}
	}
	for(;count<9;count++){
		openSquares[count]=NULL;
	}
}

int getCountOfAvailableMoves(){
	int i,count=0;
	for(i=0;i<9;i++){
		if (openSquares[i]!=NULL){
			count++;
		}else{
			break;
		}
	}
	return count;
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
		symbol=board[j][0];
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
