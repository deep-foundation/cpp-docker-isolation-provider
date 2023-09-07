#include "compiler.h"
#include "DeepClientCppWrapper.cpp"
#include <thread>
#include <mutex>

using namespace httplib;

std::mutex compile_mutex;

void compileAndExecuteThread(const std::string &code, const std::string &jwt, const std::string &gql_urn,
                             const std::string &jsonData, json &result) {
    try {
        json compile_result = Compiler::compileAndExecute(code, jwt, gql_urn, jsonData);
        std::lock_guard<std::mutex> lock(compile_mutex);
        result = compile_result;
    } catch (const std::exception &e) {
        std::lock_guard<std::mutex> lock(compile_mutex);
        result = {{"rejected", "Compilation or execution error: " + std::string(e.what())}};
    }
}

void handlePostCall(const httplib::Request& req, httplib::Response &res) {
    const auto& json_data = req.body;
    const char* gql_urn = std::getenv("GQL_URN");
    const std::string gql_urn_str = gql_urn ? std::string(gql_urn) : "http://192.168.0.135:3006/gql";

    try {
        json json_obj = json::parse(json_data);
        std::string code = json_obj["params"]["code"].get<std::string>();

        std::vector<std::thread> threads;

        std::vector<json> thread_results(json_obj["params"]["data"].size());

        for (size_t i = 0; i < json_obj["params"]["data"].size(); ++i) {
            threads.emplace_back(compileAndExecuteThread, code, json_obj["params"]["jwt"].get<std::string>(), gql_urn_str, json_obj["params"]["data"][i].dump(), std::ref(thread_results[i]));
        }

        for (auto &thread : threads) {
            thread.join();
        }

        json result_array = json::array();
        for (const auto &thread_result : thread_results) {
            result_array.push_back(thread_result);
        }

        res.set_content(result_array.dump(), "application/json");
    } catch (const std::exception& e) {
        json error_json = {{"rejected", "Invalid JSON format: " + std::string(e.what())}};
        res.set_content(error_json.dump(), "application/json");
    }
}

int main(void) {
    const char* port = std::getenv("PORT");

    Server svr;

    svr.Post("/stop-server", [&](const httplib::Request &, httplib::Response &res) {
        std::cout << "Received request to stop the server." << std::endl;
        svr.stop();
        res.set_content("Server is stopping...", "text/plain");
    });

    svr.Get("/healthz", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("{}", "application/json");
    });

    svr.Post("/init", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("{}", "application/json");
    });

    svr.Post("/call", handlePostCall);

    svr.Post("/http-call", handlePostCall);

    if (port) {
        std::cout << "PORT environment variable value: " << port << std::endl;
        svr.listen("0.0.0.0", std::stoi(port));
    } else {
        std::cout << "PORT environment variable not set." << std::endl;
        svr.listen("0.0.0.0", 29080);
    }

    std::cout << "Server has been stopped." << std::endl;
    return 0;
}
