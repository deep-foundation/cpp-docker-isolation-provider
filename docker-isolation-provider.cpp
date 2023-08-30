#include "compiler.h"

int main(void) {
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
        try {
            json json_obj = json::parse(json_data);
            std::string code = json_obj["params"]["code"].get<std::string>();

            std::string result = Compiler::compileAndExecute(code);
            res.set_content(result, "application/json");
        } catch (const std::exception& e) {
            res.set_content("Invalid JSON format: " + std::string(e.what()), "application/json");
        }
    });

    svr.Post("/http-call", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("{}", "application/json");
    });

    if (port) {
        std::cout << "PORT environment variable value: " << port << std::endl;
        svr.listen("0.0.0.0", std::stoi(port));
    } else {
        std::cout << "PORT environment variable not set." << std::endl;
        svr.listen("0.0.0.0", 29080);
    }
    return 0;
}
