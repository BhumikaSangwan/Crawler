#include <iostream>
#include <filesystem>
#include "Crawler.h"
#include "LinkedList.h"
#include "Hash.h"
#include "StringFn.h"
#include "File.h"
#include "Node.h"

using namespace std;
namespace fs = filesystem;

Crawler ::Crawler()
{
    url = nullptr;
    targetDir = nullptr;
    depth = 0;
    duplicateCount = 0;
    urlList = nullptr;
    linkCount = 20;
    currLinkCount = 0;
    urlHash = new Hash<char *, char *>;
    keywordHash = new Hash<char *, char *>;
}
Crawler ::Crawler(char *url, char *targetDir, int depth = 5, int linkCount = 20)
{
    this->url = new char[my_strlen(url) + 1];
    my_strcpy(this->url, url);

    this->targetDir = new char[my_strlen(targetDir) + 1];
    my_strcpy(this->targetDir, targetDir);
    this->depth = depth;
    duplicateCount = 0;
    this->linkCount = linkCount;
    currLinkCount = 0;
    urlList = nullptr;
    urlHash = new Hash<char *, char *>;
    keywordHash = new Hash<char *, char *>;
}

Crawler ::Crawler(const Crawler &cr)
{
    this->url = new char[my_strlen(cr.url) + 1];
    my_strcpy(this->url, cr.url);

    this->targetDir = new char[my_strlen(cr.targetDir) + 1];
    my_strcpy(this->targetDir, cr.targetDir);
    this->depth = cr.depth;
    this->duplicateCount = cr.duplicateCount;
    this->linkCount = cr.linkCount;
    this->currLinkCount = cr.currLinkCount;
    this->urlList = cr.urlList;
    this->urlHash = cr.urlHash;
    this->keywordHash = cr.keywordHash;
}

Crawler ::~Crawler()
{
    delete[] url;
    delete[] targetDir;
    delete urlHash;
    delete keywordHash;
    delete urlList;
}

void Crawler ::crawl(char *url, int currDepth = 1)
{
    if (depth == 0 || currDepth >= depth || currLinkCount >= linkCount)
    {
        return;
    }
    if (url == nullptr || targetDir == nullptr || depth == 0)
    {
        cout << "provide the seeds for crawling" << endl;
        return;
    }

    if (!isHtml(url))
    {
        cout << "not a html file : " << url << endl;
        return;
    }

    if (urlHash->search(url) != nullptr)
    {
        cout << "url already crawled : " << url << endl;
        duplicateCount++;
        return;
    }

    char filename[MAX_LEN];
    filename[0] = '\0';
    generateFileName(filename, 12 % (MAX_LEN - 5));
    if (isValidUrl(url) == 0)
    {
        currLinkCount++;
        cout << currLinkCount << " Crawling (depth: " << currDepth << ") : " << " " << url << endl;
        char filepath[MAX_LEN];
        filepath[0] = '\0';
        my_strcat(filepath, targetDir);
        my_strcat(filepath, "/");
        my_strcat(filepath, filename);

        checkDir();
        char cmd[MAX_LEN];
        cmd[0] = '\0';
        my_strcat(cmd, "wget -q ");
        my_strcat(cmd, url);
        my_strcat(cmd, " -O ");
        my_strcat(cmd, filepath);
        system(cmd);
        hashUrl(url, filepath);
        insertUrl(url, filepath);
        extractUrls(filepath, currDepth, url);
    }
    else
    {
        cout << "Invalid URL: " << url << endl;
    }
}

void Crawler ::checkDir()
{
    if (!fs::exists(targetDir))
    {
        if (fs::create_directories(targetDir))
        {
            cout << "created directory : " << targetDir << endl;
        }
        else
        {
            cerr << "failed to create directory : " << targetDir << endl;
        }
    }
}

void Crawler ::generateFileName(char *filename, int len)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    filename[0] = '\0';

    for (int i = 0; i < len; i++)
    {
        int index = rand() % (sizeof(charset) - 1);
        filename[i] = charset[index];
    }
    filename[len] = '\0';
    my_strcat(filename, ".html");
}

int Crawler ::isValidUrl(char *url)
{
    char validityCmd[MAX_LEN];
    validityCmd[0] = '\0';
    my_strcat(validityCmd, "wget --spider -q ");
    my_strcat(validityCmd, url);
    int result = system(validityCmd);
    return result;
}

