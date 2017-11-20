#include <mysql/mysql.h>

class MysqlConnection
{
public:
	MysqlConnection();
	MysqlConnection(MysqlConnection &&) = default;
	MysqlConnection(const MysqlConnection &) = default;
	MysqlConnection &operator=(MysqlConnection &&) = default;
	MysqlConnection &operator=(const MysqlConnection &) = default;
	~MysqlConnection();

private:
	
};

MysqlConnection::MysqlConnection()
{
}

MysqlConnection::~MysqlConnection()
{
}