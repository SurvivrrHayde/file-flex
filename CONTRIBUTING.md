# Contribution Guide for File Flex

**Welcome to the File Flex Contribution Guide!**

Thank you for your interest in contributing to File Flex. I value your contributions and want to make sure that the process is as easy and transparent as possible. Whether you're reporting a bug, suggesting an enhancement, or submitting a pull request, I am here to assist you.

## Prerequisites

- Familiarity with GitHub: Basic understanding of how to create issues, fork repositories, and create pull requests.
- Knowledge of Git: Comfortable with cloning repositories, creating branches, committing changes, and pushing to GitHub.

## Setting Up Your Environment

1. **Fork the Repository**
   - Navigate to the [File Flex GitHub repository](https://github.com/SurvivrrHayde/file-flex).
   - In the top-right corner of the page, click the _Fork_ button. This creates a copy of the repository in your GitHub account.

2. **Clone Your Fork**
   - On your GitHub account, go to _Your repositories_, select your fork of File Flex, and click the _Clone_ button.
   - Copy the URL provided.
   - Open your terminal, navigate to the directory where you want to clone the repository, and run:
     ```bash
     git clone https://github.com/yourusername/file-flex.git
     ```
   - Change directories to your newly cloned repository:
     ```bash
     cd file-flex
     ```

3. **Add the Original Repository as a Remote (Upstream)**
   - To keep your fork up to date with the original repository, add it as a remote:
     ```bash
     git remote add upstream https://github.com/SurvivrrHayde/file-flex.git
     ```
   - Verify the new remote named _upstream_:
     ```bash
     git remote -v
     ```

## Making Changes

1. **Create a New Branch**
   - Always create a new branch for your changes:
     ```bash
     git checkout -b feature-branch-name
     ```
   - Keep branch names informative and concise.

2. **Make Your Changes**
   - Implement your changes or fixes in your branch, adhering to coding standards and guidelines.
   - Test your changes to ensure they are functioning as expected.

3. **Commit Your Changes**
   - Commit your changes with a clear, concise commit message:
     ```bash
     git add .
     git commit -m "A descriptive commit message"
     ```

## Submitting a Pull Request

1. **Push Your Changes to GitHub**
   - Push your branch and changes to your GitHub fork:
     ```bash
     git push origin feature-branch-name
     ```

2. **Create a Pull Request**
   - On GitHub, navigate to the original File Flex repository.
   - You'll often see a "Compare & pull request" button for your recently pushed branches. Click it.
   - Ensure that the base fork is set to the original File Flex repository and the head fork to your branch.
   - Submit the pull request with a description of the changes you've made.

## Review Process

- Once submitted, I will review your pull request.
- Engage in discussions if there are comments or questions.
- If changes are requested, make them in your branch, re-push, and the pull request will automatically update.
- If your pull request is approved, I will merge it.

## Additional Guidelines

- **Code of Conduct**: Please follow the code of conduct set by the File Flex community. This typically includes respecting others, embracing diversity, and more.
- **Issue Guidelines**: If you're reporting a bug, include details like what the issue is, steps to reproduce, expected outcomes, and actual outcomes.

**Thank You!**

Your contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.
