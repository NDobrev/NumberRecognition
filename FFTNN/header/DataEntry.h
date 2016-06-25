#pragma once


class DataEntry
{
public:


	double* pattern;
	double* target;

public:
	DataEntry() : pattern(NULL), target(NULL) {}
	DataEntry(double* p, double* t) : pattern(p), target(t) {}

	~DataEntry()
	{
		delete[] pattern;
		delete[] target;
	}

};
