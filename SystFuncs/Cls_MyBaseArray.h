#ifndef MyBaseArrayH
#define MyBaseArrayH


#include <vector>
#include <memory> // ��� std::shared_ptr

class TwwStringList;

class TMyBaseArray {
public:
    std::vector<std::shared_ptr<TwwStringList>> items;


    TMyBaseArray() {}


    void Add(TwwStringList* item) {
        items.emplace_back(item);
    }


    void Insert(int index, TwwStringList* item) {
        if (index >= 0 && index <= Size()) {
            items.insert(items.begin() + index, std::shared_ptr<TwwStringList>(item));
        }
    }


    void Delete(int index) {
        if (index >= 0 && index < Size()) {
            items.erase(items.begin() + index);
        }
    }


    TwwStringList* GetItem(int index) {
        if (index >= 0 && index < Size()) {
            return items[index].get();
        }
        return nullptr;
    }


    void SetItem(int index, TwwStringList* item) {
        if (index >= 0 && index < Size()) {
            if (item == nullptr){
                items[index] = nullptr;
            } else
                items[index] = std::shared_ptr<TwwStringList>(item);
        }
    }


    size_t Size() const {
        return items.size();
    }


    void Clear() {
        items.clear();
    }


    bool IsEmpty() const {
        return items.empty();
    }


    ~TMyBaseArray() {
        Clear();
    }
};
#endif
