#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream> 
#include <vector>

#include <ncursesw/ncurses.h>
#include <locale.h>
#include <unistd.h> //sleep

enum Direction {NONE, UP, DOWN, LEFT, RIGHT };

struct Snake
{
	std::vector<int> y, x;
	//int y, x; 
	int ymax, xmax;
	int size;
	int score;
	std::vector<Direction> direction, olddirection;
	
	void display()
	{
		for(int i=0; i<size; i++) mvprintw(y[i],x[i],"@");
	}
	
	bool check_coord()
	{
		//check boundaries
		if(x.front() < 0) {x.front() = 0; return true;}
		else if(x.front() >= xmax) {x.front() = xmax - 1; return true;}

		if(y.front() < 0) {y.front() = 0; return true;}
		else if(y.front() >= ymax) {y.front() = ymax - 1; return true;}
		
		//check body
		for(int i=1; i<size; i++)
		{
			if( x.front() == x.at(i) &&  y.front() == y.at(i) ) return true;
		}
		
		return false;
	}
	
	void move(int c)
	{
		olddirection.front() = direction.front();
		
		for(int i=(size-1); i>0; i--)  direction.at(i) = direction.at(i-1);
		//for(int i=1; i<size; i++) direction.at(i) = direction.at(i-1);
		
		//get new direction
		switch(c)
		{
			case KEY_UP:    direction.front() = UP; 
				break;
			case KEY_DOWN:  direction.front() = DOWN;
				break;
			case KEY_RIGHT: direction.front() = RIGHT;
				break;
			case KEY_LEFT:  direction.front() = LEFT;
				break;
			//default:		direction = NONE;
			//	break;				
		}
		
		//can't move backwards
		if( (olddirection.front() == UP    && direction.front() == DOWN) || (olddirection.front() == DOWN && direction.front() == UP) ||
		    (olddirection.front() == RIGHT && direction.front() == LEFT) || (olddirection.front() == LEFT && direction.front() == RIGHT)) direction.front() = olddirection.front();
		
		//move
		if(direction.front() == UP)    y.front()--;
		else if(direction.front() == DOWN)  y.front()++;
		else if(direction.front() == LEFT)  x.front()--;
		//else if(direction == RIGHT) x.front()++;
		else x.front()++;
			
		//update body directions
		for(int i=1; i<size; i++)
		{
			if(direction.at(i-1) == UP)    
			{
				y.at(i) = y.at(i-1) + 1;
				x.at(i) = x.at(i-1);
			}
			else if(direction.at(i-1) == DOWN)  
			{
				y.at(i) = y.at(i-1) - 1;
				x.at(i) = x.at(i-1);
			}
			else if(direction.at(i-1) == LEFT)  
			{
				y.at(i) = y.at(i-1);
				x.at(i) = x.at(i-1) + 1;
			}
			else 
			{
				y.at(i) = y.at(i-1);
				x.at(i) = x.at(i-1) - 1;
			}
		}
		
	}
	
	void lunch()
	{
		score++;
		size++;
		
		y.push_back(y.back());
		x.push_back(x.back());
		direction.push_back(direction.back());
	}
	
	
	Snake(){}
	Snake(int yi, int xi, int ymaxi, int xmaxi, Direction directioni) : 
	ymax(ymaxi), xmax(xmaxi), score(0), size(1) 
	{
		y.push_back(yi);
		x.push_back(xi);
		direction.push_back(directioni);
		olddirection.push_back(directioni);
	}
	
	~Snake()
	{
		std::cout << "Score: " << score << std::endl;
	}
};


bool food(bool istherefood, int & yfood, int & xfood, Snake & Ekans)
{
	if(istherefood)
	{
		if( Ekans.y[0] == yfood && Ekans.x[0] == xfood) 
		{
			Ekans.lunch();
			return false;
		}
		else 
		{
			mvprintw(yfood,xfood,"•");
			return true;
		}
	}
	else
	{
		//generate new food
		yfood = rand() % Ekans.ymax;
		xfood = rand() % Ekans.xmax;
		
		mvprintw(yfood,xfood,"•");
		
		return true;
	}
}

int main()
{
	srand (time(NULL));
	
	setlocale(LC_ALL,"");
	initscr();
	
	
	keypad(stdscr, TRUE);
    noecho();
	curs_set(FALSE);
	
	timeout(200); //adjust speed of the game
	
	Direction direction;
	int xmax, ymax;
	getmaxyx(stdscr, ymax, xmax);
	
	bool istherefood = false;
	bool gameover = false;
	
	int c = 0;
	int xfood, yfood;
	
	int x, y;
	x = xmax/2;
	y = ymax/2;
	
	Snake Ekans = Snake(y,x,ymax,xmax,RIGHT);
	
	while((c = getch()) != 'q' && !gameover)
	{
		clear();
		
		//mvprintw(y,x,"☺");
		Ekans.move(c);
		
		Ekans.display();
		
		gameover = Ekans.check_coord();
		
		istherefood = food(istherefood, yfood, xfood, Ekans);		
		
		refresh();
	}
	
	if(gameover) 
	{
		clear();
		mvprintw(y-2,x-6, "GAMEOVER");
		mvprintw(y  ,x-6, "Score: %d ", Ekans.score);
		refresh();
		sleep(2);
	}
	
	endwin();
	
	return 0;

}