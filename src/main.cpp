#include "BencodeParser.h"
#include <dirent.h>
#include <string.h>
#include <iostream>

using namespace std;

#define TORRENT_PATH "./torrent/"

int main()
{
    char real_path[256];
    BencodeParser *parser;
    DIR* dir;
    struct dirent *ptr;
    dir = opendir(TORRENT_PATH);
    while((ptr = readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name, "..") == 0 || strcmp(ptr->d_name, ".") == 0)
            continue;
        size_t len1 = strlen(TORRENT_PATH);
        strncpy(real_path, TORRENT_PATH, len1);
        size_t len2 = strlen(ptr->d_name);
        strncpy(real_path + len1, ptr->d_name, len2);
        real_path[len1+len2] = 0;
        cout << ptr->d_name << endl;
        parser = new BencodeParser(real_path);
        //ret = parser->findFiles();
        //if(ret == -1)
        //{
        //    cout << "error find files." << endl;
        //}
        shared_ptr<Element> e = parser->parse();
        cout << *e <<endl;
        cout << endl;
        delete parser;
    }
    closedir(dir);
    return 0;
}

