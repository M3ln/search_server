#pragma once
#include <deque>
#include <vector>
#include <string>
#include "search_server.h"
#include "document.h"

class RequestQueue {
public:
    explicit RequestQueue(SearchServer& search_server);
    
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate);
    
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query);
    int GetNoResultRequests() const;
    
private:
    struct QueryResult {
        std::string raw_query;
        int documents_num = 0;
    };
    
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    SearchServer& search_server_;
    int empty_results_num_ = 0;
};

template <typename DocumentPredicate>
std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
    using namespace std;
    if (requests_.size() == min_in_day_) {
        auto& front_request = requests_.front();
        if (front_request.documents_num == 0) {
            --empty_results_num_;
        }
        requests_.pop_front();
    }
        
    const vector<Document> documents = search_server_.FindTopDocuments(raw_query, document_predicate);
    int documents_num = documents.size();
    requests_.push_back({raw_query, documents_num});
        
    if (documents_num == 0) {
        ++empty_results_num_;
    }
        
    return documents;
}