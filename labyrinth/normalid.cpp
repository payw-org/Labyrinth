#include "normalid.h"

NormalID::NormalID()
{

}

bool operator==(NormalID normal1, NormalID normal2)
{
    if (normal1.getUrl() == normal2.getUrl() && normal1.getID() == normal2.getID())
        return true;
    else return false;
}
