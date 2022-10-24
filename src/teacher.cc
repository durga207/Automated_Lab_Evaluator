#include "MySQL_Client.h"
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

void uploadQuestion(Teacher teacher)
{
	clearSystem();
	string readLine, questionPath, testCasesFolderPath, inputPath, outputPath;
	string testInput = "", testOutput = "";
	ifstream inputFile, outputFile;
	ofstream inputSingleFile, outputSingleFile;
	int numberOfTestCases;

	//Question file
	printf("Enter path to question file : ");
	cin>>questionPath;

	printf("Enter number of test cases : ");
	cin>>numberOfTestCases;

	printf("Enter path to folder containing all test cases : ");
	cin>>testCasesFolderPath;

	testInput = to_string(numberOfTestCases);
	testInput += '\n';
	for(int i=0; i<numberOfTestCases; i++)
	{
		//Input test cases file
		inputPath = testCasesFolderPath + "/input";
		inputPath += '0'+i/10;
		inputPath += '0'+i%10;
		inputPath += ".txt";

		inputFile.open(inputPath);
		while(!inputFile.eof())
		{
			getline(inputFile, readLine);
			if(!readLine.compare(""))
				continue;
			testInput += readLine;
			testInput += "\n";
		}

		testInput += "#\n";
		inputFile.close();

		//Input test cases file
		outputPath = testCasesFolderPath + "/output";
		outputPath += '0'+i/10;
		outputPath += '0'+i%10;
		outputPath += ".txt";

		outputFile.open(outputPath);
		while(!outputFile.eof())
		{
			getline(outputFile, readLine);
			if(!readLine.compare(""))
				continue;
			testOutput += readLine;
			testOutput += "\n";
		}

		testOutput += "#\n";
		outputFile.close();
	}

	inputSingleFile.open("inputUpload.txt");
	inputSingleFile<<testInput;
	inputSingleFile.close();

	outputSingleFile.open("outputUpload.txt");
	outputSingleFile<<testOutput;
	outputSingleFile.close();

	teacher.addQuestion(questionPath, "inputUpload.txt", "outputUpload.txt", 1);

	system("rm inputUpload.txt");
	system("rm outputUpload.txt");

	printf("\n\nUploaded Question");
	printf("\nHit Enter to continue");
	getchar();
	getchar();
}

void getReport(Teacher teacher)
{
	clearSystem();
	string commonString;

	printf("\n\n\tEnter student batch (Example \'16CO\') : ");
	cin>>commonString;

	ofstream CSVFile("src/tmp/Result.csv");
	CSVFile<<teacher.getResults(commonString);
	CSVFile.close();

	system("libreoffice src/tmp/Result.csv");
}

void viewSubmissionOfStudent(Admin admin, MySQLInteract connector)
{
	clearSystem();
	string studentUsername;
	int quesNo, submissionID;
	char choice;

	printf("\n\tEnter student username : ");
	cin>>studentUsername;

	if(!admin.isUserNamePresent(studentUsername, MySQLInteract::IS_STUDENT))
	{
		printf("\tInvalid username\n\tHit Enter to continue");
		getchar();
		getchar();
		return;
	}

	Student student(studentUsername, connector);
	clearSystem();
	cout<<student.getSubmissionLogs();

	printf("\n\nDo you want to retrieve a submission file? (y/n) : ");
	getchar();
	cin>>choice;

	if(choice != 'y' && choice != 'Y')
		return;

	printf("\nEnter the question number and the submission ID : ");
	cin>>quesNo>>submissionID;

	ofstream fout("src/tmp/submissionFile.c");
	fout<<student.getFile(quesNo, submissionID);
	fout.close();

	system("gedit src/tmp/submissionFile.c");
}

void deleteQuestion(Teacher teacher)
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

	teacher.deleteQuestion(quesNo);
	printf("\n\tQuestion has been deleted\n\tHit Enter to continue");
	getchar();
	getchar();
}

void menu(Teacher teacher, Admin admin, MySQLInteract connector)
{
	int choice;

	do
	{
		clearSystem();
		printf("\n\n\t1. Upload Question\n\t2. Get Report\n\t3. View Submissions of a student");
		printf("\n\t4. Delete Question\n\tx. Logout\n\n\tEnter your choice : ");
		cin>>choice;

		switch(choice)
		{
			case 1:
				uploadQuestion(teacher);
				break;
			case 2:
				getReport(teacher);
				break;
			case 3:
				viewSubmissionOfStudent(admin, connector);
				break;
			case 4:
				deleteQuestion(teacher);
				break;
		}
	}while(choice >=1 && choice <=4);
}

void TEACHER(MySQLInteract connector, Teacher teacher)
{
	Admin admin(connector);
	menu(teacher, admin, connector);
}

/*


bool askTeacherId(MySQLInteract connector)
{
	clearSystem();

	string teacherID;
	printf("\n\tEnter username : ");
	cin>>teacherID;

	

	if(!admin.isUserNamePresent(teacherID, MySQLInteract::IS_TEACHER))
	{
		printf("\tInvalid teacher username\n\tHit Enter to retry");
		getchar();
		getchar();
		return false;
	}

	Teacher teacher(teacherID, connector);

	printf("\n\tYou have logged in!!\n\tHit Enter to continue");
	getchar();
	getchar();
	menu(teacher, admin, connector);
	return true;
}


*/