void Crawler ::extractUrls(char *filepath, int currDepth, char *parentUrl)
{
    char *buffer = readFile(filepath);

    if (buffer == nullptr)
    {
        cout << "File not found" << endl;
        return;
    }
    char *searchPos = buffer;
    char *aTag = nullptr;
    while ((aTag = my_strcasestr(searchPos, "<a ")) != nullptr)
    {
        char *href = my_strcasestr(aTag, "href=");
        if (href == nullptr)
        {
            searchPos = aTag + 4;
            continue;
        }
        char *aEnd = my_strcasestr(aTag, "</a>");
        if (aEnd == nullptr || aEnd < href)
        {
            searchPos = aEnd + 4;
            continue;
        }
        char *quoteStart = my_strstr(href, "\"");
        if (quoteStart == nullptr)
            break;

        char *quoteEnd = my_strstr(quoteStart + 1, "\"");
        if (quoteEnd == nullptr)
            break;

        int len = quoteEnd - quoteStart - 1;
        if (len > MAX_LEN)
            len = MAX_LEN - 1;
        if (len == 1)
        {
            searchPos = quoteEnd + 4;
            continue;
        }
        getUrl(quoteStart, len, currDepth, parentUrl);

        searchPos = quoteEnd + 4;
    }
    delete[] buffer;
}

void Crawler ::getUrl(char *quoteStart, int len, int currDepth, char *parentUrl)
{
    char url[MAX_LEN];
    url[0] = '\0';
    if (quoteStart[1] == '#')
    {
        return;
    }
    size_t i = 0;
    size_t j = 0;
    if (parentUrl != nullptr)
    {
        if (quoteStart[1] == '/')
        {
            j = getDomainLen(parentUrl);
        }
        if (j > MAX_LEN)
        {
            j = MAX_LEN - 1;
        }

        while (i < j)
        {
            url[i] = parentUrl[i];
            i++;
        }
        if (len + i > MAX_LEN)
        {
            len = MAX_LEN - i - 1;
        }
        size_t k = 0;
        while (i < len + j)
        {
            url[i] = quoteStart[k + 1];
            i++;
            k++;
        }
        if (len + j > MAX_LEN)
        {
            url[MAX_LEN - 1] = '\0';
        }
        else
        {
            url[len + j] = '\0';
        }
    }
    else
    {
        while (i < len)
        {
            url[i] = quoteStart[i + 1];
            i++;
        }
        url[i] = '\0';
    }

    if (currDepth < depth)
    {
        crawl(url, currDepth + 1);
    }
}

void Crawler ::insertUrl(char *url, char *filepath)
{

    if (urlList == nullptr)
    {
        urlList = new LinkedList<char *, char *>();
    }
    char *urlCopy = new char[my_strlen(url) + 1];
    my_strcpy(urlCopy, url);
    char *fileCopy = new char[my_strlen(filepath) + 1];
    my_strcpy(fileCopy, filepath);
    urlList->append(urlCopy, fileCopy);
}

void Crawler ::hashUrl(char *url, char *filepath)
{
    char *urlCopy = new char[my_strlen(url) + 1];
    my_strcpy(urlCopy, url);
    char *fileCopy = new char[my_strlen(filepath) + 1];
    my_strcpy(fileCopy, filepath);
    urlHash->insert(urlCopy, fileCopy);
}

void Crawler ::printUrls()
{
    if (urlList == nullptr)
    {
        cout << "No URLs to print." << endl;
        return;
    }
    urlList->print();
}

void Crawler ::printHashedUrls()
{
    urlHash->display();
}

bool Crawler ::isHtml(char *url)
{
    const char *extensions[] = {
        ".pdf", ".doc", ".docx", ".xls", ".xlsx", ".ppt", ".pptx",
        ".zip", ".rar", ".tar", ".gz", ".exe", ".dmg",
        ".jpg", ".jpeg", ".png", ".gif", ".bmp", ".svg", ".webp",
        ".mp3", ".mp4", ".avi", ".mov", ".wmv", ".flv",
        ".css", ".js", ".xml", ".json", ".txt"};
    int extCount = sizeof(extensions) / sizeof(extensions[0]);
    for (int i = 0; i < extCount; i++)
    {
        if (my_strcasestr(url, extensions[i]) != nullptr)
        {
            return false;
        }
    }
    return true;
}

void Crawler ::findKeywords()
{
    LinkedList<char *, char *> *list = urlList;
    Node<char *, char *> *head = list->getHead();
    while (head != nullptr)
    {
        char *keyword = getKeyword(head->data, head->key);
            cout << "Url : " << head->key << " , keyword : " << keyword << endl;
            keywordHash->insertAll(keyword, head->key);
        head = head->next;
    }
    keywordHash->display();
    logKeywords();
}


