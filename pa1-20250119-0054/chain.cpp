#include "chain.h"
#include <cmath>
#include <iostream>


// PA1 functions
// Complete all of the missing implementation
// and submit this file for grading.

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain() {
	/* your code here */
    //IN PROGRESS
    Clear(); // I think clear is just supposed to do the same thing? idk
    /*
    if ((head_ != NULL) && (length_ != 0)) {
        Node* curr = head_;
        while (curr != NULL) {
            Node* temp = curr->next;
            delete curr;
            curr = temp;
            length_--;
        }
    }
    head_ = NULL;
    length_ = 0;
    */
}

/**
 * Inserts a new node after the node pointed to by p in the
 * chain (so p->next is the new node) and returns a pointer to
 * the newly created node.
 * If p is NULL, inserts a new head node to the chain.
 * This function **SHOULD** create a new Node and increase length_.
 *
 * @param p = The new node should be pointed to by p->next.
 *            If p is NULL, the new node becomes the head of the chain.
 * @param ndata = The data to be inserted.
 */
Chain::Node * Chain::InsertAfter(Node * p, const Block &ndata) {
	/* your code here */
    //IN PROGRESS
    Node* newNode = new Node(ndata);
    if (p == NULL) {
        newNode->next = head_;
        newNode->prev = NULL;
        if (head_ != NULL) {
            head_->prev = newNode;
        }
        head_ = newNode;
    } else {
        newNode->next = p->next;
        newNode->prev = p;
        if (p->next != NULL) {
            p->next->prev = newNode;
        }
        p->next = newNode;
    }
    length_ += 1;
	return newNode;
}

/**
 * Swaps the position in the chain of the two nodes pointed to
 * by p and q.
 * If p or q is NULL or p==q, do nothing.
 * Change the chain's head pointer if necessary.
 */
void Chain::Swap(Node *p, Node *q) {
	/* your code here */
    //IN PROGRESS
    if (p == NULL || q == NULL || p == q) return;

    if (p->next == q) {
        p->next = q->next;
        q->prev = p->prev;
        if (q->next) q->next->prev = p;
        if (p->prev) p->prev->next = q;
        q->next = p;
        p->prev = q;
    } else if (q->next == p) {
        Swap(q, p);
        return;
    } else {
        Node* pPrev = p->prev;
        Node* pNext = p->next;
        Node* qPrev = q->prev;
        Node* qNext = q->next;

        if (pPrev) pPrev->next = q;
        if (pNext) pNext->prev = q;
        if (qPrev) qPrev->next = p;
        if (qNext) qNext->prev = p;

        p->prev = qPrev;
        p->next = qNext;
        q->prev = pPrev;
        q->next = pNext;
    }

    if (head_ == p) head_ = q;
    else if (head_ == q) head_ = p;
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::Clear() {
	/* your code here */
    if ((head_ != NULL) && (length_ != 0)) {
        Node* curr = head_;
        while (curr != NULL) {
            Node* temp = curr->next;
            delete curr;
            curr = temp;
        }
    }
    head_ = NULL;
    length_ = 0;
}

/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::Copy(Chain const &other) {
	/* your code here */
	// TODO
    Clear(); 
    

    if (other.head_ == NULL) {
        head_ = NULL;
        length_ = 0;
        return;
    }

    Node* otherCurr = other.head_;
    Node* prevNew = NULL;

    while (otherCurr != NULL) {
        Node* newNode = new Node(otherCurr->data);
        if (prevNew == NULL) {
            head_ = newNode;
        } else {
            prevNew->next = newNode;
            newNode->prev = prevNew;
        }
        prevNew = newNode;
        otherCurr = otherCurr->next;
        length_++;
    }
}

/**
 * Takes the current chain and renders it into a
 * correctly sized PNG. The blocks in the chain
 * are placed in the image left to right in order
 * of their occurrence in the chain.
 */
PNG Chain::Render() {
    /* your code here */
    //probably makes a new PNG object
    //assuming the chain has anything in it
    //  PNG high is the same as block height
    //  PNG width is head_=>data width * Size()
    //runs through each link in thses code similar to 
    //Build() to write pixels in the right spots
    if (head_ == NULL) return PNG();

    unsigned W = head_->data.Width();
    unsigned H = head_->data.Height();
    PNG out(W * length_, H);

    Node* curr = head_;
    unsigned ix = 0;
    while (curr != NULL) {
        curr->data.Render(out, ix * W);
        curr = curr->next;
        ix++;
    }
    return out;
}

/* Modifies the current chain:
 * 1) Find the node with the first (leftmost) block in the unscrambled
 *    image and move it to the head of the chain.
 *	This block is the one whose closest match (to the left) is the
 *	largest.  That is, the distance (using distanceTo) to this block
 *	is big for all other blocks.
 *	For each block B, find the distanceTo B from every other block
 *	and take the minimum of these distances as B's "value".
 *	Choose the block B with the maximum value over all blocks and
 *	swap its node to the head of the chain.
 *
 * 2) Starting with the (just found) first block B, find the node with
 *    the block that is the closest match to follow B (to the right)
 *    among the remaining blocks, move (swap) it to follow B's node,
 *    then repeat to unscramble the chain/image.
 */
void Chain::Unscramble() {
	/* your code here */
    //IN PROGRESS
    if (head_ == NULL || head_->next == NULL) return;

    //might need helper function
    Node* curr = head_;
    double largestDist = -1;
    //represents the leftmost node for the image
    Node* firstNode = head_;
    while (curr != NULL) {
        //replace 27 with call to helper when it works
        double largestDiff = greatestDistanceFor(curr);
        if (largestDiff > largestDist) {
            largestDist = largestDiff;
            firstNode = curr;
        }
        curr = curr->next;
    }
    if (head_ != firstNode) {
        Swap(head_, firstNode);
    }

    sortRest(head_);
    //sortRest helper function
}


/**************************************************
* IF YOU HAVE DECLARED PRIVATE FUNCTIONS IN       *
* chain-private.h, COMPLETE THEIR IMPLEMENTATIONS *
* HERE                                            *
**************************************************/

//finds the greatest distance between the given node's left edge and the right edge of every other node
double Chain::greatestDistanceFor(Node *d) {
    double minDist = INFINITY;
    Block leftBlock = d->data;
    Node* curr = head_;
    while (curr != NULL) {
        if (curr != d) {
            double dist = curr->data.DistanceTo(leftBlock);
            if (dist < minDist) {
                minDist = dist;
            }
        }
        curr = curr->next;
    }
    return minDist;
}

//Starting with the given node, find the node with
//the block that is the closest match to follow it (to the right)
//among the remaining blocks, move (swap) it to follow its node.
//Loops through the chain to unscramble the list/image.
//Assumed to always start at the head which is always the intended 
//leftmost block of the image
void Chain::sortRest(Node* start) {
    Node* curr = start;
    while (curr != NULL) {
        //starts at infinity so the first distance measured is always smaller
        double smallestDist = INFINITY;
        Block leftBlock = curr->data;
        Node* currNext = curr->next;
        Node* nextNode = NULL;
        //finds the smallest difference node from the rest of the chain
        while (currNext != NULL) {
            Block rightBlock = currNext->data;
            if (leftBlock.DistanceTo(rightBlock) < smallestDist) {
                smallestDist = leftBlock.DistanceTo(rightBlock);   
                nextNode = currNext;
            }
        currNext = currNext->next;
        }
        //makes sure the next node is the one with the smallest distance
        if (nextNode != curr->next) {
            Swap(nextNode, curr->next);
        }
        curr = curr->next;
    }
} 