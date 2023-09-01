/*******************************************************************************************
 * 	CSC1310 PROGRAM 4 - MARKOV CHAINS
 * 	Author:		Riley Grimaud
 * 	Date: 		04/20/2023
 * 	Filename:	markovList.h
 * 	Purpose:	create an array of lists by mapping edges to "corpus" with string keys; use 
 * 				this list to also record probabilities of each word occuring after list heads.
 * 				then, using the probablities, randomly generate a pargraph of words likely to 
 * 				occur following a given randWord.
 *******************************************************************************************/

#ifndef MARKOVLIST_H
#define MARKOVLIST_H

#include<map>
#include<fstream>
#include<sstream>
#include<iostream>
#include<stdlib.h>
using namespace std;

class markovList
{
	private:
		struct edge{
			string value;					//"value" stores the word
			float nodeWeight;				//this is the probability of the word occurring
			edge* next;						//this is the pointer to the next word in list
		};
	
		int corpusSize;
		map<string, edge*> corpus;			//"array" of linked lists, where the heads of the linked lists is each word
											//used in the "Book of Pirates" source,  and every other node in each linked
											//list is every word found to occur exactly after the word at the head of the list

	public:
		markovList(string);
		~markovList();
		string generate(int);
};

/*******************************************************************************************
 * 	Function: 	Constructor
 * 	Purpose:	reading in from a source file, in this case, corpus.txt, assign the first
 * 				word in each line to the head or root of a new list in corpus[]. Then, read in 
 * 				the other words and probabilities on the line to connected edges in a linked list
 * 				structure.
 *******************************************************************************************/
markovList::markovList(string filename)
{
	ifstream file(filename);					//filename is "corpus.txt"
	stringstream parser, splitter;				
	string line, index, word;					//values to hold the whole line, the index num, and the word
	float weight;
	srand (time(0));
	
	if(file.good()) {							//check if file opens without error
		file >> corpusSize;						//the first line in "file" is the value defining corpusSize
		file.ignore(1, '\n');
		
		while(getline(file, line, '\n')) 		//map every word to a position in the "matrix", and keep the line in an 
		{										//array of buffers for later
			parser.clear();
			parser << line;
			getline(parser, index,',');			//pulls the first word of the line, which is the node for which we're making a list of neighbors

			corpus[index] = NULL;				//initialize an empty list at the index---

			while(getline(parser, word, ','))
			{
				edge* connected = new edge();	//allocate a new node in the edge list---	
				splitter.clear();
				splitter.str(word);
				splitter >> word >> weight;

				connected->nodeWeight = weight; //stick word and weight on the node you've just allocated---
				connected->value = word;
				connected->next = NULL;	

				if(corpus[index] == NULL){				//case 1: list is empty. set head to connected and you're done!
					corpus[index] = connected;
				}
				else{									//case 2: list not empty. use a holder edge to keep iterate
					edge * curr = corpus[index];		//through list until last edge is reached.

					while(curr->next != NULL){
						curr = curr->next;
					}

					curr->next = connected;				//set the last node's pointer to connected, which is the new node
				}
			}
		}
	}
}

/****************************************************************************************************************
 * 	Function: 	Destructor
 * 	Purpose: 	iterate through the map and delete dynamically-created edges as you iterate.
 ****************************************************************************************************************/
markovList::~markovList()
{
	edge* holdEdge;										//holder edge to use while iterating through

	//for loop starts the iterator at corpus.begin and increments until corpus.end is reached
	for(map<string,edge*>::iterator iterate = corpus.begin(); iterate!=corpus.end(); ++iterate)  {	
		while (iterate->second != NULL){				//while iterate's value (i.e. "second") is not at end of "list"
			holdEdge = iterate->second->next;			//save "holder" to next value in the list
			delete iterate->second;
			iterate->second = holdEdge;					//set iterate to next value in the list
		}
	}                                    
}

/****************************************************************************************************************
 * 	Function: 	generate()
 * 	Purpose:	Creates an iterator for the map, and randomly lands it somewhere in the array. Then, starting at 
 * 				this random spot, grabs the word associated with that spot, then loops however many
 * 				times the user designates with input value "length". The loop essentially picks the following 
 * 				word based on a weighted but random "roll", and appends that word to the final product. 
 * 				Rinse and repeat (length - 1) more times. 
 ****************************************************************************************************************/
string markovList::generate(int length)
{

	map<string, edge*>::iterator iterate = corpus.begin();		//initialize an iterator to find a random node in the next line
	advance(iterate,rand() % corpusSize);						//this grabs a random node from your corpus as a starting point

	string totalPhrase;											//this is what we'll return at the very end
	string present = iterate->first;							//this is the first word we'll be working with
	totalPhrase = present + " ";								//add first word to the total phrase

	for(int i = 1; i < length; i++){							//iterate "length" times to get "length" words + original word to make user's desired word output
		edge * tempEdge = corpus[present];			
		float keyWeight = (float)rand()/RAND_MAX;				//randomly grabs a value to check against the probabilities

		while ((keyWeight > tempEdge->nodeWeight) && (tempEdge != NULL)){		
			keyWeight -= tempEdge->nodeWeight;					//subtract current edge's probability from random's
			tempEdge = tempEdge->next;							//move to the next edge in the list
		}

		if(tempEdge == NULL){									//if current edge is null,
			iterate = corpus.begin();							//then start from scratch;
			advance(iterate,rand() % corpusSize);				//randomly jump to a spot in corpus
			present = iterate->first;							//reset currWord
		}
		else{
			present = tempEdge->value;							//our current edge now has the word that we want to append!
			totalPhrase.append(present);
			totalPhrase += " ";									//add a space in between each word so they don't run together.
		}
	}

	return totalPhrase;
}


#endif