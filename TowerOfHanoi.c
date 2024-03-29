#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define DELAY		300
#define POSX		10
#define POSY		15
#define DISK_CHAR	'*'
#define TOWR_CHAR	'|'

typedef struct {
	int discs;
	int bottomX, bottomY;
	int* diskSize;
}TOWER;

WINDOW* initWin();

void printMessWel(int y, int x, char* s);
void printMessQit(int y, int x, char* s);
void userInput(TOWER* k);

void solveHan(WINDOW* myWin, TOWER* k, int discs, int src, int aux, int dest);
void initTOWR(TOWER* k, int discs);
void printTOWR(WINDOW* myWin, TOWER* k, int discs);
void moveTOWR(TOWER* k, int discs, int src, int dest);
void freeTOWR(TOWER* k);

char* welcomeMess = "Visual solver";
char* quitMess = "Pres q to exit";
char* enterMess = "Enter the number of discs: ";

int discsG;
int X, Y;
int wX, wY;

int main()
{
	TOWER tower[3]; /* Potrebna su nam 3 stuba */
	int y, x;
	int discs;

	WINDOW* myWin;

	initscr();
	cbreak();
	start_color();

	getmaxyx(stdscr, y, x);		/* broj kolona i vrsta */
	Y = y; X = x; 			

	printMessWel(Y, X, welcomeMess);
	mvprintw(5, 5 , enterMess);
	scanw("%d",&discs);
	noecho();
	refresh();
	
	curs_set(FALSE);
	timeout(DELAY);

	myWin = initWin();
	wrefresh(myWin);
	getmaxyx(myWin, wX, wY);
	/*printw("Velicina prozora: %d %d\n",wX, wY);*/


	discsG = discs;
	initTOWR(tower, discs);
	printTOWR(myWin, tower, discs);
	printMessQit(Y, X, quitMess);
	solveHan(myWin, tower, discsG, 0, 1, 2);

	freeTOWR(tower);
	/*getch();*/
	endwin();

	return 0;
}
void printMessQit(int y, int x, char* s)
{
	int lenght; 
	
	lenght = x - strlen(s) - 5;

	attron(A_REVERSE);
	mvprintw(5,lenght, s);
	attroff(A_REVERSE);

	/* refresh(); */
}
void printMessWel(int y, int x, char* s)
{
	int lenght;

	init_pair(1, COLOR_GREEN, COLOR_BLACK);

	lenght = x - strlen(s);

	attron(COLOR_PAIR(1));
	attron(A_BOLD);
	mvprintw(2, lenght/2,s);
	attroff(A_BOLD);
	attroff(COLOR_PAIR(1));

	/* refresh(); */
}
void moveTOWR(TOWER* k, int discs, int src, int dest)
{
	int temp, index;

	--k[src].discs;
	index = 0;
	while(k[src].diskSize[index] == 0 && index != discs)
		index++;

	temp = k[src].diskSize[index];
	k[src].diskSize[index] = 0;

	index = 0;
	while(k[dest].diskSize[index] == 0 && index != discs)
		index++;
	index--;
	k[dest].diskSize[index] = temp;
	k[dest].discs++;
}
void printTOWR(WINDOW* myWin, TOWER* k, int discs)
{
	int i, j, x, y, size, p;

	wclear(myWin); /* Mora svakim pozivom novu sliku da nacrta */

	for(i = 0;i < 3;i++){
		mvwprintw(myWin,k[i].bottomY - discs - 1,
				k[i].bottomX,"%c", TOWR_CHAR);
	} 
	y = k[0].bottomY - discs;	/* krecemo od vrha prvog stuba */
	for(i = 0;i < 3;i++){
		for(j = 0; j < discs; j++){
			if(k[i].diskSize[j] != 0) {	/* ako postoji disk */
				size = k[i].diskSize[j];
				x = k[i].bottomX - (size / 2); 		/* pomera se pocetak diska ulevo */
				for(p = 0;p < size;p++){
					mvwprintw(myWin,y, x+p,"%c",DISK_CHAR);	/* disk se ispisuje sleva na desno */
				} 
			}
			else
				mvwprintw(myWin,y, k[i].bottomX,"%c",TOWR_CHAR);
			y++; 
		} 
		y = k[0].bottomY - discs;
	} 

	box(myWin, '!','~');
	wrefresh(myWin);
}
void initTOWR(TOWER* k, int discs)
{
	int size, half, i;

	k[0].discs = discs; /* Na pocetku su svi diskovi na prvom stubu */

	for(i = 0; i < 3;i++){
		k[i].diskSize = (int*)calloc( discs , sizeof(int));
		/* TODO ako nece izadji iz programa */
	} 

	size = 3;
	for(i = 0;i < discs; i++, size += 2){
		k[0].diskSize[i] = size;	/* Niz koji ce predstavljati diskove na prvom stubu,
					 *  prvi disk je uvek 3 ostali za 2 veci */
	} 

	half = (k[0].diskSize[discs - 1] / 2); /* prepolovljena velicina najveceg diska */

	/* Pozicije stubova u odnoso na velicinu diskova */

	k[0].bottomX = POSX + 1 + half;
	k[0].bottomY = POSY + 2 + discs;

	k[1].bottomX = k[0].bottomX + 2 + 3*half;
	k[1].bottomY = POSY + 2 + discs;

	k[2].bottomX = k[1].bottomX + 2 + 3*half;
	k[2].bottomY = POSY + 2 + discs;
	
}
void userInput(TOWER* k)
{
	int c;

	c = getch();
	/*if(c == ERR)
		return;*/
	if(c == 'q') {
		freeTOWR(k);
		endwin();
		exit(EXIT_SUCCESS);
	}
}
void  solveHan(WINDOW* myWin, TOWER* k, int discs, int src, int aux, int dest)
{
	if(discs > 0) {
		solveHan(myWin, k, discs - 1, src, dest, aux);
		moveTOWR(k, discsG, src, dest);
		printTOWR(myWin,k, discsG);
		userInput(k);
		solveHan(myWin, k, discs - 1, aux, src, dest);
	}
}
void freeTOWR(TOWER* k)
{
	int i;
	for(i = 0;i < 3;i++){
		free(k[i].diskSize);
	} 	
}
WINDOW* initWin()
{
	int height, width;
	int x, y; /* pozicije u terminalu */
	
	x = 5;
	y = x + 2;

	width = X - 10;
	height = Y - 8;

	WINDOW* temp = newwin(height, width, y, x);
	box(temp, '!','~');
	//if(!temp) {
		//exit(EXIT_FAILURE); /* TODO 
	//{
		

	return temp;
}
