////////////////////////////////////////////////////////////////////////////////
// Module Name:  skip_list.h/hpp
// Authors:      Leonid Dworzanski, Sergey Shershakov
// Version:      2.0.0
// Date:         28.10.2018
//
// This is a part of the course "Algorithms and Data Structures"
// provided by  the School of Software Engineering of the Faculty
// of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////

// !!! DO NOT include skip_list.h here, 'cause it leads to circular refs. !!!

#include <cstdlib>
#include "skip_list.h"


//==============================================================================
// class NodeSkipList
//==============================================================================

template <class Value, class Key, int numLevels>
void NodeSkipList<Value, Key, numLevels>::clear(void)
{
    for (int i = 0; i < numLevels; ++i)
        Base::nextJump[i] = 0;

    Base::levelHighest = -1;
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(void)
{
    clear();
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(const Key& tkey)
{
    clear();

    Base::Base::key = tkey;
}

//------------------------------------------------------------------------------

template <class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(const Key& tkey, const Value& val)
{
    clear();

    Base::Base::key = tkey;
    Base::Base::value = val;
}


//==============================================================================
// class SkipList
//==============================================================================

template <class Value, class Key, int numLevels>
SkipList<Value, Key, numLevels>::SkipList(double probability)
{
    _probability = probability;

    // Lets use m_pPreHead as a final sentinel element
    for (int i = 0; i < numLevels; ++i)
        Base::_preHead->nextJump[i] = Base::_preHead;

    Base::_preHead->levelHighest = numLevels - 1;
}

template<class Value, class Key, int numLevels>
void SkipList<Value, Key, numLevels>::insert(const Value &val, const Key &key)
{

    int maxLevel = -1;
    while (rand() % 100 < _probability * 100 && maxLevel < numLevels-1)
        maxLevel++;

    SkipList::Node* current = this->getPreHead();
    Node* prehead = this->getPreHead();
    Node* newObj = new Node(key,val);
    newObj->levelHighest = maxLevel;
    for (int i = numLevels-1; i >= 0; --i)
    {
        while(current->nextJump[i] != prehead && current->nextJump[i]->key < key)
            current = current->nextJump[i];
        //Если текущий уровень поиска <= maxуровню до которого необходимо вставлять элемент, то мы нашли место для вставки
        //a = current.nextJump[i]
        //current.nextJump[i] --> newObj.nextJump[i] --> a.nextJump[i]
        if (i <= maxLevel)
        {
            Node* a = current->nextJump[i];
            current->nextJump[i] = newObj;
            newObj->nextJump[i] = a;
        }
    }
    //Теперь надо вставить элемент на самом "плотном уровне"
    while(current->next != prehead && current->next->key <= key)
        current = current->next;
    Node* a = current->next;
    current->next = newObj;
    newObj->next = a;
}

template<class Value, class Key, int numLevels>
void SkipList<Value, Key, numLevels>::removeNext(SkipList::Node *nodeBefore)
{
    if (!nodeBefore)
        throw std::invalid_argument("Object is'nt node");
    if (nodeBefore->next == this->getPreHead())
        throw std::invalid_argument("Can't delete prehead");

    Node* current = this->getPreHead();
    Node* target = nodeBefore->next;
    Node* prehead  = this->getPreHead();

    for (int i = numLevels-1; i >= 0; --i)
    {
        //ищем предыдущий элемент перед таргетом
        while(current->nextJump[i] != prehead && current->nextJump[i]->key < target->key)
            current = current->nextJump[i];
        Node* hlp = current;

        if (hlp->nextJump[i] != prehead)
        {
            while (hlp->nextJump[i] != prehead && hlp->nextJump[i] != target)
                hlp = hlp->nextJump[i];
        }

        if (hlp->nextJump[i] == target)
        {
            hlp->nextJump[i] = target->nextJump[i];
        }
    }
    nodeBefore->next = target->next;

}
template<class Value, class Key, int numLevels>
typename SkipList<Value, Key, numLevels>::Node*
SkipList<Value, Key, numLevels>::findLastLessThan(const Key &key) const
{
    if (this->getPreHead()->next == this->getPreHead() || this->getPreHead()->next->key > key)
        return this->getPreHead();

    Node* current = this->getPreHead();
    Node* prehead = this->getPreHead();

    for (int i = numLevels - 1; i >= 0 ; --i)
    {
        while(current->nextJump[i] != prehead && current->nextJump[i]->key < key)
            current = current->nextJump[i];
    }
    while (current->next != prehead && current->key == current->next->key)
        current = current->next;
    return current;
}

template<class Value, class Key, int numLevels>
typename SkipList<Value, Key, numLevels>::Node*
SkipList<Value, Key, numLevels>::findFirst(const Key &key) const
{
    NodeSkipList<Value,Key, numLevels>* cur = this->findLastLessThan(key);
    while (cur->next != this->getPreHead() && cur->next->key < key)
        cur = cur->next;
    if (cur->next == this->getPreHead() || cur->next->key != key)
        return nullptr;
    return cur->next;

}

template<class Value, class Key, int numLevels>
SkipList<Value, Key, numLevels>::~SkipList()
{

}



// TODO: !!! One need to implement all declared methods !!!
