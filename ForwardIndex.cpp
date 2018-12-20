//Forward indexer for "Enron email dataset
//Group Members
//Muhammmad Shaharyar 
//Anas Imran Tasadduq
//Muhammad Saad Hussaini 
//Ahmad Ali Khan 

//Include All the Required  Libraries 
#include <iostream>
#include <fstream>
#include <regex>//To Find The required format
#include <string>
#include <algorithm>
#include <unordered_map>
#include <boost/range.hpp>
#include <boost/filesystem.hpp>//Boost Library for Fectching files in a given directory.
#include <mysqlx/xdevapi.h> //Connect with mysql server 
#include <ctime>

using namespace std;

//Specifying Sql crendentials for Connection on server
const string Dname = "Search_Engine";//Initialize Database Name
const string hostname= "localhost";
const string username = "root";
const string password = "applepie95";
const unsigned portno = 33060;
int total_duration = 0;
int totalioDuration = 0;

//Specifying subject,body Name
const string subject = "Subject";
const string body = "X-FileName";

//Intiamliazing Map of skippingwords 

unordered_map<string, int> skippingwordsmap()
{
	ifstream file;
	file.open("skippingwords");
	string word;
	int count;
	unordered_map<string, int> skippingwords;
	while (file >> word)
	{
		skippingwords.insert({ word,count });
	}
	file.close();
	return skippingwords;
}

//Forwardindex Function
void ForwardIndex(string path, int limit = -1)
{
	//Database creation and link

	mysqlx:: Session session(hostname, portno, username, password);
	mysqlx::Schema dbp = mysqlx::Schema(session, Dname);
	try
	{
		session.sql("DROP Schema " + Dname).execute();//Drop the previous schema if exists
		dbp = session.getSchema(Dname);
		session.sql("USE " + Dname).execute();
	}
	catch (mysqlx::Error e)
	{
		dbp = session.createSchema(Dname);
		session.sql("USE " + Dname).execute();
		//Create tables 
		session.sql("CREATE TABLE forwardindex(file_id INT PRIMARY KEY AUTO_INCREMENT,file_path TEXT,file_subject TEXT,subject_index LONGTEXT, body_index LONGTEXT);").execute();
		//session.sql("CREATE TABLE words(file_id INT, subject_word LONGTEXT, body_word LONGTEXT, PRIMARY KEY(file_id),FOREIGN KEY (file_id) REFERENCES files(file_id));").execute();
		
	}

	mysqlx::Table findex = dbp.getTable("forwardindex");//Get table forwardfiles
	//mysqlx::Table forwardwords = dbp.getTable("words");//Get table forwardwords
	mysqlx::TableInsert insertfindex = findex.insert("file_id","file_path","file_subject","subject_index","body_index");//Insert sepecified Columns
	//mysqlx::TableInsert insertforwardwords = forwardwords.insert("file_id", "subject_word", "body_word");//Insert Specified Columns

	//Here is file reading 
	ifstream file;
	string line;
	smatch match;
	regex emailexp("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");//Defining valid email Expression 
	regex wordexp("[A-Za-z]+");//Defining valid Word Expression     
	// Initializing unordered map 
	unordered_map<string, int> skippingwords = skippingwordsmap();
	boost::filesystem::recursive_directory_iterator it(path), end;// Using BOOSt library for traversing in to the path
	int count = 1;
	stringstream query;
	query << "INSERT IGNORE INTO words(file_id,word) VALUES";
	time_t start;
	for (boost::filesystem::directory_entry& entry : boost::make_iterator_range(it, end))//Giving the directory to boost filesystem 
	{

		if (boost::filesystem::is_regular(entry.path()))//File Directory Given
		{
			unordered_map<string, int> uniquewords;
			stringstream tempstream;
			string bodywords = "";
			string subjectwords = "";
			string p = entry.path().string();
			file.open(p);

			while (getline(file, line))//Reading lineby line from file 
			{

				if (line.rfind(body, 0) == 0)//Inserting  BodyWords
				{
					string word;
					while (file >> word)
					{
						std::transform(word.begin(), word.end(), word.begin(), ::tolower);//Transform Function Which convert words to lowercase
						if (regex_search(word, match, wordexp))//for Searching Valid Expressions 
						{
							string temp = match.str();

							if (skippingwords.count(temp) == 0)
							{
								if (uniquewords.count(temp) == 0) {
									bodywords += temp + " ";
									uniquewords[temp] = 1;
								}
							}
							
						}
					}
				}
				else if (line.rfind(subject, 0) == 0)//Inserting Subject words
				{
					string word;
					string subject = "";
					
					tempstream << line;
					bool flag = true;
					while (tempstream >> word)
					{
						if (flag)
						{
							flag = false;
							continue;
						}
						subject += word + " ";
						std::transform(word.begin(), word.end(), word.begin(), ::tolower);//Transform Function Which convert words to lowe
						if (regex_search(word, match, wordexp)){//for Searching Valid Expressions
						string temp = match.str();

							if (skippingwords.count(temp) == 0)
							{
								if (uniquewords.count(temp) == 0) {
									bodywords += temp + " ";
									uniquewords[temp] = 1;
								}
							}
						}
					}
				
				}
			}

			insertfindex.values(count,p,tempstream.str(), subjectwords, bodywords);//Inserting subject words,body words
			tempstream.str(string());
			file.close();
			count++;
		}

		if (count == limit + 1)//If limit is reached then break 
		{
			break;
		}
		else if (count == 1)
			start = time(0);
		else if (count % 30000 == 0)//If 5000 limit is reached then insert in to database
		{
			int duration = time(0) - start;
			total_duration += duration;
			cout << "Number of Files Computed: " << count << endl;
			cout << "Batch Time Taken: " << duration << " seconds" << endl;
			cout << "Inserting into database...\n";
			time_t begininsertion = time(0);
			insertfindex.execute();
			time_t endinsertion = time(0);
			//cout << "Inserting Words.\n";
			//insertforwardwords.execute();
			//insertfindex = forwardfiles.insert("path", "subject");
			int ioDuration = endinsertion - begininsertion;
			totalioDuration += ioDuration;
			insertfindex = findex.insert("file_id","file_path", "file_subject","subject_index", "body_index");
			cout << "Inserted into database\n";
			cout << "Insertion time: " << ioDuration << " seconds" << endl;
			start = time(0);
		}
	}
	int duration = time(0) - start;
	total_duration += duration;
	cout << "Number of Files Computed: " << count - 1 << endl;
	cout << " Batch Time Taken: " << duration << " seconds" << endl;
	cout << "Inserting into database...\n";
	time_t begininsertion = time(0);
	insertfindex.execute();
	time_t endinsertion = time(0);
	//insertforwardfiles.execute();
	//insertforwardwords.execute();
	int ioDuration = endinsertion - begininsertion;
	totalioDuration += ioDuration;
	cout << "Inserted into database\n";
	cout << "Insertion time: " << ioDuration << " seconds" << endl;

	session.close();
	
}

int main()
{
	clock_t begin = clock();
	time_t start = time(0);
	ForwardIndex("maildir/");
	time_t end = time(0);
	cout << "Total Time: " << end - start << " seconds" << endl;
	cout << "Computing Time: " << total_duration << " seconds" << endl;
	cout << "Insertion Time: " << totalioDuration << " seconds" << endl;
	cout << double(clock() - begin) / CLOCKS_PER_SEC << std::endl;
	//session.close();

}
