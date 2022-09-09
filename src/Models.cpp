//
//  Models.cpp
//  Breakout
//
//  Created by Austin Horn on 9/6/22.
//  Copyright © 2022 Austin Horn. All rights reserved.
//

#ifndef _MODELS__CPP
#define _MODELS__CPP

#include <iostream>
#include <cmath>
#include <map>
#include <iomanip>
#include <string>
#include <utility>
#include <vector>

///
template <typename T> struct map_init_helper {
    T& m_data;
    map_init_helper(T& data) : m_data(data) {}
    /// Overloaded function call operator for pairing process
    map_init_helper& operator() (typename T::key_type const& key, typename T::mapped_type const& value)
    {
        m_data[key] = value;
        return *this;
    }
};

/// Template function for 'pairing'
template<typename T>
map_init_helper<T> map_init(T& item) {
    return map_init_helper<T>(item);
}








///
template <typename T> class tree {
protected:
    struct node {
        T m_payload;
        struct node *m_left, *m_right;
        node() : m_left(nullptr), m_right(nullptr) { }
    };
    
    struct node *m_root;
    struct node **m_envPtr;
    unsigned int m_nodeCount, m_levelCount;

    //****************************************************************************************
    // Recursive procedure with a public wrapper: void listType::delete()
    // Called by create() and the destructor
    void altDelete(struct node *p)  {
        if (p) {
            altDelete(p->m_left);
            altDelete(p->m_right);
            delete p;
        }
    }
    
    //****************************************************************************************
    // Counts the number of levels in a tree or subtree
    int countLevels(struct node *p) {
        m_levelCount = 0;
        altCountLevels(p, 1);
        return m_levelCount;
    }

    //****************************************************************************************
    // Recursive, called by int countLevels(struct node)
    void altCountLevels(struct node *p, int i) {
        if(p) {
            if(i > m_levelCount)
                m_levelCount = i;

            altCountLevels(p->m_left, i+1);
            altCountLevels(p->m_right, i+1);
        }
            
    }

    //****************************************************************************************
    // Calculates the number of levels in a perfect tree
    int calcLvls() {
        return std::ceil(std::log(m_nodeCount+1)/std::log(2));
    }

    //****************************************************************************************
    /// Rotates tree to the m_left
    void rotL(struct node **dp) {
        struct node *p = *dp;
        *dp = p->m_right;
        p->m_right = (*dp)->m_left;
        (*dp)->m_left = p;
    }

    //****************************************************************************************
    /// Rotates tree to the m_left
    void rotR(struct node **dp) {
        struct node *p = *dp;
        *dp = p->m_left;
        p->m_left = (*dp)->m_right;
        (*dp)->m_right = p;
    }

    //****************************************************************************************
    /// Balance tree
    void balanceTree(struct node **dp) {
        if (!(*dp)) {
            return; // Already balanced
        }
        else {
            balanceTree(&(*dp)->m_left);
            balanceTree(&(*dp)->m_right);
            
            int leftSize = countLevels((*dp)->m_left);
            int rightSize = countLevels((*dp)->m_right);
            
            if (leftSize == rightSize  ||  (leftSize+1) == rightSize  ||  (leftSize-1) == rightSize)
                return;
            else {
                // Node has inside m_left grandchild
                if ((*dp)->m_left  &&  (*dp)->m_left->m_right)
                    rotL( &(*dp)->m_left );
                // Node has inside m_right grandchild
                if ((*dp)->m_right  &&  (*dp)->m_right->m_left)
                    rotR(&(*dp)->m_right);
                
                // Rotate tree to smaller side
                if (leftSize < rightSize)
                    rotL(dp);
                else if (leftSize > rightSize)
                    rotR(dp);
                // Call recursively
                balanceTree(dp);
            }
        }
    }

public:

    ///****************************************************************************************
    /// Constructor
    tree() : m_root(nullptr), m_envPtr(nullptr), m_nodeCount(0) {
        create();
    }
    
    //****************************************************************************************
    // Destructor
    ~tree() {
        altDelete(m_root);
    }

    //****************************************************************************************
    /// Initializes members for new tree
    void create() {
        altDelete(m_root);
        m_root = nullptr;
    }

    //****************************************************************************************
    /// Check if a node already exists in tree
    bool isThere(T &e) {
        m_envPtr = &m_root;
        
        while( *m_envPtr  &&  (*m_envPtr)->m_payload != e )
            if((*m_envPtr)->m_payload > e)
                m_envPtr=&(*m_envPtr)->m_left;
            else
                m_envPtr=&(*m_envPtr)->m_right;

        return *m_envPtr;
    }

    //****************************************************************************************
    /// Insert node into tree
    void insert(T e) {
        if(!isThere(e)) {
            *m_envPtr = new struct node;
            (*m_envPtr)->m_payload=e;
            m_nodeCount++;
        }
    }

    //****************************************************************************************
    /// Remove node from tree
    void remove(T &e) {
        /// Set envPtr
        if (isThere(e)) {
            /// Node does NOT have 2 children
            if (!(*m_envPtr)->m_left || !(*m_envPtr)->m_right) {
                if ( (*m_envPtr)->m_left ) {
                    struct node *p = *m_envPtr;
                    p = *this->m_envPtr;
                    *this->m_envPtr = (*this->m_envPtr)->m_left;
                    delete p;
                }
                else {
                    struct node *p = *m_envPtr;
                    p = *this->m_envPtr;
                    *this->m_envPtr = (*this->m_envPtr)->m_right;
                    delete p;
                }
            }
            /// Node DOES have 2 children
            else {
                struct node **dp = m_envPtr;
                dp = &(*m_envPtr)->m_left;
                
                while ((*dp)->m_right)
                    dp = &(*dp)->m_right;
                
                struct node *p = *m_envPtr;
                struct node *q = *dp;
                
                *dp = q->m_left;
                q->m_left = p->m_left;
                q->m_right = p->m_right;
                *m_envPtr = q;
                delete p;
                
                m_nodeCount--;
            }
        }
    }
    
    //****************************************************************************************
    // Executes balance routine
    void balance() {
        balanceTree(&m_root);
    }

    //****************************************************************************************
    ///
    void showTreeStats() {
        std::cout<<"\n\nm_nodeCount = "<<m_nodeCount<<"\tcalcLvls = "<<calcLvls()<<"\tcntLvls = "<<countLevels(m_root)<<std::endl;
    }
};


























#endif /* _MODELS__CPP */
