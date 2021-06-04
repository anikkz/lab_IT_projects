#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"

namespace database
{
    class Person{
    private:
        long _id;
        std::string _login;
        std::string _first_name;
        std::string _last_name;
        unsigned char _age;

    public:

        static Person fromJSON(const std::string & str);
        Poco::JSON::Object::Ptr toJSON() const;

        long             get_id() const;
        const std::string &get_login() const;
        const std::string &get_first_name() const;
        const std::string &get_last_name() const;
        unsigned char get_age() const;

        long&        id();
        std::string &login();
        std::string &first_name();
        std::string &last_name();
        unsigned char &age();

        static void init();
        static Person read_by_login(std::string login);
        static std::vector<Person> read_all();
        static std::vector<Person> search(std::string first_name,std::string last_name);
        void save_to_mysql();


    };
}


#endif //PERSON_H
