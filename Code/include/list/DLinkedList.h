#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include <iostream>
#include <sstream>
#include <type_traits>

#include "list/IList.h"
using namespace std;

template <class T>
class DLinkedList : public IList<T>
{
public:
    class Node;        // Forward declaration
    class Iterator;    // Forward declaration
    class BWDIterator; // Forward declaration

protected:
    Node *head; // this node does not contain user's data
    Node *tail; // this node does not contain user's data
    int count;
    bool (*itemEqual)(T &lhs, T &rhs);        // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(DLinkedList<T> *); // function pointer: be called to remove items (if they are pointer type)

public:
    DLinkedList(
        void (*deleteUserData)(DLinkedList<T> *) = 0,
        bool (*itemEqual)(T &, T &) = 0);
    DLinkedList(const DLinkedList<T> &list);
    DLinkedList<T> &operator=(const DLinkedList<T> &list);
    ~DLinkedList();
    // Inherit from IList: BEGIN
    void add(T e);
    void add(int index, T e);
    T removeAt(int index);
    bool removeItem(T item, void (*removeItemData)(T) = 0);
    bool empty();
    int size();
    void clear();
    T &get(int index);
    int indexOf(T item);
    bool contains(T item);
    string toString(string (*item2str)(T &) = 0);
    void checkIndex(int index);
    // Inherit from IList: END

    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    bool contains(T array[], int size)
    {
        int idx = 0;
        for (DLinkedList<T>::Iterator it = begin(); it != end(); it++)
        {
            if (!equals(*it, array[idx++], this->itemEqual))
                return false;
        }
        return true;
    }

