#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H

class SearchEngine
{
public:
    SearchEngine();
    ~SearchEngine();

    void searchKeyword(char *keyword);
    bool isUrlCrawled(char *url);
};

#endif