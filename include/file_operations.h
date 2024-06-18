#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <string>
#include <vector>

void batchRenamePrefix(const std::string& target, const std::string& oldPrefix, const std::string& newPrefix);
void batchRenamePrefix(const std::string& oldPrefix, const std::string& newPrefix);

void batchRenamePostfix(const std::string& target, const std::string& oldPostfix, const std::string& newPostfix);
void batchRenamePostfix(const std::string& oldPostfix, const std::string& newPostfix);

void deleteAllItems(const std::string& target);
void deleteAllItems();

void syncDirectories(const std::string& source, const std::string& target);
void syncDirectories(const std::string& target);

void findDuplicates(const std::string& target);
void findDuplicates();

void loadCommandSequences();
void createCommandSequence();
void runCommandSequence(const std::string& name, const std::vector<std::string>& args);

void searchFiles(const std::string& target, const std::string& pattern);
void searchFiles(const std::string& pattern);

void calculateDirectorySize(const std::string& target);
void calculateDirectorySize();

void createDirectoryTree(const std::string& target);
void createDirectoryTree();

void compareDirectories(const std::string& source, const std::string& target);
void compareDirectories(const std::string& target);

void encryptFile(const std::string& inputFile, const std::string& outputFile, const std::string& key);

void decryptFile(const std::string& inputFile, const std::string& outputFile, const std::string& key);

void printHelp();

#endif // FILE_OPERATIONS_H