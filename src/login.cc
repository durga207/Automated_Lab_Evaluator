#include "student.cc"
#include "teacher.cc"
#include "admin.cc"
using namespace std;

void login(MySQLInteract connector)
{
	string username,password,choice;
	int type;

	do{

		clearSystem();
		printf("\n\n\tUsername : ");
		cin>>username;
		printf("\tPassword : ");
		cin>>password;
		printf("\n\tType of User :\n\n\t1. Admin\n\t2. Teacher\n\t3. Student\n\n\tEnter your choice : ");
		cin>>type;

		Admin admin(connector);
		

		if(type == 1){
			if(!admin.isUserNamePresent(username, MySQLInteract::IS_ADMIN)){
				printf("\n\tInvalid username\n\tHit Enter to retry");
				getchar();
				getchar();
				continue;
	
			}

			else if(!admin.login(username,password,MySQLInteract::IS_ADMIN)){
				printf("\n\tInvalid password\n\tHit Enter to retry");
				getchar();
				getchar();	
				continue;		
			}

			cout<<"\n\tLog in Successfull\n";
			printf("\n\tHit Enter to continue");
			getchar();
			getchar();	

			ADMIN(connector);
		}
		else if(type == 2){
			if(!admin.isUserNamePresent(username, MySQLInteract::IS_TEACHER)){
				printf("\n\tInvalid username\n\tHit Enter to retry");
				getchar();
				getchar();
				continue;
	
			}

			else if(!admin.login(username,password,MySQLInteract::IS_TEACHER)){
				printf("\n\tInvalid password\n\tHit Enter to retry");
				getchar();
				getchar();
				continue;
			}

			cout<<"\n\tLog in Successfull";
			printf("\n\tHit Enter to continue");
			getchar();
			getchar();
			Teacher teacher(username, connector);
			TEACHER(connector, teacher);
		}
		else if(type == 3){
			if(!admin.isUserNamePresent(username, MySQLInteract::IS_STUDENT)){
				printf("\n\tInvalid username\n\tHit Enter to retry");
				getchar();
				getchar();
				continue;
			}

			else if(!admin.login(username,password,MySQLInteract::IS_STUDENT)){
				printf("\n\tInvalid password\n\tHit Enter to retry");
				getchar();
				getchar();
				continue;
			}

			cout<<"\n\tLog in Successfull\n";
			printf("\n\tHit Enter to continue");
			getchar();
			getchar();
			Student student(username, connector);
			STUDENT(connector, student);
		}
		else
		{
			printf("\n\tInvalid Type of User\n\tHit Enter to retry");
			getchar();
			getchar();
			continue;
		}


		printf("\n\n\tDo you want to continue ? [Y/N] \n\tEnter your choice : ");
		cin>>choice;
		if(choice.compare("Y") && choice.compare("y")){
			printf("\n\n\tProgram Terminated\n");
			exit(0);
		}


	}while(true);

	
}