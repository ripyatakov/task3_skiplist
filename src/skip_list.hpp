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
    int maxLevel = GetMaxLevel(this->_probability,numLevels-1);
    SkipList::Node* current = this->getPreHead();
    Node* prehead = this->getPreHead();
    Node* newObj = new Node(key,val);
    newObj->levelHighest = maxLevel;
    for (int i = numLevels-1; i > 0; --i)
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
    while(current->next != prehead && current->next->key < key)
        current = current->next;
    Node* a = current->next;
    current->next = newObj;
    newObj->next = a;
}

template<class Value, class Key, int numLevels>
void SkipList<Value, Key, numLevels>::removeNext(SkipList::Node *nodeBefore) {
    
}

//Возвращаем максимальный уровень для элемента, который хотим вставить
int GetMaxLevel(double p, int maxlvl)
{
    int answ = -1;
    while (rand() % 100 < p * 100 && answ < maxlvl)
        answ++;
    return answ;
}

// TODO: !!! One need to implement all declared methods !!!
