#include "Files.h"
#include <filesystem>
#include <algorithm>

std::string Files::EncodeString(std::string str) {
	str.erase(std::remove(str.begin(), str.end(), TEXT_END), str.end());
	return str + TEXT_END;
}

std::string Files::DecodeString(std::istream& is) {
	std::string str = "";
	std::string input;
	while (is >> input) {
		if (input.back() == TEXT_END) {
			break;
		}
		str += input + ' ';
	}
	str.pop_back();
	return str;
}

bool Files::IsFileExtension(std::string filePath, std::string extension) {
	if (extension[0] == '.') {
		extension.erase(extension.begin() + 0);
	}
	return GetFileExtension(filePath) == extension;
}

std::string Files::GetFileExtension(std::string filePath) {
	int index = -1;
	for (auto it = filePath.crbegin(); it != filePath.crend(); it++) {
		if (*it == '.') {
			index = filePath.size() - (it - filePath.crbegin());
			continue;
		}
		if (*it == '/' || *it == '\\') {
			break;
		}
	}
	if (index < 0) {
		return "";
	}
	return filePath.substr(index);
}

std::string Files::ForceFileExtension(std::string filePath, std::string extension) {
	if (extension[0] == '.') {
		extension.erase(extension.begin() + 0);
	}
	filePath = RemoveFileExtension(filePath);
	return filePath + '.' + extension;
}

std::string Files::RemoveFileExtension(std::string filePath) {
	int index = -1;
	for (auto it = filePath.crbegin(); it != filePath.crend(); it++) {
		if (*it == '.') {
			index = filePath.size() - (it - filePath.crbegin()) - 1;
			continue;
		}
		if (*it == '/' || *it == '\\') {
			break;
		}
	}
	if (index < 0) {
		return filePath;
	}
	return filePath.substr(0, index);
}

bool Files::IsFileDirectory(std::string filePath, std::string directory) {
	while (directory[directory.size() - 1] == '/' || directory[directory.size() - 1] == '\\') {
		directory.pop_back();
	}
	return GetFileDirectory(filePath) == directory;
}

std::string Files::GetFileDirectory(std::string filePath) {
	int index = -1;
	for (auto it = filePath.crbegin(); it != filePath.crend(); it++) {
		if (*it == '/' || *it == '\\') {
			index = filePath.size() - (it - filePath.crbegin());
			if (it != filePath.crend()) {
				it++;
			}
			break;
		}
	}
	if (index < 0) {
		return filePath;
	}
	return filePath.substr(0, index);
}

std::string Files::ForceFileDirectory(std::string filePath, std::string directory) {
	while (directory[directory.size() - 1] == '/' || directory[directory.size() - 1] == '\\') {
		directory.pop_back();
	}
	filePath = RemoveFileDirectory(filePath);
	return directory + GetDirectorySlash(filePath) + filePath;
}

std::string Files::RemoveFileDirectory(std::string filePath) {
	int index = -1;
	for (auto it = filePath.crbegin(); it != filePath.crend(); it++) {
		if (*it == '/' || *it == '\\') {
			index = filePath.size() - (it - filePath.crbegin());
			break;
		}
	}
	if (index < 0) {
		return filePath;
	}
	return filePath.substr(index);
}

char Files::GetDirectorySlash(std::string filePath) {
	if (filePath.find('/') < filePath.size()) {
		return '/';
	}
	if (filePath.find('\\') < filePath.size()) {
		return '\\';
	}
	return '/';
}

std::string Files::GetFileName(std::string filePath) {
	return RemoveFileExtension(RemoveFileDirectory(filePath));
}

std::string Files::ForceFilePath(std::string fileName, std::string directory, std::string extension) {
	return ForceFileExtension(ForceFileDirectory(fileName, directory), extension);
}

bool Files::DoesPathExist(std::string folderPath) {
	return std::filesystem::exists(folderPath);
}

bool Files::ForceDirectoryExistence(std::string folderPath) {
	return std::filesystem::create_directory(folderPath);
}

std::set<std::string> Files::GetFilesInDirectory(std::string folderPath) {
	std::set<std::string> files;
	for (const auto& file : std::filesystem::directory_iterator(folderPath)) {
		files.insert(file.path().string());
	}
	return files;
}

std::set<std::string> Files::GetFilesWithExtension(std::string folderPath, std::string extension) {
	return GetFilesWithExtension(GetFilesInDirectory(folderPath), extension);
}

std::set<std::string> Files::GetFilesWithExtension(std::set<std::string> files, std::string extension) {
	std::set<std::string> extensionFiles;
	for (std::string file : files) {
		if (IsFileExtension(file, extension)) {
			extensionFiles.insert(file);
		}
	}
	return extensionFiles;
}
