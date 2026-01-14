// ParallelKnapsack.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <math.h>
#include <fstream>
#include "ParallelKnapsackCalculator.h"

using namespace std;


class ParallelKnapsack
{
public:

	ParallelKnapsack(int* _weight, double* _profit, int _items, int _capacity)
	{
		items = _items;
		weight = _weight;
		profit = _profit;
		capacity = _capacity;

		n = items;
		w = capacity;
		resultVector = new bool[items];

	}


	void compute()
	{
		ParallelKnapsackCalculator::fill_profit_matrix(profitMatrix, weight, profit, resultVector, w, n);
	}

	//accessors 
	bool* getVector(){ return resultVector; }
	double getMaxGain(){ return maxGain; }
	double* getProfitMatrix(){ return profitMatrix; }

private:

	// input
	int items;
	int capacity;

	int n, w;

	// input vectors 
	int* weight;
	double* profit;

	// matrix 
	double* profitMatrix;
	bool* isIncluded;

	// results
	double maxGain;
	bool* resultVector;
};



int _tmain(int argc, _TCHAR* argv[])
{

	int* weight = new int[8] { 10, 4, 7, 4, 6, 8, 5, 2 };
	double* gain = new double[8] {5.0, 7.0, 12.0, 4.0, 5.0, 7.0, 9.0, 2.0};

	ParallelKnapsack knap(weight, gain, 8, 10);

	ofstream outputFile;
	outputFile.open("output.tsv");

	outputFile << "items:";
	for (int i = 0; i < 8; i++)
		outputFile << '\t' << "#" << i;
	outputFile << endl << "weights:";
	for (int i = 0; i < 8; i++)
		outputFile << '\t' << weight[i];
	outputFile << endl << "value:";
	for (int i = 0; i < 8; i++)
		outputFile << '\t' << gain[i];
	outputFile << endl << endl;

	knap.compute();

	outputFile << "Calculation Matrix:" << endl;

	double* gainMatrix = knap.getProfitMatrix();
	for (int i = 0; i <= 8; i++)
	{
		outputFile << '\t';
		for (int j = 0; j <= 10; j++) 
		{
			outputFile << gainMatrix[i * 11 + j] << '\t';
		}
		outputFile << endl;
	}

	outputFile << endl;

	outputFile << endl << endl;

	outputFile << "optimal set:" << knap.getMaxGain() << endl;

	outputFile << endl << "item:";
	for (int i = 0; i < 8; i++)
		outputFile << '\t' << (knap.getVector()[i] ? "Yes" : "No");
	outputFile << endl;

	system("pause");
	return 0;
}

