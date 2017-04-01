#include "newSequence.h"
#include <iostream>
Sequence::Sequence(int item)
{
	if (item < 0)
	{
		std::cout << "Size cannot be negative!" << std::endl;
		exit(1);
	}
	m_size = 0;
	size_limit = item;
	m_array = new ItemType[size_limit];
}

Sequence::Sequence(const Sequence& other)
{
	m_size = other.m_size;
	size_limit = other.size_limit;
	m_array = new ItemType[size_limit];
	for (int i = 0; i < m_size; i++)
	{
		m_array[i] = other.m_array[i];
	}
}

Sequence& Sequence::operator=(const Sequence& r)
{
	if (this != &r)
	{
		delete[] m_array;
		m_size = r.m_size;
		size_limit = r.size_limit;
		m_array = new ItemType[size_limit];
		for (int i = 0; i < m_size; i++)
			m_array[i] = r.m_array[i];
	}
	return *this;
}

Sequence::~Sequence()
{
	delete [] m_array;
}

bool Sequence::empty() const
{
	if (m_size == 0) return true;
	else return false;
}

int Sequence::size() const
{
	return m_size;
}    

bool Sequence::insert(int pos, const ItemType& value)
{
	if (pos > m_size || m_size + 1 > size_limit || pos < 0) return false;
	else {
		for (int i = m_size; i > pos; i--)
			m_array[i] = m_array[i - 1];
		m_array[pos] = value;
		m_size++;
		return true;
	}
}


int Sequence::insert(const ItemType& value)
{
	int p = -1;
	bool b;
	for (int i = 0; i < m_size; i++)
	{
		if (value <= m_array[i])
		{
			p = i;
			break;		
		}	
	}
	if (p == -1) p = m_size;
	b = insert(p, value);
	if (!b) return -1;
	else return p;
}


bool Sequence::erase(int pos)
{
	if (pos >= 0 && pos < m_size)
	{
		for (int i = pos; i < m_size - 1; i++)
			m_array[i] = m_array[i + 1];
		m_size--;
		return true;
	}
	return false;
}


int Sequence::remove(const ItemType& value)
{
	int tot = 0;
	int i = 0;
	while (i < m_size) 
	{
		if (m_array[i] == value)
		{
			erase(i);
			tot++;
		}
		else i++;
	}
	return tot;
}


bool Sequence::get(int pos, ItemType& value) const
{
	if (pos >= 0 && pos < m_size) {
		value = m_array[pos];
		return true;
	}
	else return false;
}

bool Sequence::set(int pos, const ItemType& value)
{
	if (pos >= 0 && pos < m_size) {
		m_array[pos] = value;
		return true;
	}
	else return false;
}

int Sequence::find(const ItemType& value) const
{
	int p = -1;
	for (int i = 0; i < m_size; i++) {
		if (m_array[i] == value) {
			p = i;
			break;
		}
	}
	return p;
}


void Sequence::swap(Sequence& other)
{
	Sequence t;
	t = other;
	other = *this;
	*this = t;
}
// Exchange the contents of this sequence with the other one.