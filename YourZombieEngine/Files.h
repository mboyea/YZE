#ifndef FILES_H
#define FILES_H
#pragma once

#include <string>
#include <set>

namespace Files {
	bool IsFileExtension(std::string filePath, std::string extension);
	std::string GetFileExtension(std::string filePath);
	std::string ForceFileExtension(std::string filePath, std::string extension);
	std::string RemoveFileExtension(std::string filePath);
	bool IsFileDirectory(std::string filePath, std::string directory);
	std::string GetFileDirectory(std::string filePath);
	std::string ForceFileDirectory(std::string filePath, std::string directory);
	std::string RemoveFileDirectory(std::string filePath);
	char GetDirectorySlash(std::string filePath);
	std::string GetFileName(std::string filePath);
	std::string ForceFilePath(std::string fileName, std::string directory, std::string extension);
	bool DoesPathExist(std::string folderPath);
	// return true if the directory had to be created; otherwise, return false
	bool ForceDirectoryExistence(std::string folderPath);
	std::set<std::string> GetFilesInDirectory(std::string folderPath);
	std::set<std::string> GetFilesWithExtension(std::string folderPath, std::string extension);
	std::set<std::string> GetFilesWithExtension(std::set<std::string> filePaths, std::string extension);
}

#endif // !FILES_H