void Crawler ::logKeywords()
{
    int size = keywordHash->getSize();
    Node<char *, char *> **table = keywordHash->getTable();
    for (int i = 0; i < size; i++)
    {
        Node<char *, char *> *head = table[i];
        if (head == nullptr)
        {
            continue;
        }
        Node<char *, char *> *curr = nullptr;
        Node<char *, char *> *prev = nullptr;
        char *keyword = head->key;
        size_t urlLineLen = 1000;
        size_t currLen = 0;
        char *keywordUrlLine = new char[urlLineLen];
        keywordUrlLine[0] = '\0';

        bool urlPresent = false;

        while (head != nullptr)
        {
            if (curr == nullptr)
            {
                keyword = head->key;
                curr = head;
                if (keywordUrlLine[0] != '\0')
                {
                    appendKeywordLogs(keywordUrlLine);
                }
                keywordUrlLine[0] = '\0';
                my_strcat(keywordUrlLine, keyword);
                my_strcat(keywordUrlLine, " -> ");
                currLen = my_strlen(keywordUrlLine);
                urlPresent = false;
            }
            if (my_strcmp(keyword, curr->key) == 0)
            {
                if (urlPresent)
                {
                    my_strcat(keywordUrlLine, ", ");
                }
                if (currLen + my_strlen(curr->data) + 1 >= urlLineLen)
                {
                    urlLineLen *= 2;
                    char *temp = new char[urlLineLen];
                    my_strcpy(temp, keywordUrlLine);
                    delete[] keywordUrlLine;
                    keywordUrlLine = temp;
                }
                my_strcat(keywordUrlLine, curr->data);
                visitedLog(curr->data);
                urlPresent = true;
                Node<char *, char *> *temp = curr;
                if (curr == head)
                {
                    head = head->next;
                }
                else
                {
                    if (prev != nullptr)
                    {
                        prev->next = curr->next;
                    }
                }
                curr = curr->next;
                delete temp;
            }
            else
            {
                prev = curr;
                curr = curr->next;
            }
        }
        if (keywordUrlLine[0] != '\0')
        {
            appendKeywordLogs(keywordUrlLine);
        }
        delete[] keywordUrlLine;
    }
}

char *Crawler ::getKeyword(char *filepath, char *url)
{
    char *buffer = readFile(filepath);
    char *searchPos = buffer;
    if (searchPos == nullptr)
    {
        cout << "searchPos is null for url : " << url << endl;
        return nullptr;
    }
    char *tempKeyword = nullptr;
    int keywordCount = 0;
    Hash<char *, int> *keywordHash = new Hash<char *, int>;
    while (searchPos != nullptr)
    {
        while (isSpecialChar(*searchPos))
        {
            searchPos++;
        }
        char *word = getWord(searchPos);
        if (word == nullptr)
        {
            searchPos++;
            continue;
        }
        normalizeSpace(word);
        if (word == nullptr || word[0] == '\0' || my_strlen(word) < 2)
        {
            delete[] word;
            continue;
        }
        size_t wordLen = my_strlen(word);
        size_t i = 0;
        size_t j = 0;
        for (i = 0; i <= wordLen; i++)
        {
            if (my_strcmp(word + i, " ") == 0 || word[i] == '\t' || word[i] == '\n')
            {
                j++;
            }
        }
        if (j == wordLen)
        {
            delete[] word;
            continue;
        }
        if (wordLen == 1 && *word == ' ')
        {
            continue;
        }
        my_strLowerCase(word);
        if (isValidWord(word))
        {
            char *wordCopy = new char[wordLen + 1];
            wordCopy[0] = '\0';
            my_strcpy(wordCopy, word);
            int count = keywordHash->insert(wordCopy);
            if (count > keywordCount)
            {
                tempKeyword = word;
                keywordCount = count;
            }
        }
        else
        {
            delete[] word;
        }
        if (*searchPos == '\0')
        {
            break;
        }
    }
    if (tempKeyword == nullptr)
    {
        delete keywordHash;
        return nullptr;
    }
    size_t keywordLen = my_strlen(tempKeyword);
    char *keyword = new char[keywordLen + 1];
    keyword[0] = '\0';
    my_strcpy(keyword, tempKeyword);
    delete[] tempKeyword;
    delete keywordHash;
    return keyword;
}

