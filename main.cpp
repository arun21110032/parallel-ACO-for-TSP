#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <ctime>
using namespace std;

#define INVALID -1
#define CITY_AMOUNT 80
#define POPULATION_SIZE 40
#define PHEROMONE_RATE 0.1
#define ALFA 1
#define BETA 2
#define MAX_ITERATIONS 30
#define EVAPORATION_RATE 0.5
#define POSITIVE_CONTS 0.75

struct ant {
	double distance;
	double fitness;
	int position;
	int route[CITY_AMOUNT];
};

ant ants[POPULATION_SIZE];
double pheromone_links[CITY_AMOUNT][CITY_AMOUNT];

int distance_links[CITY_AMOUNT][CITY_AMOUNT] = {
{ INVALID, 25, 40, 50, 60, 75, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 260, 270, 280, 290, 300, 310, 320, 330, 340, 350, 360, 370, 380, 390, 400, 410, 420, 430, 440, 450, 460, 470, 480, 490 },
{ 25, INVALID, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200, 205, 210, 215, 220, 225, 230, 235, 240 },
{ 40, 15, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200, 205, 210, 215, 220, 225, 230 },
{ 50, 20, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200, 205, 210, 215, 220 },
{ 60, 25, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200, 205, 210 },
{ 75, 30, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200, 205 },
{ 80, 35, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195 },
{ 90, 40, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185 },
{ 100, 45, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175 },
{ 110, 50, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165 },
{ 120, 55, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155 },
{ 130, 60, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145 },
{ 140, 65, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135 },
{ 150, 70, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130 },
{ 160, 75, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125 },
{ 170, 80, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120 },
{ 180, 85, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115 },
{ 190, 90, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110 },
{ 200, 95, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105 },
{ 210, 100, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100 },
{ 220, 105, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95 },
{ 230, 110, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90 },
{ 240, 115, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85 },
{ 250, 120, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80 },
{ 260, 125, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75 },
{ 270, 130, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70 },
{ 280, 135, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65 },
{ 290, 140, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60 },
{ 300, 145, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55 },
{ 310, 150, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45, 50 },
{ 320, 155, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40, 45 },
{ 330, 160, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35, 40 },
{ 340, 165, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30, 35 },
{ 350, 170, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25, 30 },
{ 360, 175, 165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20, 25 },
{ 370, 180, 170, 165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15, 20 },
{ 380, 185, 175, 170, 165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10, 15 },
{ 390, 190, 180, 175, 170, 165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID, 10 },
{ 400, 195, 185, 180, 175, 170, 165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID },
{ 410, 200, 190, 185, 180, 175, 170, 165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID },
{ 420, 205, 195, 190, 185, 180, 175, 170, 165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID },
{ 430, 210, 200, 195, 190, 185, 180, 175, 170, 165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID },
{ 440, 215, 205, 200, 195, 190, 185, 180, 175, 170, 165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID },
{ 450, 220, 210, 205, 200, 195, 190, 185, 180, 175, 170, 165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID },
{ 460, 225, 215, 210, 205, 200, 195, 190, 185, 180, 175, 170, 165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID },
{ 470, 230, 220, 215, 210, 205, 200, 195, 190, 185, 180, 175, 170, 165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID },
{ 480, 235, 225, 220, 215, 210, 205, 200, 195, 190, 185, 180, 175, 170, 165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID },
{ 490, 240, 230, 225, 220, 215, 210, 205, 200, 195, 190, 185, 180, 175, 170, 165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, INVALID }
};

int best_distance = 1000000000;
int worse_distance = 0;
double best_fitness;
double worse_fitness;
int best_route[CITY_AMOUNT];
int worse_route[CITY_AMOUNT];
double average = 0.0;
double variance = 0.0;
double standard_deviation = 0.0;
int greater_distance = INVALID;

void init_ant(int index);
void seed_initial_pheromone(bool random);
void get_greater_distance();
bool check_visit(int ant_index, int position);
void build_solution();
void check_best_distance();
void calculate_fitness();
void pheromone_evaporates();
void update_pheromone();

