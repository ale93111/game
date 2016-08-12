#include <ncursesw/ncurses.h>
#include <locale.h>
#include <unistd.h> //sleep

#include "brain/neural_network_trainer.h"
#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <fstream>

#include "board.h"
#include "gamesession.h"

using namespace std;
using namespace JNF_NEAT;


int main() {
	// Initilize Random Generator
	//srand((unsigned)time(0U));
	srand (1);

	//initialize ncurses
	setlocale(LC_ALL,"");
	initscr();
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	keypad(stdscr, TRUE);
	curs_set(FALSE);
	noecho();
	//resizeterm(12,12);

	// Prepare Parameters
	TrainingParameters params;
	params.numberOfInputs = 8;
	params.numberOfOutputs = 4;
	//params.advanced.structure.numberOfBiasNeurons = 4;
	params.updatesPerGeneration = 100;
	double fitness = 1.0; //select score to achieve
	size_t populationCount = 50;

	// Create Bodies
	vector<shared_ptr<IBody>> bodies;
	for (size_t i = 0; i < populationCount; ++i) {
		bodies.push_back(make_shared<Gamesession>());
	}

	// Training
	NeuralNetworkTrainer trainer(move(bodies), params);
	trainer.loggingEnabled = false;
	trainer.TrainUntilFitnessEquals(fitness);

	// Get the best Neural Network trained
	auto champ = trainer.GetTrainedNeuralNetwork();
	
	endwin();

	/*
	int xmax, ymax;
	getmaxyx(stdscr, ymax, xmax);
	Board terminal;
	terminal = Board(ymax, xmax);
	cout << terminal.ymax << " " << terminal.xmax << endl;
	*/
	cout << "Training complete!" << endl;
	cout << "press key to continue" << endl;
	
	char c;
	c = cin.get();

	initscr();
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	keypad(stdscr, TRUE);
	curs_set(FALSE);
	noecho();
	//resizeterm(12,12);

	attron(COLOR_PAIR(1));
	Gamesession gottacatchemall(500);

	gottacatchemall.AI_play(champ);


	endwin();
	
	cout << "Score" << gottacatchemall.GetFitness() << endl;
	cout << champ.GetJSON() << endl;
	
	return 0;
}