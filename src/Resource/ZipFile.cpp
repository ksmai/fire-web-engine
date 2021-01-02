#include <iostream>
#include "zlib.h"
#include "Resource/ZipFile.h"

FW::ZipFile::ZipFile(ZipFileData data, Size size):
  size{size},
  data{data} {
  std::size_t signaturePosition = findEndOfCentralDirectorySignature(data, size);
  numFiles = parseUint16LE(data + signaturePosition + 10);
  startOfCentralDirectory = parseUint32LE(data + signaturePosition + 16);
  readCentralDirectoryHeaders();
}

FW::ZipFile::Size FW::ZipFile::findEndOfCentralDirectorySignature(ZipFileData data, Size size) const {
  Size bytesSearched = 0;
  Size start = size - 22;
  while (bytesSearched < (1<<16) && start >= bytesSearched) {
    std::size_t i = start - bytesSearched;
    if (data[i] == 0x50 && data[i+1] == 0x4b && data[i+2] == 0x05 && data[i+3] == 0x06) {
      uint32_t commentLength = parseUint32LE(data + i + 20);
      if (commentLength == bytesSearched) {
        return i;
      }
    }
  }
  std::cout << "Invalid zip file: end of central directory signature not found\n";
  throw "Invalid zip file: end of central directory signature not found";
}

void FW::ZipFile::readCentralDirectoryHeaders() {
  Size offset = startOfCentralDirectory;
  files.resize(getNumFiles());
  for (Index i = 0; i < getNumFiles(); ++i) {
    offset += readCentralDirectoryHeader(i, offset);
  }
}

FW::ZipFile::Size FW::ZipFile::readCentralDirectoryHeader(Index i, Size offset) {
  if (offset + 46 > size) {
    std::cout << "Invalid central directory header\n";
    throw "Invalid central directory header";
  }
  ZipFileData start = data + offset;
  if (start[0] != 0x50 || start[1] != 0x4b || start[2] != 0x01 || start[3] != 0x02) {
    std::cout << "Central directory header signature is not correct\n";
    throw "Central directory header signature is not correct";
  }
  uint16_t nameLength = parseUint16LE(start + 28);
  uint16_t extraFieldLength = parseUint16LE(start + 30);
  uint16_t commentLength = parseUint16LE(start + 32);
  Size headerSize = 46u + nameLength + extraFieldLength + commentLength;
  if (offset + headerSize > size) {
    std::cout << "Invalid central directory header\n";
    throw "Invalid central directory header";
  }
  uint16_t compressionMethod = parseUint16LE(start + 10);
  if (compressionMethod != 8 && compressionMethod != 0) {
    std::cout << "File is not compressed with deflation\n";
    throw "File is not compressed with deflation";
  }
  files[i].compressedSize = parseUint32LE(start + 20);
  files[i].uncompressedSize = parseUint32LE(start + 24);
  files[i].name = std::string{start+46, start+46+nameLength};
  files[i].compressed = compressionMethod != 0;

  uint32_t localFileHeader = parseUint32LE(start + 42);
  if (localFileHeader + 30 > size) {
    std::cout << "Inavlid local file header\n";
    throw "Inavlid local file header";
  }
  uint16_t localFileNameLength = parseUint16LE(data + localFileHeader + 26);
  uint16_t localExtraFieldLength = parseUint16LE(data + localFileHeader + 28);
  files[i].rawDataOffset = localFileHeader + 30 + localFileNameLength + localExtraFieldLength;
  if (files[i].rawDataOffset + files[i].compressedSize > size) {
    std::cout << "Inavlid local file header\n";
    throw "Inavlid local file header";
  }

  return headerSize;
}

FW::ZipFile::FileData FW::ZipFile::getFileContent(Index i) const {
  if (!files[i].compressed) {
    return std::vector<unsigned char>{data+files[i].rawDataOffset, data+files[i].rawDataOffset+files[i].uncompressedSize};
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
    std::cout << "Inflation failed. Cannot init\n";
    throw "Inflation failed";
  }
  const Size CHUNK = 16384;
  Size bytesLeft = files[i].compressedSize;
  Size position = files[i].rawDataOffset;
  unsigned char in[CHUNK];
  unsigned char out[CHUNK];
  std::vector<unsigned char> result;
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
        std::cout << "Inflation failed. ret from inflate is ";
        if (ret == Z_STREAM_ERROR) {
          std::cout  << "Z_STREAM_ERROR\n";
        }
        if (ret == Z_NEED_DICT) {
          std::cout  << "Z_NEED_DICT\n";
        }
        if (ret == Z_DATA_ERROR) {
          std::cout  << "Z_DATA_ERROR\n";
        }
        if (ret == Z_MEM_ERROR) {
          std::cout  << "Z_MEM_ERROR\n";
        }
        throw "Inflation failed";
      }
      for (Size i = 0; i < CHUNK - strm.avail_out; ++i) {
        result.push_back(out[i]);
      }
    } while (strm.avail_out == 0);
  } while (ret != Z_STREAM_END);

  (void)inflateEnd(&strm);
  return result;
}

uint32_t FW::parseUint32LE(FW::ZipFile::ZipFileData data) {
  uint32_t result = 0u;
  result += data[0];
  result += (data[1] << 8);
  result += (data[2] << 16);
  result += (data[3] << 24);
  return result;
}

uint16_t FW::parseUint16LE(FW::ZipFile::ZipFileData data) {
  uint16_t result = 0u;
  result += data[0];
  result += (data[1] << 8);
  return result;
}
