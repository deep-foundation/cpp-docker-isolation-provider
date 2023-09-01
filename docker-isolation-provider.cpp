#include "compiler.h"
#include "DeepClientCppWrapper.cpp"

bool serverRunning = true;

void signalHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "Received SIGINT, stopping server..." << std::endl;
        serverRunning = false;
    }
}

int main(void) {
    signal(SIGINT, signalHandler);

    using namespace httplib;
    using json = nlohmann::json;
    const char* port = std::getenv("PORT");

    Server svr;

    svr.Get("/healthz", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("{}", "application/json");
    });

    svr.Post("/init", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("{}", "application/json");
    });

    svr.Post("/call", [](const httplib::Request& req, httplib::Response &res) {
        const auto& json_data = req.body;
        const char* gql_urn = std::getenv("GQL_URN");
        std::string gql_urn_str = gql_urn ? std::string(gql_urn) : "http://192.168.0.135:3006/gql";

        try {
            json json_obj = json::parse(json_data);
            std::string code = json_obj["params"]["code"].get<std::string>();
            DeepClientCppWrapper deepClient(json_obj["params"]["jwt"].get<std::string>(), gql_urn_str);

            std::string result = Compiler::compileAndExecute(code, deepClient);
            res.set_content(result, "application/json");
        } catch (const std::exception& e) {
            res.set_content("Invalid JSON format: " + std::string(e.what()), "application/json");
        }
    });

    svr.Post("/http-call", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("{}", "application/json");
    });

    while (serverRunning) {
        if (port) {
            std::cout << "PORT environment variable value: " << port << std::endl;
            svr.listen("0.0.0.0", std::stoi(port));
        } else {
            std::cout << "PORT environment variable not set." << std::endl;
            svr.listen("0.0.0.0", 29080);
        }
    }

    std::cout << "Server has been stopped." << std::endl;
    return 0;
}
