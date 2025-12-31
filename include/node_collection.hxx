//
// Created by mateuszl on 31.12.2025.
//

#include <list> // std::list was chosen because it does not invalidate pointers
#include <algorithm>
#include "types.hxx"
#include "nodes.hxx"

#ifndef NODE_COLLECTION_HXX
#define NODE_COLLECTION_HXX

template <typename Node>
class NodeCollection {
public:
    using container_t = std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    iterator begin() { return collection_.begin(); }
    iterator end() { return collection_.end(); }

    const_iterator begin() const { return collection_.begin(); }
    const_iterator end() const { return collection_.end(); }

    const_iterator cbegin() const { return collection_.cbegin(); }
    const_iterator cend() const { return collection_.cend(); }

    void add(Node&& node) {
        collection_.push_back(std::move(node));
    }

    iterator find_by_id(ElementID id) {
        return std::find_if(
            collection_.begin(),
            collection_.end(),
            [id](const Node& node) {
                return node.get_id() == id;
            }
        );
    }

    const_iterator find_by_id(ElementID id) const {
        return std::find_if(
            collection_.cbegin(),
            collection_.cend(),
            [id](const Node& node) {
                return node.get_id() == id;
            }
        );
    }

    void remove_by_id(ElementID id) {
        auto it = find_by_id(id);
        if (it != collection_.end()) {
            collection_.erase(it);
        }
    }

private:
    container_t collection_;
};


#endif //NODE_COLLECTION_HXX
