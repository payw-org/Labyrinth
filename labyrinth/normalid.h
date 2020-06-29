#ifndef NORMALID_H
#define NORMALID_H

#include "string"
#include "vector"
#include "iostream"

using namespace std;

#define PORTAL 1;
#define SNS 2;
#define SHOPPING 3;
#define BANK 4;
#define SCHOOL 5;
#define GAME 6;
#define MUSIC 7;
#define MOVIE 8;

class NormalID
{
private:
    string url;
    string normalID;
    string normalPW;
    int category;
    bool bookmark;

public:
    NormalID();
    NormalID(string ur, string id, string pw, int ct, bool bm)
    {
        url = ur;
        normalID = id;
        normalPW = pw;
        category = ct;
        bookmark = bm;
    }

    string getUrl(){return url;}
    string getID(){return normalID;}
    string getPW(){return normalPW;}
    int getCategory(){return category;}
    bool getBookmark(){return bookmark;}

    void setURL(string ur){url = ur;}
    void setNormalID(string id){normalID = id;}
    void setNormalPW(string pw){normalPW = pw;}
    void setCategory(int ct){category = ct;}
    void setBookmark(bool bm){bookmark = bm;}

    friend bool operator==(NormalID, NormalID);
};

#endif // NORMALID_H