double calculate_time(clock_t start, clock_t end);
int get_random_number(int from, int to);
void print_route(int ant_index, int route[CITY_AMOUNT], double distance);
void print_pheromone();
string number_to_String(double n);
void print_result();
void calculate_metrics();

int main(int argc, char *argv[]) {
	std::cout << "Hello World!";
	clock_t time_start = clock();
	srand(time(0));
	int iteration = 0;
	

	seed_initial_pheromone(false);
	get_greater_distance();

	while (iteration < MAX_ITERATIONS) {
		build_solution();
		check_best_distance();
		calculate_fitness();
		pheromone_evaporates();
		update_pheromone();
		for (int i = 0; i < POPULATION_SIZE; i++) {
			print_route(i, ants[i].route, ants[i].distance);
		}
		iteration++;
	}

	print_result();

	cout << "\n runtime (ACO): "
			<< calculate_time(time_start, clock()) << " ms" << endl;
	//cin.get(); 
	return 0;
}

void init_ant(int index) {
	ants[index].distance = 0;
	ants[index].fitness = 0;
	ants[index].position = 0;
	for (int j = 0; j < CITY_AMOUNT; j++) {
		ants[index].route[j] = INVALID;
	}
	int random_city = get_random_number(0, (CITY_AMOUNT - 1));
	ants[index].route[0] = random_city;
}

void seed_initial_pheromone(bool random) {
	for (int i = 0; i < CITY_AMOUNT; i++) {
		for (int j = 0; j < CITY_AMOUNT; j++) {
			if (i != j) {
				double random_pheromone = (double) get_random_number(0, 100)
						/ 100.0;
				pheromone_links[i][j] = (random == true) ? random_pheromone
						: PHEROMONE_RATE;
			} else {
				pheromone_links[i][j] = INVALID;
			}
		}
	}
}

void get_greater_distance() {
	for (int i = 0; i < CITY_AMOUNT; i++) {
		for (int j = 0; j < CITY_AMOUNT; j++) {
			if (distance_links[i][j] > greater_distance) {
				greater_distance = distance_links[i][j];
			}
		}
	}
	greater_distance += 1;
}

bool check_visit(int ant_index, int position) {
	for (int i = 0; i < CITY_AMOUNT; i++) {
		if (ants[ant_index].route[i] == position) {
			return true;
		}
	}
	return false;
}

void build_solution() {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		init_ant(i);

		while (ants[i].position < (CITY_AMOUNT - 1)) {
			int position = ants[i].position;

			double transition_probability[CITY_AMOUNT];

			double link_rate_sum = 0;
			for (int j = 0; j < CITY_AMOUNT; j++) {
				if (check_visit(i, j) == false) {
					if (pheromone_links[position][j] >= 0
							and distance_links[position][j] >= 0) {
						link_rate_sum
								+= pow(pheromone_links[position][j], ALFA)
										* pow(
												greater_distance
														- distance_links[position][j],
												BETA);
					}
				}
			}

			for (int j = 0; j < CITY_AMOUNT; j++) {
				if (check_visit(i, j) == false) {
					if (pheromone_links[position][j] >= 0
							and distance_links[position][j] >= 0) {
						transition_probability[j] = (pow(
								pheromone_links[position][j], ALFA) * pow(
								greater_distance - distance_links[position][j],
								BETA)) / link_rate_sum;
					} else {
						transition_probability[j] = 0;
					}
				} else {
					transition_probability[j] = 0;
				}
			}

			double roulette = (double) get_random_number(0, 100) / 100.0;
			double minor = 0;
			double major = 0;

			for (int j = 0; j < CITY_AMOUNT; j++) {
				if (check_visit(i, j) == false) {
					major += transition_probability[j];
					if (roulette >= minor and roulette <= major) {
						ants[i].route[position + 1] = j;
						ants[i].distance += distance_links[position][j];
						ants[i].position += 1;
						break;
					} else {
						minor = major;
					}
				}
			}
		}
	}
}

