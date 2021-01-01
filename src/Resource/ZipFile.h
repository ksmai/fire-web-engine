#ifndef __ZIP_FILE_H_
#define __ZIP_FILE_H_

#include <string>
#include <map>
#include <vector>

class ZipFile {
public:
  using ZipFileData = const char *;
  using FileName = const std::string;
  using FileData = std::vector<char>;
  using Size = uint64_t;

  ZipFile(ZipFileData, Size);

  const char* getData(FileName fileName) const;

private:
  Size findEndOfCentralDirectorySignature(ZipFileData, Size) const;

  std::vector<FileData> files;
  std::map<FileName, std::size_t> indexes;
};

#endif
