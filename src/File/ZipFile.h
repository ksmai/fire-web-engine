#ifndef __ZIP_FILE_H_
#define __ZIP_FILE_H_

#include <string>
#include <vector>
#include "File/Data.h"

namespace FW {
  class ZipFile {
  public:
    using FileName = const std::string;
    using Size = Data::size_type;
    using Index = Size;

    ZipFile(Data&&);

    Index getNumFiles() const;

    FileName getFileName(Index i) const;

    Size getFileSize(Index i) const;

    Data getFileContent(Index i) const;

  private:
    Size findEndOfCentralDirectorySignature() const;
    void readCentralDirectoryHeaders();
    Size readCentralDirectoryHeader(Index, Size);

    struct FileInfo {
      Size compressedSize;
      Size uncompressedSize;
      Size rawDataOffset;
      std::string name;
      bool compressed;
    };

    Data data;
    std::vector<FileInfo> files;
    Size startOfCentralDirectory;
    Index numFiles;
  };

  uint32_t parseUint32LE(const Data&, ZipFile::Size);
  uint16_t parseUint16LE(const Data&, ZipFile::Size);
}

#endif
