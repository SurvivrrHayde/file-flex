#include "file_operations.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <iomanip>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <cstdlib>
#include <regex>

namespace fs = std::filesystem;

const fs::path SEQUENCE_DIR = fs::path(getenv("HOME")) / ".command_sequences";
const fs::path SEQUENCE_FILE = SEQUENCE_DIR / "sequences.txt";

struct CommandSequence {
    int numArgs;
    std::vector<std::string> commands;
};

std::unordered_map<std::string, CommandSequence> commandSequences;

void checkIfValidDirectory(const std::string& directory) {
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        std::cerr << "Invalid directory: " << directory << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void ensureSequenceDirExists() {
    if (!fs::exists(SEQUENCE_DIR)) {
        fs::create_directories(SEQUENCE_DIR);
    }
}

void loadCommandSequences() {
    ensureSequenceDirExists();
    std::ifstream infile(SEQUENCE_FILE);
    if (!infile) return;

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string name;
        int numArgs;
        iss >> name >> numArgs;
        std::vector<std::string> commands;
        std::string command;
        while (iss >> std::quoted(command)) {
            commands.push_back(command);
        }
        commandSequences[name] = {numArgs, commands};
    }
}

void saveCommandSequences() {
    ensureSequenceDirExists();
    std::ofstream outfile(SEQUENCE_FILE);
    for (const auto& [name, sequence] : commandSequences) {
        outfile << name << " " << sequence.numArgs;
        for (const auto& command : sequence.commands) {
            outfile << " " << std::quoted(command);
        }
        outfile << "\n";
    }
}

void createCommandSequence() {
    std::string name;
    std::cout << "What would you like to name this sequence?: ";
    std::cin >> name;

    std::vector<std::string> commands;
    std::string command;
    std::cin.ignore();
    std::cout << "Enter your commands one at a time (use $1, $2, etc. for arguments). Type 'done' to finish:\n";
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);
        if (command == "done") break;
        commands.push_back(command);
    }

    int numArgs;
    std::cout << "How many arguments does this sequence have?: ";
    std::cin >> numArgs;

    commandSequences[name] = {numArgs, commands};
    saveCommandSequences();
    std::cout << "Saved command sequence: " << name << std::endl;
}

void runCommandSequence(const std::string& name, const std::vector<std::string>& args) {
    if (commandSequences.find(name) == commandSequences.end()) {
        std::cerr << "No command sequence found with name: " << name << std::endl;
        return;
    }

    const auto& sequence = commandSequences[name];
    if (args.size() != static_cast<size_t>(sequence.numArgs)) {
        std::cerr << "Incorrect number of arguments. Expected " << sequence.numArgs << " but got " << args.size() << "." << std::endl;
        return;
    }

    for (const auto& cmd : sequence.commands) {
        std::string command = cmd;
        for (size_t i = 0; i < args.size(); ++i) {
            std::string placeholder = "$" + std::to_string(i + 1);
            size_t pos = 0;
            while ((pos = command.find(placeholder, pos)) != std::string::npos) {
                command.replace(pos, placeholder.length(), args[i]);
                pos += args[i].length();
            }
        }

        std::cout << "Running command: " << command << std::endl;
        system(command.c_str());
    }
}

