#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	Sequence s;
	assert(s.empty());
	assert(s.find(333) == -1);
	s.insert(333);
	s.insert(332);
	assert(s.size() == 2 && s.find(333) == 1 && s.find(332) == 0);
	s.insert(1);
	s.insert(2);
	s.insert(999);
	s.insert(999);
	assert(s.remove(999) == 2);
	s.erase(3);
	s.erase(1);
	s.erase(3);
	ItemType ss;
	s.get(0, ss);
	std::cout << ss << endl;
	Sequence s1;
	s1.insert(123);
	s1.swap(s);
	cout << s.size() << endl << s1.size() << endl;
	
	cout << "Passed all tests" << endl;// copy contructor 
}

