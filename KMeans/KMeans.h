#pragma once
#include "PointKmeans.h"
#ifndef CLASS_KMEANS_H
#define CLASS_KMEANS_H
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

struct KMeans {
	
	vector<PointKmeans> readcsv(string file_name);
	void kMeansClustering(vector<PointKmeans>* points, int epochs, int k);
	void write_to_csv(vector<PointKmeans>* points, string file_name);
};


#endif