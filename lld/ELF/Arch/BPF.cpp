//===- BPF.cpp ------------------------------------------------------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "InputFiles.h"
#include "Symbols.h"
#include "Target.h"
#include "lld/Common/ErrorHandler.h"
#include "llvm/Object/ELF.h"
#include "llvm/Support/Endian.h"

using namespace llvm;
using namespace llvm::object;
using namespace llvm::support::endian;
using namespace llvm::ELF;
using namespace lld;
using namespace lld::elf;

namespace {
  class BPF final : public TargetInfo {
  public:
    BPF();
    RelExpr getRelExpr(RelType type, const Symbol &s, const uint8_t *loc) const override;
    void relocateOne(uint8_t *loc, RelType type, uint64_t val) const override;
  };
}

BPF::BPF() { 
  noneRel = R_BPF_NONE;
}

RelExpr BPF::getRelExpr(RelType type, const Symbol &s, const uint8_t *loc) const {
  switch (type) {
    case R_BPF_64_32:
    case R_BPF_64_64:
      return R_ABS;
    default:
      return R_NONE;
  };
}

void BPF::relocateOne(uint8_t *loc, RelType type, uint64_t val) const {
  switch (type) {
    case R_BPF_64_32: 
      write32le(loc, val);
      break;
    case R_BPF_64_64:
      write64le(loc, val);
      break;
    default:
      error(getErrorLocation(loc) + "unrecognized reloc " + toString(type));
  }
}

TargetInfo *elf::getBPFTargetInfo() {
  static BPF Target;
  return &Target;
}
