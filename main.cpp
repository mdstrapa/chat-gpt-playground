#include <iostream>
#include <filesystem>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;
using namespace std::chrono;

void renameFiles(const std::string& directoryPath, const std::string& templateString) {
    fs::path directory(directoryPath);

    if (!fs::is_directory(directory)) {
        std::cerr << "Error: Not a directory" << std::endl;
        return;
    }

    if (fs::is_empty(directory)) {
        std::cout << "Directory is empty. No files to rename." << std::endl;
        return;
    }

    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            std::string fileName = entry.path().filename().string();
            std::string fileExtension = entry.path().extension().string();

            // Extract file name (excluding extension)
            std::string fileNameWithoutExtension = fileName.substr(0, fileName.find_last_of('.'));

            // Generate the new file name using the template and current date
            auto now = system_clock::to_time_t(system_clock::now());
            std::stringstream timeStamp;
            timeStamp << std::put_time(std::localtime(&now), "%Y%m%d%H%M%S");

            std::string newFileName = templateString + "_" + fileNameWithoutExtension + "_" + timeStamp.str() + fileExtension;

            // Create the new file path
            fs::path newFilePath = directory / newFileName;

            // Attempt to rename the file
            try {
                fs::rename(entry.path(), newFilePath);
                std::cout << "File renamed: " << fileName << " -> " << newFileName << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Error renaming file: " << fileName << std::endl;
                std::cerr << e.what() << std::endl;
            }
        }
    }
}

int main() {
    // Specify the directory path and the template for renaming
    std::string directoryPath = "/Users/marcosstrapazon/Downloads/filesToRename";
    std::string fileRenameTemplate = "file";

    // Call the renameFiles function
    renameFiles(directoryPath, fileRenameTemplate);

    return 0;
}
