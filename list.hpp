#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class list {
protected:
    class node {
    public:
        T data;
        node *prev;
        node *next;

        // Default constructor
        node() : prev(nullptr), next(nullptr) {}

        // Constructor with data
        node(const T &value) : data(value), prev(nullptr), next(nullptr) {}

        // Constructor with data and pointers
        node(const T &value, node *p, node *n) : data(value), prev(p), next(n) {}

    };

protected:
    node *head;
    node *tail;
    size_t list_size;


    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node *insert(node *pos, node *cur) {
        if (pos == head) {
            cur->next = head;
            cur->prev = nullptr;
            if (head) {
                head->prev = cur;
            }
            head = cur;
            if (!tail) {
                tail = cur;
            }
        } else {
            cur->next = pos;
            cur->prev = pos->prev;
            pos->prev->next = cur;
            pos->prev = cur;
        }
        list_size++;
        return cur;
    }

    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
    node *erase(node *pos) {
        if (pos == head) {
            head = pos->next;
            if (head) {
                head->prev = nullptr;
            } else {
                tail = nullptr;
            }
        } else if (pos == tail) {
            tail = pos->prev;
            tail->next = nullptr;
        } else {
            pos->prev->next = pos->next;
            pos->next->prev = pos->prev;
        }
        list_size--;
        return pos;
    }

