#include <boost/program_options.hpp>
#include <iostream>
#include <csignal>
#include <cppkafka/consumer.h>
#include <cppkafka/configuration.h>
#include "config/config.h"

#include "database/person.h"
namespace po = boost::program_options;

bool running = true;

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    try
    {
        po::options_description desc{"Options"};
        desc.add_options()("help,h", "This screen")("host,", po::value<std::string>()->required(), "set db host")("port,", po::value<std::string>()->required(), "databaase port")("login,", po::value<std::string>()->required(), "database login")("password,", po::value<std::string>()->required(), "database password")("database,", po::value<std::string>()->required(), "database name")("queue,", po::value<std::string>()->required(), "queue url")("topic,", po::value<std::string>()->required(), "topic name")("group_id,", po::value<std::string>()->required(), "consumer group_id name")("cache_servers,", po::value<std::string>()->required(), "iginite cache servers");

        po::variables_map vm;
        po::store(parse_command_line(argc, argv, desc), vm);

        if (vm.count("help"))
            std::cout << desc << '\n';

        if (vm.count("host"))
            Config::get().host() = vm["host"].as<std::string>();
        if (vm.count("port"))
            Config::get().port() = vm["port"].as<std::string>();
        if (vm.count("login"))
            Config::get().login() = vm["login"].as<std::string>();
        if (vm.count("password"))
            Config::get().password() = vm["password"].as<std::string>();
        if (vm.count("database"))
            Config::get().database() = vm["database"].as<std::string>();
        if (vm.count("queue"))
            Config::get().queue_host() = vm["queue"].as<std::string>();
        if (vm.count("topic"))
            Config::get().queue_topic() = vm["topic"].as<std::string>();
        if (vm.count("group_id"))
            Config::get().queue_group_id() = vm["group_id"].as<std::string>();


        // Stop processing on SIGINT
        signal(SIGINT, [](int) { running = false; });

        // Construct the configuration
        cppkafka::Configuration config = {
                {"metadata.broker.list", Config::get().get_queue_host()},
                {"group.id", Config::get().get_queue_group_id()},
                // Disable auto commit
                {"enable.auto.commit", false}};

        // Create the consumer
        cppkafka::Consumer consumer(config);

        // Print the assigned partitions on assignment
        consumer.set_assignment_callback([](const cppkafka::TopicPartitionList &partitions) {
            std::cout << "Got assigned: " << partitions << std::endl;
        });

        // Print the revoked partitions on revocation
        consumer.set_revocation_callback([](const cppkafka::TopicPartitionList &partitions) {
            std::cout << "Got revoked: " << partitions << std::endl;
        });

        // Subscribe to the topic
        consumer.subscribe({Config::get().get_queue_topic()});

        std::cout << "Consuming messages from topic " << Config::get().get_queue_topic() << std::endl;

        // Now read lines and write them into kafka
        while (running)
        {
            // Try to consume a message
            cppkafka::Message msg = consumer.poll();
            if (msg)
            {
                // If we managed to get a message
                if (msg.get_error())
                {
                    // Ignore EOF notifications from rdkafka
                    if (!msg.is_eof())
                    {
                        std::cout << "[+] Received error notification: " << msg.get_error() << std::endl;
                    }
                }
                else
                {
                    // Print the key (if any)
                    if (msg.get_key())
                    {
                        std::cout << msg.get_key() << " -> ";
                    }
                    // Print the payload
                    std::string payload = msg.get_payload();
                    std::cout << msg.get_payload() << std::endl;
                    database::Person p = database::Person::fromJSON(payload);
                    p.save_to_mysql();

                    // Now commit the message
                    consumer.commit(msg);
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 1;
}
