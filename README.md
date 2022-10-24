# Automated-Lab-Program-Evaluator
Software Engineering Project

### Compile and Run
```
$ sudo g++ src/ALPE.cpp -o ALPE -Wall -I/usr/include/cppconn -L/usr/lib -lmysqlcppconn -std=c++11
$ ./ALPE
```

### Prerequisites
This software reqires the following pre-installed softwares and well as environment setup.

#### Pre-Installed softwares
1. GNU g++ comiler -version 5.0 and above.
2. GNU gcc compiler -version 5.0 and above.
3. MySQL server -distribution 5.6 and above.
4. MySQL Connector/C++ version 5.6 and above<br/>
	```$ sudo apt-get install libmysqlcppconn-dev```

#### Prerequisite environment setup.

- The MySQL server must be preinstalled on the server side.<br/>
```$ sudo apt-get install mysql-server-5.7```

- The MySQL server must be setup to provide access to username ```ALPE``` and password ```root```.<br/>

	```
	$ mysql -h 127.0.0.1 -P 3306 -u root -p
	```
	In MySQL:
	```MySQL
	create user 'ALPE'@'localhost' identified by 'root';
	grant all privileges on *.* to 'ALPE'@'localhost' with grant option;
	create user 'ALPE'@'%' identified by 'root';
	grant all privileges on *.* to 'ALPE'@'%' with grant option;
	flush privileges;
	exit;
	```
	```
	$ sudo service mysql restart
	```

- The MySQL server must be set-up to contain the following databases,<br/>
	```$ mysql -h 127.0.0.1 -P 3306 -u root -p```
	1. LOGIN
	```MySQL
	create database LOGIN;
	```
	2. TEACHER
	```MySQL
	create database TEACHER;
	```
	3. STUDENT
	```MySQL
	create database STUDENT;
	```
- The "LOGIN" database must have the following tables with their Descriptions to match that shown on the image,<br />
```$ use LOGIN;```

1. ADMIN<br />
	```MySQL
	create table ADMIN(
		ID int not null auto_increment,
		username longtext,
		password longtext,
		primary key(ID)
	);
	```
![picture alt](https://github.com/Aj163/Automated-Lab-Program-Evaluator/blob/master/Diagrams/ADMIN.png "Table ADMIN")<br />

2. TEACHER<br />
	```MySQL
	create table TEACHER(
		ID int not null auto_increment,
		username longtext,
		password longtext,
		primary key(ID)
	);
	```
![picture alt](https://github.com/Aj163/Automated-Lab-Program-Evaluator/blob/master/Diagrams/TEACHER.png "Table TEACHER")<br />

3.  STUDENT<br />
	```MySQL
	create table STUDENT(
		ID int not null auto_increment,
		username longtext,
		password longtext,
		primary key(ID)
	);
	```
![picture alt](https://github.com/Aj163/Automated-Lab-Program-Evaluator/blob/master/Diagrams/STUDENT.png "Table STUDENT")


