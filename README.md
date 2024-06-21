# File Flex

## About

File-Flex is a cross-platform tool designed to simplify file system operations, making it easy for users to manage files and directories.
## Installation
### macOS
To install File-Flex using Homebrew, run the following commands:

```bash
brew tap survivrrHayde/homebrew-file-flex
brew install file-flex
```
### Linux
To install File-Flex using Homebrew, run the following commands:

```bash
brew tap survivrrHayde/homebrew-file-flex
brew install file-flex
```

Add the following line to your .bash_profile or .bashrc:

```bash
export PATH="/home/linuxbrew/.linuxbrew/bin:$PATH"
```

Then, source your profile to update your current session:

```bash
source ~/.bashrc  # or source ~/.bash_profile
```

### Windows
Not available at the moment.
## Usage

### Directory Structure Operations

#### Create Tree View
- **Usage**:
  - `create_tree <directory>`: Displays a tree view of the specified directory structure.
  - `create_tree`: Displays a tree view of the current directory structure.

#### Compare Directories
- **Usage**:
  - `compare_directories <source> <target>`: Compares two directories for differences in files.
  - `compare_directories <target>`: Compares the current directory with the target directory for differences in files.

### File Operations

#### Batch Rename
- **Usage**:
  - `batch_rename <oldPrefix> <newPrefix>`: Renames files with a specified prefix.

#### Delete All
- **Usage**:
  - `delete_all <directory>`: Deletes all items in the specified directory.
  - `delete_all`: Deletes all items in the current directory.

#### Sync Directories
- **Usage**:
  - `sync_directories <source> <target>`: Synchronizes two directories.
  - `sync_directories <target>`: Synchronizes the current directory with the target directory.

#### Find Duplicates
- **Usage**:
  - `find_duplicates <directory>`: Finds duplicate files in a specified directory based on content.
  - `find_duplicates`: Finds duplicate files in the current directory based on content.

### Utility Functions

#### Create Sequence
- **Usage**:
  - `create_sequence`: Creates a sequence of commands.

#### Run Sequence
- **Usage**:
  - `run_sequence <name> <args...>`: Runs a saved sequence of commands with optional arguments.

#### Search Files
- **Usage**:
  - `search_files <directory> <pattern>`: Searches for files in the specified directory that match the given pattern.
  - `search_files <pattern>`: Searches for files in the current directory that match the given pattern.

#### Calculate Directory Size
- **Usage**:
  - `calculate_directory_size <directory>`: Calculates the total size of the specified directory.
  - `calculate_directory_size`: Calculates the total size of the current directory.

### Encryption and Decryption
***Parameters***:
- `<inputFile>`: The path to the file that needs to be encrypted.
- `<outputFile>`: The path where the encrypted file will be saved.
- `<key>`: A secret key used for the encryption. This key should be exactly 32 bytes long (256 bits) and must be kept secure.

#### Encrypt File
- **Usage**:
  - `encrypt_file <inputFile> <outputFile> <key>`: Encrypts a file using AES-256-CBC.

#### Decrypt File
- **Usage**:
  - `decrypt_file <inputFile> <outputFile> <key>`: Decrypts a file using AES-256-CBC.

## Contributing

Thank you for your interest in contributing to File-Flex. I value your contributions and want to make sure that the process is as easy and transparent as possible. Whether you're reporting a bug, suggesting an enhancement, or submitting a pull request. Please refer to my [contributing guide](https://github.com/SurvivrrHayde/file-flex/blob/main/CONTRIBUTING.md).
## Feedback and Community

At the moment there is not much of a community! But, feel free to contact me at wsd6vn@virgina.edu
## License

File-Flex is open-source under the [MIT license](https://github.com/SurvivrrHayde/file-flex/blob/main/LICENSE).
