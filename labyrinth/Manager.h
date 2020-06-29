#ifndef MANAGER_H
#define MANAGER_H

#include "QDataStream"
#include "MasterID.h"
#include "NormalID.h"
#include "vector"

class Manager
{
private:
    vector<MasterID> masterList;

public:
    Manager();
    vector<MasterID>* getMasterVector(){return &masterList;}
    void addMasterID(MasterID);
    bool findMasterID(MasterID inputMaster);
    int getIndexOfFoundMasterID(MasterID inputMaster);
    string dataToString();
    void stringToData(string data);
    const vector<string> explode(const string& s, const char& c);
};

#endif // MANAGER_H
