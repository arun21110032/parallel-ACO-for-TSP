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

int distance_links[CITY_AMOUNT][CITY_AMOUNT] = {  {INVALID, 71, 13, 20, 70, 22, 19, 70, 44, 77, 71, 89, 37, 100, 76, 26, 11, 38, 19, 99, 100, 5, 63, 24, 43, 15, 13, 20, 91, 93, 2, 89, 19, 39, 20, 97, 77, 56, 4, 100, 37, 41, 35, 26, 33, 94, 34, 30, 100, 33},
{26, INVALID, 17, 74, 18, 66, 78, 45, 19, 50, 94, 31, 21, 91, 15, 88, 60, 35, 95, 2, 85, 31, 13, 71, 32, 35, 89, 31, 36, 45, 92, 58, 33, 85, 16, 51, 21, 74, 86, 24, 17, 38, 31, 67, 80, 38, 36, 16, 100, 33},
{47, 53, INVALID, 29, 40, 49, 43, 34, 80, 55, 23, 11, 48, 93, 77, 15, 15, 31, 18, 62, 79, 67, 32, 29, 60, 31, 84, 81, 67, 14, 94, 1, 28, 41, 48, 100, 56, 1, 57, 55, 96, 16, 78, 99, 72, 83, 24, 76, 51, 47},
{88, 70, 40, INVALID, 23, 5, 100, 13, 41, 72, 66, 44, 99, 51, 32, 64, 12, 19, 2, 66, 13, 86, 100, 15, 78, 51, 33, 80, 99, 85, 45, 82, 11, 28, 27, 3, 4, 97, 44, 58, 24, 29, 59, 78, 56, 21, 74, 61, 65, 82},
{83, 95, 35, 17, INVALID, 27, 6, 94, 7, 8, 84, 68, 27, 23, 74, 4, 93, 44, 78, 63, 36, 35, 54, 56, 44, 41, 67, 95, 22, 52, 71, 96, 33, 97, 100, 8, 7, 33, 61, 35, 36, 25, 99, 46, 5, 60, 82, 70, 80, 50},
{93, 69, 92, 47, 8, INVALID, 100, 42, 18, 67, 50, 93, 3, 3, 32, 11, 89, 52, 62, 44, 76, 88, 82, 57, 31, 28, 95, 90, 13, 96, 9, 94, 18, 69, 12, 86, 71, 76, 41, 73, 94, 52, 99, 19, 61, 62, 38, 61, 82, 74},
{3, 14, 62, 21, 61, 62, INVALID, 72, 41, 88, 59, 85, 86, 58, 10, 81, 37, 32, 77, 41, 22, 63, 61, 1, 95, 93, 29, 89, 94, 49, 3, 47, 57, 69, 80, 58, 18, 73, 81, 72, 25, 31, 84, 70, 3, 74, 59, 13, 26, 33},
{15, 50, 98, 51, 35, 10, 42, INVALID, 51, 89, 80, 94, 6, 9, 22, 81, 2, 47, 87, 14, 7, 50, 33, 82, 24, 92, 22, 42, 11, 27, 43, 68, 18, 42, 59, 47, 67, 35, 83, 32, 24, 11, 85, 79, 34, 33, 79, 17, 83, 92},
{46, 21, 68, 13, 92, 29, 87, 41, INVALID, 55, 83, 100, 22, 45, 100, 99, 49, 68, 66, 73, 14, 78, 42, 77, 79, 30, 75, 8, 3, 33, 28, 32, 10, 89, 40, 57, 84, 4, 68, 26, 86, 1, 38, 70, 88, 93, 10, 82, 48, 46},
{95, 51, 91, 46, 76, 51, 84, 59, 47, INVALID, 70, 57, 69, 97, 78, 86, 37, 29, 27, 66, 85, 91, 42, 30, 39, 20, 14, 16, 3, 83, 89, 9, 67, 86, 2, 89, 10, 72, 49, 68, 65, 21, 86, 52, 4, 40, 80, 73, 22, 38},
{79, 83, 1, 31, 96, 28, 29, 37, 89, 84, INVALID, 80, 42, 10, 49, 1, 30, 81, 76, 35, 67, 97, 78, 61, 23, 60, 98, 10, 51, 17, 49, 73, 5, 83, 57, 16, 78, 72, 64, 54, 51, 11, 17, 3, 17, 73, 65, 71, 41, 21},
{85, 17, 48, 18, 58, 31, 26, 82, 79, 79, 92, INVALID, 63, 32, 77, 96, 55, 4, 11, 25, 83, 25, 7, 44, 67, 25, 6, 38, 76, 73, 25, 2, 69, 73, 60, 54, 37, 80, 78, 79, 70, 71, 91, 99, 69, 18, 80, 79, 89, 73},
{77, 29, 83, 86, 84, 80, 20, 80, 71, 57, 13, 34, INVALID, 94, 61, 89, 16, 33, 80, 70, 46, 86, 79, 55, 20, 96, 19, 87, 76, 61, 50, 85, 10, 9, 7, 62, 97, 40, 9, 78, 64, 22, 17, 1, 52, 34, 6, 59, 62, 68},
{66, 28, 49, 25, 96, 44, 93, 69, 34, 95, 95, 2, 85, INVALID, 20, 87, 100, 10, 47, 68, 15, 76, 73, 69, 77, 45, 94, 73, 11, 48, 37, 4, 64, 79, 52, 35, 71, 23, 48, 39, 42, 82, 2, 49, 66, 33, 40, 49, 98, 94},
{76, 26, 22, 5, 35, 86, 27, 38, 66, 91, 69, 13, 10, 51, INVALID, 89, 58, 93, 11, 75, 41, 97, 91, 61, 89, 94, 36, 3, 51, 25, 70, 82, 71, 58, 99, 31, 67, 95, 92, 52, 37, 1, 73, 16, 23, 56, 25, 89, 31, 39},
{32, 43, 12, 94, 87, 75, 25, 58, 30, 8, 14, 6, 90, 87, 28, INVALID, 34, 60, 28, 27, 92, 86, 54, 80, 14, 79, 15, 87, 12, 9, 64, 10, 46, 58, 91, 92, 67, 100, 98, 12, 95, 62, 38, 50, 86, 37, 55, 71, 70, 45},
{44, 56, 44, 30, 47, 100, 41, 39, 19, 38, 32, 29, 26, 86, 97, 17, INVALID, 87, 95, 92, 56, 75, 14, 80, 44, 31, 4, 78, 21, 37, 94, 97, 42, 52, 28, 2, 65, 52, 96, 49, 92, 53, 32, 58, 67, 2, 69, 20, 88, 63},
{50, 95, 50, 88, 71, 80, 24, 95, 89, 93, 49, 82, 73, 74, 70, 60, 97, INVALID, 23, 39, 98, 79, 21, 49, 89, 3, 17, 90, 79, 81, 38, 95, 91, 63, 8, 83, 91, 51, 19, 87, 35, 38, 81, 17, 62, 19, 16, 20, 72, 75},
{4, 43, 33, 72, 80, 81, 90, 26, 46, 23, 14, 91, 38, 93, 7, 44, 34, 42, INVALID, 70, 43, 17, 83, 53, 97, 56, 89, 36, 76, 26, 62, 98, 100, 14, 60, 39, 2, 77, 66, 85, 49, 69, 69, 67, 2, 12, 93, 43, 32, 41},
{70, 97, 31, 69, 36, 83, 100, 90, 86, 5, 77, 88, 29, 35, 93, 49, 3, 99, 60, INVALID, 82, 15, 49, 6, 12, 78, 47, 15, 36, 100, 87, 76, 4, 64, 36, 88, 91, 54, 50, 75, 42, 24, 56, 78, 70, 62, 36, 71, 8, 76},
{100, 42, 50, 16, 63, 50, 49, 75, 2, 30, 11, 41, 57, 10, 94, 32, 8, 37, 73, 48, INVALID, 17, 74, 29, 26, 86, 76, 83, 19, 94, 94, 91, 66, 55, 70, 63, 87, 88, 44, 44, 10, 17, 18, 54, 78, 57, 32, 71, 67, 22},
{40, 89, 96, 42, 69, 49, 70, 32, 99, 73, 9, 49, 33, 96, 25, 4, 24, 80, 87, 69, 51, INVALID, 94, 51, 39, 55, 27, 73, 29, 87, 70, 8, 72, 14, 75, 63, 87, 94, 50, 77, 91, 89, 97, 89, 88, 91, 7, 14, 26, 42},
{39, 29, 95, 15, 12, 26, 2, 21, 24, 42, 48, 16, 8, 72, 30, 48, 61, 24, 26, 82, 3, 33, INVALID, 22, 77, 45, 39, 97, 96, 3, 7, 62, 75, 43, 37, 21, 93, 83, 24, 40, 39, 27, 47, 37, 49, 12, 10, 75, 79, 15},
{94, 54, 59, 38, 99, 36, 67, 77, 33, 30, 97, 38, 7, 98, 83, 57, 30, 28, 91, 97, 47, 97, 99, INVALID, 49, 54, 93, 8, 2, 98, 74, 5, 43, 86, 91, 37, 42, 31, 57, 92, 27, 67, 15, 71, 3, 48, 32, 70, 73, 17},
{48, 41, 59, 29, 60, 9, 69, 85, 34, 19, 34, 65, 87, 52, 67, 57, 15, 65, 16, 98, 4, 19, 2, 87, INVALID, 72, 83, 23, 37, 100, 21, 18, 34, 38, 88, 26, 7, 8, 30, 68, 28, 6, 87, 97, 92, 34, 41, 32, 18, 33},
{27, 2, 38, 94, 98, 65, 93, 54, 89, 1, 85, 11, 13, 36, 34, 14, 21, 15, 53, 89, 79, 100, 40, 82, 62, INVALID, 20, 22, 1, 19, 57, 40, 75, 57, 48, 72, 86, 76, 20, 63, 97, 19, 15, 22, 84, 43, 16, 81, 19, 75},
{74, 20, 85, 90, 4, 96, 68, 45, 20, 12, 13, 63, 57, 25, 62, 42, 93, 70, 71, 30, 95, 18, 98, 43, 96, 41, INVALID, 48, 81, 21, 4, 44, 78, 93, 67, 37, 44, 16, 3, 21, 25, 67, 42, 11, 70, 70, 61, 8, 28, 12},
{14, 60, 26, 41, 100, 20, 66, 9, 85, 9, 93, 68, 8, 29, 37, 96, 28, 30, 76, 55, 2, 66, 99, 80, 95, 89, 85, INVALID, 17, 64, 93, 68, 14, 61, 63, 82, 29, 75, 36, 24, 93, 29, 65, 12, 59, 5, 36, 99, 21, 86},
{29, 4, 93, 59, 52, 28, 12, 61, 35, 62, 84, 56, 45, 66, 51, 19, 24, 64, 37, 45, 67, 56, 1, 43, 96, 13, 90, 57, INVALID, 63, 23, 28, 83, 43, 92, 93, 27, 51, 53, 33, 20, 21, 96, 27, 40, 36, 86, 71, 5, 71},
{8, 32, 38, 3, 37, 4, 99, 27, 43, 25, 76, 32, 55, 58, 25, 84, 37, 86, 65, 45, 5, 79, 18, 6, 46, 44, 37, 62, 98, INVALID, 68, 21, 10, 35, 94, 3, 21, 73, 37, 64, 9, 95, 63, 100, 8, 10, 36, 50, 69, 1},
{4, 76, 20, 44, 66, 8, 5, 4, 27, 20, 5, 94, 72, 85, 58, 78, 30, 1, 93, 26, 84, 88, 94, 58, 98, 84, 3, 28, 67, 57, INVALID, 34, 78, 76, 44, 96, 11, 45, 91, 28, 48, 91, 40, 29, 100, 48, 10, 98, 62, 1},
{36, 83, 31, 44, 59, 32, 81, 37, 15, 100, 6, 86, 20, 82, 5, 20, 47, 19, 14, 35, 31, 28, 51, 36, 52, 94, 51, 22, 62, 51, 9, INVALID, 57, 50, 47, 95, 28, 14, 61, 44, 100, 49, 81, 84, 12, 68, 95, 98, 30, 65},
{44, 72, 80, 15, 88, 71, 14, 67, 88, 3, 88, 84, 89, 42, 54, 63, 58, 77, 41, 24, 69, 25, 19, 84, 43, 8, 53, 64, 47, 46, 18, 73, INVALID, 40, 15, 22, 62, 35, 31, 95, 24, 16, 29, 32, 71, 71, 83, 61, 98, 14},
{44, 98, 45, 25, 87, 27, 98, 69, 30, 59, 86, 6, 14, 5, 72, 50, 58, 60, 7, 48, 53, 43, 97, 13, 29, 56, 96, 73, 80, 73, 19, 34, 80, INVALID, 76, 53, 93, 53, 47, 4, 45, 43, 69, 89, 2, 31, 32, 82, 35, 52},
{2, 16, 52, 30, 56, 46, 20, 46, 1, 19, 44, 32, 70, 90, 19, 91, 68, 58, 50, 67, 79, 18, 36, 53, 85, 76, 70, 27, 27, 9, 63, 9, 57, 27, INVALID, 2, 75, 94, 96, 66, 100, 84, 11, 32, 89, 58, 54, 14, 75, 69},
{69, 48, 52, 17, 3, 74, 59, 69, 40, 71, 95, 5, 58, 78, 18, 8, 10, 74, 28, 18, 53, 62, 96, 93, 56, 56, 58, 78, 9, 90, 86, 78, 97, 96, 89, INVALID, 35, 91, 94, 16, 81, 50, 94, 8, 27, 25, 56, 57, 10, 7},
{91, 44, 77, 53, 35, 29, 65, 21, 12, 71, 48, 46, 89, 97, 47, 30, 25, 40, 15, 2, 36, 64, 6, 15, 35, 19, 36, 70, 47, 60, 89, 47, 49, 48, 87, 8, INVALID, 64, 80, 51, 99, 85, 39, 69, 31, 80, 60, 48, 53, 46},
{53, 14, 10, 100, 11, 82, 14, 4, 38, 6, 89, 55, 43, 28, 44, 87, 79, 90, 23, 33, 48, 99, 98, 61, 66, 96, 63, 94, 32, 3, 47, 21, 48, 83, 5, 78, 52, INVALID, 79, 42, 61, 36, 51, 57, 35, 15, 22, 60, 81, 79},
{52, 9, 95, 8, 79, 68, 41, 60, 82, 10, 71, 1, 72, 93, 56, 8, 61, 74, 68, 36, 79, 45, 65, 85, 81, 74, 54, 52, 28, 36, 51, 91, 25, 90, 38, 7, 21, 85, INVALID, 71, 79, 40, 71, 99, 50, 66, 11, 42, 98, 98},
{6, 72, 8, 62, 51, 85, 56, 73, 75, 63, 91, 78, 54, 20, 80, 8, 20, 91, 22, 14, 63, 68, 55, 96, 73, 60, 12, 27, 33, 37, 53, 52, 98, 31, 95, 90, 78, 84, 89, INVALID, 91, 41, 67, 2, 97, 50, 48, 86, 14, 39},
{97, 40, 46, 59, 33, 98, 72, 43, 92, 90, 37, 52, 60, 25, 4, 89, 41, 37, 12, 80, 45, 48, 92, 97, 60, 4, 50, 76, 76, 50, 16, 53, 31, 4, 67, 4, 82, 28, 43, 48, INVALID, 53, 43, 21, 51, 64, 95, 45, 51, 41},
{19, 40, 9, 61, 99, 5, 69, 78, 84, 84, 12, 28, 73, 96, 16, 45, 50, 80, 38, 75, 4, 23, 89, 17, 4, 4, 8, 93, 20, 52, 37, 87, 4, 85, 16, 5, 86, 90, 78, 41, 71, INVALID, 25, 18, 39, 12, 85, 1, 2, 77},
{15, 11, 55, 44, 35, 36, 30, 31, 90, 3, 39, 29, 6, 94, 14, 67, 49, 70, 3, 20, 14, 17, 77, 45, 49, 50, 26, 72, 50, 90, 30, 94, 97, 58, 92, 64, 51, 20, 58, 6, 6, 84, INVALID, 89, 17, 68, 60, 42, 29, 92},
{16, 73, 3, 26, 71, 64, 2, 66, 11, 12, 56, 71, 92, 74, 16, 28, 18, 23, 10, 73, 14, 57, 43, 70, 25, 41, 20, 8, 44, 28, 80, 24, 29, 43, 79, 38, 1, 70, 49, 61, 65, 57, 93, INVALID, 79, 63, 94, 23, 44, 11},
{65, 14, 53, 78, 100, 12, 82, 79, 90, 87, 52, 72, 59, 91, 97, 24, 18, 69, 56, 66, 75, 99, 78, 33, 32, 71, 60, 15, 69, 1, 27, 55, 33, 2, 17, 59, 5, 20, 44, 34, 36, 27, 7, 68, INVALID, 27, 65, 5, 1, 17},
{57, 3, 37, 54, 60, 30, 22, 91, 28, 94, 95, 100, 18, 31, 34, 20, 88, 81, 21, 66, 85, 36, 6, 99, 100, 4, 49, 98, 56, 11, 54, 23, 100, 14, 98, 74, 16, 81, 72, 31, 4, 3, 28, 79, 73, INVALID, 58, 78, 30, 63},
{85, 28, 14, 54, 64, 77, 22, 38, 43, 47, 70, 19, 84, 57, 46, 85, 82, 90, 88, 11, 27, 88, 13, 75, 80, 63, 88, 30, 76, 97, 15, 65, 3, 6, 91, 27, 89, 24, 60, 69, 52, 59, 40, 58, 63, 81, INVALID, 66, 77, 20},
{43, 27, 50, 3, 49, 55, 34, 46, 28, 46, 95, 47, 37, 65, 15, 52, 32, 82, 97, 78, 51, 64, 74, 13, 44, 10, 97, 39, 37, 19, 17, 25, 53, 47, 1, 58, 41, 79, 17, 55, 43, 9, 18, 75, 21, 17, 72, INVALID, 75, 25},
{40, 92, 38, 74, 17, 73, 6, 58, 99, 67, 34, 63, 68, 61, 34, 6, 98, 11, 16, 100, 19, 89, 64, 46, 56, 58, 35, 74, 89, 32, 47, 83, 89, 56, 70, 40, 56, 78, 43, 61, 82, 36, 66, 53, 81, 46, 87, 1, INVALID, 37},
{21, 88, 40, 45, 45, 52, 40, 33, 26, 68, 30, 100, 85, 12, 79, 49, 2, 7, 68, 43, 20, 56, 97, 27, 24, 97, 1, 90, 37, 67, 48, 65, 5, 93, 56, 63, 87, 92, 74, 20, 41, 51, 21, 49, 21, 37, 58, 57, 55, INVALID}};
 
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
