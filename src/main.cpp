#include <iostream>
#include <string>
#include <vector>
#include "file_operations.h"

std::vector<std::string> parseArguments(int argc, char* argv[], int startIndex) {
    std::vector<std::string> args;
    for (int i = startIndex; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    return args;
}

int main(int argc, char* argv[]) {
    loadCommandSequences();

    if (argc < 2) {
        std::cerr << "Usage: <command> <args>" << std::endl;
        return 1;
    }

    std::string command = argv[1];
    if (command == "batch_rename_prefix") {
        if (argc == 4) {
            batchRenamePrefix(argv[2], argv[3]);
        } else if (argc == 5){
            batchRenamePrefix(argv[2], argv[3], argv[4]);
        } else {
            std::cerr << "Usage: batch_rename_prefix <target> <old_prefix> <new_prefix> or batch_rename_prefix <old_prefix> <new_prefix>" << std::endl;
            return 1;
        }
    } else if (command == "batch_rename_postfix") {
        if (argc == 4) {
            batchRenamePostfix(argv[2], argv[3]);
        } else if (argc == 5){
            batchRenamePostfix(argv[2], argv[3], argv[4]);
        } else {
            std::cerr << "Usage: batch_rename_postfix <target> <old_prefix> <new_prefix> or batch_rename_prefix <old_prefix> <new_prefix>" << std::endl;
            return 1;
        }
    } else if (command == "delete_all") {
        if (argc == 2) {
            deleteAllItems();
        } else if (argc == 3) {
            deleteAllItems(argv[2]);
        } else {
            std::cerr << "Usage: delete_all or delete_all <target>" << std::endl;
            return 1;
        }
    } else if (command == "sync_directories") {
        if (argc == 3) {
            syncDirectories(argv[2]);
        } else if (argc == 4) {
            syncDirectories(argv[2], argv[3]);
        } else {
            std::cerr << "Usage: sync_directories <source> <target> or sync_directories <target>" << std::endl;
            return 1;
        }
    } else if (command == "find_duplicates") {
        if (argc == 3) {
            findDuplicates(argv[2]);
        } else if (argc == 2) {
            findDuplicates();
        } else {
            std::cerr << "Usage: find_duplicates or find_duplicates <target>" << std::endl;
            return 1;
        }
    } else if (command == "create_sequence") {
        if (argc == 2) {
            createCommandSequence();
        } else {
            std::cerr << "Usage: create_sequence" << std::endl;
            return 1;
        }
    } else if (command == "run_sequence") {
        if (argc >= 3) {
            std::string name = argv[2];
            std::vector<std::string> args = parseArguments(argc, argv, 3);
            runCommandSequence(name, args);
        } else {
            std::cerr << "Usage: run_sequence <sequence_name> <argument 1> <argument 2> ... <argument n>" << std::endl;
            return 1;
        }
    } else if (command == "search_files") {
        if (argc == 3) {
            searchFiles(argv[2]);
        } else if (argc == 4) {
            searchFiles(argv[2], argv[3]);
        } else {
            std::cerr << "Usage: search_files <regex> or search_files <target> <regex>" << std::endl;
            return 1;
        }
    } else if (command == "calculate_directory_size") {
        if (argc == 2) {
            calculateDirectorySize();
        } else if (argc == 3) {
            calculateDirectorySize(argv[2]);
        } else {
            std::cerr << "Usage: calculate_directory_size or calculate_directory_size <target>" << std::endl;
            return 1;
        }
    } else if (command == "create_tree") {
        if (argc == 2) {
            createDirectoryTree();
        } else if (argc == 3) {
            createDirectoryTree(argv[2]);
        } else {
            std::cerr << "Usage: create_tree or create_tree <target>" << std::endl;
            return 1;
        }
    } else if (command == "compare_directories") {
        if (argc == 3) {
            compareDirectories(argv[2]);
        } else if (argc == 4) {
            compareDirectories(argv[2], argv[3]);
        } else {
            std::cerr << "Usage: compare_directories <target> or compare_directories <source> <target>" << std::endl;
            return 1;
        }
    } else if (command == "encrypt_file") {
        if (argc == 5) {
            encryptFile(argv[2], argv[3], argv[4]);
        } else {
            std::cerr << "Usage: encrypt_file <inputFile> <outputFile> <key>" << std::endl;
            return 1;
        }
    } else if (command == "decrypt_file") {
        if (argc == 5) {
            decryptFile(argv[2], argv[3], argv[4]);
        } else {
            std::cerr << "Usage: decrypt_file <inputFile> <outputFile> <key>" << std::endl;
            return 1;
        }
    } else if (command == "help") {
        printHelp();
    } else {
        std::cerr << "Unknown command. Use command \"help\" to discover other commands." << std::endl;
    }
    return 0;
}
