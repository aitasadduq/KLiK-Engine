
//Reverse indexer for "Enron email dataset
//Group Members
//Muhammmad Shaharyar 
//Anas Imran Tasadduq
//Muhammad Saad Hussaini 
//Ahmad Ali Khan 

//Include All the Required  Librarie
#include <mysqlx/xdevapi.h>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <time.h>//Time calculations
using namespace std;


//Specifying Sql crendentials for Connection on server
const string hostname = "localhost";
const unsigned portno = 33060;
const string username = "root";
const string password = "Hunter123@";
const string Dname = "Search_Engine";
const int email_num = 517401;
 int total_duration = 0;
 int totalioDuration = 0;


void ReverseIndex(int limit = -1)//Reverse Index Function

{	//mysql session object creation
	mysqlx::Session session(hostname, portno, username, password);
	mysqlx::Schema database = session.getSchema(Dname);
	mysqlx::Table wordsTable = database.getTable("forwardindex");
	
	mysqlx::RowResult reverseindex = wordsTable.select("document_index").execute();
	
	int count = 1;
	unordered_map<string, stringstream> rowmap;// Initailizing Maps of string and stringstreams for maping words 
	unordered_map<string, double> doc_freq;
	stringstream document;
	
	session.sql("USE " + Dname).execute();//Using schema
	//Create tabel 
	session.sql("CREATE TABLE reverseindex(word VARCHAR(200), document_files LONGTEXT, idf VARCHAR(20), PRIMARY KEY(word));").execute();
	
	time_t start;
	for (mysqlx::Row row : reverseindex)//Iterate through reverseindex tables row by row
	{
		if (count == limit)
		{
			break;
		}
		else if (count == 1) {
			start = time(0);
		}
		else if (count % 1000== 0)//If 10000 limit is reached then insert in to database
		{
			int duration = time(0) - start;
			total_duration += duration;
			cout << "Number of Files Computed: " << count << endl;
			cout << "Batch Time Taken: " << duration << " seconds" << endl;
			cout << "Inserting into database...\n";
			time_t begininsertion = time(0);
			string query = "INSERT INTO reverseindex(word, document_files) VALUES";//Inserting words,subject,body files
			for (auto &insertrow : rowmap)
			{
				query += "('" + insertrow.first + "','" + insertrow.second.str() +  "'),";
			}
			query[query.length() - 1] = ' ';
			query += "ON DUPLICATE KEY UPDATE document_files = CONCAT(document_files,VALUES(document_files),' ');";
			session.sql(query).execute();
			time_t endinsertion = time(0);
			int ioDuration = endinsertion - begininsertion;
			totalioDuration += ioDuration;
			cout << "Inserted into database\n";
			cout << "Insertion time: " << ioDuration << " seconds" << endl;
			rowmap.clear();
		}
		
		
		//cout << "File: " << count << endl;
		document << row[0];
		string value;
	    
		while (document >> value)//For body words
		{
			rowmap[value.substr(0,value.find(','))] << count << "," << value.substr(value.find(',') + 1) << " ";
			doc_freq[value.substr(0,value.find(','))] += 1;
		}
		
		document.clear();
		document.str(string());
		
		count++;
	}
	
	int duration = time(0) - start;
	total_duration += duration;
	cout << "Number of Files Computed: " << count << endl;
	cout << "Batch Time Taken: " << duration << " seconds" << endl;
	string query = "INSERT INTO reverseindex(word, document_files) VALUES";//Inserting 
	for (auto &insertrow : rowmap)
	{
		query += "('" + insertrow.first + "','" + insertrow.second.str() +  "'),";
		
	}
	
	query[query.length() - 1] = ' ';
	query += "ON DUPLICATE KEY UPDATE document_files = CONCAT(document_files,VALUES(document_files),' ');";
	time_t begininsertion = time(0);
	session.sql(query).execute();
	time_t endinsertion = time(0);
	int ioDuration = endinsertion - begininsertion;
	totalioDuration += ioDuration;
	cout << "Inserted into database\n";
	cout << "Insertion time: " << ioDuration << " seconds" << endl;


	time_t idfstart = time(0);
	start = time(0);
	for (auto &df : doc_freq) {
		df.second = log2(email_num / df.second);
	}
	query = "INSERT INTO reverseindex(word, idf) VALUES";
	for (auto &df : doc_freq){
		query += "('" + df.first + "','" + to_string(df.second) + "'),";
	}
	query[query.length() - 1] = ' ';
	query += "ON DUPLICATE KEY UPDATE idf = VALUES(idf);";
	time_t idfend = time(0);
	int idfcalc = idfend - idfstart;
	cout << "IDF calculation time: " << idfcalc << endl;
	totalioDuration += idfcalc;
	time_t beginidfinsertion = time(0);
	session.sql(query).execute();
	time_t endidfinsertion = time(0);
	int idfioDuration = endidfinsertion - beginidfinsertion;
	totalioDuration += ioDuration;
	cout << " idf Inserted into database\n";
	cout << "idf Insertion time: " << ioDuration << " seconds" << endl;



	session.close();
}

int main()
{
	clock_t begin  = clock();
	time_t start = time(0);
	ReverseIndex();
	time_t end = time(0);
	cout << "Total Time: " << end - start << " seconds" << endl;
	cout << "Total Computing Time: " << total_duration << " seconds" << endl;
	cout << "Total Insertion Time: " << totalioDuration << " seconds" << endl;
	
	system("pause");
}