void check_best_distance() {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		if (ants[i].distance < best_distance) {
			best_distance = ants[i].distance;
			for (int j = 0; j < CITY_AMOUNT; j++) {
				best_route[j] = ants[i].route[j];
			}
		} else if (ants[i].distance > worse_distance) {
			worse_distance = ants[i].distance;
			for (int j = 0; j < CITY_AMOUNT; j++) {
				worse_route[j] = ants[i].route[j];
			}
		}
	}
}

void calculate_fitness() {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		double fitness = (double) ants[i].distance / (double) best_distance;
		if (fitness < best_fitness) {
			best_fitness = fitness;
		} else if (fitness > worse_fitness) {
			worse_fitness = fitness;
		}
		ants[i].fitness = fitness;
	}
}

void pheromone_evaporates() {
	for (int i = 0; i < CITY_AMOUNT; i++) {
		for (int j = 0; j < CITY_AMOUNT; j++) {
			if (pheromone_links[i][j] != INVALID) {
				pheromone_links[i][j] = (1 - EVAPORATION_RATE)
						* pheromone_links[i][j];
			}
		}
	}
}

void update_pheromone() {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		double pheromone_to_sum = POSITIVE_CONTS / ants[i].fitness;
		for (int j = 0; j < (CITY_AMOUNT - 1); j++) {
			int city = ants[i].route[j];
			int next_city = ants[i].route[j + 1];
			if (pheromone_links[city][next_city] != INVALID) {
				pheromone_links[city][next_city] += pheromone_to_sum;
			}
		}
	}
}

int get_random_number(int from, int to) {
	return (from < to) ? (rand() % to) + from : 0;
}

double calculate_time(clock_t start, clock_t end) {
	return 1000.0 * ((double) (end - start) / (double) CLOCKS_PER_SEC);
}

void print_route(int ant_index, int route[CITY_AMOUNT], double distance) {
	string temp = "Ant route " + number_to_String(ant_index) + " : ";
	for (unsigned int i = 0; i < CITY_AMOUNT; i++) {
		temp += number_to_String(route[i]);
		if ((i + 1) != CITY_AMOUNT) {
			temp += ", ";
		}
	}
	temp += ". Size: " + number_to_String(distance) + "\n";
	cout << temp << endl;
}

void print_pheromone() {
	string temp = "Pheromone Rates:";
	temp += "\n{";
	for (int i = 0; i < CITY_AMOUNT; i++) {
		temp += "{";
		for (int j = 0; j < CITY_AMOUNT; j++) {
			temp += number_to_String(pheromone_links[i][j]);
			if ((j + 1) != CITY_AMOUNT) {
				temp += ", ";
			}
		}
		if ((i + 1) != CITY_AMOUNT) {
			temp += "}, ";
		} else {
			temp += "}";
		}
	}
	temp += "}\n";
	cout << temp << endl;
}

void print_result() {
	cout << "Worst result:" << endl;
	cout << "f(x):" << worse_fitness << endl;
	print_route(0, worse_route, worse_distance);

	cout << "Best result:" << endl;
	cout << "f(x):" << best_fitness << endl;
	print_route(0, best_route, best_distance);

	calculate_metrics();
	cout << "Avg:" << average << endl;
	cout << "variance:" << variance << endl;
	cout << "Standard deviation:" << standard_deviation << endl;
}

string number_to_String(double n) {
	stringstream out;
	out << n;
	return out.str();
}

void calculate_metrics() {
	double sum = 0;
	for (int i = 0; i < POPULATION_SIZE; i++) {
		sum += ants[i].distance;
	}
	average = (double) sum / (double) POPULATION_SIZE;

	sum = 0;
	for (int i = 0; i < POPULATION_SIZE; i++) {
		sum += pow(ants[i].distance - average, 2);
	}
	variance = (double) sum / (double) POPULATION_SIZE;

	standard_deviation = pow(variance, 0.5);
}