    /*
     * free(DLinkedList<T> *list):
     *  + to remove user's data (type T, must be a pointer type, e.g.: int*, Point*)
     *  + if users want a DLinkedList removing their data,
     *      he/she must pass "free" to constructor of DLinkedList
     *      Example:
     *      DLinkedList<T> list(&DLinkedList<T>::free);
     */
    static void free(DLinkedList<T> *list)
    {
        typename DLinkedList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

    /* begin, end and Iterator helps user to traverse a list forwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::Iterator it;
     for(it = list.begin(); it != list.end(); it++){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    Iterator begin()
    {
        return Iterator(this, true);
    }
    Iterator end()
    {
        return Iterator(this, false);
    }

    /* last, beforeFirst and BWDIterator helps user to traverse a list backwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::BWDIterator it;
     for(it = list.last(); it != list.beforeFirst(); it--){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    BWDIterator bbegin()
    {
        return BWDIterator(this, true);
    }
    BWDIterator bend()
    {
        return BWDIterator(this, false);
    }

protected:
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }
    void copyFrom(const DLinkedList<T> &list);
    void removeInternalData();
    Node *getPreviousNodeOf(int index);

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    class Node
    {
    public:
        T data;
        Node *next;
        Node *prev;
        friend class DLinkedList<T>;

    public:
        Node(Node *next = 0, Node *prev = 0)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node *next = 0, Node *prev = 0)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    class Iterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        Iterator(DLinkedList<T> *pList = 0, bool begin = true)
        {
            if (begin)
            {
                if (pList != 0)
                    this->pNode = pList->head->next;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->tail;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        Iterator &operator=(const Iterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->prev; // MUST prev, so iterator++ will go to end
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*()
        {
            return pNode->data;
        }
        bool operator!=(const Iterator &iterator)
        {
            return pNode != iterator.pNode;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            //if(pNode == nullptr)
                pNode = pNode->next;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };
    class BWDIterator
    {
    private:
      Node* pNode;
      DLinkedList<T> *pList;
    public:
      BWDIterator(DLinkedList<T> *pList, bool begin)
      {
        if (begin)
            {
                if (pList != 0)
                    this->pNode = pList->tail->prev; 
                else
                    pNode = 0;
            }
            else
            {
              if (pList != 0)
                this->pNode = pList->head;
              else
                pNode = 0;
            }
            this->pList = pList;
      }
      BWDIterator &operator=(const BWDIterator &iterator)
      {
        this->pNode = iterator.pNode;
        this->pList = iterator.pList;
        return *this;
      }
      T &operator*()
      {
        if (pNode == nullptr)
            throw runtime_error("null");
        return pNode->data;
      }
      bool operator!=(const BWDIterator &iterator)
      {
        return pNode != iterator.pNode;
      }
      BWDIterator &operator++()
      {
        if(pNode == nullptr)
        {
            pNode = this->pList->tail;
        }
        else
        {
            pNode = pNode->prev;
        }
        return *this;
      }
      // Postfix -- overload
      BWDIterator operator++(int)
      {
        BWDIterator iterator = *this;
        ++*this;
        return iterator;
      }
      void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->next; // MUST next, so iterator-- will go to begin
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }
    };
};
/////////////////////////////////////////////////////////////////////
// Define a shorter name for DLinkedList:

template <class T>
using List = DLinkedList<T>;

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
DLinkedList<T>::DLinkedList(void (*deleteUserData)(DLinkedList<T> *), bool (*itemEqual)(T &, T &))
{
    // Nhớ tạo trong constructor của xMap có thêm deleteUserData........................
    this->deleteUserData = deleteUserData;
    this->itemEqual = itemEqual;
    this->count = 0;
    this->head = new Node();
    this->tail = new Node();
    /// link 2 node tail and head
    head->next = tail;
    tail->prev = head;
}

template <class T>
void DLinkedList<T>::add(T e)
{
    //cout << "add 1\n";
    Node* newnode = new Node(e, NULL, NULL);
    //cout << "add 2\n";
    Node* temp = tail->prev;
    // insert newnode between temp and tail
    //cout << "add 3\n";
    newnode->next = tail;
    //cout << "add 4\n";
    tail->prev = newnode;

    //cout << "add 5\n";
    temp->next = newnode;
    //cout << "add 6\n";
    newnode->prev = temp;
    count++;
}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list)
{
   
    this->head = new Node();
    this->tail = new Node();
    this->count = 0;
    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;
    head->next = tail;
    tail->prev = head;
    Node *current = list.head->next;
    while (current != list.tail)
    {
        this->add(current->data);
        current = current->next;
    }
}

template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list)
{
    this->head = list.head;
    this->tail = list.tail;
    this->count = list.count;
    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;

    return *this;
}

template <class T>
DLinkedList<T>::~DLinkedList()
{
    if(deleteUserData) deleteUserData(this);

    Node *cur = head->next;
    while (cur != tail)
    {
        Node *t = cur;
        cur = cur->next;
        delete t;
    }
    delete head;
    delete tail;
    count = 0;
}
template <class T>
void DLinkedList<T>::removeInternalData()
{
    /**
     * Clears the internal data of the list by deleting all nodes and user-defined data.
     * If a custom deletion function is provided, it is used to free the user's data stored in the nodes.
     * Traverses and deletes each node between the head and tail to release memory.
     */
    // TODO
    if(deleteUserData) deleteUserData(this);

    Node *cur = head->next;
    while (cur != tail)
    {
        Node *t = cur;
        cur = cur->next;
        delete t;
    }
    head->next = tail;
    tail->prev = head;
    count = 0;
}


template <class T>
void DLinkedList<T>::add(int index, T e)
{
    if(index > count || index < 0) throw out_of_range("Index is out of range!");
    Node* newnode = new Node(e, NULL, NULL);
    if(index == 0 && count == 0)
    {
        head->next = newnode;
        newnode->prev = head;

        newnode->next = tail;
        tail->prev = newnode;
    }
    else
    {
        Node* cur = head;
        for (int i = 0; i < index; ++i)
        {
            cur = cur->next;
        }
        Node *nex = cur->next;
        cur->next = newnode;
        newnode->prev = cur;

        newnode->next = nex;
        nex->prev = newnode;
    }
    count++;
}

template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index)
{
    /**
     * Returns the node preceding the specified index in the doubly linked list.
     * If the index is in the first half of the list, it traverses from the head; otherwise, it traverses from the tail.
     * Efficiently navigates to the node by choosing the shorterif() path based on the index's position.
     */
    checkIndex(index);
    Node *cur = head->next;
    for (int i = 0; i < index && cur != NULL; ++i)
    {
        cur = cur->next;
    }
    return cur;
}

