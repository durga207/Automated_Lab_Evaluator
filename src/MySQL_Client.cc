//sudo g++ -Wall -I/usr/include/cppconn MySQL_Client.h -L/usr/lib -lmysqlcppconn -std=c++11
//sudo apt-get install libmysqlcppconn-dev

bool MySQLInteract::setUser(string user_name){
	USER = user_name;
	return true;
}

string MySQLInteract::getUser() const{
	return USER;
}

string MySQLInteract::getHost() const{
	return HOST;
}

string MySQLInteract::getPassword() const{
	return PASSWORD;
}

string MySQLInteract::getPort() const{
	return PORT;
}

string MySQLInteract::getUserName() const{
	return USER_NAME;
}



MySQLInteract::~MySQLInteract(){
	// cout<<"~MySQLInteract"<<endl;
	// if(con!=NULL)
	// 	delete con;
}






// When DATABASE == LOGIN, and when login info needs to accessed, follwing methods are utilised.
 

bool Admin::isUserNamePresent(string user_name,int type){
	con->setSchema("LOGIN");
	
	string table;
	switch(type){
		case 0:
			table = "ADMIN";
			break;
		case 1:
			table = "TEACHER";
			break;
		case 2:
			table = "STUDENT";
			break;
	}


	pstmt = con->prepareStatement("SELECT ID FROM " + table + " WHERE EXISTS (SELECT username FROM " + table + " WHERE username = ? )");

	pstmt->setString(1,user_name);
	res = pstmt->executeQuery();

	return res->next() != false ? true:false;
}


bool Admin::login(string user_name, string password, int type){
	con->setSchema("LOGIN");

	string table;
	switch(type){
		case 0:
			table = "ADMIN";
			break;
		case 1:
			table = "TEACHER";
			break;
		case 2:
			table = "STUDENT";
			break;
	}

	pstmt = con->prepareStatement("SELECT password FROM " + table + " WHERE username = ? ");

	pstmt->setString(1,user_name);
	res = pstmt->executeQuery();

	delete pstmt;

	while(res->next()){
		string str = res->getString("password");
		if(password.compare(str) == 0){
			delete res;
			return true;
		}
	}

	delete res;

	return false;

}


bool Admin::createNewUser(string user_name, string password, int type){
	con->setSchema("LOGIN");

	string table;
	switch(type){
		case 0:
			table = "ADMIN";
			break;
		case 1:
			table = "TEACHER";
			break;
		case 2:
			table = "STUDENT";
			break;
	}
	// con->reconnect();

	pstmt = con->prepareStatement("INSERT INTO " + table + " (username, password) VALUES (?,?)");
	pstmt->setString(1,user_name);
	pstmt->setString(2,password);

	pstmt->executeUpdate();
	delete pstmt;


	return true;

}


bool Admin::removeUser(string user_name, int type){
	con->setSchema("LOGIN");

	string table;
	switch(type){
		case 0:
			table = "ADMIN";
			break;
		case 1:
			table = "TEACHER";
			break;
		case 2:
			table = "STUDENT";
			break;
	}

	if(isUserNamePresent(user_name,type)){
		pstmt = con->prepareStatement("DELETE FROM " + table + " WHERE username = ?");
		pstmt->setString(1,user_name);

		pstmt->executeUpdate();

		con->setSchema(table);
		pstmt = con->prepareStatement("DROP TABLE " + user_name);
		pstmt->executeUpdate();

		con->setSchema("LOGIN");
		return true;
	}

	return false;
}


bool Admin::isUserNameValid(string user_name, int type){
	con->setSchema("LOGIN");
	

	string table;
	switch(type){
		case 0:
			table = "ADMIN";
			break;
		case 1:
			table = "TEACHER";
			break;
		case 2:
			table = "STUDENT";
			break;
	}

	pstmt = con->prepareStatement("SELECT ID FROM " + table + " WHERE EXISTS (SELECT username FROM " + table + " WHERE username = ? )");

	pstmt->setString(1,user_name);
	res = pstmt->executeQuery();

	delete pstmt;

	if(res->next()){

		delete res;
		return false;
	}

	delete res;
	return true;
}

