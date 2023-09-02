#include "compiler.h"
#include "DeepClientCppWrapper.cpp"

using namespace httplib;
using json = nlohmann::json;

static DeepClientCppWrapper* deepClient = nullptr;
static int objectCreated = 0;

void handlePostCall(const httplib::Request& req, httplib::Response &res) {
    const auto& json_data = req.body;
    const char* gql_urn = std::getenv("GQL_URN");
    std::string gql_urn_str = gql_urn ? std::string(gql_urn) : "http://192.168.0.135:3006/gql";

    try {
        /*json json_obj = json::parse(json_data);
        std::string code = json_obj["params"]["code"].get<std::string>();
        auto deepClient = new DeepClientCppWrapper(json_obj["params"]["jwt"].get<std::string>(), gql_urn_str);
        std::string result = Compiler::compileAndExecute(code, deepClient);
        delete deepClient;*/
        json json_obj = json::parse(json_data);
        std::string code = json_obj["params"]["code"].get<std::string>();
        std::string result = "test";
        if (objectCreated) {
            result = "objectCreated";
        } else {
            deepClient = new DeepClientCppWrapper(json_obj["params"]["jwt"].get<std::string>(), gql_urn_str);
            objectCreated = 1;
        }
        //std::string result = deepClient->select(std::make_shared<IntValue>(1));
        res.set_content(result, "application/json");
    } catch (const std::exception& e) {
        res.set_content("Invalid JSON format: " + std::string(e.what()), "application/json");
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