public:
    class const_iterator;
    class iterator {
    private:
        node *ptr;

    public:
        iterator() : ptr(nullptr) {}
        iterator(node *p) : ptr(p) {}

        /**
         * iter++
         */
        iterator operator++(int) {
            iterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }
        /**
         * ++iter
         */
        iterator & operator++() {
            ptr = ptr->next;
            return *this;
        }
        /**
         * iter--
         */
        iterator operator--(int) {
            iterator tmp = *this;
            ptr = ptr->prev;
            return tmp;
        }
        /**
         * --iter
         */
        iterator & operator--() {
            ptr = ptr->prev;
            return *this;
        }
        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (!ptr) {
                throw sjtu::invalid_iterator();
            }
            return ptr->data;
        }
        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (!ptr) {
                throw sjtu::invalid_iterator();
            }
            return &(ptr->data);
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        // Allow const_iterator to access private members
        friend class const_iterator;
    };
    /**
     * TODO
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
    class const_iterator {
    private:
        const node *ptr;

    public:
        const_iterator() : ptr(nullptr) {}
        const_iterator(const node *p) : ptr(p) {}
        const_iterator(const iterator &it) : ptr(it.ptr) {}

        /**
         * iter++
         */
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }
        /**
         * ++iter
         */
        const_iterator & operator++() {
            ptr = ptr->next;
            return *this;
        }
        /**
         * iter--
         */
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            ptr = ptr->prev;
            return tmp;
        }
        /**
         * --iter
         */
        const_iterator & operator--() {
            ptr = ptr->prev;
            return *this;
        }
        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        const T & operator *() const {
            if (!ptr) {
                throw sjtu::invalid_iterator();
            }
            return ptr->data;
        }
        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        const T * operator ->() const {
            if (!ptr) {
                throw sjtu::invalid_iterator();
            }
            return &(ptr->data);
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
    };
    /**
     * TODO Constructs
     * Atleast two: default constructor, copy constructor
     */
    list() : head(nullptr), tail(nullptr), list_size(0) {}
    
    list(const list &other) : head(nullptr), tail(nullptr), list_size(0) {
        // Copy elements from other list
        for (const auto &item : other) {
            push_back(item);
        }
    }
    
    /**
     * TODO Destructor
     */
    virtual ~list() {
        clear();
    }
    
    /**
     * TODO Assignment operator
     */
    list &operator=(const list &other) {
        if (this != &other) {
            clear();
            for (const auto &item : other) {
                push_back(item);
            }
        }
        return *this;
    }
    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {
        if (empty()) {
            throw sjtu::container_is_empty();
        }
        return head->data;
    }
    
    const T & back() const {
        if (empty()) {
            throw sjtu::container_is_empty();
        }
        return tail->data;
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(head);
    }
    
    const_iterator cbegin() const {
        return const_iterator(head);
    }
    
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(nullptr);
    }
    
    const_iterator cend() const {
        return const_iterator(nullptr);
    }
    /**
     * checks whether the container is empty.
     */
    virtual bool empty() const {
        return list_size == 0;
    }
    
    /**
     * returns the number of elements
     */
    virtual size_t size() const {
        return list_size;
    }

    /**
     * clears the contents
     */
    virtual void clear() {
        while (head) {
            node *tmp = head;
            head = head->next;
            delete tmp;
        }
        tail = nullptr;
        list_size = 0;
    }
    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    virtual iterator insert(iterator pos, const T &value) {
        if (pos.ptr == nullptr) {
            // Insert at the end
            if (empty()) {
                node *new_node = new node(value);
                head = tail = new_node;
                list_size = 1;
                return iterator(new_node);
            } else {
                node *new_node = new node(value, tail, nullptr);
                tail->next = new_node;
                tail = new_node;
                list_size++;
                return iterator(new_node);
            }
        } else {
            // Insert before pos
            node *new_node = new node(value);
            if (pos.ptr == head) {
                new_node->next = head;
                head->prev = new_node;
                head = new_node;
                list_size++;
                return iterator(new_node);
            } else {
                new_node->next = pos.ptr;
                new_node->prev = pos.ptr->prev;
                pos.ptr->prev->next = new_node;
                pos.ptr->prev = new_node;
                list_size++;
                return iterator(new_node);
            }
        }
    }
    
    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
    virtual iterator erase(iterator pos) {
        if (empty() || pos.ptr == nullptr) {
            throw sjtu::invalid_iterator();
        }
        
        node *next_node = pos.ptr->next;
        node *removed = erase(pos.ptr);
        delete removed;
        
        if (next_node) {
            return iterator(next_node);
        } else {
            return end();
        }
    }
    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        if (empty()) {
            node *new_node = new node(value);
            head = tail = new_node;
            list_size = 1;
        } else {
            node *new_node = new node(value, tail, nullptr);
            tail->next = new_node;
            tail = new_node;
            list_size++;
        }
    }
    
    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (empty()) {
            throw sjtu::container_is_empty();
        }
        
        if (head == tail) {
            // Only one element
            delete head;
            head = tail = nullptr;
            list_size = 0;
        } else {
            node *old_tail = tail;
            tail = tail->prev;
            tail->next = nullptr;
            delete old_tail;
            list_size--;
        }
    }
    
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        if (empty()) {
            node *new_node = new node(value);
            head = tail = new_node;
            list_size = 1;
        } else {
            node *new_node = new node(value, nullptr, head);
            head->prev = new_node;
            head = new_node;
            list_size++;
        }
    }
    
    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (empty()) {
            throw sjtu::container_is_empty();
        }
        
        if (head == tail) {
            // Only one element
            delete head;
            head = tail = nullptr;
            list_size = 0;
        } else {
            node *old_head = head;
            head = head->next;
            head->prev = nullptr;
            delete old_head;
            list_size--;
        }
    }
    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {
        if (empty() || list_size == 1) {
            return;
        }
        
        // Create a temporary array to hold the elements
        T *arr = new T[list_size];
        
        // Copy elements from list to array
        node *current = head;
        for (size_t i = 0; i < list_size; i++) {
            arr[i] = current->data;
            current = current->next;
        }
        
        // Sort the array using the provided sjtu::sort function
        sjtu::sort(arr, arr + list_size, std::less<T>());
        
        // Copy elements back to the list
        current = head;
        for (size_t i = 0; i < list_size; i++) {
            current->data = arr[i];
            current = current->next;
        }
        
        // Clean up
        delete[] arr;
    }
    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {
        // If other list is empty, nothing to merge
        if (other.empty()) {
            return;
        }
        
        // If this list is empty, just transfer all nodes from other
        if (empty()) {
            head = other.head;
            tail = other.tail;
            list_size = other.list_size;
            other.head = other.tail = nullptr;
            other.list_size = 0;
            return;
        }
        
        // Both lists are non-empty and sorted
        node *this_ptr = head;
        node *other_ptr = other.head;
        node *prev = nullptr;
        
        // Merge the two lists by rearranging pointers
        while (this_ptr && other_ptr) {
            if (other_ptr->data < this_ptr->data) {
                // Take node from other list
                node *next_other = other_ptr->next;
                
                if (prev) {
                    prev->next = other_ptr;
                    other_ptr->prev = prev;
                } else {
                    // This is the new head
                    head = other_ptr;
                    other_ptr->prev = nullptr;
                }
                
                prev = other_ptr;
                other_ptr = next_other;
                list_size++;
                other.list_size--;
            } else {
                // Keep node from this list
                prev = this_ptr;
                this_ptr = this_ptr->next;
            }
        }
        
        // If there are remaining nodes in other list, append them
        if (other_ptr) {
            prev->next = other_ptr;
            other_ptr->prev = prev;
            // Update tail to the last node of other list
            while (other_ptr->next) {
                other_ptr = other_ptr->next;
            }
            tail = other_ptr;
            list_size += other.list_size;
            other.list_size = 0;
        }
        
        // Clear other list
        other.head = other.tail = nullptr;
    }
    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {
        if (empty() || list_size == 1) {
            return;
        }
        
        node *current = head;
        node *temp = nullptr;
        
        // Swap next and prev pointers for all nodes
        while (current) {
            temp = current->prev;
            current->prev = current->next;
            current->next = temp;
            current = current->next; // Move to next node (which is now in prev)
        }
        
        // Swap head and tail
        temp = head;
        head = tail;
        tail = temp;
    }
    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {
        if (empty() || list_size == 1) {
            return;
        }
        
        node *current = head;
        
        while (current && current->next) {
            if (current->data == current->next->data) {
                node *duplicate = current->next;
                current->next = duplicate->next;
                
                if (duplicate->next) {
                    duplicate->next->prev = current;
                } else {
                    // If duplicate is the tail, update tail
                    tail = current;
                }
                
                delete duplicate;
                list_size--;
            } else {
                current = current->next;
            }
        }
    }
};

}

#endif //SJTU_LIST_HPP
