#include "MySQL_Client.h"
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

bool addStudent(string username,string password, Admin admin, MySQLInteract connector){
	if(!admin.isUserNameValid(username, MySQLInteract::IS_STUDENT)){
		printf("\n\tUsername already present\nHit Enter to retry");
		getchar();
		getchar();
		return false;
	}
	admin.createNewUser(username,password,MySQLInteract::IS_STUDENT);
	Student student(username,connector);
	student.createStudent(username);

	printf("\n\tAccount was created successfully\n\tHit Enter to continue");
	getchar();
	getchar();
		

	return true;
}

bool addTeacher(string username,string password, Admin admin, MySQLInteract connector){
	if(!admin.isUserNameValid(username,MySQLInteract::IS_TEACHER)){
		printf("\n\tUsername already present\n\tHit Enter to retry");
		getchar();
		getchar();
		return false;
	}
	admin.createNewUser(username,password,MySQLInteract::IS_TEACHER);
	Teacher teacher(username,connector);
	teacher.createTeacher(username);

	printf("\n\tAccount was created successfully\n\tHit Enter to continue");
	getchar();
	getchar();
		
	return true;
}

bool removeStudent(string username, Admin admin){
	if(!admin.isUserNamePresent(username,MySQLInteract::IS_STUDENT)){
		printf("\n\tNo such user is present\n\tHit Enter to retry");
		getchar();
		getchar();
		return false;
	}
	admin.removeUser(username,MySQLInteract::IS_STUDENT);

	printf("\n\tAccount was removed successfully\n\tHit Enter to continue");
	getchar();
	getchar();
	
	return true;
}

bool removeTeacher(string username, Admin admin){
	if(!admin.isUserNamePresent(username, MySQLInteract::IS_TEACHER)){
		printf("\n\tNo such user present\n\tHit Enter to retry");
		getchar();
		getchar();
		return false;
	}
	admin.removeUser(username,MySQLInteract::IS_TEACHER);

	printf("\n\tAccount was removed successfully\n\tHit Enter to continue");
	getchar();
	getchar();
	
	return true;
}

void ADMIN(MySQLInteract connector)
{	
	string username,password;
	int choice;

	Admin admin(connector);
		
	do{
		clearSystem();
		printf("\n\t1. Add Teacher\n\t2. Add Student\n\t3. Remove Teacher\n\t4. Remove Student\n\tx. Logout\n\tEnter your choice : ");
		cin>>choice;
		
		if(choice == 1)
		{
			printf("\n\tNew Username : ");
			cin>>username;
			printf("\n\tNew Password : ");
			cin>>password;
			addTeacher(username,password,admin, connector);
		}
		else if(choice == 2)
		{
			printf("\n\tNew Username : ");
			cin>>username;
			printf("\n\tNew Password : ");
			cin>>password;
			addStudent(username,password,admin, connector);
		}
		else if(choice == 3)
		{
			printf("\n\tUsername : ");
			cin>>username;
			removeTeacher(username,admin);
		}
		else if(choice == 4)
		{
			printf("\n\tUsername : ");
			cin>>username;
			removeStudent(username,admin);
		}

	}while(choice>=1 && choice<=4);	
}