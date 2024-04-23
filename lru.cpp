#include <iostream>
#include <unordered_map>

using namespace std;

struct Node
{
    int key;
    int value;
    Node *prev;
    Node *next;

    Node(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

class LRUCache
{
private:
    unordered_map<int, Node *> cacheMap;
    int capacity;
    Node *head;
    Node *tail;

    void addToFront(Node *node)
    {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

    void removeNode(Node *node)
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void evictLRU()
    {
        Node *toRemove = tail->prev;
        cacheMap.erase(toRemove->key);
        removeNode(toRemove);
        delete toRemove;
    }

public:
    LRUCache(int cap) : capacity(cap)
    {
        head = new Node(-1, -1);
        tail = new Node(-1, -1);
        head->next = tail;
        tail->prev = head;
    }

    int get(int key)
    {
        if (cacheMap.find(key) != cacheMap.end())
        {
            Node *node = cacheMap[key];
            removeNode(node);
            addToFront(node);
            return node->value;
        }
        return -1;
    }

    void put(int key, int value)
    {
        if (cacheMap.find(key) != cacheMap.end())
        {
            Node *node = cacheMap[key];
            node->value = value;
            removeNode(node);
            addToFront(node);
        }
        else
        {
            if (cacheMap.size() == capacity)
            {
                evictLRU();
            }
            Node *newNode = new Node(key, value);
            cacheMap[key] = newNode;
            addToFront(newNode);
        }
    }

    int count() const
    {
        return cacheMap.size();
    }
};

int main()
{
    LRUCache cache1(2);
    cache1.put(1, 1);
    cache1.put(2, 2);
    cout << cache1.get(1) << endl; // returns 1
    cache1.put(3, 3);
    cout << cache1.get(2) << endl; // returns -1 (not found)
    cache1.put(4, 4);
    cout << cache1.get(1) << endl; // returns -1 (not found)
    cout << cache1.get(3) << endl; // returns 3
    cout << cache1.get(4) << endl; // returns 4
    cout << "Cache contains " << cache1.count() << " items." << endl;

    LRUCache cache2(3);
    cache2.put(1, 1);
    cache2.put(2, 2);
    cache2.put(3, 3);
    cout << cache2.get(1) << endl; // returns 1
    cache2.put(4, 4);
    cout << cache2.get(2) << endl; // returns -1 (not found)
    cout << "Cache contains " << cache2.count() << " items." << endl;

    LRUCache cache3(1);
    cache3.put(1, 1);
    cout << cache3.get(1) << endl; // returns 1
    cache3.put(2, 2);
    cout << cache3.get(1) << endl; // returns -1 (not found)
    cout << cache3.get(2) << endl; // returns 2
    cout << "Cache contains " << cache3.count() << " items." << endl;

    return 0;
}
