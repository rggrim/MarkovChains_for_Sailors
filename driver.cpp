/*******************************************************************************************
 * 	CSC1310 PROGRAM 4 - MARKOV CHAINS
 * 	Author:		Riley Grimaud
 * 	Date: 		04/20/2023
 * 	Filename:	driver.cpp
 * 	Purpose:	Create an array of lists by mapping edges to "corpus" with string keys; use 
 * 				this list to also record probabilities of each word occuring after list heads.
 * 				Then, using the probablities, randomly generate a paragraph of words likely to 
 * 				occur following a given randWord. User can define length of paragraph generated. 
 *******************************************************************************************/

#include <iostream>
#include "markovList.h"
using namespace std;

int main()
{
	int userDesiredNum;

	cout << "\n\tHello! Welcome to the everyday pirate's probabilistic random phrase generator." << endl;
	cout << "We have calculated which words are likely to follow a given word, by pulling data from" << endl;
	cout << "Howard Pyle's Book of Pirates." << endl;
	cout << "\n\tPlease enter the desired length of your \"random\" phrase. Enter a number 1-1000." << endl;

	do{
		cin >> userDesiredNum;

		if ((userDesiredNum > 1000) || (userDesiredNum < 1)){
			cout << "\nPlease enter an integer between 1 and 1000." << endl;
		}

	} while ((userDesiredNum > 1000) || (userDesiredNum < 1));


	markovList* chain = new markovList("corpus.txt");
	cout << chain->generate(userDesiredNum) << endl;

	return 0;
}