#include "PointKmeans.h"
#include "KMeans.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "KMeans.h"

using namespace std;

/**
* Reads in the data.csv file into a vector of points
* @return vector of points
*
*/
vector<PointKmeans> KMeans::readcsv(string file_name){
	
	vector<PointKmeans> points;
	string line;
	ifstream file(file_name);
	while (getline(file, line)) {
		stringstream lineStream(line);
		string bit;
		double x, y;
		getline(lineStream, bit, ',');
		x = stof(bit);
		getline(lineStream, bit, '\n');
		y = stof(bit);

		points.push_back(PointKmeans(x, y));
	}
	return points;
}


/**
* Perform k-means clustering
* @param points - pointer to vector of points
* @param epochs - number of k means iterations
* @param k - the number of initial centroids
*/
void KMeans::kMeansClustering(vector<PointKmeans>* points, int epochs, int k) {
	int n = points->size();

	// Randomly initialise centroids
	// The index of the centroid within the centroids vector
	// represents the cluster label.
	vector<PointKmeans> centroids;
	srand(time(0));
	for (int i = 0; i < k; ++i) {
		centroids.push_back(points->at(rand() % n));
	}

	for (int i = 0; i < epochs; ++i) {
		// For each centroid, compute distance from centroid to each point
		// and update point's cluster if necessary
		for (vector<PointKmeans>::iterator c = begin(centroids); c != end(centroids);
			++c) {
			int clusterId = c - begin(centroids);

			for (vector<PointKmeans>::iterator it = points->begin();
				it != points->end(); ++it) {
				PointKmeans p = *it;
				double dist = c->distance(p);
				if (dist < p.minDist) {
					p.minDist = dist;
					p.cluster = clusterId;
				}
				*it = p;
			}
		}

		// Create vectors to keep track of data needed to compute means
		vector<int> nPoints;
		vector<double> sumX, sumY;
		for (int j = 0; j < k; ++j) {
			nPoints.push_back(0);
			sumX.push_back(0.0);
			sumY.push_back(0.0);
		}

		// Iterate over points to append data to centroids
		for (vector<PointKmeans>::iterator it = points->begin(); it != points->end();
			++it) {
			int clusterId = it->cluster;
			nPoints[clusterId] += 1;
			sumX[clusterId] += it->x;
			sumY[clusterId] += it->y;

			it->minDist = DBL_MAX;  // reset distance
		}
		// Compute the new centroids
		for (vector<PointKmeans>::iterator c = begin(centroids); c != end(centroids);
			++c) {
			int clusterId = c - begin(centroids);
			c->x = sumX[clusterId] / nPoints[clusterId];
			c->y = sumY[clusterId] / nPoints[clusterId];
		}
	}
	// Write to csv
	write_to_csv(points, "output_clusters.csv");

}

void KMeans::write_to_csv(vector<PointKmeans>* points, string file_name)
{
	// Write to csv
	ofstream myfile;
	myfile.open(file_name);
	myfile << "x,y, cluster" << endl;
	cout << "x,y, cluster" << endl;
	for (vector<PointKmeans>::iterator it = points->begin(); it != points->end();
		++it) {
		myfile << it->x << "," << it->y << "," << it->cluster << endl;
		cout << it->x << "," << it->y << "," << it->cluster << endl;
	}
	myfile.close();
}

int main() {
	KMeans km;
	string file_name = "data_clusters.csv";
	vector<PointKmeans> points = km.readcsv(file_name);

	// Run k-means with 100 iterations and for 4 clusters
	km.kMeansClustering(&points, 100, 4);
}

