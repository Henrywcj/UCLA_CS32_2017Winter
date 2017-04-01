#include "ScoreList.h"

ScoreList::ScoreList()
{
	Sequence m_s;
}
// Create an empty score list.

bool ScoreList::add(unsigned long score)
{
	if (score >= 0 && score <= 100 && m_s.insert(size(), score))
	{
		return true;
	}	
	else return false;
}


bool ScoreList::remove(unsigned long score)
{
	int pos = m_s.find(score);
	if (pos == -1) return false; 
	else
	{
		m_s.erase(pos);
		return true;
	}
}

int ScoreList::size() const
{
	return m_s.size();
}


unsigned long ScoreList::minimum() const
{
	if (size() == 0) return NO_SCORE;
	int min = 101;
	for (int i = 0; i < size(); i++)
	{
		unsigned long g;
		m_s.get(i, g);
		if (g < min) 
			min = g;
	}
	return min;
}


unsigned long ScoreList::maximum() const
{
	if (size() == 0) return NO_SCORE;
	int max = 0;
	for (int i = 0; i < size(); i++)
	{
		unsigned long g;
		m_s.get(i, g);
		if (g > max) max = g;
	}
	return max;
}
