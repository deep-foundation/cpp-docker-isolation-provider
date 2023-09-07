#include "compiler.h"

std::string escapeDoubleQuotes(const std::string& input) {
    std::string result;
    for (char c : input) {
        if (c == '"') {
            result += "\\\"";
        } else {
            result += c;
        }
    }
    return result;
}


json Compiler::compileAndExecute(const std::string &code, const std::string &jwt, const std::string &gql_urn,
                                 const std::string &jsonData) {
    std::string random_folder_name = "tmp/cpp_" + std::to_string(rand());

    std::string source_path = random_folder_name + "/temp.cpp";
    std::string exec_path = random_folder_name + "/temp";

    int create_dir_result = mkdir(random_folder_name.c_str(), 0700);
    if (create_dir_result != 0) {
        return {{"rejected", "Failed to create temporary directory: " + random_folder_name}};
    }

    std::string code_template = R"(
#include <iostream>
#include "compiler.h"
#include "DeepClientCppWrapper.cpp"
#include <stdexcept>

)"+ code + R"(

int main() {
    try {
        auto deepClient = new DeepClientCppWrapper(")"+ jwt + R"(", ")"+ gql_urn + R"(");
        auto params = new HandlerParameters(deepClient, ")"+ escapeDoubleQuotes(jsonData) + R"(");
        std::cout << fn(params) << std::endl;
        delete deepClient;
        return 0;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}
)";

    std::ofstream source_file(source_path);
    if (!source_file) {
        return {{"rejected", "Failed to create source file."}};
    }
    source_file << code_template;
    source_file.close();

    std::string compile_command = "g++ -o " + exec_path + " " + source_path + " -I. -I./python/ "
                                                                              "-L. -lprovider-cpp "
                                                                              "-L/usr/lib/x86_64-linux-gnu/ -lpython3.10 2>&1";

    FILE* compile_pipe = popen(compile_command.c_str(), "r");
    if (!compile_pipe) {
        return {{"rejected", "Compilation execution failed."}};
    }

    std::string compile_output;
    char compile_buffer[128];
    while (fgets(compile_buffer, sizeof(compile_buffer), compile_pipe) != nullptr) {
        compile_output += compile_buffer;
    }

    int compile_result = pclose(compile_pipe);
    if (compile_result != 0) {
        return {{"rejected", "Compilation failed:\n" + compile_output}};
    }

    const std::string& execute_command = "LD_LIBRARY_PATH=. " + exec_path;
    try {
        std::string execute_output;
        {
            FILE* execute_pipe = popen(execute_command.c_str(), "r");
            if (!execute_pipe) {
                return {{"rejected", "Execution failed."}};
            }

            char buffer[128];
            while (fgets(buffer, sizeof(buffer), execute_pipe) != nullptr) {
                execute_output += buffer;
            }

            pclose(execute_pipe);
        }

        std::string remove_dir_command = "rm -r " + random_folder_name;
        int remove_dir_result = system(remove_dir_command.c_str());
        if (remove_dir_result != 0) {
            return {{"rejected", "Failed to remove temporary directory."}};
        }

        const std::regex regex_pattern("(error|exception|failed|Error|Exception)");
        if (std::regex_search(execute_output, regex_pattern)) {
            return {{"rejected", "Runtime error: " + execute_output}};
        } else {
            return {{"resolved", execute_output}};
        }
    } catch (const std::runtime_error& e) {
        return {{"rejected", e.what()}};
    }
}
