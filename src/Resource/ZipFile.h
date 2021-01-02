#ifndef __ZIP_FILE_H_
#define __ZIP_FILE_H_

#include <string>
#include <map>
#include <vector>

namespace FW {
  class ZipFile {
  public:
    using ZipFileData = const unsigned char *;
    using FileName = const std::string;
    using FileData = std::vector<unsigned char>;
    using Size = uint64_t;
    using Index = uint16_t;

    ZipFile(ZipFileData, Size);

    Index getNumFiles() const {
      return numFiles;
    }

    std::string getFileName(Index i) const {
      return files[i].name;
    }

    Size getFileSize(Index i) const {
      return files[i].uncompressedSize;
    }

    FileData getFileContent(Index i) const;

  private:
    Size findEndOfCentralDirectorySignature(ZipFileData, Size) const;
    void readCentralDirectoryHeaders();
    Size readCentralDirectoryHeader(Index, Size);

    struct FileInfo {
      Size compressedSize;
      Size uncompressedSize;
      Size rawDataOffset;
      std::string name;
      bool compressed;
    };

    Size size;
    ZipFileData data;
    std::vector<FileInfo> files;
    Size startOfCentralDirectory;
    Index numFiles;
  };

  uint32_t parseUint32LE(ZipFile::ZipFileData);
  uint16_t parseUint16LE(ZipFile::ZipFileData);
}

#endif