std::string calculateSHA256(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (!context) {
        throw std::runtime_error("Failed to create EVP_MD_CTX");
    }

    const EVP_MD* md = EVP_sha256();
    if (EVP_DigestInit_ex(context, md, nullptr) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to initialize digest");
    }

    std::vector<char> buffer(8192);
    while (file.read(buffer.data(), buffer.size()) || file.gcount() > 0) {
        if (EVP_DigestUpdate(context, buffer.data(), file.gcount()) != 1) {
            EVP_MD_CTX_free(context);
            throw std::runtime_error("Failed to update digest");
        }
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash = 0;
    if (EVP_DigestFinal_ex(context, hash, &lengthOfHash) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to finalize digest");
    }

    EVP_MD_CTX_free(context);

    std::stringstream ss;
    for (unsigned int i = 0; i < lengthOfHash; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}

void findDuplicates(const std::string& target) {
    checkIfValidDirectory(target);
    std::unordered_map<std::string, std::vector<std::string>> hashToFileMap;

    for (const auto& entry : fs::recursive_directory_iterator(target)) {
        if (fs::is_regular_file(entry.path())) {
            try {
                std::string fileHash = calculateSHA256(entry.path().string());
                hashToFileMap[fileHash].push_back(entry.path().string());
            } catch (const std::exception& e) {
                std::cerr << "Error processing file " << entry.path() << ": " << e.what() << std::endl;
            }
        }
    }

    for (const auto& [hash, files] : hashToFileMap) {
        if (files.size() > 1) {
            std::cout << "Duplicate files for hash " << hash << ":\n";
            for (const auto& file : files) {
                std::cout << "  " << file << "\n";
            }
        }
    }
}

void findDuplicates() {
    findDuplicates(".");
}

void batchRenamePrefix(const std::string& target, const std::string& oldPrefix, const std::string& newPrefix) {
    checkIfValidDirectory(target);
    for (const auto& entry : fs::directory_iterator(target)) {
        std::string filename = entry.path().filename().string();
        if (filename.find(oldPrefix) == 0) {
            std::string newFilename = newPrefix + filename.substr(oldPrefix.size());
            fs::rename(entry.path(), newFilename);
            std::cout << "Renamed: " << filename << " to " << newFilename << std::endl;
        }
    }
}

void batchRenamePrefix(const std::string& oldPrefix, const std::string& newPrefix) {
    batchRenamePrefix(".", oldPrefix, newPrefix);
}

void batchRenamePostfix(const std::string& target, const std::string& oldPostfix, const std::string& newPostfix) {
    checkIfValidDirectory(target);
    for (const auto& entry : fs::directory_iterator(target)) {
        std::string filename = entry.path().filename().string();
        if (filename.find(oldPostfix) == 0) {
            std::string newFilename = newPostfix + filename.substr(oldPostfix.size());
            fs::rename(entry.path(), newFilename);
            std::cout << "Renamed: " << filename << " to " << newFilename << std::endl;
        }
    }
}

void batchRenamePostfix(const std::string& oldPostfix, const std::string& newPostfix) {
    batchRenamePostfix(".", oldPostfix, newPostfix);
}

void deleteAllItems(const std::string& target) {
    checkIfValidDirectory(target);
    for (const auto& entry : fs::directory_iterator(target)) {
        fs::remove_all(entry.path());
        std::cout << "Deleted: " << entry.path() << std::endl;
    }
}

void deleteAllItems() {
    deleteAllItems(".");
}

void syncDirectories(const std::string& source, const std::string& target) {
    checkIfValidDirectory(source);
    if (!fs::exists(target)) {
        fs::create_directories(target);
    }

    for (const auto& entry : fs::recursive_directory_iterator(source)) {
        const auto& sourcePath = entry.path();
        auto relativePath = fs::relative(sourcePath, source);
        auto targetPath = fs::path(target) / relativePath;

        if (fs::is_directory(sourcePath)) {
            if (!fs::exists(targetPath)) {
                fs::create_directory(targetPath);
            }
        } else {
            if (!fs::exists(targetPath) || fs::last_write_time(sourcePath) > fs::last_write_time(targetPath)) {
                fs::copy_file(sourcePath, targetPath, fs::copy_options::overwrite_existing);
                std::cout << "Copied: " << sourcePath << " to " << targetPath << std::endl;
            }
        }
    }

    for (const auto& entry : fs::recursive_directory_iterator(target)) {
        const auto& targetPath = entry.path();
        auto relativePath = fs::relative(targetPath, target);
        auto sourcePath = fs::path(source) / relativePath;

        if (!fs::exists(sourcePath)) {
            fs::remove_all(targetPath);
            std::cout << "Removed: " << targetPath << std::endl;
        }
    }
}

void syncDirectories(const std::string& target) {
    syncDirectories(".", target);
}

void searchFiles(const std::string& target, const std::string& pattern) {
    checkIfValidDirectory(target);
    std::regex regexPattern(pattern);
    for (const auto& entry : fs::recursive_directory_iterator(target)) {
        if (fs::is_regular_file(entry.path())) {
            std::string filename = entry.path().filename().string();
            if (std::regex_search(filename, regexPattern)) {
                std::cout << "Found: " << entry.path() << std::endl;
            }
        }
    }
}

void searchFiles(const std::string& pattern) {
    searchFiles(".", pattern);
}

void calculateDirectorySize(const std::string& target) {
    checkIfValidDirectory(target);
    uintmax_t totalSize = 0;
    for (const auto& entry : fs::recursive_directory_iterator(target)) {
        if (fs::is_regular_file(entry.path())) {
            totalSize += fs::file_size(entry.path());
        }
    }
    std::cout << "Total size of " << target << ": " << totalSize << " bytes" << std::endl;
}

void calculateDirectorySize() {
    calculateDirectorySize(".");
}

void printDirectoryTree(const fs::path& directory, const std::string& prefix = "") {
    for (auto it = fs::directory_iterator(directory); it != fs::directory_iterator(); ++it) {
        std::cout << prefix << (it->is_directory() ? "[D] " : "[F] ") << it->path().filename().string() << "\n";
        if (it->is_directory()) {
            printDirectoryTree(it->path(), prefix + "  ");
        }
    }
}

void createDirectoryTree(const std::string& target) {
    checkIfValidDirectory(target);
    printDirectoryTree(target);
}

void createDirectoryTree() {
    createDirectoryTree(".");
}

void encryptFile(const std::string& inputFile, const std::string& outputFile, const std::string& key) {
    std::ifstream in(inputFile, std::ios::binary);
    std::ofstream out(outputFile, std::ios::binary);

    if (!in || !out) {
        std::cerr << "Failed to open files." << std::endl;
        return;
    }

    unsigned char iv[AES_BLOCK_SIZE];
    memset(iv, 0x00, AES_BLOCK_SIZE);

    const EVP_CIPHER* cipher = EVP_aes_256_cbc();
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, cipher, nullptr, reinterpret_cast<const unsigned char*>(key.data()), iv);

    std::vector<unsigned char> buffer(4096);
    std::vector<unsigned char> outBuffer(buffer.size() + AES_BLOCK_SIZE);
    int outLen;

    while (in.read(reinterpret_cast<char*>(buffer.data()), buffer.size()) || in.gcount() > 0) {
        EVP_EncryptUpdate(ctx, outBuffer.data(), &outLen, buffer.data(), in.gcount());
        out.write(reinterpret_cast<char*>(outBuffer.data()), outLen);
    }

    EVP_EncryptFinal_ex(ctx, outBuffer.data(), &outLen);
    out.write(reinterpret_cast<char*>(outBuffer.data()), outLen);

    EVP_CIPHER_CTX_free(ctx);
}

void decryptFile(const std::string& inputFile, const std::string& outputFile, const std::string& key) {
    std::ifstream in(inputFile, std::ios::binary);
    std::ofstream out(outputFile, std::ios::binary);

    if (!in || !out) {
        std::cerr << "Failed to open files." << std::endl;
        return;
    }

    unsigned char iv[AES_BLOCK_SIZE];
    memset(iv, 0x00, AES_BLOCK_SIZE);

    const EVP_CIPHER* cipher = EVP_aes_256_cbc();
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, cipher, nullptr, reinterpret_cast<const unsigned char*>(key.data()), iv);

    std::vector<unsigned char> buffer(4096);
    std::vector<unsigned char> outBuffer(buffer.size() + AES_BLOCK_SIZE);
    int outLen;

    while (in.read(reinterpret_cast<char*>(buffer.data()), buffer.size()) || in.gcount() > 0) {
        EVP_DecryptUpdate(ctx, outBuffer.data(), &outLen, buffer.data(), in.gcount());
        out.write(reinterpret_cast<char*>(outBuffer.data()), outLen);
    }

    EVP_DecryptFinal_ex(ctx, outBuffer.data(), &outLen);
    out.write(reinterpret_cast<char*>(outBuffer.data()), outLen);

    EVP_CIPHER_CTX_free(ctx);
}

