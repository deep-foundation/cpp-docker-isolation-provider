#include "compiler.h"

int main(void) {
    using namespace httplib;
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
        std::string result = Compiler::compileAndExecute(json_data);
        res.set_content(result, "application/json");
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
