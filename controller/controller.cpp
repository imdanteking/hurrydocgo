/*
 * @Description: 
 * @version: 
 * @Author: @imdanteking
 * @Date: 2022-11-24 19:43:38
 * @LastEditTime: 2022-11-28 16:39:39
 */
#include "controller.h"
namespace hurrydocgo
{

    /**
     * @brief Construct a new Controller:: Controller object
     * 
     */
    Controller::Controller(): m_searcher(new Searcher) {}

    /**
     * @brief Destroy the Controller:: Controller object
     * 
     */
    Controller::~Controller() {
        delete m_searcher;
    }

    /**
     * @brief Init controller 
     * 
     */
    void Controller::Init()
    {
        BuildPreProcessor();
        BuildSearcher();
        BuildQueryService();
    }

    /**
     * @brief build forword_index, inverted_index and searcher
     * 
     */
    void Controller::BuildPreProcessor()
    {
        // enumerate urls
        std::vector<std::string> file_list;
        bool ret = hurrydocgo::PreProcessor::EnumFile(hurrydocgo::g_input_path, &file_list);

        if (!ret)
        {
            std::cout << "Enumerate failing..." << std::endl;
            return;
        }

        // traverse the html directory and deal with every html file
        int ans = 0;
        std::ofstream output_file(hurrydocgo::g_output_path.c_str());
        if (!output_file.is_open())
        {
            std::cout << "open output_file failing..." << std::endl;
        }
        for (auto file_path = file_list.begin(); file_path != file_list.end(); ++file_path)
        {
            std::cout << *file_path << std::endl;
            ++ans;
            // create DocInfo structure to store every html file infomation
            DocInfo doc_info;
            //
            ret = hurrydocgo::PreProcessor::ParseFile(*file_path, &doc_info);
            if (!ret)
            {
                std::cout << "Parse file failing:" << *file_path << std::endl;
                continue;
            }

            // wirte dec_info into file
            hurrydocgo::PreProcessor::WriteOutput(doc_info, output_file);
        }
        std::cout << "There are " << ans << " html files in total..." << std::endl;
    }

    /**
     * @brief build http server and provide search service
     * 
     */
    void Controller::BuildSearcher()
    {
        // 初始化构建索引
        m_searcher->Init(g_output_path);
    }

    /**
     * @brief build http server and provide search service
     * 
     */
    void Controller::BuildQueryService()
    {
        std::cout << "starting server..." << std::endl;
        using namespace httplib;
        Server server;
        std::cout << "server is running on http://127.0.0.1:10002" << std::endl;
        server.Get("/searcher", [this](const Request &req, Response &resp) {
            (void)req;
            if(!req.has_param("query")) {
              resp.set_content("Invalid Paramenter", "text/plain; charset=utf-8");
            }
		    std::string query = req.get_param_value("query");
		    std::string results;
            m_searcher->Search(query, &results);
            resp.set_content(results, "application/json; charset=utf-8"); 
        });
        // set static resources path
        server.set_base_dir("../wwwroot");
        // 3.Start Server
        server.listen("0.0.0.0", 10002);
        std::cout << "start server success"<< std::endl;
    }

} // namespace end

int main()
{
    hurrydocgo::Controller *p_controller = new hurrydocgo::Controller;
    p_controller->Init();
    delete p_controller;
    return 0;
}
