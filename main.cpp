#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

void updateDoxyfile(const std::string& path) {
    std::ifstream infile(path);
    if (!infile.is_open()) {
        std::cerr << "Failed to open " << path << std::endl;
        return;
    }

    std::string content;
    std::string line;
    while (std::getline(infile, line)) {
        if (line.find("EXTRACT_ALL") == 0)
            line = "EXTRACT_ALL = YES";
        else if (line.find("HAVE_DOT") == 0)
            line = "HAVE_DOT = YES";
        else if (line.find("UML_LOOK") == 0)
            line = "UML_LOOK = YES";
        else if (line.find("CALL_GRAPH") == 0)
            line = "CALL_GRAPH = YES";
        else if (line.find("CALLER_GRAPH") == 0)
            line = "CALLER_GRAPH = YES";
        else if (line.find("DOT_IMAGE_FORMAT") == 0)
            line = "DOT_IMAGE_FORMAT = svg";

        content += line + "\n";
    }

    infile.close();

    std::ofstream outfile(path);
    if (!outfile.is_open()) {
        std::cerr << "Failed to write to " << path << std::endl;
        return;
    }

    outfile << content;
    outfile.close();
}

void extractImages() {
    std::vector<std::string> imageFiles = {
        "inherit_graph_1.svg",
        "classleveldb_1_1_block_1_1_iter__inherit__graph.svg",
        "inherit_graph_13.svg",
        "inherit_graph_2.svg",
        "classleveldb_1_1_d_b_impl__inherit__graph.svg",
        "inherit_graph_78.svg",
        "inherit_graph_80.svg",
        "classleveldb_1_1_internal_filter_policy__inherit__graph.svg",
        "classleveldb_1_1_internal_key_comparator__inherit__graph.svg",
        "classleveldb_1_1_key_converting_iterator__inherit__graph.svg",
        "classleveldb_1_1_mem_table_iterator__inherit__graph.svg",
        "classleveldb_1_1_model_d_b__inherit__graph.svg",
        "classleveldb_1_1_posix_logger__inherit__graph.svg",
        "classleveldb_1_1_snapshot_impl__inherit__graph.svg",
        "struct_comparator__inherit__graph.svg",
        "classleveldb_1_1_special_env__inherit__graph.svg",
        "structleveldb_1_1_comparator__inherit__graph.svg",
        "classleveldb_1_1_version_1_1_level_file_num_iterator__inherit__graph.svg",
        "structleveldb__filterpolicy__t__inherit__graph.svg",
        "classleveldb_1_1_windows_logger__inherit__graph.svg"
    };

    fs::path srcDir = "leveldb/html";
    fs::path destRoot = "important_uml";
    fs::path classDir = destRoot / "classlevel";
    fs::path inheritDir = destRoot / "inherit";
    fs::path extraDir = destRoot / "extra";
    fs::path umlDir = destRoot / "uml";

    fs::create_directories(classDir);
    fs::create_directories(inheritDir);
    fs::create_directories(extraDir);
    fs::create_directories(umlDir);

    for (const auto& file : imageFiles) {
        fs::path srcPath = srcDir / file;
        fs::path destPath;

        if (file.rfind("classleveldb", 0) == 0)
            destPath = classDir / file;
        else if (file.rfind("inherit_graph", 0) == 0)
            destPath = inheritDir / file;
        else
            destPath = extraDir / file;

        if (fs::exists(srcPath)) {
            fs::copy_file(srcPath, destPath, fs::copy_options::overwrite_existing);
            std::cout << "Extracted: " << file << " -> " << destPath << std::endl;
        } else {
            std::cerr << "Missing: " << file << std::endl;
        }
    }

    fs::path pumlSrcDir = "leveldb/figures";
    for (const auto& entry : fs::directory_iterator(pumlSrcDir)) {
        if (entry.path().extension() == ".svg") {
            fs::path destPath = umlDir / entry.path().filename();
            fs::copy_file(entry.path(), destPath, fs::copy_options::overwrite_existing);
            std::cout << "Copied PlantUML SVG: " << entry.path().filename() << std::endl;
        }
    }
}

void calculateLinesOfCode() {
    std::cout << "\nCalculating lines of code using cloc..." << std::endl;
    int result = std::system("cloc . --include-lang=\"C++,C/C++ Header,C,CMake,Makefile\"");
    if (result != 0) {
        std::cerr << "Failed to run cloc. Make sure it is installed and available in PATH." << std::endl;
    }
}

int main() {
    // Step 1: Run Doxygen to generate docs
    std::cout << "Running Doxygen to generate documentation..." << std::endl;
    int result = std::system("cd leveldb && doxygen Doxyfile");
    if (result != 0) {
        std::cerr << "Failed to run Doxygen." << std::endl;
        return 1;
    }

    // Step 2: Extract selected images
    extractImages();

    std::cout << "Image extraction completed." << std::endl;

    // Step 3: Calculate lines of code
    calculateLinesOfCode();

    return 0;
}
