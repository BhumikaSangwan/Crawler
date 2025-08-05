#include <iostream>
#include <fstream>

using namespace std;

char* readFile(char* filepath) {
    ifstream file(filepath, ios::in | ios::binary);
    if (!file.is_open()) {
        cout << "File not found" << endl;
        return nullptr;
    }
    file.seekg(0, ios::end);
    size_t len = file.tellg();
    file.seekg(0, ios::beg);
    char* buffer = new char[len + 1];
    file.read(buffer, len);
    buffer[len] = '\0';
    file.close();

    return buffer;
}