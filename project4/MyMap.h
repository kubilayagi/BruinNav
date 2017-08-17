//
//  MyMap.h
//  project4
//
//  Created by Kubilay Agi on 3/8/17.
//  Copyright © 2017 Kubilay Agi. All rights reserved.
//

#ifndef MyMap_h
#define MyMap_h

#include <string>


// Skeleton for the MyMap class template.  You must implement the first six
// member functions.




template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap() {
		root = nullptr;
		sizeOfMap = 0;
	}
	
	~MyMap() {
		clear();		//these do the same thing
	};
		
    void clear() {
        clear(getRoot());		//calls a helper function
		sizeOfMap = 0;
    }
    
    
    int size() const {
        return sizeOfMap;
    }
    
    
	void associate(const KeyType& key, const ValueType& value) {
		if (root == nullptr) {
			root = new node(key, value);
			sizeOfMap++;
			//std::cout << "adding first node" << std::endl;
			return;
		}
		
		if (find(key) != nullptr) {
			ValueType* check = find(key);		//if key already exists in the tree, change the value associate with it
			*check = value;
			//std::cout << "found node" << std::endl;
			//std::cout << *check << std::endl;
			return;
		}
		node* cur = root;
		while (true) {
			if (key < cur->m_key) {
				if (cur->left != nullptr)
					cur = cur->left;
				else {
					cur->left = new node(key, value);
					sizeOfMap++;
					//std::cout << "adding second node" << std::endl;
					return;
				}
			}
			else if (key > cur->m_key) {
				if (cur->right != nullptr)
					cur = cur->right;
				else {
					cur->right = new node(key, value);
					sizeOfMap++;
					//std::cout << "adding third node" << std::endl;
					return;
				}
			}
		}
	}
	
	
    // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const {
		node* ptr = root;
		while (ptr != NULL) {
			if (key == ptr->m_key) return(&(ptr->m_value));
			else if (key < ptr->m_key)
				ptr = ptr->left;
			else
				ptr = ptr->right;
		}
		return(nullptr); // nope
	}
	
	
    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
    }
	
	
    
    // C++11 syntax for preventing copying and assignment
    MyMap(const MyMap&) = delete;
    MyMap& operator=(const MyMap&) = delete;
    
private:
    struct node {
		node(KeyType key, ValueType value) {
			m_key = key;
			m_value = value;
			left = nullptr;
			right = nullptr;
		}
        KeyType m_key;
        ValueType m_value;
        node* left;
        node* right;
    };
    node* root;
    
    node* getRoot() const {return root;};	//helper function
    
    void clear(node* node) {			//helper function
        if (node == nullptr)
            return;
		//std::cout << "deleting node" << std::endl;
        clear(node->left);
        clear(node->right);
        delete node;
    }
	
	
	
    int sizeOfMap;
};






#endif /* MyMap_h */



/*

for keeping track of all of the attractions, you could try using a vector of binary search trees. each segment would be kept within a slot in the array and then that segment would point to a binary tree of the attractions on it
 
 when finding the next street segment to folow, just move straight to the end point of the new segment you are at
 
 as you move to the end of a segment, add to a running total distance for that path (i don't know how this would work with a strack because it has so many different paths that it takes.



*/


