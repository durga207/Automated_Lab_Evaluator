#ifndef __MySQLClient__
#define __MySQLClient__


/*
*	Class that will bw used to communicate with
*	MySQL server
*/

//Basic libraries
#include <bits/stdc++.h>
#include <stdexcept>
#include <ctime>

//Libraries of Connetion/C++
#include "mysql_connection.h"
#include <boost/scoped_ptr.hpp>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;

class MySQLInteract{
private:
	string HOST, USER_NAME, PASSWORD, DATABASE, TABLE, PORT;
	string USER;

public:
	static const int IS_ADMIN; 	  //USE THIS if type of user is ADMIN
	static const int IS_TEACHER;  //USE THIS if type of user is TEACHER
	static const int IS_STUDENT;  //USE THIS if type of user is STUDENT
	sql::Driver *driver;
	sql::Connection *con;

	MySQLInteract(string host, string port, string user, string pwd, string db){
		HOST = host;
		USER_NAME = user;
		PASSWORD = pwd;
		DATABASE = db;
		PORT = port;
				
		PORT = "3306"; //always same. Change if required.

		string hostInfo = "tcp://" + HOST + ":" + PORT;
		


		//creating connection.
		//cout<<"Creating connection to server......."<<endl;
		driver = get_driver_instance();
		con = driver->connect(hostInfo,USER_NAME,PASSWORD);


		// if(con->isValid()){
		// 	//cout<<"Connetction to server " + HOST + " was successfully created"<<endl;
		// }
		// else{
		// 	//cout<<"Connetction to server " + HOST + " could not be created"<<endl;
		// 	//cout<<"Terminating programme..."<<endl<<"Terminated"<<endl;
		// 	exit(1);
		// }


		con->setSchema(DATABASE);

	}

	
	MySQLInteract(const MySQLInteract &connector, string type){
		this->HOST = connector.getHost();
		this->USER_NAME = connector.getUserName();
		this->PASSWORD = connector.getPassword();
		this->DATABASE = type;
		this->PORT = connector.getPort();

		this->con = connector.con;
		//int maxAttempt = 20;
		// while(!this->con->isValid() && maxAttempt-->0){
		// 	this->con->reconnect();
		// }	
		// if(maxAttempt==0){
		// 	//cout<<"Connetction to server " + HOST + " could not be created"<<endl;
		// 	//cout<<"Terminating programme..."<<endl<<"Terminated"<<endl;
		// 	exit(1);
		// }

		con->setSchema(DATABASE);


	}


	bool setUser(string user_name);

	string getUser() const;

	string getHost() const;

	string getPassword() const;

	string getPort() const;

	string getUserName() const;

	~MySQLInteract();
};


const int MySQLInteract::IS_ADMIN = 0;
const int MySQLInteract::IS_TEACHER = 1;
const int MySQLInteract::IS_STUDENT = 2;


// When DATABASE == LOGIN, and when login info needs to accessed, follwing methods are utilised.
class Admin : public MySQLInteract{
private:
	sql::Statement *stmt;
  	sql::ResultSet *res;
  	sql::PreparedStatement *pstmt;

public:
	Admin(const MySQLInteract& connector) : MySQLInteract(connector, "LOGIN"){

		setUser("ADMIN");
	}

	
	/*
	*	#Checks if the USERNAME is Already taken or not.
	*	@param
	*		1.USERNAME to be checked 
	*		2.TYPE of user
	*	@return
	*		true, if USERNAME already taken.
	*		false, if not taken.
	*
	*/

	bool isUserNamePresent(string user_name,int type);


	/*
	*	#Checks if the lgoin credentials are correct or not.
	*	@param
	*		1.USERNAME  
	*		2.PASSWORD
	*		3.TYPE of user
	*	@return
	*		true, if the credentials are correct.
	*		false, if not.
	*
	*/

	bool login(string user_name, string password, int type);

	/*
	*	#Creates new account for the user.
	*	@param
	*		1.NEW USERNAME  
	*		2.NEW PASSWORD
	*		3.TYPE of user.
	*	@return
	*		true always.
	*
	*/

	bool createNewUser(string user_name, string password, int type);
	

	/*
	*	#Checks if the USERNAME is valid or not.
	*	@param
	*		1.USERNAME to be checked 
	*		2.TYPE of user
	*	@return
	*		true, if USERNAME is not taken already.
	*		false, if otherwise.
	*
	*/

	bool isUserNameValid(string user_name, int type);


	/*
	*	#Checks if the USERNAME is Already taken or not.
	*	@param
	*		1.USERNAME to be checked 
	*		2.TYPE of user
	*	@return
	*		true, if USERNAME already taken.
	*		false, if not taken.
	*
	*/

	bool removeUser(string user_name, int type);


	~Admin();
};


// When DATABASE == STUDENT
class Student : public MySQLInteract{
private:
	sql::Statement *stmt;
  	sql::ResultSet *res;
  	sql::PreparedStatement *pstmt;

public:
	Student(string user_name, const MySQLInteract& connector) : MySQLInteract(connector, "STUDENT"){
		setUser(user_name);
	}


	/*
	*	#Creates a new TABLE with the USERNAME as its name.
	*	@param
	*		1.USERNAME
	*	@return
	*		true always.
	*
	*/

