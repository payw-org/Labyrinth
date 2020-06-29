#ifndef MASTERID_H
#define MASTERID_H

#include "NormalID.h"

class MasterID
{
private:
    string masterID;
    string masterPW;
    vector<NormalID> normalList;

public:
    MasterID();
    MasterID(string, string);

    string getMasterID(){return masterID;}
    string getMasterPW(){return masterPW;}
    vector<NormalID>* getNormalID(){return &normalList;}
    int getIndexOfFoundNormalID(NormalID inputNormal);

    void addID(NormalID);
    void deleteID(NormalID);
    void checkBookmark(NormalID);
    void uncheckBookmark(NormalID);
    void changeCategory(NormalID, int);

    friend bool operator==(MasterID, MasterID);
};

#endif // MASTERID_H
