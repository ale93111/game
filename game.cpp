#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream> 

#include <ncursesw/ncurses.h>
#include <locale.h>

enum Direction {NONE, UP, DOWN, LEFT, RIGHT };

struct Guy
{
	std::vector<int> y, x;
	//int y, x; 
	int ymax, xmax;
	int size;
	int score;
	std::vector<Direction> direction, olddirection;
	
	void display()
	{
		mvprintw(y,x,"@");
	}
	
	void check_coord(int & y, int & x, int ymax, int xmax)
	{
		if(x < 0) x = 0;
		else if(x >= xmax) x = xmax - 1;

		if(y < 0) y = 0;
		else if(y >= ymax) y = ymax - 1;
	}
	
	void update (int c)
	{
		olddirection = direction;
			
		switch(c)
		{
			case KEY_UP:    direction = UP; 
				break;
			case KEY_DOWN:  direction = DOWN;
				break;
			case KEY_RIGHT: direction = RIGHT;
				break;
			case KEY_LEFT:  direction = LEFT;
				break;
			//default:		direction = NONE;
			//	break;				
		}

		if( (olddirection == UP    && direction == DOWN) || (olddirection == DOWN && direction == UP) ||
		    (olddirection == RIGHT && direction == LEFT) || (olddirection == LEFT && direction == RIGHT)) direction = olddirection;
			
		if(direction == UP)    y--;
		else if(direction == DOWN)  y++;
		else if(direction == LEFT)  x--;
		//else if(direction == RIGHT) x++;
		else x++;
		
		check_coord(y,x,ymax,xmax);
	}
	
	
	Guy(){}
	Guy(int yi, int xi, int ymaxi, int xmaxi, Direction directioni) : 
	y(yi), x(xi), ymax(ymaxi), xmax(xmaxi), /*direction(directioni), olddirection(directioni),*/ score(0) 
	{
		y.pushback(yi);
		x.pushback(xi);
		direction.pushback(directioni);
		olddirection.pushback(directioni);
	}
	
	~Guy()
	{
		std::cout << "Score: " << score << std::endl;
	}
};


bool food(bool istherefood, int & yfood, int & xfood, Guy & littleguy)
{
	if(istherefood)
	{
		if( littleguy.y == yfood && littleguy.x == xfood) 
		{
			littleguy.score++;
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
		yfood = rand() % littleguy.ymax;
		xfood = rand() % littleguy.xmax;
		
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
	timeout(50);
	
	Direction direction;
	int xmax, ymax;
	getmaxyx(stdscr, ymax, xmax);
	
	bool istherefood = false;
	int c = 0;
	int xfood, yfood;
	
	int x, y;
	x = xmax/2;
	y = ymax/2;
	
	Guy littleguy = Guy(y,x,ymax,xmax,RIGHT);
	
	while((c = getch()) != 'q')
	{
		clear();
		
		//mvprintw(y,x,"☺");
		littleguy.display();
		
		istherefood = food(istherefood, yfood, xfood, littleguy);		
		
		refresh();
		
		littleguy.update(c);
	}
	
	endwin();
	
	return 0;

}