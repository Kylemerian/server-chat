#include <iostream>
#include "base64.hpp"
#include "sha1.hpp"
#include <cstring>

using namespace std;

string encode(string key) {
    string newKey = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    //char * byted = newKey.c_str();
    string result = sha1(newKey);
    unsigned char bytes[result.size()];
    strcpy((char*)bytes, result.c_str());
    string encodedData = base64_encode(bytes, result.size());
    return encodedData;
}

string getLine(string str, int *len) {
    string line = "";
    int count = 0;

    for(char c : str) {
        if (c == '\n' || c == '\0')
            break;
        line += c;
        count++;
    }
    *len += count + 1;
    return line;

}

bool startsWith(const char *first, const char *second) {
    //cout << "\n!" << first << " " << second << "!\n";
    int firstLen = strlen(first);
    //int secondLen = strlen(second);
    if (firstLen < 19)
        return false;
    for (int i = 0; i < 18; i++) {
        //cout << first[i] << second[i] << "\n";
        if (first[i] != second[i]){
            return false;
        }
    }
    return true;
}

string firstWordAfterSpace(const char* str) {
    int len = strlen(str);
    string word = "";
    bool flag  = false;

    for (int i = 0; i < len; i++) {
        if (flag && str[i] != ' ' && str[i] != '\n') {
            word += str[i];
            continue;
        } else {
            if (str[i] == ' ' || str[i] == '\n') {
                break;
            }
        }
        if (str[i] == ' ')
            flag = true;
    }
    return word;
}

string handshake(string in) {
    string line;
    string key = "";
    int count = 0;
    while (true) {
        line = getLine(in.substr(count, in.size()), &count);
        //cout << line << "\n";   
        if (startsWith(line.c_str(), "Sec-WebSocket-Key:")) {
            key = firstWordAfterSpace(line.c_str());
            break;
        }
        //cout << "HRE\n";
    }
    string output = "";
    output += "HTTP/1.1 101 Switching Protocols\n";
    output += "Upgrade: websocket\n";
    output += "Connection: Upgrade\n";
    output += "Sec-WebSocket-Accept: " + encode(key) + '\n';
    return output;
}