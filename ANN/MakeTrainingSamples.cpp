#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
using namespace std;

int main()
{

	ofstream myfile;
	myfile.open("train.txt");	

	int in, inter_layers, out, number, sum=0;
	cout << "Enter topology variables (3 numbers): "; // Type a number and press enter
	cin >> in >> inter_layers >>out; // Get user input from the keyboard

	cout << "\nEnter number of data: "; // Type a number and press enter
	cin >> number; 

	myfile << "topology: "<< in <<" " << inter_layers << " "<< out << endl;
	for(int i = number; i >= 0; --i)
	{
		myfile << "in: ";
		sum = (int)(20.0 * rand() / double(RAND_MAX));
		myfile << sum << " ";
		for (int numer_input = 0; numer_input < in-1; numer_input++) {
			int n1 = (int)(20.0 * rand() / double(RAND_MAX));
			myfile << n1 << " ";
			sum ^= n1;
		}
		myfile << endl;
		myfile << "out: " << sum << ".0" << endl;
	
		
	}
	myfile.close();
}

