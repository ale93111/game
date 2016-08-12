#pragma once
#include <iostream>     // std::cout
#include <algorithm>    // std::max
#include <stdlib.h> 

#include "brain/neural_network.h"
#include "brain/body.h"
#include "brain/training_parameters.h"
#include "brain/genome.h"
#include "brain/organism.h"
#include "brain/species.h"
#include "brain/trained_neural_network.h"
#include "brain/logger.h"

#include "board.h"
#include "snake.h"
#include "food.h"

class Gamesession : public JNF_NEAT::IBody {
	public:
		Board terminal;
		Snake Ekans;
		Food pidgey; //or rattata

		//snake starting point
		int xstart, ystart;
		int gamespeed; // bigger = slower (gamespeed->delay in millisecond)

		bool gameover;
		int command;

		virtual void Reset() override;
		virtual void Update(const std::vector<float>& networkOutputs) override;
		virtual double GetFitness() const override;

		virtual std::vector<float> ProvideNetworkWithInputs() const override;

		void play();
		void AI_play(TrainedNeuralNetwork & champ);

		Gamesession();
		Gamesession(int gamespeedin);
};

Gamesession::Gamesession() : gamespeed(0), gameover(false), command(0) 
{
	timeout(gamespeed);

	int xmax, ymax;
	getmaxyx(stdscr, ymax, xmax);

	terminal = Board(ymax, xmax);

	xstart = terminal.xmax/2;
	ystart = terminal.ymax/2;

	Ekans  = Snake(ystart,xstart,RIGHT);
	pidgey = Food(terminal);
}

Gamesession::Gamesession(int gamespeedin) : gamespeed(gamespeedin), gameover(false), command(0) 
{
	timeout(gamespeed);

	int xmax, ymax;
	getmaxyx(stdscr, ymax, xmax);

	terminal = Board(ymax, xmax);

	xstart = terminal.xmax/2;
	ystart = terminal.ymax/2;

	Ekans  = Snake(ystart,xstart,RIGHT);
	pidgey = Food(terminal);
}

void Gamesession::Reset()
{
	gameover = false;
	command = 0;
	Ekans.reset(ystart, xstart, RIGHT);
	pidgey = Food(terminal);
}

void Gamesession::Update(const std::vector<float>& networkOutputs)
{    
	command = 0;

	//direction is selected by choosing the outputneuron who fired the most
	auto maxelement = std::max_element(std::begin(networkOutputs), std::end(networkOutputs));
	command = std::distance(std::begin(networkOutputs), maxelement);

	if      (command == 3) command = KEY_UP;
	else if (command == 2) command = KEY_DOWN;
	else if (command == 1) command = KEY_LEFT;
	else if (command == 0) command = KEY_RIGHT;
	//clear();
	Ekans.move(command);
	//Ekans.display();
	gameover = Ekans.check_coord(terminal);	
	pidgey.check(Ekans, terminal);
	//pidgey.display();
	//refresh();
		
	if(gameover) Reset();
}

double Gamesession::GetFitness() const
{
	return Ekans.score;
}

std::vector<float> Gamesession::ProvideNetworkWithInputs() const
{
	std::vector<float> inputs = { Ekans.y.front()/float(terminal.ymax), 
								  Ekans.x.front()/float(terminal.xmax),
								 (Ekans.y.front() - pidgey.y)/float(terminal.ymax),
								 (Ekans.x.front() - pidgey.x)/float(terminal.xmax),
								 (Ekans.direction.front() == UP?    1.0f:0.0f),
								 (Ekans.direction.front() == DOWN?  1.0f:0.0f),
								 (Ekans.direction.front() == LEFT?  1.0f:0.0f), 
								 (Ekans.direction.front() == RIGHT? 1.0f:0.0f)};
	return inputs;
}

void Gamesession::play()
{
	while((command = getch()) != 'q' && !gameover)
	{
		clear();
			
		Ekans.move(command);
		Ekans.display();
		
		gameover = Ekans.check_coord(terminal);
			
		pidgey.check(Ekans, terminal);
		pidgey.display();

		refresh();
	}
	
	if(gameover) 
	{
		clear();
		//center end screen
		mvprintw(ystart-2,xstart-6, "GAME OVER");
		mvprintw(ystart  ,xstart-6, "Score: %d ", Ekans.score);
		refresh();
		sleep(2);
	}
}

void Gamesession::AI_play(TrainedNeuralNetwork & champ)
{
	std::vector<float> inputs;
	std::vector<float> networkOutputs;
	gameover = false;
	while((command = getch()) != 'q' && !gameover)
	{
		clear();
		inputs = ProvideNetworkWithInputs();
		networkOutputs = champ.GetOutputsUsingInputs(inputs);

		command = 0;
		auto maxelement = std::max_element(std::begin(networkOutputs), std::end(networkOutputs));
		command = std::distance(std::begin(networkOutputs), maxelement);

		if      (command == 3) {command = KEY_UP;    mvprintw(0,0,"Command: UP");   }
		else if (command == 2) {command = KEY_DOWN;  mvprintw(0,0,"Command: DOWN"); }
		else if (command == 1) {command = KEY_LEFT;  mvprintw(0,0,"Command: LEFT"); }
		else if (command == 0) {command = KEY_RIGHT; mvprintw(0,0,"Command: RIGHT");}

		
		Ekans.move(command);
		Ekans.display();
			
		gameover = Ekans.check_coord(terminal);
				
		pidgey.check(Ekans, terminal);
		pidgey.display();

		refresh();
	}
	
	if(gameover) 
	{
		clear();
		//center end screen
		mvprintw(ystart-2,xstart-6, "GAME OVER");
		mvprintw(ystart  ,xstart-6, "Score: %d ", Ekans.score);
		refresh();
		//sleep(2);
	}
}