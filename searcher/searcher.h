#include <iostream>                                           
#include <stdint.h>
#include <unistd.h>
#include <string>
#include <vector>                                  
#include <unordered_map>      
#include <algorithm>
#include "../third_party/jsoncpp/include/json/json.h"
#include "../index/index.h"
                                                     

namespace hurrydocgo{
  class Searcher {
    private:

      Index* index;

    public:

      Searcher() : index(new Index()){}

      bool Init(const std::string& input_path);

      bool Search(const std::string& query, std::string* output);
 
    private:

      string GenerateDescription(const std::string& content, const std::string& word);

  };
} // namesapce end