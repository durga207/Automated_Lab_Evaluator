#include "MySQL_Client.h"
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<errno.h>

using namespace std;

#define maxNumberOfTestCases 100
#define MAXTIME 1000000
	//1 second

pid_t pid=-1;

bool compareFiles(const char file1[], const char file2[])
{
	string readLineFile1, readLineFile2;
	ifstream fin1(file1), fin2(file2);

	bool flag = true;
	while(true)
	{
		fin1>>readLineFile1;
		fin2>>readLineFile2;

		if(readLineFile2.compare(readLineFile1))
		{
			flag = false;
			break;
		}

		if(fin1.eof() && fin2.eof())
			break;
		if(fin2.eof() || fin1.eof())
		{
			flag = false;
			break;
		}
	}

	fin1.close();
	fin2.close();

	return !flag;
}

void handler(int sig){
	wait(NULL);
}

int executeCode(const char runCommand[])
{


int fd[2];
  if (pipe(fd) == -1) {
    printf("\nError on opening pipe\n");
    return 5;
  }
  int pid = fork();
  if (pid == -1) {
    printf("\nError in forking\n");
    return 5;
  }
  if (pid == 0) {
    close(fd[0]);
    int ret=-1;
    ret = system(runCommand);
    if (write(fd[1], &ret, sizeof(int)) == -1) {
      printf("\nError in writing\n");
      return 5;
    }
    close(fd[1]);
  }
  else {
    close(fd[1]);
    int ret=-1;
    if (read(fd[0], &ret, sizeof(int)) == -1) {
      printf("\nError in reading\n");
      return 5;
    }
    signal(SIGCHLD,handler);
		usleep(MAXTIME);

		if(kill(pid,0)==0){
			kill(pid,SIGKILL);
			return 2;
		}
		else{
			if(ret != 0)
				return 3;
			return 0;
		}
    close(fd[0]);
  }


//	pid = fork();
//	int ret=-1;
//	if(pid==0)
//	{
//		ret = system(runCommand);
//		//wait(NULL);
//		//exit(0);
//	}
//	else
//	{
//		signal(SIGCHLD,handler);
//		usleep(MAXTIME);
//
//		if(kill(pid,0)==0){
//			kill(pid,SIGKILL);
//			return 2;
//		}
//		else{
//			if(ret != 0)
//				return 3;
//			return 0;
//		}
//	}

	return 0;
}

int makeTestCaseFolder(string testInput, string testOutput)
{
	ofstream inputFile("inputFile.txt"), outputFile("outputFile.txt");
	inputFile<<testInput;
	outputFile<<testOutput;
	inputFile.close();
	outputFile.close();


	system("mkdir testCases");
	ifstream inputTestCases("inputFile.txt"), outputTestCases("outputFile.txt");
	string readLine;
	char filename[100];
	int numberOfTestCases;

	inputTestCases>>numberOfTestCases;
	for(int i=0; i<numberOfTestCases; i++)
	{
		strcpy(filename, "testCases/input");
		filename[15] = '0' + i/10;
		filename[16] = '0' + i%10;
		filename[17] = '\0';
		strcat(filename, ".txt");

		inputFile.open(filename);
		while(true)
		{
			inputTestCases>>readLine;
			if(!readLine.compare("#"))
				break;
			inputFile<<readLine<<endl;
		}
		inputFile.close();


		strcpy(filename, "testCases/output");
		filename[16] = '0' + i/10;
		filename[17] = '0' + i%10;
		filename[18] = '\0';
		strcat(filename, ".txt");

		outputFile.open(filename);
		while(true)
		{
			outputTestCases>>readLine;
			if(!readLine.compare("#"))
				break;
			outputFile<<readLine<<endl;
		}
		outputFile.close();
	}

	inputTestCases.close();
	outputTestCases.close();

	system("rm inputFile.txt");
	system("rm outputFile.txt");

	return numberOfTestCases;
}

void submit(Teacher teacher, Student student)
{
	char user_filepath[500];
	int results[maxNumberOfTestCases];
	int quesNo;
	clearSystem();

	printf("Enter question number : ");
	cin>>quesNo;

	if(teacher.getNoOfQuestions() < quesNo || quesNo<=0)
	{
		printf("Invalid Question Number\n");
		return;
	}

	//GET TEST CASES, and number of test cases
	//Put into folder
	string testInput = teacher.getFile(quesNo, Teacher::TEST_IN_FILE);
	string testOutput = teacher.getFile(quesNo, Teacher::TEST_OUT_FILE);
	int numberOfTestCases = makeTestCaseFolder(testInput, testOutput);

	printf("Enter relative path to file to submit : ");
	cin>>user_filepath;

	char compile[] = "gcc -o exec.out ";
	strcat(compile, user_filepath);

	if(system(compile))
	{
		printf("\n\n\tCompilation Error\n");
		printf("\tHit Enter to continue");
		getchar();
		getchar();
		system("rm -r testCases");

		return;
	}

	int score=0;
		//0 - AC
		//1 - WA
		//2 - TLE
		//3 - RE

	string resultString;
	int finalResult = 0;

	clearSystem();
	for(int i=0; i<numberOfTestCases; i++)
	{
		char runCommand[] = "./exec.out < testCases/input";
		char tmp[5];
		tmp[0] = '0' + i/10;
		tmp[1] = '0' + i%10;
		tmp[2] = '\0';
		strcat(runCommand, tmp);
		strcat(runCommand, ".txt > output.txt");

		printf("Running on test case %2d ", i+1);
		fflush(stdout);
		for(int i=0; i<3; i++)
		{
			usleep(200000);
			printf(".");
			fflush(stdout);
		}
		printf("\n");
		int codeResult = executeCode(runCommand);
		if(pid == 0) {
            printf("---");
			exit(0);
		}
		if(codeResult == 3){
			printf("Runtime Error\n\n");
			results[i] = 3;
			finalResult = max(finalResult, results[i]);
			continue;
		}
		else if(codeResult == 2){

			printf("Time Limit Exceeded %d\n\n", pid);
			results[i] = 2;
			finalResult = max(finalResult, results[i]);
			continue;
		}


		char answerPath[100] = "testCases/output";
		strcat(answerPath, tmp);
		strcat(answerPath, ".txt");

		if(compareFiles("output.txt", answerPath)){
			results[i] = 1;
			printf("Wrong Answer\n\n");
		}
		else{
			results[i] = 0;
			printf("Correct Answer %d\n\n", pid);
			score++;
		}

		finalResult = max(finalResult, results[i]);
	}

	score = score*1.0/numberOfTestCases*100;
	//TODO : Print result

	switch(finalResult)
	{
		case 0:
			resultString = "AC";
			break;
		case 1:
			resultString = "WA";
			break;
		case 2:
			resultString = "TLE";
			break;
		case 3:
			resultString = "RE";
			break;
	}
	student.storeFileWithResult(quesNo, user_filepath, "C", resultString, score);

	// Files to be deleted
	system("rm exec.out");
	system("rm output.txt");
	system("rm -r testCases");

	printf("\n\nQuestion Number : %d\n", quesNo);
	cout<<"Result : "<<resultString<<endl;
	printf("Score : %d / 100\n", score);
	printf("\n\nHit Enter to continue");
	pid = 0;
	getchar();
	getchar();


}
