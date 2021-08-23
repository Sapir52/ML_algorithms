#pragma once
#ifndef CLASS_NAIVEBAYES_H
#define CLASS_NAIVEBAYES_H
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <complex>
#include <unordered_map>
#include <sstream>
using namespace std;
class NaiveBayes {
private:
	// <class id, class probility> <C, P(C)>
	unordered_map<int, double> classes;
	// <class id, <attribute id, probability>> <C, <x, P(x|C)>>
	unordered_map<int, unordered_map<int, double>> attributesPerClass;
public:
	NaiveBayes(vector<vector<int>> &data, int DimSize);
	int predict(vector<int> attributes);
};
#endif