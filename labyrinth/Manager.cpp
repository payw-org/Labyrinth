#include "Manager.h"

Manager::Manager()
{

}

bool Manager::findMasterID(MasterID inputMaster)
{
    if ( std::find(masterList.begin(), masterList.end(), inputMaster) != masterList.end() )
       return true;
    else
       return false;
}

void Manager::addMasterID(MasterID newMaster)
{
    masterList.push_back(newMaster);
    return;
}

int Manager::getIndexOfFoundMasterID(MasterID inputMaster)
{
    int pos = find(masterList.begin(), masterList.end(), inputMaster) - masterList.begin();
    return pos;
}

string Manager::dataToString()//data to string(for the file)
    {

        string data;

        for (int i=0;i<masterList.size();i++)
        {
            for (int j=0;j<masterList.at(i).getNormalID()->size();j++)//for save the data
            {

                data.append(masterList.at(i).getMasterID());
                data.append(":");
                data.append(masterList.at(i).getMasterPW());
                data.append(":");
                data.append(masterList.at(i).getNormalID()->at(j).getUrl());
                data.append(":");
                data.append(masterList.at(i).getNormalID()->at(j).getID());
                data.append(":");
                data.append(masterList.at(i).getNormalID()->at(j).getPW());
                data.append(":");
                data.append(to_string(masterList.at(i).getNormalID()->at(j).getCategory()));
                data.append(":");
                data.append(to_string(masterList.at(i).getNormalID()->at(j).getBookmark()));
                data.append("\n");

            }
        }

        return data;
    }

void Manager::stringToData(string data) //String to data(for the Database)
    {
        string tempData=data;//temp data
        string subData;//sub data
        int start = 0;
        int end;
        int newMID=1;
        int newNID=1;
        int number;
        for (int i=0;i<(int)data.size();i++)
        {
            char delimiterChars =  ':' ;
            end=(int)tempData.find("\n");//find line
            subData= tempData.substr(start,end);//for splitdata
            tempData = tempData.substr(end+1,tempData.size());//except of subData

            vector<string> splitdata = explode(subData, delimiterChars);//split data

            if(end==-1)//no found data
            {
                break;
            }

            for (int j=0;j<masterList.size();j++)
            {
                if (splitdata.at(0).compare(masterList.at(j).getMasterID())==0)
                {
                    newMID = 0;
                    number = j;
                }
            }
            if (newMID==1)
            {
                NormalID nNormalID;
                if (splitdata.at(6) == "0")
                {
                    NormalID newNormalID(splitdata.at(2), splitdata.at(3), splitdata.at(4), atoi(splitdata.at(5).c_str()), false);
                    nNormalID = newNormalID;
                }
                else
                {
                    NormalID newNormalID(splitdata.at(2), splitdata.at(3), splitdata.at(4), atoi(splitdata.at(5).c_str()), true);
                    nNormalID = newNormalID;
                }
                MasterID newMasterID(splitdata.at(0), splitdata.at(1));
                newMasterID.addID(nNormalID);
                addMasterID(newMasterID);
            }
            else
            {
                NormalID nNormalID;
                if (splitdata.at(6) == "0")
                {
                    NormalID newNormalID(splitdata.at(2), splitdata.at(3), splitdata.at(4), atoi(splitdata.at(5).c_str()), false);
                    nNormalID = newNormalID;
                }
                else
                {
                    NormalID newNormalID(splitdata.at(2), splitdata.at(3), splitdata.at(4), atoi(splitdata.at(5).c_str()), true);
                    nNormalID = newNormalID;
                }
                masterList.at(number).addID(nNormalID);
            }

        }

    }

const vector<string> Manager::explode(const string& s, const char& c)
    {
        string buff{ "" };
        vector<string> v;

        for (auto n : s)
        {
            if (n != c) buff += n; else
                if (n == c && buff != "") { v.push_back(buff); buff = ""; }
        }
        if (buff != "") v.push_back(buff);

        return v;
    }
