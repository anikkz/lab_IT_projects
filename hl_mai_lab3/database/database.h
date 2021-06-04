#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>

namespace database{
    class Database{
        private:
            std::string _connection_string;
            Database();
        public:
            static Database& get();
            static std::string sharding_hint(std::string key, size_t max_shards);
            Poco::Data::Session create_session();
    };
}
#endif