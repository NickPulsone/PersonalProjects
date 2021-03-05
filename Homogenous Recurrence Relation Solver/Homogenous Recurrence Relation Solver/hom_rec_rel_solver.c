/*
   Homogenous Reccurence Relation Solver by Nicholas Pulsone
   Created after first college semester, Fall 2020

   The following program will solve a homogenous reccurence relation.
   It will use a file containing either 2 or 3 terms of a reccurence
   relation in homogenous form as well as 1 or 2 initial conditions
   respectively.
   Example: to solve S(k) - 10S(k-1) + 9S(k-2) = 0, S(0) = 3, S(1) = 11,
			write the following in input.txt:
			3
			1 -10 9
			2
			0 3
			1 11
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define QUAD_TERM_CT (3) // Maximum number of terms that can be handled by the program

// Custom inital condition type
struct init_c
{
	int index; // index of a term in a recursive equation
	double value; // value of recursive equation at index
};
typedef struct init_c Init_c;

// Prototypes
int get_data(double* coeffs, Init_c* initial_conditions);

void get_roots(double* roots, double* coeffs, int terms);

void solve_relation(double* roots, Init_c* conditions, double* results, int terms);

void output_solution(double* results, double* roots, int terms);


int main(int argc, char* argv[])
{
	/*Local variables*/
	int terms; // number of terms of the recursive sequence
	double coeffs[QUAD_TERM_CT]; // stores coefficients of recusive terms in file
	Init_c conditions[QUAD_TERM_CT - 1]; // stores initial conditions for recursive sequence
	double roots[QUAD_TERM_CT - 1]; // stores the characteristic roots
	double results[QUAD_TERM_CT - 1]; // stores coefficients for terms in the solution

	/*Executables*/
	terms = get_data(coeffs, conditions);
	get_roots(roots, coeffs, terms);
	solve_relation(roots, conditions, results, terms);
	output_solution(results, roots, terms);
	return 0;
}

/*
	Get terms and coefficients from input file

	Precondition: "input.txt" file containing two parts. One part
	is the number of terms in the original recursive equation along
	with the coefficients of the terms themselves, both separated by a
	new line. The second part is the number of initial conditions as well
	as the inital conditions themselves (index with corresponging value)
	all separated by a newlines (Will be 4 or 5 total lines per below):
	2 lines for coefficients: 1st is # of them, 2nd is the coeffs themselves
	2/3 lines for initial conditions: 1st is # of them, then one or two lines
									  follow each containing the index and value.
	Reminder of an example:
	To solve S(k) - 0.25S(k-1) = 0, S(0) = 6
			write the following in input.txt:
			2
			1 -0.25
			1
			0 6

	Postcondition: Two output paramaters
	@param double* coeffs -> Will store coefficients of original
							 recursive equation in homogenous form
	@param Init_c* conditions - > Will store initial conditions
								  of the recursive equation per
								  custom initail conditions type
	Returns number of terms in the recursive relation
*/
int get_data(double* coeffs, Init_c* initial_conditions)
{
	int i;
	int terms;
	int conditions;
	double num;
	// Insert different file name here if needed
	char filename[] = "input.txt";
	FILE* fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("Could not open file: %s. Please check that the file exists and contains proper data.\n", filename);
		exit(1);
	}

	// Get number of terms
	fscanf(fp, "%d", &terms);
	if (terms < (QUAD_TERM_CT - 1))
	{
		printf("Too few terms. Check your data.\n");
		exit(1);
	}

	// Get terms
	for (i = 0; (i < terms); i++)
	{
		fscanf(fp, "%lf", &num);
		coeffs[i] = num;

	}

	// Get number of conditions
	fscanf(fp, "%d", &conditions);
	if (conditions < (QUAD_TERM_CT - 2))
	{
		printf("Too few conditions. Check your data.\n");
		exit(1);
	}

	// Get conditions
	for (i = 0; i < conditions; i++)
	{
		fscanf(fp, "%lf", &num);
		initial_conditions[i].index = (int)num;
		fscanf(fp, "%lf", &num);
		initial_conditions[i].value = num;
	}
	fclose(fp);

	return terms;
}

/*
	Get roots of characteristic equation based on givens coefficients
	Precondition: Two input parameters
	@param double* coeffs -> Will store coefficients of original
							 recursive equation in homogenous form
	@param int terms -> Number of terms in given recursive equation
	Postcondition: One output parameter
	double* roots -> Stores the roots of the recursive equation
					 in characteristic form

*/
void get_roots(double* roots, double* coeffs, int terms)
{
	// Linear case
	if (terms < 3)
	{
		roots[0] = (-1 * coeffs[1]) / coeffs[0];
	}
	// Quadratic case
	else
	{
		if (4 * coeffs[0] * coeffs[2] > pow(coeffs[1], 2))
		{
			printf("No real solution\n");
			exit(0);
		}
		roots[0] = ((-1 * coeffs[1]) + sqrt(pow(coeffs[1], 2) - 4 * coeffs[0] * coeffs[2])) / (2 * coeffs[0]);
		printf("Root 1: %d\n", roots[0]);
		roots[1] = ((-1 * coeffs[1]) - sqrt(pow(coeffs[1], 2) - 4 * coeffs[0] * coeffs[2])) / (2 * coeffs[0]);
		printf("Root 2: %d\n", roots[1]);
	}
}

/*
	Find the coefficients of the solution to the given recursive equation
	based on roots and initial conditions
	Precondition: Three input parameters
	@param doulble* roots -> Stores the roots of the recursive equation
							 in characteristic form
	@param double* coeffs -> Will store coefficients of original
							 recursive equation in homogenous form
	@param int terms -> Number of terms in given recursive equation
	Postcondition: One output parameter
	@param results -> Stores the resulting coefficients of the terms in the solution
					  to the given recursive equation
*/
void solve_relation(double* roots, Init_c* conditions, double* results, int terms)
{
	// Linear case
	if (terms < 3)
	{
		results[0] = conditions[0].value / (pow(roots[0], conditions[0].index));
	}
	// Quadratic case
	else
	{
		results[1] = (conditions[1].value - conditions[0].value * \
			(pow(roots[0], ((double)conditions[1].index - (double)conditions[0].index)))) \
			/ (pow(roots[1], conditions[1].index) - pow(roots[0], ((double)conditions[1].index - (double)conditions[0].index)) \
				* pow(roots[1], conditions[0].index));
		results[0] = (conditions[0].value - results[1] * pow(roots[1], conditions[0].index)) / pow(roots[0], conditions[0].index);
	}
}

/*
	Print the results of the previous computation the screen
	Precondition: Three input parameters
	@param double* results -> Stores the resulting coefficients of the terms in the solution
							  to the given recursive equation
	@param doulble* roots -> Stores the roots of the recursive equation
							 in characteristic form
	@param int terms -> Number of terms in given recursive equation
	Postcondition: Solution to given homogenous recursive equation printed
				   and formatted on stdout.
*/
void output_solution(double* results, double* roots, int terms)
{
	// Linear Case
	if (terms < 3)
	{
		printf("Your solution: S(k) = %lf(%lf)^k\n", results[0], roots[0]);
	}
	// Quadratic Case
	else
	{
		printf("Your solution: S(k) = %.3lf(%.3lf)^k + %.3lf(%.3lf)^k\n", results[0], roots[0], results[1], roots[1]);
	}
}