char *Crawler ::getWord(char *&searchPos)
{
    while (*searchPos != '\0')
    {
        while (isSpecialChar(*searchPos))
        {
            searchPos++;
        }
        char *start = searchPos;
        if (*searchPos == '<')
        {
            while (*searchPos != '\0' && *searchPos != '>')
            {
                if (start != searchPos && *searchPos == '<')
                {
                    searchPos = start + 1;
                    break;
                }
                searchPos++;
            }
            size_t len = searchPos - start;
            char *word = new char[len + 2];
            size_t i = 0;
            while (i <= len)
            {
                word[i] = start[i];
                i++;
            }
            word[len + 1] = '\0';
            if ( my_strcmp(word, "<script>") == 0 || my_strncmp(word, "<script", 7) == 0)
            {
                char *scriptEnd = my_strcasestr(searchPos, "</script>");
                if (scriptEnd != nullptr)
                {
                    searchPos = scriptEnd + 9;
                }
                else
                {
                    searchPos += 8;
                }
                delete[] word;
                continue;
            }
            char *styleAdd = my_strcasestr(searchPos, "<style>");
            if (my_strcmp(word, "<style>") == 0 || my_strncmp(word, "<style", 6) == 0)
            {
                char *styleEnd = my_strcasestr(searchPos, "</style>");
                if (styleEnd != nullptr)
                {
                    searchPos = styleEnd + 8;
                }
                else
                {
                    searchPos += 7;
                }
                delete[] word;
                continue;
            }
            delete[] word;
            if (*searchPos == '>')
                searchPos++;
            else
                break;
            continue;
        }
        else
        {
            char *end = searchPos + 1;
            while (*end != '\0' && *end != '>' && *end != '<' && !isSpecialChar(*end))
            {
                end++;
            }
            searchPos = end;
            size_t len = end - start;
            char *word = new char[len + 1];
            size_t i = 0;
            while (i < len)
            {
                word[i] = start[i];
                i++;
            }
            word[len] = '\0';
            if (len == 0)
            {
                delete[] word;
                return nullptr;
            }
            return word;
        }
    }
    return nullptr;
}

bool Crawler ::isSpecialChar(char ch)
{
    if (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '\"' || ch == '\'' || ch == ' ' || ch == '\n' || ch == '\t' || ch == ';' || ch == ',' || ch == '.' || ch == ':' || ch == '-' || ch == '_' || ch == '/' || ch == '@' || ch == '#' || ch == '&' || ch == '!' || ch == '=' || ch == '?' || ch == '%')
    {
        return true;
    }
    return false;
}

bool Crawler ::isValidWord(char *word)
{
    const char *stopWords[] = {
        "hour",
        "deg",
        "amp",
        "nbsp",
        "a",
        "an",
        "and",
        "are",
        "as",
        "at",
        "be",
        "by",
        "for",
        "from",
        "has",
        "he",
        "in",
        "is",
        "it",
        "its",
        "of",
        "on",
        "that",
        "the",
        "to",
        "was",
        "will",
        "with",
        "would",
        "you",
        "your",
        "have",
        "had",
        "this",
        "these",
        "they",
        "them",
        "their",
        "there",
        "then",
        "than",
        "or",
        "but",
        "not",
        "can",
        "could",
        "should",
        "would",
        "may",
        "might",
        "must",
        "shall",
        "will",
        "do",
        "does",
        "did",
        "done",
        "been",
        "being",
        "about",
        "after",
        "all",
        "also",
        "any",
        "because",
        "before",
        "both",
        "but",
        "each",
        "even",
        "first",
        "her",
        "here",
        "him",
        "his",
        "how",
        "if",
        "into",
        "just",
        "like",
        "make",
        "many",
        "me",
        "more",
        "most",
        "my",
        "new",
        "no",
        "now",
        "only",
        "other",
        "our",
        "out",
        "over",
        "said",
        "same",
        "see",
        "she",
        "so",
        "some",
        "such",
        "take",
        "ist",
        "time",
        "two",
        "up",
        "use",
        "very",
        "way",
        "we",
        "well",
        "were",
        "what",
        "when",
        "where",
        "which",
        "who",
        "why",
        "work",
        "page",
        "home",
        "contact",
        "about",
        "services",
        "products",
        "news",
        "blog",
        "login",
        "register",
        "search",
        "help",
        "support",
        "privacy",
        "terms",
        "policy",
        "www",
        "quot",
        "http",
        "https",
        "com",
        "org",
        "net",
        "edu",
        "gov",
        "io",
        "co",
        "uk",
        "us",
        "info",
        "site",
        "online",
        "web",
        "page",
        "html",
        "www",
        "www.",
        "http://",
        "https://",
    };

    int len = sizeof(stopWords) / sizeof(stopWords[0]);
    for (int i = 0; i < len; i++)
    {
        if (my_strcmp(word, stopWords[i]) == 0)
        {
            return false;
        }
    }
    bool ch = false;
    for (int i = 0; i < my_strlen(word); i++)
    {
        if (!(word[i] >= '0' && word[i] <= '9'))
        {
            ch = true;
            break;
        }
    }
    return ch;
}

size_t Crawler ::getDomainLen(char *url)
{
    size_t len = 0;
    if (my_strncmp(url, "http://", 7) == 0)
    {
        len = 7;
    }
    else if (my_strncmp(url, "https://", 8) == 0)
    {
        len = 8;
    }
    len++;
    while (url[len] != '\0' && url[len] != '/')
    {
        len++;
    }
    return len;
}
