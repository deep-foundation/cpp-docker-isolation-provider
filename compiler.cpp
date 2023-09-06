#include "compiler.h"

std::string Compiler::compileAndExecute(const std::string &code, const std::string &jwt, const std::string &gql_urn) {
    srand(static_cast<unsigned int>(time(nullptr)));
    std::string random_folder_name = "tmp/cpp_" + std::to_string(rand());

    std::string source_path = random_folder_name + "/temp.cpp";
    std::string exec_path = random_folder_name + "/temp";

    int create_dir_result = mkdir(random_folder_name.c_str(), 0700);
    if (create_dir_result != 0) {
        return "Failed to create temporary directory.";
    }

    std::string code_template = R"(
#include <iostream>
#include "compiler.h"
#include "DeepClientCppWrapper.cpp"

int* returnArray() {
    static int data[] = {1, 2, 3};
    return data;
}

int main() {
    int* arrayPtr = returnArray();

    auto deepClient = new DeepClientCppWrapper(")"+ jwt + R"(", ")"+ gql_urn + R"(");

    auto deepClientSelect = deepClient->select(std::make_shared<IntValue>(1));
    deepClientSelect->print();

    for (int i = 0; i < 3; ++i) {
        std::cout << arrayPtr[i] << " ";
    }

    return 0;
}
)";

    std::ofstream source_file(source_path);
    if (!source_file) {
        return "Failed to create source file.";
    }
    source_file << code_template;
    source_file.close();

    std::string compile_command = "g++ -o " + exec_path + " " + source_path + " -I. -I./python/ -L.cpp-docker-isolation-provider-lib 2>&1";
    FILE* compile_pipe = popen(compile_command.c_str(), "r");
    if (!compile_pipe) {
        return "Compilation execution failed.";
    }

    std::string compile_output;
    char compile_buffer[128];
    while (fgets(compile_buffer, sizeof(compile_buffer), compile_pipe) != nullptr) {
        compile_output += compile_buffer;
    }

    int compile_result = pclose(compile_pipe);
    if (compile_result != 0) {
        return "Compilation failed:\n" + compile_output;
    }

    const std::string& execute_command = exec_path;
    std::string execute_output;
    {
        FILE* execute_pipe = popen(execute_command.c_str(), "r");
        if (!execute_pipe) {
            return "Execution failed.";
        }

        char buffer[128];
        while (fgets(buffer, sizeof(buffer), execute_pipe) != nullptr) {
            execute_output += buffer;
        }

        pclose(execute_pipe);
    }

    /*std::string remove_dir_command = "rm -r " + random_folder_name;
    int remove_dir_result = system(remove_dir_command.c_str());
    if (remove_dir_result != 0) {
        return "Failed to remove temporary directory.";
    }*/

    return execute_output;
}