template <class T>
T DLinkedList<T>::removeAt(int index)
{
    checkIndex(index);
    Node* cur = head->next;
    for(int i = 0; i < index && cur != NULL; ++i)
    {
        cur = cur->next;
    }
    Node* p = cur->prev;
    Node* n = cur->next;
    T t = cur->data;
    delete cur;
    p->next = n;
    n->prev = p;
    count--;
    return t;
}

template <class T>
bool DLinkedList<T>::empty()
{
    // TODO
    return (count == 0 || head->next == tail) ? true : false;
}

template <class T>
int DLinkedList<T>::size()
{
    // TODO
    return count;
}

template <class T>
void DLinkedList<T>::clear()
{
    // TODO
    if (head->next == tail) return;
    if (deleteUserData != nullptr)
    {
        deleteUserData(this);
    }

    Node *cur = head->next;
    while (cur != tail)
    {
        Node *t = cur;
        cur = cur->next;
        delete t;
    }
    head->next = tail;
    tail->prev = head;
    count = 0;
}

template <class T>
T &DLinkedList<T>::get(int index)
{
    // TODO
    checkIndex(index);
    Node *cur = head->next;
    for (int i = 0; i < index; ++i)
    {
        cur = cur->next;
    }
    return cur->data;
}

template <class T>
int DLinkedList<T>::indexOf(T item)
{
    Node *cur = head->next;
    for (int i = 0; i < count; ++i)
    {
        if (equals(cur->data, item, *itemEqual))
            return i;
        else
        {
            if(cur->data == item) return i;
        }
        cur = cur->next;
    }
    return -1;
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T))
{
    Node* cur = head->next;
    while (cur != tail)
    {
        if (equals(item, cur->data, itemEqual))
        {
            break;
        }
        else
        {
            if (item == cur->data)
                break;
        }
        cur = cur->next;
    }
    if(cur == tail) return false;
    Node* p = cur->prev;
    Node* n = cur->next;
    p->next = n;
    n->prev = p;
    if(removeItemData) removeItemData(cur->data);
    delete cur;
    count--;
    return true;
}

template <class T>
bool DLinkedList<T>::contains(T item)
{
    Node* cur = head->next;
    while(cur->next != NULL)
    {
        if(equals(cur->data, item, itemEqual)) return true;
        cur = cur->next;
    }
    return false;
}

template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &))
{
    /**
     * Converts the list into a string representation, where each element is formatted using a user-provided function.
     * If no custom function is provided, it directly uses the element's default string representation.
     * Example: If the list contains {1, 2, 3} and the provided function formats integers, calling toString would return "[1, 2, 3]".
     *
     * @param item2str A function that converts an item of type T to a string. If null, default to string conversion of T.
     * @return A string representation of the list with elements separated by commas and enclosed in square brackets.
     */
    ostringstream o;
    Node* cur = head->next;
    o << "[";
    while(cur->next != NULL)
    {
        if(item2str)
        {
            o << item2str(cur->data);
        }
        else
        {
         
                o << cur->data;
          
        }
        cur = cur->next;
        if(cur->next != NULL) o << ", ";
       
    }
    o <<"]";
    return o.str();
}


template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list)
{
    /**
     * Copies the contents of another doubly linked list into this list.
     * Initializes the current list to an empty state and then duplicates all data and pointers from the source list.
     * Iterates through the source list and adds each element, preserving the order of the nodes.
     */

    clear();
    Node* cur = this->head->next;
    while(cur != tail)
    {
        this->add(cur->data);
        cur = cur->next;
    }
    this->head = list.head;
}

template <class T>
void DLinkedList<T>::checkIndex(int index)
{
    // TODO
    if(index >= count || index < 0)
    {
        throw out_of_range("Index is out of range! co thay doi de test");
    }
}

#endif /* DLINKEDLIST_H */
