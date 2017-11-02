#ifndef FIB_HEAP_H
#define FIB_HEAP_H

#include <algorithm>
#include <cmath>
#include <list>
#include "priority_queue.h"

// OVERVIEW: A specialized version of the 'heap' ADT implemented as a 
//           Fibonacci heap.
template <typename TYPE, typename COMP = std::less<TYPE> >
class fib_heap : public priority_queue<TYPE, COMP> {
public:
    typedef unsigned size_type;

    // EFFECTS: Construct an empty heap with an optional comparison functor.
    //          See test_heap.cpp for more details on functor.
    // MODIFIES: this
    // RUNTIME: O(1)
    fib_heap(COMP comp = COMP());

    // EFFECTS: Add a new element to the heap.
    // MODIFIES: this
    // RUNTIME: O(1)
    virtual void enqueue(const TYPE &val);

    // EFFECTS: Remove and return the smallest element from the heap.
    // REQUIRES: The heap is not empty.
    // MODIFIES: this
    // RUNTIME: Amortized O(log(n))
    virtual TYPE dequeue_min();

    // EFFECTS: Return the smallest element of the heap.
    // REQUIRES: The heap is not empty.
    // RUNTIME: O(1)
    virtual const TYPE &get_min() const;

    // EFFECTS: Get the number of elements in the heap.
    // RUNTIME: O(1)
    virtual size_type size() const;

    // EFFECTS: Return true if the heap is empty.
    // RUNTIME: O(1)
    virtual bool empty() const;

private:
    // Note: compare is a functor object
    COMP compare;

private:
    struct node {
        TYPE key = TYPE();
        node *parent = NULL;

        std::list<node> children_list = NULL;
// Initial intention to implement it with a list later to find not so
// intuitive as directly use pointers.

        size_type degree = 0;
    };
    size_type n = 0; // Number of elements
//    node *min_node;
    std::list<node>::iterator min_node;
//    std::list<node> root_list;
    std::list<node> root_list;

    virtual void consolidate();

    virtual void Fib_Heap_Link(node y, node x);
};

template <typename TYPE, typename COMP>
fib_heap<TYPE, COMP>::fib_heap(COMP comp) {
    n = 0;
    compare = comp;
    min_node = root_list.end();
}

template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::enqueue(const TYPE &val) {
    node *new_node = new node;
    new_node->key = val;
    if (this->min_node == root_list.end()) {
        root_list.push_front(*new_node);
        *this->min_node = *new_node;
    }
    else {
        root_list.push_back(*new_node);
        if (new_node->key < min_node->key) {
            *this->min_node = *new_node;
        }
        this->n++;
    }
}

template <typename TYPE, typename COMP>
TYPE fib_heap<TYPE, COMP>::dequeue_min() {
    node *z = &(*this->min_node);
    if (z != NULL) {
        while (!z->children_list.empty()) {
            root_list.push_back(z->children_list.back());
            z->children_list.pop_back();
        }
        root_list.remove(*z);
        this->n--;
        if (n == 0) this->min_node = root_list.end();
        else consolidate();
    }
    return z;
}

template <typename TYPE, typename COMP>
const TYPE &fib_heap<TYPE, COMP>::get_min() const {
    return min_node;
}

template <typename TYPE, typename COMP>
fib_heap<TYPE, COMP>::size_type fib_heap::size() const {
    return this->n;
}


template <typename TYPE, typename COMP>
bool fib_heap<TYPE, COMP>::empty() const {
    return this->size() == 0;
}

template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::consolidate() {
    size_type size_of_A;
    auto n0 = n;
    double temp = log(n0) / log(1.618);
    size_of_A = static_cast<size_type>(floor(temp)) + 1;
//    std::list<node> *A = new std::list<node>(size_of_A + 1);
    typename std::list<node>::iterator A[size_of_A];
    int i;
    for (i = 0; i <= size_of_A; i++) {
        A[i] = root_list.end();
    }
    std::list<node>::iterator it;
    for (it = root_list.begin(); it != root_list.end(); it++) {
        node x = *it;
        node y;
        int d = x.degree;
//            std::list<node>::iterator A_it;
//            A_it=A->begin()+d;
        while (A[d] != root_list.end()) {
            y.key = (A[d])->key;
            y.degree = (A[d])->degree;
            y.parent = (A[d])->parent;
            y.children_list = (A[d])->children_list;
            if (x.key = y.key) {
                // swap
                std::swap(x, y);
                auto temp = it;
                it = A[d];
                A[d] = temp;
            }
            Fib_Heap_Link(y, x);
            A[d] = root_list.end();
            d++;
        }
        *A[d] = x;
    }
    this->min_node = root_list.end();
    for (it = root_list.begin(); it != root_list.end(); it++) {
        if (A[it] != root_list.end()) {
            if (this->min_node == root_list.end()) {
                root_list.push_back(*A[it]);
                this->min_node = A[it];
            }
            else {
                root_list.push_back(*A[it]);
                if ((*A[it]).key < (*min_node).key) {
                    this->min_node = A[it];
                }
            }
        }
    }
}


template <typename TYPE, typename COMP>
void fib_heap<TYPE, COMP>::Fib_Heap_Link(node y, node x) {
    auto *temp = &y;
    root_list.remove(y);
    x.children_list.push_back(*temp);
    *temp->parent = x;
    x.degree++;
    //y.mark=false
};

// Add the definitions of the member functions here. Please refer to
// binary_heap.h for the syntax.

#endif //FIB_HEAP_H
