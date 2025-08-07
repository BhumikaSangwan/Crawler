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
}
Crawler ::Crawler(char *url, char *targetDir, int depth = 5, int linkCount = 20)
{
    this->url = url;
    this->targetDir = targetDir;
    this->depth = depth;
    duplicateCount = 0;
    this->linkCount = linkCount;
    currLinkCount = 0;
    urlList = nullptr;
    urlHash = new Hash<char *, char *>;
}

Crawler ::Crawler(const Crawler &cr)
{
    this->url = cr.url;
    this->targetDir = cr.targetDir;
    this->depth = cr.depth;
    this->duplicateCount = cr.duplicateCount;
    this->linkCount = cr.linkCount;
    this->currLinkCount = cr.currLinkCount;
    this->urlList = cr.urlList;
    this->urlHash = cr.urlHash;
}

Crawler ::~Crawler()
{
    delete url;
    delete targetDir;
    delete urlList;
    delete urlHash;
}

void Crawler ::crawl(char *url, int currDepth = 1)
{
    if (depth == 0 || currDepth > depth || currLinkCount >= linkCount)
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
    generateFileName(filename, 12 % 95);

    if (isValidUrl(url) == 0)
    {
        cout << currLinkCount << " " << url << endl;
        currLinkCount++;
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
}

void Crawler ::checkDir()
{
    if (!fs::exists(targetDir))
    {
        cout << "directory doesn't exist : " << targetDir << endl;
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
    char *ahref = nullptr;
    while ((ahref = my_strcasestr(searchPos, "<a href=\"")) != nullptr)
    {
        char *quoteStart = my_strstr(ahref, "\"");
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
            searchPos = quoteEnd + 1;
            continue;
        }
        getUrl(quoteStart, len, currDepth, parentUrl);

        searchPos = quoteEnd + 1;
    }
    delete buffer;
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
    if (parentUrl != nullptr && quoteStart[1] == '/')
    {
        j = my_strlen(parentUrl);

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
        if (url[i - 1] == '/')
        {
            if (quoteStart[1] == '/')
            {
                j--;
                k++;
            }
        }
        while (i < len + j)
        {
            url[i] = quoteStart[k + 1];
            i++;
            k++;
        }
        url[len + j] = '\0';
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

    if (currDepth < depth - 1)
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
    urlList->print();
}

void Crawler ::printHashedUrls()
{
    urlHash->display();
}

bool Crawler ::isHtml(char *url)
{
    const char *extensions[] = {".jpeg", ".jpg", ".png", ".img", ".svg", ".mp4", ".mp3", ".css", ".js", ".gif"};
    for (int i = 0; i < 10; i++)
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
        head = head->next;
    }
}

char *Crawler ::getKeyword(char *filepath, char *url)
{
    char *buffer = readFile(filepath);
    char *searchPos = buffer;
    char *tempKeyword;
    int keywordCount = 0;
    Hash<char *, int> *keywordHash = new Hash<char *, int>;
    while (searchPos != nullptr)
    {
        while (*searchPos == ' ' || *searchPos == '\n' || *searchPos == '\t' ||
               *searchPos == ';' || *searchPos == ',' || *searchPos == '.' ||
               *searchPos == ':' || *searchPos == '|')
        {
            searchPos++;
        }
        char *word = getWord(searchPos);
        if (word == nullptr)
        {
            break;
        }
        normalizeSpace(word);
        if (word == nullptr || word[0] == '\0' || my_strlen(word) == 0)
        {
            delete[] word;
            continue;
        }
        size_t wordLen = my_strlen(word);
        size_t i = 0;
        size_t j = 0;
        for(i = 0; i <= wordLen; i++) {
            if (my_strcmp(word + i, " ") == 0 || word[i] == '\t' || word[i] == '\n') {
                j++;
            }
        }
        if (j == wordLen) {
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
    // keywordHash->display();
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
        while (*searchPos == ' ' || *searchPos == '\n' || *searchPos == '\t' ||
               *searchPos == ';' || *searchPos == ',' || *searchPos == '.' ||
               *searchPos == ':' || *searchPos == '|' || *searchPos == '{' || *searchPos == '}')
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
            char *word = new char[len + 1];
            size_t i = 0;
            while (i < len)
            {
                word[i] = start[i];
                i++;
            }
            word[len] = '\0';
            if (my_strcmp(word, "<script>") == 0)
            {
                char *newStart = my_strcasestr(searchPos, "</script>");
                if (newStart != nullptr)
                {
                    searchPos = newStart + 9;
                }
                delete[] word;
                continue;
            }
            if (my_strcmp(word, "<style>") == 0)
            {
                char *newStart = my_strcasestr(searchPos, "</style>");
                if (newStart != nullptr)
                {
                    searchPos = newStart + 8;
                }
                delete[] word;
                continue;
            }
            if (*searchPos == '>')
                searchPos++;
            else
                break;
            continue;
        }
        else
        {
            char *end = searchPos + 1;
            while (*end != '\0' && *end != ' ' && *end != '\n' && *end != '\t' && *end != ';' && *end != ',' && *end != '.' && *end != ':' && *end != '<')
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


bool Crawler ::isValidWord(char *word)
{
    const char *stopwords[] = {
        // Articles
        "the", "a", "an",

        // Coordinating conjunctions
        "and", "but", "or", "nor", "for", "so", "yet",

        // Subordinating conjunctions
        "although", "because", "since", "unless", "while", "whereas", "if", "when", "after", "before", "until",

        // Prepositions
        "in", "on", "at", "by", "with", "about", "against", "among", "around", "as", "before", "behind",
        "beneath", "beside", "between", "beyond", "during", "except", "for", "from", "inside", "into",
        "near", "of", "off", "out", "over", "through", "to", "toward", "under", "up", "upon", "within", "without",

        // Auxiliary (helping) verbs
        "is", "am", "are", "was", "were", "be", "being", "been", "do", "does", "did",
        "have", "has", "had", "will", "would", "shall", "should", "can", "could", "may", "might", "must",

        // Pronouns (common)
        "i", "you", "he", "she", "it", "we", "they",
        "me", "him", "her", "us", "them",
        "my", "your", "his", "her", "its", "our", "their",
        "mine", "yours", "hers", "ours", "theirs",

        // Determiners / Others
        "this", "that", "these", "those", "some", "any", "each", "every", "few", "many", "much", "all", "none"};

    int len = sizeof(stopwords) / sizeof(stopwords[0]);
    for (int i = 0; i < len; i++)
    {
        if (my_strcmp(word, stopwords[i]) == 0)
        {
            return false;
        }
    }
    return true;
}
