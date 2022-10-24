#include <bits/stdc++.h>
#include <fstream>

#include "../src/MySQL_Client.h"

int main(){

	string host,port,user,pwd,db;
	cout<<"Host = ";
	cin>>host;

	cout<<"Port = ";
	cin>>port;

	cout<<"username = ";
	cin>>user;

	cout<<"password = ";
	cin>>pwd;

	db = "test";

	MySQLInteract mysqlConnector(host,port,user,pwd,db);


	Admin admin(mysqlConnector);

	string type;
	cout<<"........LOGIN........."<<endl<<"Type of user admin/teacher/student = ";
	cin>>type;

	cout<<"USERNAME = ";
	cin>>user;
	cout<<"PASSWORD = ";
	cin>>pwd;

	int typeFlag = -1;
	if(type.compare("admin")==0)
		typeFlag = MySQLInteract::IS_ADMIN;

	else if(type.compare("teacher")==0)
		typeFlag = MySQLInteract::IS_TEACHER;

	else
		typeFlag = MySQLInteract::IS_STUDENT;

	cout<<typeFlag<<endl;
	if(!admin.isUserNamePresent(user,typeFlag)){
		if(admin.isUserNameValid(user,typeFlag))
			if(admin.createNewUser(user,pwd,typeFlag))
				cout<<"New account created"<<endl<<endl;


	}
	else
		cout<<"SORRY"<<endl;

	if(admin.login(user,pwd,typeFlag))
		cout<<"LOGGED IN"<<endl;
	else
		cout<<"BYE SORRY"<<endl;


	/*
	
		TESTING STUDENT.	
	*/

	if(typeFlag==2){
		Student student(user,mysqlConnector);
		student.createStudent(user);

		student.storeFileWithResult(2,"/home/praveen/Desktop/Simply/mysql.cpp","C","AC",100);
		string str = student.getSubmissionLogs(2);
		cout<<endl<<str<<endl;
		
		str = student.getSubmissionLogs();
		
		cout<<endl<<str<<endl;
		ofstream out;
		out.open("test.cpp");
		str = student.getFile(2,1);

		out<<str;
	}

	/*
	
		TESTING TEEACHER
	*/


	if(typeFlag == 1){

		Teacher teacher(user,mysqlConnector);
		teacher.createTeacher(user);
		string ques = "/home/praveen/Desktop/Simply/mysql.cpp" ;
		string in = "/home/praveen/Desktop/Simply/h.cpp";
		string Out = "/home/praveen/Desktop/Simply/mvt.c";
		teacher.addQuestion(ques,in,Out,20);
		cout<<"Marks per test = "<<teacher.getMarksPerTest(1)<<endl;
		cout<<"No of questions = "<<teacher.getNoOfQuestions()<<endl;
		ofstream out1;
		out1.open("question.cpp");
		out1<< teacher.getFile(1,0);

		ofstream out2;
		out2.open("in.txt");

		out2<<teacher.getFile(1,1);
		
		ofstream out3;
		out3.open("out.txt");

		out3<<teacher.getFile(1,2);
		
	}


	return 0;
}