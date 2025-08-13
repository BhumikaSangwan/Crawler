#include <iostream>
#include "SearchEngine.h"
#include "StringFn.h"
#include "File.h"

using namespace std;

SearchEngine::SearchEngine() {}

SearchEngine::~SearchEngine() {}

void SearchEngine ::searchKeyword(char *keyword) {
    cout << "searching keyword : " << keyword << endl;
    char* urls = fetchUrls(keyword);
    if(urls == nullptr) {
        cout << "keyword not found" << endl;
        return;
    }
    cout << "urls fetched : " << urls << endl;
    delete[] urls;
}

bool SearchEngine :: isUrlCrawled(char* url) {
    char* buffer = readFile((char*)(visitedFile));
    if (buffer == nullptr) {
        return false;
    }
    if (my_strcasestr(buffer, url) == nullptr) {
        return false;
    }
    return true;
}