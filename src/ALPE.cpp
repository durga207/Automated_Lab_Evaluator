#include "login.cc"

int main()
{
	string IPAddress;

	clearSystem();
	printf("\n\tEnter server IP Address : ");
	cin>>IPAddress;

	MySQLInteract connector(IPAddress, "3306", "ALPE", "root", "LOGIN");
	login(connector);
}