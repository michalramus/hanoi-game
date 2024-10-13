#pragma once

template<class T>
class List;

template<class T>
struct Node {
    friend class List<T>;

    Node();

    void remove();

    T item;

private:
    Node *previousNode;
    Node *nextNode;
};

template<class T>
class List {
public:
    List();

    ~List();

    void pushBack(T item);

    void pushFront(T item);

    void push(T item, int index);

    void pop(int index);

    void popBack();

    void popFront();

    /**
     * @brief
     *
     * @tparam T
     * @param index if index is -1 then return the last Node
     * @return T&
     */
    Node<T> *getNode(int index);

    /**
     * @brief Get the Next Node object
     * error when trying to access next node, passing a deleted node
     * @param node
     * @return Node<T>*
     */
    static Node<T> *getNextNode(Node<T> *node);

    /**
     * @brief Get the Previous Node object
     *  error when trying to access node, passing a deleted node
     * @param node
     * @return Node<T>*
     */
    static Node<T> *getPreviousNode(Node<T> *node);

private:
    Node<T> *firstNode;
    Node<T> *lastNode;

    /**
     * @brief
     *
     * @param index if -1 then returned last element
     * @return Node<T>* nullptr if not found;
     */
    Node<T> *findNode(int index);
};

template<class T>
Node<T>::Node() {
    this->previousNode = nullptr;
    this->nextNode = nullptr;
}

template<class T>
void Node<T>::remove() {
    if (nextNode != nullptr) {
        nextNode->remove();
    }

    delete this;
}

template<class T>
List<T>::List() {
    this->firstNode = nullptr;
    this->lastNode = nullptr;
}

template<class T>
List<T>::~List() {
    delete firstNode;
}

template<class T>
void List<T>::pushBack(T item) {
    auto *newNode = new Node<T>();
    newNode->item = item;

    if (this->firstNode == nullptr) // List is empty
    {
        this->firstNode = newNode;
        this->lastNode = newNode;
    } else {
        newNode->previousNode = lastNode;
        lastNode->nextNode = newNode;
        lastNode = newNode;
    }
}

template<class T>
void List<T>::pushFront(T item) {
    auto *newNode = new Node<T>();
    newNode->item = item;

    if (firstNode == nullptr) // List is empty
    {
        this->firstNode = newNode;
        this->lastNode = newNode;
    } else {
        newNode->nextNode = firstNode;
        firstNode->previousNode = newNode;
        firstNode = newNode;
    }
}

template<class T>
void List<T>::push(T item, int index) {
    if (index < 0) throw std::out_of_range("Index cannot be negative\n");

    if (index == 0) {
        pushFront(item);
        return;
    }

    Node<T> *current = findNode(index - 1);
    if (current == nullptr) throw std::out_of_range("Index out of range\n");

    auto *newNode = new Node<T>();
    newNode->item = item;

    // update pointers in item 0
    newNode->nextItem = current->nextItem;
    newNode->previousItem = current;

    // update pointers in item 1
    if (current->nextItem != nullptr) {
        current->nextItem->previousItem = newNode;
    }

    // update pointers in item -1
    current->nextItem = newNode;

    if (newNode->nextItem == nullptr) {
        lastNode = newNode;
    }
}

template<class T>
void List<T>::pop(int index) {
    if (firstNode == nullptr) {
        std::cout << "List is empty";
        return;
    }

    Node<T> *target = findNode(index);
    if (target == nullptr) {
        std::cout << "Index out of range\n";
        return;
    }

    if (target->previousNode != nullptr)
        target->previousNode->nextNode = target->nextNode;
    else
        firstNode = target->nextNode;

    if (target->nextNode != nullptr)
        target->nextNode->previousNode = target->previousNode;
    else
        lastNode = target->previousNode;

    delete target;
}

template<class T>
void List<T>::popBack() {
    auto last = lastNode;

    if (lastNode == nullptr) {
        std::cout << "List is empty";
        return;
    }

    this->lastNode = this->lastNode->previousNode;
    this->lastNode->nextNode = nullptr;
    delete last;
}

template<class T>
void List<T>::popFront() {
    pop(0);
}

template<class T>
Node<T> *List<T>::getNode(int index) {
    Node<T> *target = findNode(index);
    return target;
}

template<class T>
Node<T> *List<T>::getNextNode(Node<T> *node) {
    if (node->nextNode == nullptr) return nullptr;
    return node->nextNode;
}

template<class T>
Node<T> *List<T>::getPreviousNode(Node<T> *node) {
    if (node->previousNode == nullptr) return nullptr;
    return node->previousNode;
}

template<class T>
Node<T> *List<T>::findNode(int index) {
    if (index == -1) {
        return this->lastNode;
    }

    if (index < 0) {
        return nullptr;
    }

    Node<T> *current = firstNode;
    for (int i = 0; i < index; ++i) {
        if (current == nullptr) return nullptr;
        current = current->nextNode;
    }

    if (current == nullptr) return nullptr;

    return current;
}
