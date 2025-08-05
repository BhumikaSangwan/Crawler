#include <iostream>
#include "Crawler.h"
#include "StringFn.h"

using namespace std; 

int main(int argc, char* argv[]) {
    srand(time(0));
    if(argv[1] == nullptr || argv[2] == nullptr || argv[3] == nullptr) {
        cout << "Enter the url, target directory and depth" << endl;
        return 1;
    }
    Crawler cr1(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]));
    cr1.crawl(argv[1], atoi(argv[3]), 1); // url, depth, currDepth
    cout << "Urls : " << endl;
    cr1.printUrls();

    return 0;
}