Admin::~Admin(){
	// cout<<"~ADMIN"<<endl;
	// delete stmt;
	// delete res;
	// delete pstmt;
}



// When DATABASE == STUDENT


bool Student::createStudent(string user_name){
	con->setSchema("STUDENT");

	pstmt = con->prepareStatement("CREATE TABLE " + user_name + " (ID INT NOT NULL AUTO_INCREMENT,Question INT NOT NULL, DateTime TEXT, Language varchar(255), File LONGTEXT, Result varchar(255), Marks INT, PRIMARY KEY(ID))");
	pstmt->executeUpdate();

	delete pstmt;

	return true;

}



bool Student::storeFileWithResult(int quesNo, string pathToFile, string language, string result, int marks){
	con->setSchema("STUDENT");

	pstmt = con->prepareStatement("INSERT INTO " + getUser() + " (Question,DateTime,Language,File,Result,Marks) VALUES  (?,?,?,?,?,?)");
	
	pstmt->setInt(1,quesNo);
	
	
	//Current date/time based on current system
	time_t now = time(0);
   	//Convert now to string form
   	string time = ctime(&now);
	time.resize(time.length() - 1); //to remove the "\n" char.
   	pstmt->setString(2,time);
	
	
	pstmt->setString(3,language);
	
	
	//Copying the file into a string;
	ifstream ifs (pathToFile);
	std::string content( (std::istreambuf_iterator<char>(ifs) ),(std::istreambuf_iterator<char>() ) );
	pstmt->setString(4,content);

	pstmt->setString(5,result);
	pstmt->setInt(6,marks);

	pstmt->executeUpdate();

	delete pstmt;


	return true;

}


string Student::getFile(int quesNo, int submissionNo){
	con->setSchema("STUDENT");

	pstmt = con->prepareStatement("SELECT File FROM " + getUser() + " WHERE Question = ? AND ID = ?");

	pstmt->setInt(1,quesNo);
	pstmt->setInt(2,submissionNo);
	res = pstmt->executeQuery();

	while(res->next()){
		string toReturn = res->getString("File");
		delete res;
		delete pstmt;

		return toReturn;
	}

	delete res;
	delete pstmt;

	return "";

}



string Student::getSubmissionLogs(int quesNo){
	con->setSchema("STUDENT");

	int flag = 0;

	pstmt = con->prepareStatement("SELECT CONCAT('      ',ID,' \t|    ',DateTime,'   |\t    ',Language,'   \t|\t',Result,'\t|\t',Marks) AS _subLog FROM " + getUser() + " WHERE Question = ? ORDER BY ID ASC");
	pstmt->setInt(1,quesNo);

	res = pstmt->executeQuery();


	string toReturn = "Submission ID\t|\t    Date-Time   \t|\tLanguage\t|     Result  \t|\tMarks\n";
	while(res->next()){
		toReturn = toReturn + res->getString("_subLog") + "\n";
		flag = 1;
	}

	delete pstmt;
	delete res;

	return (flag == 1)? toReturn : "";

}


string Student::getSubmissionLogs(){
	con->setSchema("STUDENT");

	int flag = 0;

	pstmt = con->prepareStatement("SELECT CONCAT('    ',Question,'   \t|\t    ',ID,'   \t|    ',DateTime,'   |\t    ',Language,'   \t|\t',Result,'\t|\t',Marks) AS _subLog FROM " + getUser() +" ORDER BY Question ASC, ID DESC");

	res = pstmt->executeQuery();


	string toReturn = "Question No\t|\tSubmission ID\t|\t    Date-Time   \t|\tLanguage\t|     Result  \t|\tMarks\n";
	while(res->next()){
		toReturn = toReturn + res->getString("_subLog") + "\n";
		flag = 1;
	}

	delete pstmt;
	delete res;

	return (flag == 1)? toReturn : "";

}

