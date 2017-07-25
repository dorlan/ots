// Copyright (c) 2009-2017 The OTS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "sile.h"

namespace ots {

bool OpenTypeSILE::Parse(const uint8_t* data, size_t length) {
  Buffer table(data, length);

  if (!table.ReadU32(&this->version) || this->version >> 16 != 1) {
    return Error("Failed to read valid version");
  }
  if (!table.ReadU32(&this->checksum)) {
    return Error("Failed to read checksum");
  }
  if (!table.ReadU32(&this->createTime[0]) ||
      !table.ReadU32(&this->createTime[1])) {
    return Error("Failed to read createTime");
  }
  if (!table.ReadU32(&this->modifyTime[0]) ||
      !table.ReadU32(&this->modifyTime[1])) {
    return Error("Failed to read modifyTime");
  }

  if (!table.ReadU16(&this->fontNameLength)) {
    return Error("Failed to read fontNameLength");
  }
  this->fontName.resize(this->fontNameLength);
  for (unsigned i = 0; i < this->fontNameLength; ++i) {
    if (!table.ReadU16(&this->fontName[i])) {
      return Error("Failed to read fontName[%u]", i);
    }
  }

  if (!table.ReadU16(&this->fontFileLength)) {
    return Error("Failed to read fontFileLength");
  }
  this->baseFile.resize(this->fontFileLength);
  for (unsigned i = 0; i < this->fontFileLength; ++i) {
    if (!table.ReadU16(&this->baseFile[i])) {
      return Error("Failed to read baseFile[%u]", i);
    }
  }

  if (table.remaining()) {
    return Warning("%zu bytes unparsed", table.remaining());
  }
  return true;
}

bool OpenTypeSILE::Serialize(OTSStream* out) {
  if (!out->WriteU32(this->version) ||
      !out->WriteU32(this->checksum) ||
      !out->WriteU32(this->createTime[0]) ||
      !out->WriteU32(this->createTime[1]) ||
      !out->WriteU32(this->modifyTime[0]) ||
      !out->WriteU32(this->modifyTime[1]) ||
      !out->WriteU16(this->fontNameLength) ||
      !SerializeParts(this->fontName, out) ||
      !out->WriteU16(this->fontFileLength) ||
      !SerializeParts(this->baseFile, out)) {
    return Error("Failed to write table");
  }
  return true;
}

}  // namespace ots