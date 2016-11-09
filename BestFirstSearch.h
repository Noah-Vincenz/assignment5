#ifndef BEST_FIRST_SEARCH_H
#define BEST_FIRST_SEARCH_H

#include "Searchable.h"

#include <memory>
#include <queue>
#include <deque>
using std::deque;

using std::unique_ptr;


class BestFirstSearch {

protected:

    /// Make sure you increment this every time you 'expand' a node, by getting it successors and putting them on the queue        
    int nodes = 0;
    deque<unique_ptr<Searchable> > Q;
//    std::priority_queue<unique_ptr<Searchable>, std::vector<unique_ptr<Searchable> >, ComparatorA> myQ;



public:

    BestFirstSearch(std::unique_ptr<Searchable> && startFrom) {
        // TODO Put startFrom onto the queue:
        myQ.push(std::move(startFrom));

    }

    
    int getNodesExpanded() const {
        return nodes;
    }
    
    Searchable * solve() {
            
        // TODO Implement search, returning a pointer to the solution (if one is found)
        while (!Q.empty()) { //will be 1 at the beginning



            // If the solution on the front of the queue is a solution
            if (Q.front()->isSolution()) {
                return Q.front().get(); // return the pointer
            }


            ++nodes; // we've now looked at one more node, increment our counter

            // Steal the pointer to the board at the front of the queue, by moving it into a unique_ptr here
            // After this, the pointer on the front of the queue is `nullptr`...
            unique_ptr<Searchable> current(std::move(Q.front()));

            // ...which can then be popped off the front of the queue
            Q.pop_front(); //elems are poppped from back / top

            // Get the successors...
            vector<unique_ptr<Searchable>> successors = current->successors();


            for (auto & successor : successors) {
                // and push each one onto the back of queue.  --------------here it needs to be changed; succerssors need to be added in ascending order of their heuristicValue
                myQ.push(std::move(successor));

            }
        }
        /*
            for (auto & successor : successors) {
                // and push each one onto the back of queue.
                bool emplaced = false;
                auto itr = Q.begin();
                unique_ptr<Searchable> tempQ;
                tempQ.reset(*itr);
                //unique_ptr<Searchable> * tempQ = new unique_ptr<Searchable>(*itr);
                for (int i = 0; i < Q.size(); ++i) {
                    if (successor.get()->heuristicValue() <= tempQ.get()->heuristicValue()) {
                        Q.emplace(itr, std::move(successor));
                        emplaced = true;
                    }
                    else {
                        ++itr;
                        tempQ = *itr;
                    }
                }
                if (emplaced == false) {
                    Q.emplace_back(std::move(successor));
                }
            }
        }
*/






        return nullptr;
    }
};
/*
class ComparatorA
{
public:
    bool operator() (const unique_ptr<Searchable> s1, const unique_ptr<Searchable> s2)
    {
        return s1.get()->heuristicValue()<s2.get()->heuristicValue();
    }
};
 */



// Do not edit below this line

#endif
