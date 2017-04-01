#ifndef MYMAP_INCLUDED
#define MYMAP_INCLUDED
// MyMap.h
//120 lines
template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap(); //constructor
	~MyMap();
	void clear();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);
	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;
	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}
	// C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	struct Node
	{
		KeyType m_key;
		ValueType m_value;
		Node* left;
		Node* right;
	};
	Node* head;
	int m_size;
	void clearHelper(Node* currentNode);
	const ValueType* findHelper(const KeyType& key, Node* currentNode) const;
	void associateHelper(const KeyType& key, const ValueType& value, Node*& currentNode, Node* previous);
};

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap()
{
	head = nullptr;
	m_size = 0;
}
template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap()
{
	//delete everything here;
	clear();
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear()
{
	clearHelper(head);
}
template<typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const
{
	return m_size;
}
template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	associateHelper(key, value, head, nullptr);
}
template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const //why a ValueType pointer?? Do I need to change private member??
{
	return findHelper(key, head);
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clearHelper(Node* currentNode)
{
	//use recursion to free memory of the subtree with head Node pointed by currentNode
	if (currentNode == nullptr) return;
	Node* left = currentNode->left;
	Node* right = currentNode->right;
	clearHelper(left);
	clearHelper(right);
	delete currentNode;
}
template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::findHelper(const KeyType& key, Node* currentNode) const
{
	if (currentNode == nullptr) return nullptr;
	if (currentNode->m_key == key) return &(currentNode->m_value);
	if (currentNode->m_key < key) return findHelper(key, currentNode->right);
	if (currentNode->m_key > key) return findHelper(key, currentNode->left);
}
template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associateHelper(const KeyType& key, const ValueType& value, Node*& currentNode, Node* previous)
{
	if (currentNode == nullptr)
	{
		m_size++;
		currentNode = new Node;
		currentNode->m_key = key;
		currentNode->m_value = value;
		currentNode->left = nullptr;
		currentNode->right = nullptr;
		if (previous != nullptr)
		{
			if (previous->m_key > key) previous->left = currentNode;
			else previous->right = currentNode;
		}
		return;
	}
	if (currentNode->m_key == key)
	{
		currentNode->m_value = value;
		return;
	}
	if (currentNode->m_key > key) return associateHelper(key, value, currentNode->left, currentNode);
	else return associateHelper(key, value, currentNode->right, currentNode);
}
#endif // !MYMAP_INCLUDED


