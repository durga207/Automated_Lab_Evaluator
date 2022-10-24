#include "submitFile.cc"

void displayQuestion(Teacher teacher)
{
	clearSystem();
	int quesNo;

	printf("\n\n\tTotal number of questions present : %d", teacher.getNoOfQuestions());
	printf("\n\tEnter question number : ");
	cin>>quesNo;

	if(quesNo <=0 || quesNo > teacher.getNoOfQuestions())
	{
		printf("\n\tInvalid Question Number\n\tHit Enter to continue");
		getchar();
		getchar();
		return;
	}

	clearSystem();
	cout<<teacher.getFile(quesNo, Teacher::QUESTION_FILE);
	printf("\n\nHit Enter to continue");
	getchar();
	getchar();
}

void getSubmissionLogs(Student student)
{
	clearSystem();
	int quesNo, submissionID;
	char choice;

	cout<<student.getSubmissionLogs();
	
	printf("\n\nDo you want to retrieve a submission file? (y/n) : ");
	getchar();
	cin>>choice;

	if(choice != 'y' && choice != 'Y')
		return;

	printf("\nEnter the question number and submission ID : ");
	cin>>quesNo>>submissionID;

	ofstream fout("src/tmp/submissionFile.c");
	fout<<student.getFile(quesNo, submissionID);
	fout.close();

	system("gedit src/tmp/submissionFile.c");
}

void getSubmissionLogs(Student student, int quesNo)
{
	clearSystem();
	int submissionID;
	char choice;

	cout<<student.getSubmissionLogs(quesNo);

	printf("\n\nDo you want to retrieve a submission file? (y/n) : ");
	getchar();
	cin>>choice;

	if(choice != 'y' && choice != 'Y')
		return;

	printf("\nEnter the submission ID : ");
	cin>>submissionID;

	ofstream fout("src/tmp/submissionFile.c");
	fout<<student.getFile(quesNo, submissionID);
	fout.close();

	system("gedit src/tmp/submissionFile.c");
}

void menu(Teacher teacher, Student student)
{
	int choice;
	int quesNo;

	do
	{
		clearSystem();
		printf("\n\n\t1. View Question\n\t2. Submit Code\n\t3. View Submissions");
		printf("\n\t4. View Submissions for a particular question\n\tx. Logout\n\n\tEnter your choice : ");
		cin>>choice;

		switch(choice)
		{
			case 1:
				displayQuestion(teacher);
				break;
			case 2:
				submit(teacher, student);
				break;
			case 3:
				getSubmissionLogs(student);
				break;
			case 4:
				printf("\n\tTotal number of questions present : %d", teacher.getNoOfQuestions());
				printf("\n\tEnter question number : ");
				cin>>quesNo;
				if(quesNo <=0 || quesNo > teacher.getNoOfQuestions())
				{
					printf("\n\tInvalid Question Number\n\tHit Enter to continue");
					getchar();
					getchar();
				}
				else
					getSubmissionLogs(student, quesNo);

		}
	}while(choice >=1 && choice <=4);
}

bool askTeacherId(MySQLInteract connector, Student student)
{
	clearSystem();

	string teacherID;
	printf("\n\tEnter teacher username conducting this test : ");
	cin>>teacherID;

	Admin admin(connector);

	if(!admin.isUserNamePresent(teacherID, MySQLInteract::IS_TEACHER))
	{
		printf("\tInvalid teacher username\n\tHit Enter to retry");
		getchar();
		getchar();
		return false;
	}

	Teacher teacher(teacherID, connector);
	
	printf("\n\tYou have entered the test!!\n\tHit Enter to continue");
	getchar();
	getchar();
	menu(teacher, student);
	return true;
}

void STUDENT(MySQLInteract connector, Student student)
{
	while(askTeacherId(connector, student) == false);
}