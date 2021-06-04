#include <gtest/gtest.h>
#include "config/config.h"
#include "database/database.h"
#include "database/person.h"
//#include "web_server/http_request_factory.h"
//#include "web_server/handlers/person_handler.h"
#include <Poco/Data/SessionFactory.h>
//#include "Poco/Net/HTTPServerRequestImpl.h"
//#include "Poco/Net/SocketAddress.h"

using Poco::Data::Session;
using Poco::Data::Statement;
using namespace Poco::Data::Keywords;
//using Poco::Net::HTTPServerRequestImpl;
//using Poco::Net::SocketAddress;
//using Poco::Net::HTTPServerResponse;

class TestApp : public ::testing::Test {
protected:
    TestApp() {
        Config::get().host() = "127.0.0.1";
        Config::get().database() = "itprojects_labs_test";
        Config::get().port() = "3306";
        Config::get().login() = "test";
        Config::get().password() = "pzjqUkMnc7vfNHET";
    }
    ~TestApp() {
        Poco::Data::Session session = database::Database::get().create_session();
        Statement drop(session);
        drop << "DELETE FROM Person", now;
        Statement reset_ai(session);
        reset_ai << "ALTER TABLE Person AUTO_INCREMENT = 1", now;
    }
     void SetUp() {}
     void TearDown() {}

protected:
};

//class TestServerRequest : public HTTPServerRequest
//{
//    TestServerRequest();
//    ~TestServerRequest();
//    std::istream& stream();
//
//    const SocketAddress& clientAddress() {}const;
//    const SocketAddress& serverAddress() {} const;
//    const HTTPServerParams& serverParams() const;
//    HTTPServerResponse& response() const;
//    bool secure() const;
//};

//TEST(TestApp, FunctionalTest) {
//    HTTPRequestFactory requestFactory("");
//    HTTPServerRequestImpl httpServerRequest(HTTPRequest::HTTP_GET, "http://localhost:8080/person?login=alex-12345", HTTPRequest::HTTP_1_1);
//    TestServerRequest request();
//    HTTPRequestHandler* requestHandler = requestFactory.createRequestHandler(request);
//    ASSERT_TRUE(true);
//}

TEST_F(TestApp, TestPerson) {

    database::Person person;
    person.login() = "anikkz";
    person.first_name() = "Anna";
    person.last_name() = "Nikitina";
    person.age() = 24

    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:1\n");

    person.login() = "anikkz_2";
    person.first_name() = "Annaa";
    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:2\n");
    person.login() = "anik";
    person.last_name() = "Niki";
    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:3\n");

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