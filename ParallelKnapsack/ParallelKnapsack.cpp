// ParallelKnapsack.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <math.h>
#include <fstream >
#include "knapsack_kernel.h"

using namespace std;


class ParallelKnapsack
{
public:

	ParallelKnapsack(int* _weight, double* _gain, int _items, int _capacity)
	{
		items = _items;
		weight = _weight;
		gain = _gain;
		capacity = _capacity;

		n = items + 1;
		w = capacity;
		resultVector = new bool[items];

		initMatrix();
	}

	void compute()
	{
		knapsack_kernel(gainMatrix, weight, gain, isIncluded, w, n);
		calculateVector();
	}

	//accessors 
	bool* getVector(){ return resultVector; }
	double getMaxGain(){ return maxGain; }

private:

	// input
	int items;
	int capacity;

	int n, w;

	// input vectors 
	int* weight;
	double* gain;

	// matrix 
	double* gainMatrix;
	bool* isIncluded;

	// results
	double maxGain;
	bool* resultVector;

	// define matrix of reduced problems
	void initMatrix()
	{
		gainMatrix = new double[n*w];
		isIncluded = new bool[n*w];

		for (int i = 0; i < n; i++)
		{
			double _gain = 0;
			for (int j = 0; j < w; j++)
			{
				gainMatrix[i*w + j] = 0;
				isIncluded[i*w + j] = false;

			}// for j
		}// for i 

	}// initMatrix()


	void calculateVector()
	{
		int i = n - 1;
		int j = w - 1;

		maxGain = gainMatrix[i*w + j];

		while (i > 0)
		{
			resultVector[i - 1] = isIncluded[i*w + j];
			if (isIncluded[i*w + j])
			{
				j -= weight[i - 1];
			}

			i--;
		}
	}// calculate vector

};



int _tmain(int argc, _TCHAR* argv[])
{

	int* weight = new int[20];
	double* gain = new double[20];

	for (int i = 0; i < 20; i++)
	{
		weight[i] = 2 + i + i % 3 - i % 2 + i % 7;
		gain[i] = 3 + i % 5 + i % 7 - i % 3 + 0.1*(i % 5) + 0.01*(i % 13);
	}

	ParallelKnapsack knap(weight, gain, 20, 45);

	ofstream outputFile;
	outputFile.open("output.txt");

	outputFile << "items:";
	for (int i = 0; i < 20; i++)
		outputFile << '\t' << "#" << i;
	outputFile << endl << "weights:";
	for (int i = 0; i < 20; i++)
		outputFile << '\t' << weight[i];
	outputFile << endl << "value:";
	for (int i = 0; i < 20; i++)
		outputFile << '\t' << gain[i];
	outputFile << endl;

	knap.compute();

	outputFile << "optimal set:" << knap.getMaxGain() << endl;

	outputFile << endl << "item:";
	for (int i = 0; i < 20; i++)
		outputFile << '\t' << knap.getVector()[i] ? "Yes" : "No";
	outputFile << endl;

	system("pause");
	return 0;
}

