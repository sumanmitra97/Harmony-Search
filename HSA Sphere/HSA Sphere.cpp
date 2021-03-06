#include "pch.h"
#include <iostream>
#include <chrono>
#include <random>

//Sphere

float optmization_function(const float vector[], const size_t length)
{
	float sum = 0.f;
	for (size_t i = 0; i < length; i++)
	{
		sum += powf(vector[i], 2.f);
	}
	return sum;
}


int main() {
	auto start = std::chrono::high_resolution_clock::now();

	const float low = -5.12f;
	const float high = 5.12f;

	const size_t nvar = 5;
	const size_t hms = 10000;
	const size_t max_iteration = 50000;
	const float hmcr = 0.9f;
	const float par = 0.4f;
	const float bw = 0.2f;

	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<int> dist_int(0, hms - 1);
	std::uniform_real_distribution<float> dist_01(0.f, 1.f);
	std::uniform_real_distribution<float> dist_range(low, high);


	/*float **harmony_memory = new float*[hms];
	for (size_t i = 0; i < hms; i++)
	{
		harmony_memory[i] = new float[nvar];
	}
	float *harmony_memory_values = new float[hms];

	float *best_vector = new float[max_iteration];
	float *worst_vector = new float[max_iteration];

	float *nchv = new float[nvar];
	float *nchv_value = new float[max_iteration];*/

	float harmony_memory[hms][nvar];
	float harmony_memory_values[hms];

	float best_vector[max_iteration];
	float worst_vector[max_iteration];

	float nchv[nvar];
	float nchv_value[max_iteration];

	size_t worst_vector_index = -1;

	size_t itr = 0;
	float r_f = 0.f;
	int r_i = 0;


	for (size_t i = 0; i < hms; i++)
	{
		for (size_t j = 0; j < nvar; j++)
		{
			r_f = dist_range(gen);
			harmony_memory[i][j] = r_f;
		}
	}

	for (size_t i = 0; i < hms; i++)
	{
		harmony_memory_values[i] = optmization_function(harmony_memory[i], nvar);
	}

	worst_vector[itr] = harmony_memory_values[0];
	best_vector[itr] = harmony_memory_values[0];

	for (size_t i = 1; i < hms; i++)
	{
		if (best_vector[itr] > harmony_memory_values[i])
		{
			best_vector[itr] = harmony_memory_values[i];
		}
		if (worst_vector[itr] < harmony_memory_values[i])
		{
			worst_vector[itr] = harmony_memory_values[i];
			worst_vector_index = i;
		}
	}

	while (itr < max_iteration)
	{
		for (size_t index = 0; index < nvar; index++)
		{
			r_f = dist_01(gen);
			if (hmcr > r_f)
			{
				r_i = dist_int(gen);
				nchv[index] = harmony_memory[r_i][index];
				r_f = dist_01(gen);
				if (par > r_f)
				{
					float t = nchv[index];
					r_f = dist_01(gen);
					if (0.5 <= r_f)
					{
						r_f = dist_01(gen);
						t += r_f * bw;
						if (t <= high)
						{
							nchv[index] = t;
						}
					}
					else
					{
						r_f = dist_01(gen);
						t -= r_f * bw;
						if (t >= low)
						{
							nchv[index] = t;
						}
					}
				}
			}
			else
			{
				r_f = dist_range(gen);
				nchv[index] = r_f;
			}

			if (nchv[index] > high || nchv[index] < low)
			{
				std::cout << "Error: " << nchv[index];
			}
		}
		nchv_value[itr] = optmization_function(nchv, nvar);
		float t = worst_vector[(itr == 0) ? itr : itr - 1];
		if (nchv_value[itr] < t)
		{
			for (size_t index = 0; index < nvar; index++)
			{
				harmony_memory[worst_vector_index][index] = nchv[index];
			}
			harmony_memory_values[worst_vector_index] = nchv_value[itr];
		}

		worst_vector[itr] = harmony_memory_values[0];
		best_vector[itr] = harmony_memory_values[0];

		for (size_t i = 1; i < hms; i++)
		{
			if (best_vector[itr] > harmony_memory_values[i])
			{
				best_vector[itr] = harmony_memory_values[i];
			}
			if (worst_vector[itr] < harmony_memory_values[i])
			{
				worst_vector[itr] = harmony_memory_values[i];
				worst_vector_index = i;
			}
		}
		itr++;
	}

	auto end = std::chrono::high_resolution_clock::now();
	double time_elapsed = double(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());

	std::cout << std::endl << "Elapsed Time(ms): " << time_elapsed << std::endl;
	std::cout << "Best Vector: " << best_vector[max_iteration - 1] << std::endl << std::endl;

	//test

	/*std::cout << "\n\n------------------------------------final memory------------------------------------" << std::endl << std::endl ;

	for (size_t i = 0; i < hms; i++)
	{
		std::cout << (i+1) << " : " << "[";
		for (size_t j = 0; j < nvar-1; j++)
		{
			std::cout << harmony_memory[i][j] << ", ";
		}
		std::cout << harmony_memory[i][nvar - 1] << "] : " << harmony_memory_values[i] << std::endl;
	}

	std::cout << std::endl << std::endl << "--------------------------------------------------------------------------------------------------------------------" << std::endl;

	std::cout << std::endl << "-----------------------------------------------------------------------------------------------------------\n best worst nchv\n";

	for (size_t i = 0; i < max_iteration; i++)
	{
		std::cout << best_vector[i] << " " << worst_vector[i] << " " << nchv_value[i] << std::endl;
	}

	std::cout << "------------------------------------------------------------------------------------------------------\n";*/

	//test

	return 0;
}
