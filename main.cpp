#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <assert.h>

using namespace std;


double trip_to_casino(int num_rounds, int initial_total, int initial_wager, double wager_growth_rate, int trip_num, ofstream& output) {

	output << "   -- Trip " << trip_num << " --" << endl;

	// Initialize Variables
	int biggest_loss = 0;
	int current_bet = initial_wager;
	int current_total = initial_total;
	int loss = 0;
	int loss_number = 0;
	int num_red = 0;
	int num_black = 0;
	char round_result = NULL;
	char color_choice = 'R';
	

	for (int i = 0; i < num_rounds; ++i) {
		int round_result_num = rand() % 38 + 1;
		assert(round_result_num < 39);
		if (round_result_num < 19) { 
			round_result = 'B'; 
			++num_black;
		}
		else if (round_result_num > 36) { round_result = 'O'; }
		else {
			round_result = 'R';
			++num_red;
		}
		assert(round_result != NULL);

		output << "\tRound " << i+1 << ": ";

		if (round_result == color_choice) {
			output << "W, Bet: " << current_bet << ", Total cash: " << current_total+current_bet << endl;
			current_total += current_bet;
			current_bet = initial_wager;
			loss = 0;
			loss_number = 0;
		}
		else {
			++loss_number;
			output << "L, Bet: " << current_bet << ", Total loss: " << loss+current_bet << ", Choice: " << color_choice << endl;

			current_total -= current_bet;
			loss += current_bet;
			if (loss > biggest_loss) { biggest_loss = loss; }
			current_bet = loss * wager_growth_rate;
			if (current_bet > current_total) {
				// cout << "BROKE\n";
				return -1;
			}
		}
		// output << "num red: " << num_red << ", num black: " << num_black << endl;
		if (num_red > num_black) { color_choice = 'B'; }
		else { color_choice = 'R'; }

	}

	// cout << "Winnings: " << current_total - initial_total << endl;
	double percent_gain = (((double)current_total - (double)initial_total) / (double)initial_total) * 100.00;
	// cout << "Percent Profit: " << percent_gain << endl;
	return percent_gain;
}


/* This function expects:
	arg[1] = name of file to be read. file should contain one continuous string consisting of any combination of the set of characters {'R', 'B', 'O'}
	arg[2] = total money
	arg[3] = initial wager
	arg[4] (optional) = wager growth rate (default: 1.25)
*/
int main(int argc, char* argv[]) {

	// Check args
	if (argc < 5) {
		cerr << "Not enough arguments\n";
		cout << "Arguement format: str filename, int total_money, int initial_wager, (optional) int wager_growth_rate\n";
		cout << "Example: input.txt 10000 50\n";
		return -1;
	}
	else if (argc > 6) {
		cerr << "Too many arguments\n";
		cout << "Arguement format: str filename, int total_money, int initial_wager, (optional) int wager_growth_rate\n";
		cout << "Example: input.txt 10000 50\n";
		return -1;
	}

	// Read args
	int num_trips = atoi(argv[1]);
	int _num_rounds = atoi(argv[2]);
	int _initial_total = atoi(argv[3]);
	int _initial_wager = atoi(argv[4]);
	double _wager_growth_rate = 1.5;
	if (argc == 6) { _wager_growth_rate = atoi(argv[5]); }
	// cout << "Arguments recieved: " << _num_rounds << " " << _initial_total << " " << _initial_wager << " " << _wager_growth_rate << endl;

	// Initialize output file
	ofstream output_file("logs.txt");
	
	int trip_result = NULL;
	int num_busts = 0;
	double total_percent_gain = 0;
	srand((unsigned int)time(NULL));
	for (int i = 0; i < num_trips; ++i) {
		// cout << "   -- Trip " << i+1 << " --" << endl;
		trip_result = trip_to_casino(_num_rounds, _initial_total, _initial_wager, _wager_growth_rate, i+1, output_file);
		if (trip_result == -1) { ++num_busts; }
		else { total_percent_gain += trip_result; }
	}
	cout << endl << "Average percent profit: " << total_percent_gain/(double)num_trips << endl;
	cout << "Risk for bust: " << (double)num_busts/(double)num_trips << endl;

	return 0;
}