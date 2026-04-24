#ifndef BEST_FIRST_SEARCH_H
#define BEST_FIRST_SEARCH_H

#include "Searchable.h"

#include <memory>
#include <queue>
#include <vector>
#include <functional>

using std::unique_ptr;

// min-heap comparator: lower heuristicValue has higher priority
struct SearchableComparator {
    bool operator()(const unique_ptr<Searchable> & a,
                    const unique_ptr<Searchable> & b) const {
        return a->heuristicValue() > b->heuristicValue();
    }
};

class BestFirstSearch {

protected:

    int nodes = 0;

    std::priority_queue<unique_ptr<Searchable>,
                        std::vector<unique_ptr<Searchable>>,
                        SearchableComparator> Q;

public:

    BestFirstSearch(std::unique_ptr<Searchable> && startFrom) {
        Q.push(std::move(startFrom));
    }

    int getNodesExpanded() const {
        return nodes;
    }

    Searchable * solve() {
        while (!Q.empty()) {
            // priority_queue::top() returns const&, so const_cast is needed to move
            // the element out. this is safe because pop() is called immediately after,
            // discarding the moved-from shell before the queue reheapifies
            unique_ptr<Searchable> current(
                std::move(const_cast<unique_ptr<Searchable> &>(Q.top()))
            );
            Q.pop();

            if (current->isSolution()) {
                // push back so the caller can access the solution pointer
                Searchable * result = current.get();
                Q.push(std::move(current));
                return result;
            }

            ++nodes;

            vector<unique_ptr<Searchable>> successors = current->successors();

            for (auto & successor : successors) {
                Q.push(std::move(successor));
            }
        }

        return nullptr;
    }
};

// Do not edit below this line

#endif
