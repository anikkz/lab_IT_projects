#include <gtest/gtest.h>
#include "../config/config.h"
#include "../database/database.h"
#include "../database/person.h"
#include <Poco/Data/SessionFactory.h>

using Poco::Data::Session;
using Poco::Data::Statement;
using namespace Poco::Data::Keywords;

class TestApp : public ::testing::Test {
protected:
    TestApp() {
        Config::get().host() = "127.0.0.1";
        Config::get().database() = "sql_test";
        Config::get().port() = "6043";
        Config::get().login() = "test";
        Config::get().password() = "pzjqUkMnc7vfNHET";
    }
    ~TestApp() {
	    for(int i =0; i < 3; i++){
		Poco::Data::Session session = database::Database::get().create_session();
		Statement drop(session);
		std::string sql_request = "DELETE FROM Person"; 
		sql_request += " -- sharding:"; 
		sql_request += std::to_string(i);
		drop << sql_request, now;
		Statement reset_ai(session);
		sql_request = "ALTER TABLE Person AUTO_INCREMENT = 1" ; 
		sql_request += " -- sharding:"; 
		sql_request += std::to_string(i);
		reset_ai << sql_request, now;
	     }
    }
     void SetUp() {}
     void TearDown() {}

protected:
};

TEST_F(TestApp, TestPerson) {

    database::Person person;
    person.login() = "anikkz";
    person.first_name() = "Anna";
    person.last_name() = "Nikitina";
    person.age() = 24;

    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "INSERT INTO Person (login, first_name, last_name, age) VALUES(?, ?, ?, ?) -- sharding:0\n");

    person.login() = "anikkz_2";
    person.first_name() = "Annaa";
    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "INSERT INTO Person (login, first_name, last_name, age) VALUES(?, ?, ?, ?) -- sharding:1\n");
    person.login() = "anik";
    person.last_name() = "Niki";
    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "INSERT INTO Person (login, first_name, last_name, age) VALUES(?, ?, ?, ?) -- sharding:0\n");

   
    database::Person result = database::Person::read_by_login("anikkz");
    ASSERT_EQ(result.get_first_name(), "Anna");
    ASSERT_EQ(result.get_last_name(), "Nikitina");

    database::Person result2 = database::Person::read_by_login("anik");
    ASSERT_EQ(result2.get_first_name(), "Annaa");
    ASSERT_EQ(result2.get_last_name(), "Niki");

    auto results = database::Person::read_all();
    ASSERT_EQ(results.size(), 3);
    ASSERT_EQ(results.at(0).get_first_name(), "Anna");

    auto results2 = database::Person::search("Ann", "Nik");
    ASSERT_EQ(results2.size(), 3);
    ASSERT_EQ(results2.at(0).get_first_name(), "Anna");

    auto results3 = database::Person::search("Anna", "Nikitina");
    ASSERT_EQ(results3.size(), 1);
    ASSERT_EQ(results3.at(0).get_login(), "anikkz_2");

}


int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
