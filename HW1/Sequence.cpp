#include "Sequence.h"

Sequence::Sequence()
{
	m_size = 0;
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
	if (pos > m_size || m_size + 1 > DEFAULT_MAX_ITEMS || pos < 0) return false;
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
	ItemType t;
	int a = size(), b = other.size();
	if (a <= b) {
		for (int i = 0; i < a; i++) {
			t = m_array[i];
			other.get(i, m_array[i]);
			other.set(i, t);

		} // switching
		
		for (int i = a; i < b; i++)
		{
			other.get(i, t);
			insert(i, t);
		}
		for (int i = b - 1; i >= a; i--)
			other.erase(i);
	}
	else {
		for (int i = 0; i < b; i++) {
			t = m_array[i];
			other.get(i, m_array[i]);
			other.set(i, t);

		} // switching
		for (int i = b; i < a; i++)
		{
			t = m_array[i];
			other.insert(i, t);
		}
		for (int i = a - 1; i >= b; i--)
			erase(i);
	}
}
// Exchange the contents of this sequence with the other one.