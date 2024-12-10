#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

struct FileBlock {
    int id;
    int size;
    int startPos;  // Added to track file position
};

std::vector<int> createDiskRepresentation(const std::string& input, std::vector<FileBlock>& files) {
    std::vector<int> disk;
    int currentPos = 0;
    int fileId = 0;

    for (size_t i = 0; i < input.length(); i += 2) {
        int fileSize = input[i] - '0';
        FileBlock block = {fileId, fileSize, currentPos};
        files.push_back(block);

        // Add file blocks
        for (int j = 0; j < fileSize; j++) {
            disk.push_back(fileId);
        }
        currentPos += fileSize;

        // Add spaces if not at end
        if (i + 1 < input.length()) {
            int spaceSize = input[i + 1] - '0';
            for (int j = 0; j < spaceSize; j++) {
                disk.push_back(-1);
            }
            currentPos += spaceSize;
        }
        fileId++;
    }
    return disk;
}

// Part 1 functions
void defragment(std::vector<int>& disk) {
    size_t size = disk.size();

    while (true) {
        int rightmost = size - 1;
        while (rightmost >= 0 && disk[rightmost] == -1) rightmost--;

        int leftmost = 0;
        while (leftmost < size && disk[leftmost] != -1) leftmost++;

        if (leftmost >= rightmost) break;

        disk[leftmost] = disk[rightmost];
        disk[rightmost] = -1;
    }
}

// New functions for Part 2
void moveWholeFile(std::vector<int>& disk, const FileBlock& file, int newStartPos) {
    // Copy file to new position
    for (int i = 0; i < file.size; i++) {
        disk[newStartPos + i] = file.id;
    }

    // Clear old position
    for (int i = 0; i < file.size; i++) {
        disk[file.startPos + i] = -1;
    }
}

bool findFreeSpace(const std::vector<int>& disk, int fileSize, int currentPos, int& freeSpaceStart) {
    for (int i = 0; i < currentPos; i++) {
        if (disk[i] == -1) {
            // Check if we have enough continuous space
            bool hasSpace = true;
            for (int j = 0; j < fileSize; j++) {
                if (i + j >= disk.size() || disk[i + j] != -1) {
                    hasSpace = false;
                    break;
                }
            }
            if (hasSpace) {
                freeSpaceStart = i;
                return true;
            }
            // Skip to end of this non-viable free space
            while (i < disk.size() && disk[i] == -1) i++;
            i--;
        }
    }
    return false;
}

void defragmentWholeFiles(std::vector<int>& disk, std::vector<FileBlock>& files) {
    // Sort files by ID in descending order
    std::sort(files.begin(), files.end(),
        [](const FileBlock& a, const FileBlock& b) { return a.id > b.id; });

    // Try to move each file
    for (FileBlock& file : files) {
        int newPos;
        if (findFreeSpace(disk, file.size, file.startPos, newPos)) {
            moveWholeFile(disk, file, newPos);
            file.startPos = newPos;  // Update file position
        }
    }
}

long long calculateChecksum(const std::vector<int>& disk) {
    long long checksum = 0;
    for (size_t i = 0; i < disk.size(); i++) {
        if (disk[i] != -1) {
            checksum += static_cast<long long>(i) * disk[i];
        }
    }
    return checksum;
}

int main() {
    std::ifstream inFile("input.txt");
    if (!inFile.is_open()) {
        std::cerr << "Error opening input.txt" << std::endl;
        return 1;
    }

    std::string input;
    inFile >> input;
    inFile.close();

    // Part 1
    std::vector<FileBlock> files1;
    std::vector<int> disk1 = createDiskRepresentation(input, files1);
    defragment(disk1);
    long long checksum1 = calculateChecksum(disk1);
    std::cout << "Part 1 Checksum: " << checksum1 << std::endl;

    // Part 2
    std::vector<FileBlock> files2;
    std::vector<int> disk2 = createDiskRepresentation(input, files2);
    defragmentWholeFiles(disk2, files2);
    long long checksum2 = calculateChecksum(disk2);
    std::cout << "Part 2 Checksum: " << checksum2 << std::endl;

    return 0;
}
