#include <stdio.h>
#include <iostream>
#include <string.h>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>
// #include <stringstream>

using namespace std;

#define MAX_OBS 92
#define userfiles "userfiles.txt"

struct item
{
	int total;
	int total_acc;
};

typedef struct item item;

struct data
{
	map<string, item> itemset;
};

typedef struct data data;

struct observation	//here, the data needs to be stored is that what problems each user attempted (no. of times) and got accepted (no of times)
{
	map<int, data> user_data;	//for each obs period, it has user with its rank and all its submissions in this
}observations[MAX_OBS];

map<int, data>::iterator it1;
map<string, item>::iterator it2;

int getIndex(int date, int month, int year)
{
	int idx = 0;
	if(year<2009)
		return 0;
	if(year==2012 and month>9)
		return 91;
	idx++;

	idx += (year-2009)*24;
	idx += (month-1)*2;
	if(date>15)
		idx++;
	return idx;
}

int do_something(string signedlistname)	//handles a user's signed list completely
{
	size_t found;
	string time_, pidTemp, pid, resultTemp, result;
	string userRank;

	string username, line;
	int userId;
	int year, month, date;

	found = signedlistname.find("_");
	userRank = signedlistname.substr(found+1);
	found = userRank.find("_");
	userRank = userRank.substr(0, found);

	userId = atoi(userRank.c_str());

	//user_set.clear();

	bool add, att;

	int c=0;
	signedlistname = "./Data/" + signedlistname;
	ifstream signlist(signedlistname.c_str());

	if(signlist.is_open())
	{
		while(signlist.good())
		{
			getline(signlist, line);
			found=line.find("|");
			if (found!=string::npos)
			{
				c++; //removing the first 2 lines
				if(c>2) //means we have useful data now
				{
					line = line.substr(found+1);
					found=line.find("|"); //now this have id before this
					line = line.substr(found+1); //line now starts from date
					found=line.find("|");
					time_ =  line.substr(0,found);
					found=line.find("|");
					line = line.substr(found+1); //line now starts from problem id
					found=line.find("|");
					pidTemp =  line.substr(0,found);
					found=line.find("|");
					line = line.substr(found+1); //line now starts from problem result
					resultTemp =line.substr(0,found);

					pid = "";
					for(size_t i=0; i<pidTemp.length(); i++)
						if(pidTemp.at(i)!=' ')
							pid += pidTemp.at(i);

					//change result to remove spaces
					result = "";
					for(size_t i=0; i<resultTemp.length(); i++)
						if(resultTemp.at(i)!=' ')
							result += resultTemp.at(i);
					
					//cout << date << endl << pid << endl << result << endl;
					//edit here to get date, month and year
					found = time_.find(" ");
					time_ = time_.substr(found+1);
					found = time_.find(" ");
					time_ = time_.substr(0, found);	//now we have data like yyyy-month-date
					found = time_.find("-");
					string yearTemp = time_.substr(0, found);
					year = atoi(yearTemp.c_str());

					found = time_.find("-");
					time_ = time_.substr(found+1);	//now month-date
					found = time_.find("-");
					string monthTemp = time_.substr(0, found);
					month = atoi(monthTemp.c_str());

					found = time_.find("-");
					time_ = time_.substr(found+1);	//now only date
					date = atoi(time_.c_str());

					int idx = getIndex(date, month, year);	//what observation week is going on

					observation obs = observations[idx];	//which obs is being used
					
					map<int, data> obs_data = obs.user_data;
					it1 = obs_data.find(userId);	//which user's data is being updated

					if(it1!=obs_data.end())
					{
						//this users data is somewhat already there in this period, so lets change that only
						data userData = obs_data[userId];
						map<string, item> userItemData = userData.itemset;

						it2 = userItemData.find(pid);
						if(it2!=userItemData.end())	// he already saw this problem
						{
							item subs = userItemData[pid];
							if(strcmp(result.c_str(), "AC")==0)
							{
								subs.total_acc++;
							}
							subs.total++;
							userItemData[pid] = subs;
						}
						else
						{
							item newItem;
							newItem.total=1;
							if (strcmp(result.c_str(), "AC")==0)
							{
								newItem.total_acc=1;
							}
							userItemData[pid] = newItem;
						}
						userData.itemset = userItemData;
						obs_data[userId] = userData;
					}
					else
					{
						//user's first activity in this obs period
						data newData;
						map<string, item> newItemSet;
						item newItem;
						newItem.total=1;
						if (strcmp(result.c_str(), "AC")==0)
						{
							newItem.total_acc=1;
						}
						newItemSet[pid] = newItem;
						newData.itemset = newItemSet;
						obs_data[userId] = newData;
					}
					obs.user_data = obs_data;
					observations[idx] = obs;
				}
			}
		}
	}
	else
		cout <<"cant open file for user " << signlist << " using path " << signedlistname<< endl;
	return userId;
}

string convertInt(int number)
{
    if (number == 0)
        return "0";
    string temp="";
    string returnvalue="";
    while (number>0)
    {
        temp+=number%10+48;
        number/=10;
    }
    for (int i=0;i<temp.length();i++)
        returnvalue+=temp[temp.length()-i-1];
    return returnvalue;
}

int main()
{
	ifstream myfile (userfiles);
	int presentUser, ctr=0;
	string username;
	if (myfile.is_open())
	{
		while ( myfile.good() )
		{
			getline (myfile,username);
			presentUser = do_something(username);
			ctr++;
			/*if(ctr==10)
				break;*/
			if(ctr%100==0)
				cout << "ctr at " << ctr << " for user " << presentUser << endl;
		}
		myfile.close();

		//now print each observation
		string outputFile, str;
		
		for(int i=0; i<MAX_OBS; i++)
		{
			observation obs = observations[i];
			
			str = convertInt(i);
			string folder = "./observations/obs_peroid_";
			outputFile = folder+str;
			ofstream outFile(outputFile.c_str());
			outFile << "UserId \n pid total total_acc pid total total_acc \n";
			map<int, data> matrix = obs.user_data;
			for(it1=matrix.begin(); it1!=matrix.end(); it1++)
			{
				outFile << (*it1).first << endl;
				data itemListStruct = (*it1).second;
				map<string, item> itemList = itemListStruct.itemset;
				for(it2=itemList.begin(); it2!=itemList.end();it2++)
				{
					outFile << (*it2).first<< " " << ((*it2).second).total << " " << ((*it2).second).total_acc<< " ";
				}
				outFile << endl;
			}
		}
	}
	else
		cout << "cant open file userfiles.txt\n"<<endl;
	return 0;
}
