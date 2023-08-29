#include "compiler.h"

std::string Compiler::compileAndExecute(const std::string &code) {
    std::string temp_dir = "/tmp";
    std::string source_path = temp_dir + "/temp.cpp";
    std::string exec_path = temp_dir + "/temp";

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

    //std::remove(source_path.c_str());
    //std::remove(exec_path.c_str());

    return execute_output;
}
