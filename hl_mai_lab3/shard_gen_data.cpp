#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

int main() {
    size_t max_shards = 3;
    size_t shard_number;

    std::string line;
    std::ifstream infile("infrastructure/db_prepare/gen_data100k.sql");
    std::ofstream outfile("infrastructure/db_prepare/sharded_gen_data100k.sql");

    std::string content1;
    std::string content2;
    std::string content3;

    content1 = "use sql_test;\nINSERT INTO Person (login, first_name,last_name,age) VALUES\n";
    content2 = "use sql_test;\nINSERT INTO Person (login, first_name,last_name,age) VALUES\n";
    content3 = "use sql_test;\nINSERT INTO Person (login, first_name,last_name,age) VALUES\n";

    std::smatch match;
    std::regex pattern("([0-9]{3}-[0-9]{2}-[0-9]{4})");

    while (std::getline(infile, line)) {
        if (std::regex_search(line, match, pattern)) {
            shard_number = std::hash<std::string>{}(match[0]) % max_shards;
//            std::cout << match[0] << line << shard_number << "\n";
            switch (shard_number) {
                case 0:
                    content1+= line +'\n';
                    break;
                case 1:
                    content2+= line +'\n';
                    break;
                case 2:
                    content3+= line +'\n';
                    break;
            }
        };
    }
    content1.pop_back();
    content1.pop_back();
    content1 += "\n -- sharding:0\n;\n";
    content2.pop_back();
    content2.pop_back();
    content2 += "\n -- sharding:1\n;\n";
    content3.pop_back();
    content3.pop_back();
    content3 += "\n -- sharding:2\n;\n";

    outfile << content1 << content2 << content3 <<std::endl;

    infile.close();
    outfile.close();
    return 0;
}