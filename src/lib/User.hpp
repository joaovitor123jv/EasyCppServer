#include<iostream>

class User
{
public:
	User(void);
	User(std::string login, std::string password);

	void setLogin(std::string login);
	void setPassword(std::string password);

	std::string getLogin(void);

private:
	std::string login;
	std::string password;
};