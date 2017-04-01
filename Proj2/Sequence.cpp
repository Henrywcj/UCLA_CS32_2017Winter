#include "Sequence.h"

Sequence::Sequence()
{
	m_size = 0;
	head = nullptr;
}
//passed
bool Sequence::empty() const
{
	if (m_size == 0) return true;
	else return false;
}
//passed 
int Sequence::size() const
{
	return m_size;
}
//passed
bool Sequence::insert(int pos, const ItemType& value)
{
	if (pos < 0 || pos > size()) return false;
	m_size++;
	if (head == nullptr)
	{
		head = new Node;
		head->value = value;
		head->next = nullptr;
		head->previous = nullptr;
		return true;
	}
	
	if (pos == 0)
	{
		Node *q = new Node;
		q->value = value;
		q->previous = nullptr;
		q->next = head;
		head->previous = q;
		head = q;
		return true;
	}
	Node *p = head;
	for (int i = 0; i < pos - 1; i++) p = p->next;
	Node *q = new Node;
	q->value = value;
	q->previous = p;
	q->next = p->next;
	p->next = q;
	if (q->next != nullptr) q->next->previous = q;
	return true;
}
//passed 
int Sequence::insert(const ItemType& value)
{
	m_size++;
	if (head == nullptr) {
		head = new Node;
		head->value = value;
		head->next = nullptr;
		head->previous = nullptr;
		return 0;
	}
	else
	{
		Node *p = head;
		int pos = 0;
		bool finish = false;
		while (p != nullptr && p->value > value)
		{
			if (p->next == nullptr)
			{
				Node *q = new Node;
				q->next = nullptr;
				q->value = value;
				q->previous = p;
				p->next = q;
				finish = true;
				return m_size - 1;
			}
			else
			{
				p = p->next;
				pos++;
			}
		}
		if (!finish) {
			Node *q = new Node;
			q->next = p;
			q->previous = p->previous;
			p->previous = q;
			if (q->previous != nullptr) q->previous->next = q;
			q->value = value;
			if (q->previous == nullptr) head = q;
			return pos;
		}
	}
}
//passed
bool Sequence::erase(int pos)
{
	if (pos < 0 || pos >= size()) return false;
	m_size--;
	if (pos == 0)
	{
		if (head->next == nullptr)
		{
			delete head;
			head = nullptr;
			return true;
		}
		head->next->previous = nullptr;
		Node *q = head->next;
		delete head;
		head = q;
		return true;
	}
	Node *p = head;
	for (int i = 0; i < pos - 1; i++) p = p->next;
	Node *q = p->next;
	p->next = p->next->next;
	if (p->next != nullptr) p->next->previous = p;
	delete q;
	return true;
}
//passed 
int Sequence::remove(const ItemType& value)
{
	Node *p = head;
	int total = 0;
	int pos = 0;
	while (p != nullptr)
	{
		if (p->value == value) {
			p = p->next;
			erase(pos);
			total++;
		}
		else
		{
			p = p->next;
			pos++;
		}
	}
	return total;
}
//passed 
bool Sequence::get(int pos, ItemType& value) const
{
	if (pos < 0 || pos >= size()) return false;
	Node *p = head;
	for (int i = 0; i < pos; i++) p = p->next;
	value = p->value;
	return true;
}
//passed
bool Sequence::set(int pos, const ItemType& value)
{
	if (pos < 0 || pos >= size()) return false;
	Node *p = head;
	for (int i = 0; i < pos; i++) p = p->next;
	p->value = value;
	return true;
} 
//passed
int Sequence::find(const ItemType& value) const
{
	int pos = 0;
	Node *p = head;
	while (p != nullptr && p->value != value)
	{
		pos++;
		p = p->next;
	}
	if (p == nullptr) return -1;
	else return pos;
} 
//passed
void Sequence::swap(Sequence& other)
{
	Sequence temp;
	temp = other;
	other = *this;
	*this = temp;
}
//passed
Sequence::Sequence(const Sequence &other)
{
	Node *p;
	m_size = other.m_size;
	head = new Node;
	head->value = other.head->value;
	head->previous = nullptr;
	p = head;
	Node *q = other.head->next;
	while (q != nullptr)
	{
		Node *w = new Node;
		w->value = q->value;
		p->next = w;
		w->previous = p;
		q = q->next;
		p = w;
	}
	p->next = nullptr;
}
//passed
Sequence& Sequence::operator=(const Sequence &other)
{
	if (this != &other)
	{
		Node *p = head;
		while (p != nullptr)
		{
			Node *n = p->next;
			delete p;
			p = n;
		}
		m_size = other.m_size;
		head = new Node;
		head->value = other.head->value;
		head->previous = nullptr;
		p = head;
		Node *q = other.head->next;
		while (q != nullptr)
		{
			Node *w = new Node;
			w->value = q->value;
			p->next = w;
			w->previous = p;
			q = q->next;
			p = w;
		}
		p->next = nullptr;
	}
	return *this;
}
//passed
Sequence::~Sequence()
{
	Node *p = head;
	while (p != nullptr)
	{
		Node *n = p->next;
		delete p;
		p = n;
	}
}
//passed
int subsequence(const Sequence& seq1, const Sequence& seq2)
{
	int pos = 0;
	for (int i = 0; i < seq1.size(); i++)
	{
		ItemType temp1, temp2;
		seq1.get(i, temp1);
		seq2.get(0, temp2);
		if (temp1 == temp2)
		{
			int j = i, k = 1;
			bool mark = true;
			while (k < seq2.size() && mark)
			{
				ItemType temp11, temp22;
				seq1.get(j + k, temp11);
				seq2.get(k, temp22);
				if (temp11 == temp22) k++;
				else mark = false;
			}
			if (mark == true) return i;
		}
	}
	return -1;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
	Sequence seq3;
	int i = 0, j = 0;
	while (i < seq1.size() && j < seq2.size())
	{
		ItemType temp;
		seq1.get(i, temp);
		seq3.insert(i + j, temp);
		i++;
		seq2.get(j, temp);
		seq3.insert(i + j, temp);
		j++;
	}
	if (i == seq1.size()) 
		while (j < seq2.size())
		{
			ItemType temp;
			seq2.get(j, temp);
			seq3.insert(i + j, temp);
			j++;
		}
	if (j == seq2.size())
		while (i < seq1.size())
		{
			ItemType temp;
			seq1.get(i, temp);
			seq3.insert(i + j, temp);
			i++;
		}
	result = seq3;
}