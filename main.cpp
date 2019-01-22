#include <iostream>
#include <unistd.h>
#include <curses.h>
#include <string.h>
#define H 10
#define W 40
#define SIDE_CHAR '#'
#define TITLE " SNAKE "

//g++ main.cpp -lncurses

typedef struct _Snake
{
	int pX;
	int pY;
	struct _Snake *tail;
} Snake;

typedef struct _Eat
{
	int pX;
	int pY;
} Eat;

typedef struct _Map
{
	int h;
	int w;
	char **array;
	char *title;
	char *bottom;
	Snake *snake;
	int dir;
	bool eaten;
	Eat eat;
} Map;

void drawMap(Map *map);
void initMap(Map *map, int h, int w); 	
void clearMap(Map *map);
bool updateMap(Map *map);

int main()
{
	Map map;
	initMap(&map,H,W);
	while(updateMap(&map))
	{
		drawMap(&map);
	}
	std::cout << "loose" << std::endl;
}

bool updateMap(Map *map)
{
	initscr();
	cbreak();
	noecho();
	timeout(300);
	int c = getch();
	//std::cout << c<< std::endl << std::endl;
	endwin();
	switch(c)
		{
		case 'z':
		case  KEY_UP:
			if(map->dir!=2)
				map->dir = -2;
			break;
		case 's':
		case  KEY_DOWN:
			if(map->dir!=-2)
				map->dir = 2;
			break;
		case 'q':
		case  KEY_LEFT:
			if(map->dir!=1)
				map->dir = -1;
			break;
		case 'd':
		case  KEY_RIGHT:
			if(map->dir!=-1)
				map->dir = 1;
			break;
		}
	
	Snake *snk = map->snake;
	int adir = abs(map->dir);

	int pX = snk->pX, nPx;
	int pY = snk->pY, nPy;

	nPx = snk->pX + ((adir==1)?map->dir/adir:0);
	nPy = snk->pY + ((adir==2)?map->dir/adir:0);

	if(nPy < 0 || nPy > map->w-1 || map->array[nPy][nPx] == '#' || map->array[nPy][nPx] == 'o')
		return false;
	else if(map->array[nPy][nPx] == '+')
		{
			map->eaten=true;
			map->eat.pX = rand()%map->w;
			map->eat.pY = rand()%map->h;
			printf( "%c\n",'\a'); 
		}
	clearMap(map);
	snk->pX = nPx;
	snk->pY = nPy;

	
	
	while(snk!=NULL)
		{
			map->array[snk->pY][snk->pX]='o';
			if(snk->tail!=NULL)
				{
					snk = snk->tail;	
					nPx = snk->pX;
					nPy = snk->pY;
					snk->pX = pX;
					snk->pY = pY;
					pX = nPx;
					pY = nPy;
				}
			else if (map->eaten)
				{
					snk->tail = new Snake;
					snk = snk->tail;
					snk->pX = pX;
					snk->pY = pY;
					snk->tail = NULL;
					map->eaten = false;
					snk=NULL;
				}
			else
				snk=NULL;
			
			
		}
	return true;
}

void clearMap(Map *map)
{
	for(int i=0;i<map->h;i++)
	{			
		map->array[i][0] = SIDE_CHAR;
		for(int j=1;j<map->w-1;j++)
		{
			map->array[i][j] = ' ';
		}
		map->array[i][W-1] = SIDE_CHAR;
	}
	map->array[map->eat.pY][map->eat.pX] = '+';
	
}

void initMap(Map *map, int h, int w)
{
	map->h = h;
	map->w = w;

	map->eaten = false;
	map->dir = 1;
	
	map->snake = new Snake;
	map->snake->pX = 5;
	map->snake->pY = 5;
	map->snake->tail=NULL;

	map->eat.pX = 10;
	map->eat.pY = 5;
	

	std::cout << map->snake  << std::endl << std::endl;
	std::cout << map->snake->tail << std::endl << std::endl;
	
	map->title = new char[(w+1)*3];
	for(int i =0;i<(w+1)*3;i++)
		map->title[i]=SIDE_CHAR;
	map->title[w]='\n';
	map->title[2*w+1]='\n';
	map->title[3*w+2]='\0';
	strncpy(map->title+int(1.5*(w+1)-strlen(TITLE)/2)," SNAKE ",strlen(TITLE));
       
	
	
	map->bottom = new char[w+1];
	for(int i =0;i<w;i++)
		map->bottom[i]=SIDE_CHAR;
	map->bottom[w]='\0';
	
	//sprintf(map->title ,"%-*d\0",20,9);
	
	map->array = new char*[h];
	for(int i=0;i<map->h;i++)
	{	
		map->array[i] = new char[w];		
		map->array[i][0] = SIDE_CHAR;
		for(int j=1;j<map->w-1;j++)
		{
			map->array[i][j] = ' ';
		}
		map->array[i][W-1] = SIDE_CHAR;
	}
	drawMap(map);
}

void drawMap(Map *map)
{
	//clear screen
	for(int i = 0 ;i<30;i++)
		std::cout << std::endl;

	std::cout << map->title << std::endl;
	for(int i=0;i<map->h;i++)
		{
			for(int j=0;j<map->w;j++)
				std::cout << map->array[i][j];
			std::cout << std::endl;
		}
	std::cout << map->bottom << std::endl;

	
}

  
