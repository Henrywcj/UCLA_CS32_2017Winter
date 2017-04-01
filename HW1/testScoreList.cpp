#include "ScoreList.h"
#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;

 int main()
{
	ScoreList s;
	s.add(30);
	s.add(101);
	assert(s.minimum() == 30);
	assert(s.maximum() == 30);
	cout << s.size() << endl;
	s.remove(30);
	assert(s.size() == 0);
	cout << "Pass all tests";
} 