void compareDirectories(const std::string& source, const std::string& target) {
    checkIfValidDirectory(source);
    checkIfValidDirectory(target);
    std::unordered_map<std::string, fs::file_time_type> filesInDir1;
    std::unordered_map<std::string, fs::file_time_type> filesInDir2;

    for (const auto& entry : fs::recursive_directory_iterator(source)) {
        if (fs::is_regular_file(entry.path())) {
            filesInDir1[entry.path().filename().string()] = fs::last_write_time(entry.path());
        }
    }

    for (const auto& entry : fs::recursive_directory_iterator(target)) {
        if (fs::is_regular_file(entry.path())) {
            filesInDir2[entry.path().filename().string()] = fs::last_write_time(entry.path());
        }
    }

    std::cout << "Files only in " << source << ":\n";
    for (const auto& [filename, _] : filesInDir1) {
        if (filesInDir2.find(filename) == filesInDir2.end()) {
            std::cout << "  " << filename << "\n";
        }
    }

    std::cout << "\nFiles only in " << target << ":\n";
    for (const auto& [filename, _] : filesInDir2) {
        if (filesInDir1.find(filename) == filesInDir1.end()) {
            std::cout << "  " << filename << "\n";
        }
    }

    std::cout << "\nFiles with different last modification times:\n";
    for (const auto& [filename, time1] : filesInDir1) {
        if (filesInDir2.find(filename) != filesInDir2.end() && time1 != filesInDir2[filename]) {
            std::cout << "  " << filename << "\n";
        }
    }
}

