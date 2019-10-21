

void knapsack_kernel(double *gainMatrix, int *weight, double *gain, bool *isIncluded, int w, int n)
{


	for (int row = 1; row < n; row++)
	{
		int add_weight = (row == 1 ? 0 : weight[row - 1]);

		for (int col = 1; col < w; col++)
		{

			if (add_weight > col) // new item is too heavy
			{
				// new item is not added - same gain as without it
				gainMatrix[w*row + col] = gainMatrix[w*(row - 1) + col];
				isIncluded[w*row + col] = false;
			}
			else // new item can fit
			{
				double exclude_gain = gainMatrix[w*(row - 1) + col];
				double include_gain = gainMatrix[w*(row - 1) + col - add_weight] + gain[row - 1];

				// select better option
				if (exclude_gain >= include_gain)
				{
					//exclude
					gainMatrix[w*row + col] = exclude_gain;
					isIncluded[w*row + col] = false;
				}
				else
				{
					//include
					gainMatrix[w*row + col] = include_gain;
					isIncluded[w*row + col] = true;
				}
			}// assigning new gain 
		}

	}// for

}// knapsack_kernel