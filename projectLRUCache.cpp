#include <iostream>
#include <unordered_map> //
#include <mutex>         //
#include <shared_mutex>  //
#include <vector>        //
#include <thread>        //
#include <math.h>        //
#include <atomic>        // 

// Class representing the LRU Cache
class LRUCache {
private:
    mutable std::shared_mutex mtx;  // 

public:
    class Node{
        public:
        int key;
        int value;
        Node* prev;
        Node* next;

        Node(int akey, int avalue){
            key = akey;
            value = avalue;
        }
    };    

    int cap;
    std::unordered_map<int, Node*> mpp; //
    Node* head = new Node(-1,-1);
    Node* tail = new Node(-1,-1);

    // Constructor to initialize LRU cache
    LRUCache(int capacity) {
        cap = capacity;
        mpp.clear();
        head->next = tail;
        tail->prev = head;
    }

    void deleteNode(Node* node){
        Node* agla = node->next;
        Node* pichchla = node->prev;
        pichchla->next = agla;
        agla->prev = pichchla;
    }

    void addNodeAfterHead(Node* node){
        Node* agla = head->next;
        head->next = node;
        node->next = agla;
        node->prev = head;
        agla->prev = node;
    }

    // Function to get value from cache
    int get(int key_) {
        std::unique_lock<std::shared_mutex> lock(mtx); //
        if(mpp.find(key_) == mpp.end()) return -1;
        Node* node = mpp[key_];
        deleteNode(node);
        addNodeAfterHead(node);
        return node->value;
    }

    // Function to put key-value into cache
    void put(int key_, int value) {
        std::unique_lock<std::shared_mutex> lock(mtx); //
        if(mpp.find(key_) != mpp.end()){
            Node* node = mpp[key_];
            node->value = value;
            deleteNode(node);
            addNodeAfterHead(node);
        } else {
            if(mpp.size() == cap){
                Node* node = tail->prev;
                mpp.erase(node->key);
                deleteNode(node);
            }
            Node* newNode = new Node(key_, value);
            mpp[key_] = newNode;
            addNodeAfterHead(newNode);
        }
    }
};

std::mutex coutMtx;
void safePrint(const std::string& msg) { // For clear output in terminal
        std::lock_guard<std::mutex> lock(coutMtx);
        std::cout << msg << std::endl;
}

// Driver code
int main() {
    // Create cache with capacity 100
    LRUCache cache(10000);            //

    std::vector<std::thread> readers; //
    std::vector<std::thread> writers; //

    for (int i=1; i<=5; i++){
        writers.emplace_back([&cache, i](){
            for (int j=1; j<=100; j++){
                cache.put(i+j,i+j);
            }
        });
    }

    for(auto& t : writers){
        t.join();
    }

    std::atomic<int> cnt{1};                      //
    for (int i=1; i<=50; i++){
        readers.emplace_back([&cache, i, &cnt](){ // Took atomic by reference
            for (int j=1; j<=1000; j++){
                safePrint(std::to_string(cnt) + " " + std::to_string(cache.get(rand()%500+1)));
                cnt++;
            }
        });
    }

    for(auto& t : readers){
        t.join();
    }

    return 0;
}