void compareDirectories(const std::string& target) {
    compareDirectories(".", target);
}

void printHelp() {
    std::cout << "Available commands:\n";
    std::cout << "  create_tree <directory>              : Displays a tree view of the directory structure.\n";
    std::cout << "  create_tree                          : Displays a tree view of the current directory structure.\n";
    std::cout << "  compare_directories <source> <target>: Compares two directories for differences in files.\n";
    std::cout << "  compare_directories <target>         : Compares current directory and target directory for differences in files.\n";
    std::cout << "  batch_rename <oldPrefix> <newPrefix> : Renames files with a specified prefix.\n";
    std::cout << "  delete_all <directory>               : Deletes all items in the specified directory.\n";
    std::cout << "  delete_all                           : Deletes all items in the current directory.\n";
    std::cout << "  sync_directories <source> <target>   : Synchronizes two directories.\n";
    std::cout << "  sync_directories <target>            : Synchronizes the current directory with the target directory.\n";
    std::cout << "  find_duplicates <directory>          : Finds duplicate files in a directory based on content.\n";
    std::cout << "  find_duplicates                      : Finds duplicate files in the current directory based on content.\n";
    std::cout << "  create_sequence                      : Creates a sequence of commands.\n";
    std::cout << "  run_sequence <name> <args...>        : Runs a saved sequence of commands with optional arguments.\n";
    std::cout << "  search_files <directory> <pattern>   : Searches for files in the specified directory that match the given pattern.\n";
    std::cout << "  search_files <pattern>               : Searches for files in the current directory that match the given pattern.\n";
    std::cout << "  calculate_directory_size <directory> : Calculates the total size of the specified directory.\n";
    std::cout << "  calculate_directory_size             : Calculates the total size of the current directory.\n";
    std::cout << "  encrypt_file <inputFile> <outputFile> <key> : Encrypts a file using AES-256-CBC.\n";
    std::cout << "  decrypt_file <inputFile> <outputFile> <key> : Decrypts a file using AES-256-CBC.\n";
}
