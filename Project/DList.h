#pragma once

template<class data = int>
class DList
{

private:

	struct node
	{
		data nodeData;
		node* next, *prev;
		node(const data& _d, node* prev) : nodeData(_d), prev(prev), next(NULL) {}
	};

	int theCount;
	node* first, *last;

public:

	DList();
	~DList();

	void PushBack(const data& _d);
	void PushFront(const data& _d);
	int Size() const { return theCount; }
	void Erase(int index);

	data operator[](int index);
	const data operator[](int index) const;

	void Clear();
};

template<typename data>
void DList<data>::PushBack(const data& _d)
{
	node* temp = new node(_d, last);

	if (theCount == 0)
		first = temp;
	else
		last->next = temp;

	last = temp;

	++theCount;
}

template<typename data>
void DList<data>::PushFront(const data& _d)
{
	node* temp = new node(_d, NULL);

	if (theCount == 0) 
		last = temp;
	else{
		first->prev = temp;
		temp->next = first;
	}

	first = temp;

	++theCount;
}

template<typename data>
data DList<data>::operator[](int index)
{
	node* temp = first;

	int i = 0;
	for (; i < index; ++i)
		temp = temp->next;

	return temp->nodeData;
}

template<typename data>
const data DList<data>::operator[](int index) const
{
	node* temp = first;

	int i = 0;
	for (; i < index; ++i)
		temp = temp->next;

	return temp->nodeData;
}


template<typename data>
DList<data>::DList()
{
	first = last = NULL;
	theCount = 0;
}

template<typename data>
void DList<data>::Clear()
{
	node* temp = first;

	int i = 0;
	for (; i < theCount; ++i)
	{
		first = first->next;
		delete temp;
		temp = first;
	}

	first = last = NULL;
	theCount = 0;
}

template<typename data>
void DList<data>::Erase(int index)
{
	node* temp = first;

	for (int i = 0; i < index; i++)
	{
		temp = temp->next;
	}

	if (last == first)
	{
		delete temp;
		first = last = NULL;
		theCount = 0;
	}

	else if (temp == first)
	{
		first = first->next;
		first->prev = NULL;
		delete temp;
		--theCount;
	}

	else if (temp == last)
	{
		last = last->prev;
		last->next = NULL;
		delete temp;
		--theCount;
	}

	else 
	{
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
		delete temp;
		--theCount;
	}

}

template<typename data>
DList<data>::~DList()
{
	node* temp = first;

	int i = 0;
	for (; i < theCount; ++i)
	{
		first = first->next;
		delete temp;
		temp = first;
	}


}