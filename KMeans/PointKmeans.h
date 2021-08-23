#pragma once
#ifndef CLASS_POINTKMEANS_H
#define CLASS_POINTKMEANS_H
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

struct PointKmeans {
	double x, y;     // coordinates
	int cluster;     // no default cluster
	double minDist;  // default infinite distance to nearest cluster

	PointKmeans() : x(0.0), y(0.0), cluster(-1), minDist(DBL_MAX) {}
	PointKmeans(double x, double y) : x(x), y(y), cluster(-1), minDist(DBL_MAX) {}

	/**
	* Computes the (square) euclidean distance between this point and another
	*/
	double distance(PointKmeans p) {
		return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y);
	}
};
#endif