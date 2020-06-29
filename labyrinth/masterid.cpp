#include "masterid.h"

MasterID::MasterID()
{

}

MasterID::MasterID(string id, string pw)
{
    this->masterID=id;
    this->masterPW=pw;
}

bool operator==(MasterID master1, MasterID master2)
{
    if (master1.getMasterID() == master2.getMasterID() && master1.getMasterPW() == master2.getMasterPW())
        return true;
    else return false;
}

void MasterID::addID(NormalID normal)
{
    normalList.push_back(normal);
    return;
}

void MasterID::deleteID(NormalID normal)
{
    std::vector<NormalID>::iterator it;
    it = find(normalList.begin(), normalList.end(), normal);
    normalList.erase(it);
}

int MasterID::getIndexOfFoundNormalID(NormalID inputNormal)
{
    int pos = find(normalList.begin(), normalList.end(), inputNormal) - normalList.begin();

    return pos;
}