Student::~Student(){
	// delete stmt;
	// delete res;
	// delete pstmt;
}



// When DATABASE == TEACHER	

bool Teacher::createTeacher(string user_name){
	con->setSchema("TEACHER");

	pstmt = con->prepareStatement("CREATE TABLE " + user_name + " (ID INT NOT NULL AUTO_INCREMENT,Question INT NOT NULL,Question_File LONGTEXT,Test_In_File LONGTEXT,Test_Out_File LONGTEXT,Marks_For_Each_Test INT, PRIMARY KEY(ID))");
	pstmt->executeUpdate();

	delete pstmt;

	return true;
}



int Teacher::getNoOfQuestions(){
	con->setSchema("TEACHER");
	
	int flag = 0;

	pstmt = con->prepareStatement("SELECT MAX(Question) AS noOfQues FROM " + getUser());

	res = pstmt->executeQuery();

	int toReturn;
	while(res->next()){
		toReturn = res->getInt("noOfQues");
		flag = 1;
	}

	// pstmt = con->prepareStatement("SELECT Question FROM " + getUser());
	// res = pstmt->executeQuery();
	// int cnt = 0;
	// while(res->next()){
	// 	cnt++;
	// }
	delete pstmt;
	delete res;



	return (flag == 1) ? toReturn : 0;

}



string Teacher::getFile(int quesNo, int type){

	// pstmt = con->prepareStatement("SELECT Question FROM " + getUser());
	// res = pstmt->executeQuery();
	// int cnt = 0,realQuesNo = -1;
	// while(cnt<quesNo && res->next()){
	// 	realQuesNo = res->getInt("Question");
	// 	cnt++;
	// }

	// if(quesNo != cnt)
	// 	return NULL;
	con->setSchema("TEACHER");
	
	string column;
	switch(type){
		case 0:
			column = "Question_File";
			break;
		case 1:
			column = "Test_In_File";
			break;
		case 2:
			column = "Test_Out_File";
			break;
	}

	pstmt = con->prepareStatement("SELECT " + column + " FROM " + getUser() + " WHERE Question = ?");
	pstmt->setInt(1,quesNo);
	res = pstmt->executeQuery();

	while(res->next()){
		string toReturn = res->getString(column);
		delete res;
		delete pstmt;

		return toReturn;
	}

	delete res;
	delete pstmt;

	return "";

}



bool Teacher::addQuestion(string pathToQuestionFile, string pathToInputTest, string pathToOutputTest, int marksPerTest){
	con->setSchema("TEACHER");

	pstmt = con->prepareStatement("SELECT MAX(Question) AS noOfQues FROM " + getUser());
	res = pstmt->executeQuery();
	int quesNo = 0;
	while(res->next()){
		quesNo = res->getInt("noOfQues");
	}

	quesNo++;
	
	pstmt = con->prepareStatement("INSERT INTO " + getUser() + " (Question,Question_File, Test_In_File, Test_Out_File, Marks_For_Each_Test) VALUES  (?,?,?,?,?)");
	
	pstmt->setInt(1,quesNo);

	//Copying the file into a string;
	ifstream ifs1 (pathToQuestionFile);
	std::string content1( (std::istreambuf_iterator<char>(ifs1) ),(std::istreambuf_iterator<char>() ) );

	pstmt->setString(2,content1);
	
	
	//Copying the file into a string;
	ifstream ifs2 (pathToInputTest);
	std::string content2( (std::istreambuf_iterator<char>(ifs2) ),(std::istreambuf_iterator<char>() ) );

	pstmt->setString(3,content2);
	
	
	//Copying the file into a string;
	ifstream ifs3 (pathToOutputTest);
	std::string content3( (std::istreambuf_iterator<char>(ifs3) ),(std::istreambuf_iterator<char>() ) );

	pstmt->setString(4,content3);

	pstmt->setInt(5,marksPerTest);

	pstmt->executeUpdate();

	delete pstmt;


	return true;

}


