#pragma once
#include <iostream>
#include <vector>
#include <map>

template <typename Iterator>
class Paginator {
public:
    Paginator(Iterator begin, Iterator end, size_t page_size) {
        if (page_size == 0) {
            return;
        }
        for (; begin < end; advance(begin, page_size)) {
            auto range_end = next(begin, page_size);
            if (distance(range_end, end) > 0) {
                pages_.push_back({begin, range_end});
            } else {
                pages_.push_back({begin, end});
            }
        }
    };
    
    auto begin() const {
        return pages_.begin();
    };
    
    auto end() const {
        return pages_.end();
    };
    
private:
    std::vector<std::pair<Iterator, Iterator>> pages_;
};

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, std::pair<Iterator, Iterator> page) {
    using namespace std;
    for (auto result = page.first; result != page.second; ++result) {
        os << "{ document_id = "s << result->id << ", relevance = " 
        << result->relevance << ", rating = " << result->rating << " }"; 
    }
    return os;
}

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}