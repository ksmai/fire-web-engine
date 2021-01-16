#include "zlib.h"
#include "App/Logger.h"
#include "App/abort.h"
#include "File/ZipFile.h"

FW::ZipFile::ZipFile(Data&& data):
  data{std::move(data)}
{
  std::size_t signaturePosition = findEndOfCentralDirectorySignature();
  numFiles = parseUint16LE(this->data, signaturePosition + 10);
  startOfCentralDirectory = parseUint32LE(this->data, signaturePosition + 16);
  readCentralDirectoryHeaders();
}

FW::ZipFile::Size FW::ZipFile::findEndOfCentralDirectorySignature() const {
  Size bytesSearched{0};
  Size start{data.size() - 22};
  while (bytesSearched < (1<<16) && start >= bytesSearched) {
    Size i{start - bytesSearched};
    if (data[i] == 0x50 && data[i+1] == 0x4b && data[i+2] == 0x05 && data[i+3] == 0x06) {
      uint32_t commentLength{parseUint16LE(data, i + 20)};
      if (commentLength == bytesSearched) {
        return i;
      }
    }
    ++bytesSearched;
  }
  Logger::error("Invalid zip file: end of central directory signature not found");
  abort();
  return 0;
}

void FW::ZipFile::readCentralDirectoryHeaders() {
  Size offset = startOfCentralDirectory;
  files.resize(getNumFiles());
  for (Index i = 0; i < getNumFiles(); ++i) {
    offset += readCentralDirectoryHeader(i, offset);
  }
}

FW::ZipFile::Size FW::ZipFile::readCentralDirectoryHeader(Index i, Size offset) {
  if (offset + 46 > data.size()) {
    Logger::error("Invalid central directory header: offset too big");
    abort();
  }
  if (data[offset+0] != 0x50 || data[offset+1] != 0x4b || data[offset+2] != 0x01 || data[offset+3] != 0x02) {
    Logger::error("Central directory header signature is not correct");
    abort();
  }
  uint16_t nameLength{parseUint16LE(data, offset + 28)};
  uint16_t extraFieldLength{parseUint16LE(data, offset + 30)};
  uint16_t commentLength{parseUint16LE(data, offset + 32)};
  Size headerSize{46u + nameLength + extraFieldLength + commentLength};
  if (offset + headerSize > data.size()) {
    Logger::error("Invalid central directory header: headerSize too big");
    abort();
  }
  uint16_t compressionMethod{parseUint16LE(data, offset + 10)};
  if (compressionMethod != 8 && compressionMethod != 0) {
    Logger::error("File is not compressed with deflation");
    abort();
  }
  files[i].compressedSize = parseUint32LE(data, offset + 20);
  files[i].uncompressedSize = parseUint32LE(data, offset + 24);
  files[i].name = std::string{&data[0]+offset+46, &data[0]+offset+46+nameLength};
  files[i].compressed = compressionMethod != 0;
  fileIndexes[files[i].name] = i;

  uint32_t localFileHeader{parseUint32LE(data, offset + 42)};
  if (localFileHeader + 30 > data.size()) {
    Logger::error("Inavlid local file header");
    abort();
  }
  uint16_t localFileNameLength{parseUint16LE(data, localFileHeader + 26)};
  uint16_t localExtraFieldLength{parseUint16LE(data, localFileHeader + 28)};
  files[i].rawDataOffset = localFileHeader + 30 + localFileNameLength + localExtraFieldLength;
  if (files[i].rawDataOffset + files[i].compressedSize > data.size()) {
    Logger::error("Inavlid local file header");
    abort();
  }

  return headerSize;
}

FW::ZipFile::Index FW::ZipFile::getNumFiles() const {
  return numFiles;
}

const FW::ZipFile::FileName FW::ZipFile::getFileName(Index i) const {
  return files[i].name;
}

FW::ZipFile::Size FW::ZipFile::getFileSize(Index i) const {
  return files[i].uncompressedSize;
}

FW::Data FW::ZipFile::getFileContent(Index i) const {
  if (!files[i].compressed) {
    return Data{
      &data[0] + files[i].rawDataOffset,
      &data[0] + files[i].rawDataOffset + files[i].uncompressedSize,
    };
  }
  // following zlib's official example closely
  // https://zlib.net/zlib_how.html
  z_stream strm;
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  strm.avail_in = 0;
  strm.next_in = Z_NULL;
  // https://stackoverflow.com/questions/18700656/zlib-inflate-failing-with-3-z-data-error
  // must use inflateInit2 if we are extracting the deflate stream from a zip ourselves
  int ret = inflateInit2(&strm, -MAX_WBITS);
  if (ret != Z_OK) {
    Logger::error("Inflation failed. Cannot init");
    abort();
  }
  const Size CHUNK = 16384;
  Size bytesLeft = files[i].compressedSize;
  Size position = files[i].rawDataOffset;
  unsigned char in[CHUNK];
  unsigned char out[CHUNK];
  Data result;
  result.reserve(files[i].uncompressedSize);
  do {
    strm.avail_in = bytesLeft > CHUNK ? CHUNK : bytesLeft;
    bytesLeft -= strm.avail_in;
    if (strm.avail_in == 0) {
      break;
    }
    for (Size i = 0; i < strm.avail_in; ++i) {
      in[i] = data[position+i];
    }
    if (strm.avail_in < CHUNK) {
      in[strm.avail_in] = 0;
    }
    strm.next_in = in;
    position += strm.avail_in;

    do {
      strm.avail_out = CHUNK;
      strm.next_out = out;
      ret = inflate(&strm, Z_NO_FLUSH);
      if (ret == Z_STREAM_ERROR || ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
        if (ret == Z_STREAM_ERROR) {
          Logger::error("Inflation failed: Z_STREAM_ERROR");
        }
        if (ret == Z_NEED_DICT) {
          Logger::error("Inflation failed: Z_NEED_DICT");
        }
        if (ret == Z_DATA_ERROR) {
          Logger::error("Inflation failed: Z_DATA_ERROR");
        }
        if (ret == Z_MEM_ERROR) {
          Logger::error("Inflation failed: Z_MEM_ERROR");
        }
        abort();
      }
      result.insert(result.end(), out, out + CHUNK - strm.avail_out);
    } while (strm.avail_out == 0);
  } while (ret != Z_STREAM_END);

  (void)inflateEnd(&strm);
  return result;
}

FW::Data FW::ZipFile::getFileContent(const FileName& fileName) const {
  auto it{fileIndexes.find(fileName)};
  if (it == fileIndexes.end()) {
    Logger::error("File not found in the zip file: %s", fileName.c_str());
    abort();
  }
  return getFileContent(it->second);
}

uint32_t FW::parseUint32LE(const Data& data, ZipFile::Size offset) {
  uint32_t result = 0u;
  result += data[offset+0];
  result += (data[offset+1] << 8);
  result += (data[offset+2] << 16);
  result += (data[offset+3] << 24);
  return result;
}

uint16_t FW::parseUint16LE(const Data& data, ZipFile::Size offset) {
  uint16_t result = 0u;
  result += data[offset+0];
  result += (data[offset+1] << 8);
  return result;
}
