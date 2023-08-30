#include "compiler.h"

std::string Compiler::compileAndExecute(const std::string &code) {
    srand(static_cast<unsigned int>(time(nullptr)));
    std::string random_folder_name = "/tmp/cpp_" + std::to_string(rand());

    std::string source_path = random_folder_name + "/temp.cpp";
    std::string exec_path = random_folder_name + "/temp";

    int create_dir_result = mkdir(random_folder_name.c_str(), 0700);
    if (create_dir_result != 0) {
        return "Failed to create temporary directory.";
    }

    std::ofstream source_file(source_path);
    if (!source_file) {
        return "Failed to create source file.";
    }
    source_file << code;
    source_file.close();

    std::string compile_command = "g++ -o " + exec_path + " " + source_path + " 2>&1";
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

    std::string execute_command = exec_path;
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
