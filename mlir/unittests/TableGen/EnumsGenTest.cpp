//===- EnumsGenTest.cpp - TableGen EnumsGen Tests -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "mlir/Support/LLVM.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/StringSwitch.h"
#include "gmock/gmock.h"
#include <type_traits>

// Pull in generated enum utility declarations
#include "EnumsGenTest.h.inc"
// And definitions
#include "EnumsGenTest.cpp.inc"

// Test namespaces and enum class/utility names
using Outer::Inner::ConvertToEnum;
using Outer::Inner::ConvertToString;
using Outer::Inner::StrEnum;

TEST(EnumsGenTest, GeneratedStrEnumDefinition) {
  EXPECT_EQ(0u, static_cast<uint64_t>(StrEnum::CaseA));
  EXPECT_EQ(10u, static_cast<uint64_t>(StrEnum::CaseB));
}

TEST(EnumsGenTest, GeneratedI32EnumDefinition) {
  EXPECT_EQ(5u, static_cast<uint64_t>(I32Enum::Case5));
  EXPECT_EQ(10u, static_cast<uint64_t>(I32Enum::Case10));
}

TEST(EnumsGenTest, GeneratedDenseMapInfo) {
  llvm::DenseMap<StrEnum, std::string> myMap;

  myMap[StrEnum::CaseA] = "zero";
  myMap[StrEnum::CaseB] = "one";

  EXPECT_EQ(myMap[StrEnum::CaseA], "zero");
  EXPECT_EQ(myMap[StrEnum::CaseB], "one");
}

TEST(EnumsGenTest, GeneratedSymbolToStringFn) {
  EXPECT_EQ(ConvertToString(StrEnum::CaseA), "CaseA");
  EXPECT_EQ(ConvertToString(StrEnum::CaseB), "CaseB");
}

TEST(EnumsGenTest, GeneratedStringToSymbolFn) {
  EXPECT_EQ(llvm::Optional<StrEnum>(StrEnum::CaseA), ConvertToEnum("CaseA"));
  EXPECT_EQ(llvm::Optional<StrEnum>(StrEnum::CaseB), ConvertToEnum("CaseB"));
  EXPECT_EQ(llvm::None, ConvertToEnum("X"));
}

TEST(EnumsGenTest, GeneratedUnderlyingType) {
  bool v = std::is_same<uint32_t, std::underlying_type<I32Enum>::type>::value;
  EXPECT_TRUE(v);
}

TEST(EnumsGenTest, GeneratedBitEnumDefinition) {
  EXPECT_EQ(0u, static_cast<uint32_t>(BitEnumWithNone::None));
  EXPECT_EQ(1u, static_cast<uint32_t>(BitEnumWithNone::Bit1));
  EXPECT_EQ(4u, static_cast<uint32_t>(BitEnumWithNone::Bit3));
}

TEST(EnumsGenTest, GeneratedSymbolToStringFnForBitEnum) {
  EXPECT_EQ(stringifyBitEnumWithNone(BitEnumWithNone::None), "None");
  EXPECT_EQ(stringifyBitEnumWithNone(BitEnumWithNone::Bit1), "Bit1");
  EXPECT_EQ(stringifyBitEnumWithNone(BitEnumWithNone::Bit3), "Bit3");
  EXPECT_EQ(
      stringifyBitEnumWithNone(BitEnumWithNone::Bit1 | BitEnumWithNone::Bit3),
      "Bit1|Bit3");
}

TEST(EnumsGenTest, GeneratedStringToSymbolForBitEnum) {
  EXPECT_EQ(symbolizeBitEnumWithNone("None"), BitEnumWithNone::None);
  EXPECT_EQ(symbolizeBitEnumWithNone("Bit1"), BitEnumWithNone::Bit1);
  EXPECT_EQ(symbolizeBitEnumWithNone("Bit3"), BitEnumWithNone::Bit3);
  EXPECT_EQ(symbolizeBitEnumWithNone("Bit3|Bit1"),
            BitEnumWithNone::Bit3 | BitEnumWithNone::Bit1);

  EXPECT_EQ(symbolizeBitEnumWithNone("Bit2"), llvm::None);
  EXPECT_EQ(symbolizeBitEnumWithNone("Bit3|Bit4"), llvm::None);

  EXPECT_EQ(symbolizeBitEnumWithoutNone("None"), llvm::None);
}

TEST(EnumsGenTest, GeneratedOperator) {
  EXPECT_TRUE(bitEnumContains(BitEnumWithNone::Bit1 | BitEnumWithNone::Bit3,
                              BitEnumWithNone::Bit1));
  EXPECT_FALSE(bitEnumContains(BitEnumWithNone::Bit1 & BitEnumWithNone::Bit3,
                               BitEnumWithNone::Bit1));
}

TEST(EnumsGenTest, GeneratedSymbolToCustomStringFn) {
  EXPECT_EQ(stringifyPrettyIntEnum(PrettyIntEnum::Case1), "case_one");
  EXPECT_EQ(stringifyPrettyIntEnum(PrettyIntEnum::Case2), "case_two");
}

TEST(EnumsGenTest, GeneratedCustomStringToSymbolFn) {
  auto one = symbolizePrettyIntEnum("case_one");
  EXPECT_TRUE(one);
  EXPECT_EQ(*one, PrettyIntEnum::Case1);

  auto two = symbolizePrettyIntEnum("case_two");
  EXPECT_TRUE(two);
  EXPECT_EQ(*two, PrettyIntEnum::Case2);

  auto none = symbolizePrettyIntEnum("Case1");
  EXPECT_FALSE(none);
}