	bool createStudent(string user_name);


	/*
	*	#Stores the contents of the file along with other info into the table.
	*	@param
	*		1.QUESTION NO for which the given file corresponds to. 
	*		2.PATH to the file
	*		3.PROGRAMMING LANGUAGE in which the file is written.
	*		4.RESULT of testing, i.e AC,WA,TLE or RE.
	*		5.MARKS obtained after testing.
	*	@return
	*		true always.
	*
	*/

	bool storeFileWithResult(int quesNo, string pathToFile, string language, string result, int marks);


	/*
	*	#Returns the contents of the requested file.
	*	@param
	*		1.QUESTION NO for which the requested file was submitted.
	*		2.SUBMISSION NO of the file.
	*	@return
	*		If the file is present, a string containing the contents of the requested file.
	*		NULL string otherwise.
	*
	*/

	string getFile(int quesNo, int submissionNo);

	/*
	*	#Returns the submission logs for a perticular question.
	*	@param
	*		1.QUESTION NO.
	*	@return
	*		If the question is present, a string containing the Submission log.
	*		NULL string otherwise.
	*
	*/


	string getSubmissionLogs(int quesNo);

	/*
	*	#Returns the submission logs for the user.
	*	@param
	*		void
	*	@return
	*		If the any submission is present, a string containing the Submission log.
	*		NULL string otherwise.
	*
	*/

	string getSubmissionLogs();

	~Student();
};


// When DATABASE == TEACHER	
class Teacher : public MySQLInteract{
private:
	sql::Statement *stmt;
  	sql::ResultSet *res;
  	sql::PreparedStatement *pstmt;

public:
	static const int QUESTION_FILE;		//USE THIS if requested file contains QUESTION
	static const int TEST_IN_FILE;		//USE THIS if requested file contains INPUT TEST CASES
	static const int TEST_OUT_FILE; 	//USE THIS if requested file contains EXPECTED OUTPUTS

	Teacher(string user_name, const MySQLInteract& connector) : MySQLInteract(connector, "TEACHER"){
		setUser(user_name);
	}


	/*
	*	#Creates a new TABLE with the USERNAME as its name.
	*	@param
	*		1.USERNAME
	*	@return
	*		true always.
	*
	*/

	bool createTeacher(string user_name);


	/*
	*	#Returns the Number of questions added by the user.
	*	@param
	*		void.
	*	@return
	*		If any question is present, an int containing the number of questions.
	*		0 otherwise.
	*
	*/

	int getNoOfQuestions();


	/*
	*	#Returns the contents of the requested file.
	*	@param
	*		1.QUESTION NO for which the requested file was submitted.
	*		2.TYPE of the file.
	*	@return
	*		If the file is present, a string containing the contents of the requested file.
	*		NULL string otherwise.
	*
	*/


	string getFile(int quesNo, int type);


	/*
	*	#Stores the contents of the files along with other info into the table.
	*	@param
	*		1.PATH to the FILE CONTAINING THE QUESTION 
	*		2.PATH to the FILE CONTATING THE INPUT TEST CASES.
	*		3.PATH to the FILE COTAINING THE EXPECTED OUTPUTS for each test case.
	*		4.MARKS alloted pers test cases.
	*	@return
	*		true always.
	*
	*/

	
	bool addQuestion(string pathToQuestionFile, string pathToInputTest, string pathToOutputTest, int marksPerTest);

	/*
	*	#Returns the Marks aloocted per test case for a perticular question.
	*	@param
	*		1.QUESTION NO.
	*	@return
	*		If the question is present, an int containing the marks per test case for that question.
	*		0 otherwise.
	*
	*/

	int getMarksPerTest(int quesNo);

	
	/*
	*	#Checks if a perticular question exists or not.
	*	@param
	*		1.QUESTION NO.
	*	@return
	*		true if Question is present.
	*.		false otherwise
	*
	*/

	bool questionExists(int quesNo);


	
	/*
	*	#Deletes a perticular question.
	*	@param
	*		1.QUESTION NO.
	*	@return
	*		Always true.
	*
	*/

	bool deleteQuestion(int quesNo);
	


	/*
	*	#Returns the results of all the students who attended the test as a string 
	*	which can be directly written into a .csv or .odx or any other spreadheet file.
	*	@param
	*		1.A string which is common to all the usernames of the students who have attended the test.
	*	@return
	*		1. A string with the results, structured to be written directly into a .csv file, the format of the string would be,
	*		Username, Marks in Question 1,Marks in Question 2, ........ , Total score 
	*		2. An empty string if no student attended the test.
	*
	*/

	string getResults(string commonCharacters);

	~Teacher();
};


const int Teacher::QUESTION_FILE = 0;
const int Teacher::TEST_IN_FILE = 1;
const int Teacher::TEST_OUT_FILE = 2;


void clearSystem()
{
	system("clear");
	printf("  ======================================\n");
	printf("\t             __   __ ");
	printf("\n\t  /\\   |    |  | |   ");
	printf("\n\t /__\\  |    |__| |-- " );
	printf("\n\t/    \\ |___ |    |__ ");
	printf("\n\t            |        \n\n");
	printf("  ======================================\n\n");
}


#include "MySQL_Client.cc"
#endif