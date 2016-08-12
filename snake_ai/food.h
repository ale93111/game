#ifndef FOOD_H
#define FOOD_H

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "board.h"
#include "snake.h"

struct Food
{
	int y, x;
	bool istherefood;
	
	void display()
	{
		if(istherefood) mvprintw(y,x,"•");
	}
	
	void generateon(Board & terminal)
	{
		y = rand() % terminal.ymax;
		x = rand() % terminal.xmax;
	}
	
	bool eatenby(Snake & Ekans)
	{
		if( Ekans.y.front() == y && Ekans.x.front() == x) 
		{
			Ekans.lunch();
			return true;
		}
		else return false;
	}

	void check(Snake & Ekans, Board & terminal)
	{
		if(istherefood)
		{
			//istherefood = eatenby(Ekans)? false : true;
			if(eatenby(Ekans)) istherefood = false;
			else istherefood = true;
		}
		else
		{
			generateon(terminal);
			istherefood = true;
		}

		//display();
		
	}
		
	Food(){}
	Food(Board & terminal) : istherefood(true) 
	{
		generateon(terminal);
	}
	~Food(){}
};

#endif