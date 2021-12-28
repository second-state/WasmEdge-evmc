#include "evmc/evmc.h"
#include "evmc/evmc.hpp"
#include "evmc/loader.h"
#include "example_host.h"

#include <cassert>
#include <cstring>
using namespace evmc::literals;

typedef unsigned char *bytes;

// environment constants
const int64_t gas = 0x999999;
const evmc::uint256be block_difficulty = evmc::uint256be{0xDEADBEEF};
const int block_gas_limit = 4000000;
const evmc::address block_coinbase =
    0x0000000000000000000000000000000077777777_address;
const int64_t block_number = 0x400;
const int64_t block_timestamp = 0x88888888;
const evmc::uint256be tx_gas_price = evmc::uint256be{0x66666666};
const evmc::address tx_origin =
    0x0000000000000000000000000000000033333333_address;
const evmc::uint256be chain_id = evmc::uint256be{1};
const evmc::address default_sender =
    0x0000000000000000000000000000000011111111_address;
const evmc::uint256be default_value = evmc::uint256be{0x55555555};

const char *evmc_library_default = "./libwasmedge-evmc.so";
const char *evmc_library;

evmc_tx_context tx_context = {};
evmc_host_context *context;
const evmc_host_interface *host_interface;

extern "C" {

__attribute__((constructor)) void vm_constructor() {
  evmc_library = getenv("EVMC_RUNTIME_LIB");
  if (!evmc_library)
    evmc_library = evmc_library_default;

  tx_context.block_difficulty = block_difficulty;
  tx_context.block_gas_limit = block_gas_limit;
  tx_context.block_coinbase = block_coinbase;
  tx_context.block_number = block_number;
  tx_context.block_timestamp = block_timestamp;

  tx_context.tx_gas_price = tx_gas_price;
  tx_context.tx_origin = tx_origin;

  tx_context.chain_id = chain_id;

  context = example_host_create_context(tx_context);
  host_interface = example_host_get_interface();
}

void evmc_vm_deploy(bytes deploy_wasm, size_t deploy_wasm_size,
                    evmc_result *result) {
  enum evmc_loader_error_code err;
  struct evmc_vm *vm = evmc_load_and_create(evmc_library, &err);
  assert((err == EVMC_LOADER_SUCCESS) && "Initialize evmc failed");

  evmc::address sender = default_sender;

  evmc::address destination = {};
  evmc::uint256be value = default_value;
  evmc_bytes32 create2_salt = {};

  evmc_message msg{EVMC_CALL,   0,           0,           gas,
                   destination, sender,      deploy_wasm, deploy_wasm_size,
                   value,       create2_salt};
  *result = vm->execute(vm, host_interface, context, EVMC_MAX_REVISION, &msg,
                        deploy_wasm, deploy_wasm_size);
}

evmc::address bytes_to_address(bytes SenderStr) {
  evmc::address address;
  for (int i = 0; i < 20; i++) {
    address.bytes[i] = SenderStr[i];
  }
  return address;
}

void evmc_vm_execute(bytes calldata, size_t calldata_size, bytes sender,
                     bytes destination, bytes wasm, size_t wasm_size,
                     evmc_result *result) {
  evmc::address _sender = bytes_to_address(sender);
  evmc::address _destination = bytes_to_address(destination);

  enum evmc_loader_error_code err;
  struct evmc_vm *vm = evmc_load_and_create(evmc_library, &err);
  assert((err == EVMC_LOADER_SUCCESS) && "Initialize evmc failed");

  evmc::uint256be value = default_value;
  evmc_bytes32 create2_salt = {};

  evmc_message msg{EVMC_CALL,    0,           0,        gas,
                   _destination, _sender,     calldata, calldata_size,
                   value,        create2_salt};
  *result = vm->execute(vm, host_interface, context, EVMC_MAX_REVISION, &msg,
                        wasm, wasm_size);
}

void evmc_get_storage(const bytes addr, const bytes key, bytes ret) {
  evmc::address address = bytes_to_address(addr);
  evmc::bytes32 _ret;
  _ret = host_interface->get_storage(context, &address, (evmc::bytes32 *)key);
  for (int i = 0; i < 32; i++)
    ret[i] = _ret.bytes[i];
}
}
