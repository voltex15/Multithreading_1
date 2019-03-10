// Project_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include <string>
#include <fstream>

using namespace std;

bool searchNumber(int x)
{
	int sum = 0;
	float halfX = x / 2;
	halfX = (int)halfX;

	for (int i = 1; i <= halfX; i++)
	{
		if ((x % i) == 0)
		{
			sum += i;
		}
	}

	if (sum == x)
	{
		return true;
	}
	else
	{
		return false;
	}
}

double calculatePercent(double pattern, double patternPercent)
{
	double result = ((patternPercent * 100) / pattern );
	return result;
}


void saveResultInFile(
	int maxPerfectNumber, 
	double measurementTimeOneThread, 
	double measurementTimeTwoThreads, 
	double measurementTimeMultithreading)
{
	double percentTwoThreads = calculatePercent(measurementTimeOneThread, measurementTimeTwoThreads);
	double percentMultithreads = calculatePercent(measurementTimeOneThread, measurementTimeMultithreading);

	ofstream myFile;
	myFile.open("result.csv", ios_base::app);
	myFile << "\n"	+ to_string(maxPerfectNumber) 
					+ "," + to_string(measurementTimeOneThread) 
					+ "," + to_string(measurementTimeTwoThreads) 
					+ "," + to_string(percentTwoThreads)
					+ "," + to_string(measurementTimeMultithreading)
					+ "," + to_string(percentMultithreads);
	myFile.close();
}

int main()
{
	cout << "---------------------------------------" << endl;
	cout << "Program do wyszukiwania liczb idealnych" << endl;
	cout << "---------------------------------------" << endl << endl;

	int maxPerfectNumber;

	cout << "Wprowadz liczbe: ";
	cin >> maxPerfectNumber;
	cout << endl;

	vector<int> foundPerfectNumbers;

	// Jeden watek
	clock_t beginTime = clock();

	for (int x = 1; x < maxPerfectNumber; x++)
	{
		if (searchNumber(x) == true)
		{
			foundPerfectNumbers.push_back(x);
		}
	}

	clock_t endTime = clock();

	double measurementTimeOneThread = double(endTime - beginTime) / CLOCKS_PER_SEC;

	cout << "Maksymalna liczba: " << maxPerfectNumber << endl;
	cout << "Liczby doskonale: ";
	for (int i = 0; i < foundPerfectNumbers.size(); i++)
	{
		cout << foundPerfectNumbers[i] << ", ";
	}
	cout << endl << endl << "---------------------------------------";
	cout << endl << "Czas dla jednego watku: " << measurementTimeOneThread << endl;
	cout << "---------------------------------------" << endl;

	// dwa watki
	beginTime = 0;
	endTime = 0;

	beginTime = clock();

	#pragma omp parallel num_threads(2)
	#pragma omp parallel for
	for (int x = 1; x < maxPerfectNumber; x++)
	{
		if (searchNumber(x) == true)
		{
			foundPerfectNumbers.push_back(x);
		}
	}

	endTime = clock();

	double measurementTimeTwoThreads = double(endTime - beginTime) / CLOCKS_PER_SEC;

	cout << "---------------------------------------";
	cout << endl << "Czas dla dwoch watkow: " << measurementTimeTwoThreads << endl;
	cout << "---------------------------------------" << endl;

	// Cztery watki
	beginTime = 0;
	endTime = 0;

	beginTime = clock();

	#pragma omp parallel for
	for (int x = 1; x < maxPerfectNumber; x++)
	{
		if (searchNumber(x) == true)
		{
			foundPerfectNumbers.push_back(x);
		}
	}

	endTime = clock();

	double measurementTimeMultithreading = double(endTime - beginTime) / CLOCKS_PER_SEC;

	cout << "---------------------------------------";
	cout << endl << "Czas dla czterech watkow: " << measurementTimeMultithreading << endl;
	cout << "---------------------------------------" << endl << endl;

	saveResultInFile(
		maxPerfectNumber, 
		measurementTimeOneThread, 
		measurementTimeTwoThreads, 
		measurementTimeMultithreading);
	cout << "Zapisano do pliku results.csv" << endl;

	return 0;
}
