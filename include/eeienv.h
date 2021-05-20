// SPDX-License-Identifier: Apache-2.0
#pragma once

#include "common/types.h"
#include "common/value.h"
#include "evmc/evmc.hpp"
#include "common/hexstr.h"

#include <algorithm>
#include <string>
#include <vector>

namespace WasmEdge {
namespace Host {

class EVMEnvironment {
public:
  EVMEnvironment() = delete;
  EVMEnvironment(uint64_t &CostLimit, uint64_t &CostSum,
                 const evmc_host_interface *IHost, evmc_host_context *Cxt)
      : GasLimit(CostLimit), GasUsed(CostSum), EVMCContext(*IHost, Cxt) {}
  ~EVMEnvironment() = default;

  /// Getter of remain gas. Gas limit can be set by EnvironmentManager.
  uint64_t getGasLeft() { return GasLimit - GasUsed; }

  /// Consume gas.
  bool consumeGas(const uint64_t Gas);

  /// Return gas.
  bool returnGas(const uint64_t Gas);

  /// Getter and setter of depth.
  uint32_t &getDepth() { return Depth; }

  /// Getter and setter of flag.
  uint32_t &getFlag() { return Flag; }

  /// Getter and setter of call kind.
  evmc_call_kind &getCallKind() { return CallKind; }

  /// Getter of caller and converting into hex string.
  std::string getCallerStr() {
    std::string Str;
    convertBytesToHexStr(Caller, Str, 40);
    return Str;
  }

  /// Getter of caller in EVMC version.
  evmc::address getCallerEVMC() {
    evmc::address Addr;
    std::copy_n(Caller.cbegin(), 20, Addr.bytes);
    return Addr;
  }

  /// Getter of caller vector.
  std::vector<Byte> &getCaller() { return Caller; }

  /// Setter of caller by hex string.
  void setCaller(const std::string &Str) {
    convertHexStrToBytes(Str, Caller, 40);
  }

  /// Getter of call value and converting into hex string.
  std::string getCallValueStr() {
    std::string Str;
    convertValVecToHexStr(CallValue, Str, 64);
    return Str;
  }

  /// Getter of call value in EVMC version.
  evmc::bytes32 getCallValueEVMC() {
    evmc::bytes32 Val;
    std::copy_n(CallValue.cbegin(), 32, Val.bytes);
    return Val;
  }

  /// Getter of call value vector.
  std::vector<Byte> &getCallValue() { return CallValue; }

  /// Setter of call value by hex string.
  void setCallValue(const std::string &Str) {
    convertHexStrToValVec(Str, CallValue, 64);
  }

  /// Getter of call data vector.
  std::vector<Byte> &getCallData() { return CallData; }

  /// Getter of address and converting into hex string.
  std::string getAddressStr() {
    std::string Str;
    convertBytesToHexStr(Address, Str, 40);
    return Str;
  }

  /// Getter of address in EVMC version.
  evmc::address getAddressEVMC() {
    evmc::address Addr;
    std::copy_n(Address.cbegin(), 20, Addr.bytes);
    return Addr;
  }

  /// Getter of address vector.
  std::vector<Byte> &getAddress() { return Address; }

  /// Setter of address by hex string.
  void setAddress(const std::string &Str) {
    convertHexStrToBytes(Str, Address, 40);
  }

  /// Getter of return data vector.
  std::vector<Byte> &getReturnData() { return ReturnData; }

  /// Getter of code vector.
  std::vector<Byte> &getCode() { return Code; }

  /// Getter of EVMC context.
  evmc::HostContext &getEVMCContext() { return EVMCContext; }

  /// Initialize by EVMC message.
  void setEVMCMessage(const struct evmc_message *Msg);

  /// Set code by EVMC.
  void setEVMCCode(const uint8_t *Buf, const uint32_t Size) {
    Code = std::vector<Byte>(Buf, Buf + Size);
  }

  /// Getter of is revert.
  bool &getIsRevert() { return IsRevert; }

private:
  /// Gas measurement
  uint64_t &GasLimit;
  uint64_t &GasUsed;

  /// Caller: 20 bytes sendor address.
  std::vector<Byte> Caller;
  /// CallValue: 32 bytes little endian. Reversed value.
  std::vector<Byte> CallValue;
  /// CallData: inputs, may be 0-length.
  std::vector<Byte> CallData;
  /// Address: 20 bytes destination address.
  std::vector<Byte> Address;
  /// ReturnData: return value list.
  std::vector<Byte> ReturnData;
  /// Code:
  std::vector<Byte> Code;
  /// Depth:
  uint32_t Depth = 0;
  /// Call flag:
  uint32_t Flag = 0;
  /// Call kind:
  evmc_call_kind CallKind = evmc_call_kind::EVMC_CALL;
  /// EVMC context:
  evmc::HostContext EVMCContext;
  /// Is revert:
  bool IsRevert = false;
};

} // namespace Host
} // namespace WasmEdge
