#ifndef CACHE_H
#define CACHE_H

#include <string>

namespace database
{
    class Cache
    {
    private:
        Cache();

    public:
        static Cache get();
        void put(std::string key, const std::string& val);
        bool get(std::string key, std::string& val);
        size_t size();
        void remove(std::string key);
        void remove_all();
    };
}

#endif