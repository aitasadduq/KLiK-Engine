//Reverse indexer for "Enron email dataset
//Group Members
//Ahmad Ali Khan
//Anas Imran Tasadduq
//Muhammad Saad Hussaini
//Muhammmad Shaharyar 

//Include All the Required  Librarie
#include <mysqlx/xdevapi.h>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iostream>
#include <time.h>//Time calculations
using namespace std;


//Specifying Sql crendentials for Connection on server
const string hostname = "localhost";
const unsigned portno = 33060;
const string username = "root";
const string password = "applepie95";
const string Dname = "Search_Engine";


void ReverseIndex(int limit = -1)//Reverse Index Function

{	//mysql session object creation
	mysqlx::Session session(hostname, portno, username, password);
	mysqlx::Schema database = session.getSchema(Dname);
	mysqlx::Table findextable = database.getTable("forwardindex");
	mysqlx::RowResult reverseindex = findextable.select("subject_index", "body_index").execute();
	int count = 1;
	unordered_map<string, pair<stringstream, stringstream>> rowmap;// Initailizing Maps of string and stringstreams for maping words 
	stringstream subject;
	stringstream body;
	session.sql("USE " + Dname).execute();//Using schema
	//Create tabel 
	session.sql("CREATE TABLE reverseindex(word VARCHAR(200), subject_files LONGTEXT, body_files LONGTEXT, PRIMARY KEY(word));").execute();
	for (mysqlx::Row row : reverseindex)//Iterate through reverseindex tables row by row
	{
		if (count == limit)
		{
			break;
		}
		else if (count % 50000 == 0)//If 10000 limit is reached then insert in to database
		{
			string query = "INSERT INTO reverseindex(word, subject_files, body_files) VALUES";//Inserting words,subject,body files
			for (auto &insertrow : rowmap)
			{
				query += "('" + insertrow.first + "','" + insertrow.second.first.str() + "','" + insertrow.second.second.str() + "'),";
				
			}
			query[query.length() - 1] = ' ';
			query += "ON DUPLICATE KEY UPDATE subject_files = CONCAT(subject_files,VALUES(subject_files),' ') AND body_files = CONCAT(body_files,VALUES(body_files),' ');";
			session.sql(query).execute();
			rowmap.clear();
		}
		
		cout << "File: " << count << endl;
		subject << row[0];
		body << row[1];
		string value;
	    
		while (body >> value)//For body words
		{
			rowmap[value.substr(0,value.find(','))].second << count << "," << value.substr(value.find(',')+1) << " ";
		}
		while (subject >> value) //For subject words
		{
			rowmap[value.substr(0,value.find(','))].first << count << "," << value.substr(value.find(',')+1) << " ";
			//rowmap[value].first << count << " ";
		}
		subject.clear();
		subject.str(string());
		body.clear();
		body.str(string());
		count++;
	}
	string query = "INSERT INTO reverseindex(word, subject_files, body_files) VALUES";//Inserting 
	for (auto &insertrow : rowmap)
	{
		query += "('" + insertrow.first + "','" + insertrow.second.first.str() + "','" + insertrow.second.second.str() + "'),";
		
	}
	query[query.length() - 1] = ' ';
	query += "ON DUPLICATE KEY UPDATE subject_files = CONCAT(subject_files,VALUES(subject_files),' ') AND body_files = CONCAT(body_files,VALUES(body_files),' ');";
	session.sql(query).execute();
	session.close();
}

int main()
{
	clock_t start = clock();
	ReverseIndex();
	cout << double(clock() - start) / CLOCKS_PER_SEC << endl;
}