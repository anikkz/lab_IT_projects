#include "cache.h"
#include "../config/config.h"

#include <ignite/thin/ignite_client.h>
#include <ignite/thin/ignite_client_configuration.h>
#include <ignite/thin/cache/cache_peek_mode.h>
#include <exception>

static ignite::thin::IgniteClient _client;
static ignite::thin::cache::CacheClient<std::string, std::string> _cache;

namespace database
{
    Cache::Cache()
    {
        ignite::thin::IgniteClientConfiguration cfg;
        cfg.SetEndPoints(Config::get().get_cache_servers());
        cfg.SetPartitionAwareness(true);
        try
        {
            _client = ignite::thin::IgniteClient::Start(cfg);
            _cache = _client.GetOrCreateCache<std::string, std::string>("persons");
        }
        catch (ignite::IgniteError &err)
        {
            std::cout << "error:" << err.what() << std::endl;
            throw;
        }
    }

    Cache Cache::get()
    {
        static Cache instance;
        return instance;
    }

    void Cache::put(std::string key, const std::string& val){
        _cache.Put(key,val);
    }

    void Cache::remove(std::string key){
        _cache.Remove(key);
    }

    size_t Cache::size(){
        return _cache.GetSize(ignite::thin::cache::CachePeekMode::ALL);
    }

    void Cache::remove_all(){
        _cache.RemoveAll();;
    }

    bool Cache::get(std::string key, std::string& val){
        try{
            val = _cache.Get(key);
            return true;
        }catch(...){
        std::cout << "dsfsd" << std::endl;
            throw std::logic_error("key not found in cache");
        }
    }
}