int Teacher::getMarksPerTest(int quesNo){
	con->setSchema("TEACHER");

	int flag = 0;

	pstmt = con->prepareStatement("SELECT Marks_For_Each_Test FROM " + getUser() + " WHERE Question = ?");
	pstmt->setInt(1,quesNo);

	res = pstmt->executeQuery();

	int toReturn;
	while(res->next()){
		toReturn = res->getInt("Marks_For_Each_Test");
		flag = 1;
	}

	delete pstmt;
	delete res;

	return (flag == 1)? toReturn : 0;

}



bool Teacher::questionExists(int quesNo){
	con->setSchema("TEACHER");

	pstmt = con->prepareStatement("SELECT Question FROM " + getUser());
	res = pstmt->executeQuery();

	while(res->next()){
		if(quesNo == res->getInt("Question")){

			delete res;
			delete pstmt;
			return true;
		}
	}


	delete res;
	delete pstmt;
	return false;
}



bool Teacher::deleteQuestion(int quesNo){
	con->setSchema("TEACHER");

	pstmt = con->prepareStatement("DELETE FROM " + getUser() + " WHERE Question = ?");
	pstmt->setInt(1,quesNo);

	pstmt->executeUpdate();

	pstmt = con->prepareStatement("SELECT ID FROM " + getUser() + " WHERE Question > ?");
	pstmt->setInt(1,quesNo);

	res = pstmt->executeQuery();

	int realNo = quesNo;

	while(res->next()){
		pstmt = con->prepareStatement("UPDATE " + getUser() + " SET Question = ? WHERE ID = ?");
		pstmt->setInt(1,realNo);
		pstmt->setInt(2,res->getInt("ID"));
		pstmt->executeUpdate();

		realNo++;
	}

	delete res;
	delete pstmt;
	return true;
}
	
string Teacher::getResults(string commonCharacters){
	con->setSchema("TEACHER");

	int noOfQues = getNoOfQuestions();
	string results = "Name of Student";

	for(int i = 1;i<=noOfQues;i++){
		results = results + ",Marks in Question ";
		int t = i;
		while(t>0){
			char c = t%10 + '0';
			results = results + c;
			t = t/10;
		}
	}

	results = results + ",Total score" + '\n';



	pstmt = con->prepareStatement("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = \"STUDENT\" AND TABLE_NAME LIKE \"%" + commonCharacters + "%\"");
	res = pstmt->executeQuery();

	con->setSchema("STUDENT");

	int flag = 0;
	while(res->next()){
		flag = 1;
		string temp = res->getString("TABLE_NAME");
		string table = temp;
		temp = temp + ",";
		int sum = 0;

		for(int i = 1;i<=noOfQues;i++){
			pstmt = con->prepareStatement("SELECT MAX(Marks) AS Result FROM " + table + " WHERE Question = ?");
			pstmt->setInt(1,i);

  			sql::ResultSet *tmp;
  			tmp = pstmt->executeQuery();
  	

			int flag = 0;
  			while(tmp->next()){
  				int t = tmp->getInt("Result");
  				sum = sum+t;
  				string dum;
  				while(t>0){
  					flag = 1;
  					char c = t%10 + '0';
  					dum = dum+c;
  					t = t/10;
  				}		
  				reverse(dum.begin(),dum.end());
  				temp = temp + dum;
  			}

  			if(flag==0)
  				temp = temp+'0';

  			temp = temp + ",";
  			delete tmp;
		}

		int flag = 0;
		string dum;

		while(sum>0){
			flag = 1;
			char c = sum%10 + '0';
			dum = dum+c;
			sum = sum/10;
		}

		reverse(dum.begin(),dum.end());
		if(flag==0)
			temp = temp+'0';
		else
			temp = temp+dum;

		results = results + temp + '\n';
  				
	}

	con->setSchema("TEACHER");
	delete pstmt;

	return (flag == 1) ? results : "";
}


Teacher::~Teacher(){
	
	// delete stmt;
	// delete res;
	// delete pstmt;
}
