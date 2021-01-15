#include "evmc/evmc.hpp"
#include "evmc/loader.h"

#include "example_host.h"

#include <cassert>
#include <cstring>
#include <cstdio>

typedef unsigned char * bytes;

const char *evmc_library = "./libssvm-evmc.so";
evmc_host_context *context;
const evmc_host_interface *host_interface;

extern "C"{

__attribute__((constructor))
void vm_constructor(){
  context = example_host_create_context(evmc_tx_context{});
  host_interface = example_host_get_interface();
}

void evmc_vm_deploy(bytes deploy_wasm, size_t deploy_wasm_size, evmc_result *result){
  enum evmc_loader_error_code err;
  struct evmc_vm *vm = evmc_load_and_create(evmc_library, &err);
  assert( (err == EVMC_LOADER_SUCCESS) && "Initialize evmc failed" );

  evmc::address sender({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x7f, 0xff, 0xff, 0xff});
  evmc::address destination = {};
  int64_t gas = 999999;
  evmc::uint256be value = {};
  evmc_bytes32 create2_salt = {};

  evmc_message msg{EVMC_CALL,
                   0,
                   0,
                   gas,
                   destination,
                   sender,
                   deploy_wasm,
                   deploy_wasm_size,
                   value,
                   create2_salt};
  *result =
      vm->execute(vm, host_interface, context, EVMC_MAX_REVISION, &msg,
                  deploy_wasm, deploy_wasm_size);

}

evmc::address bytes_to_address(bytes SenderStr) {
  evmc::address address;
  for (int i = 0; i < 20; i++) {
    address.bytes[i] = SenderStr[i];
  }
  return address;
}

void evmc_vm_execute(bytes calldata, size_t calldata_size, bytes sender, bytes destination,bytes wasm, size_t wasm_size, evmc_result *result) {
  evmc::address _sender = bytes_to_address(sender);
  evmc::address _destination = bytes_to_address(destination);

  enum evmc_loader_error_code err;
  struct evmc_vm *vm = evmc_load_and_create(evmc_library, &err);
  assert( (err == EVMC_LOADER_SUCCESS) && "Initialize evmc failed" );


  int64_t gas = 999999;
  evmc::uint256be value = {};
  evmc_bytes32 create2_salt = {};

  evmc_message msg{EVMC_CALL,
                   0,
                   0,
                   gas,
                   _destination,
                   _sender,
                   calldata,
                   calldata_size,
                   value,
                   create2_salt};
  *result =
      vm->execute(vm, host_interface, context, EVMC_MAX_REVISION, &msg,
                  wasm, wasm_size);

}



void evmc_get_storage(const bytes addr, const bytes key, bytes ret) {
  evmc::address address = bytes_to_address(addr);
  evmc::bytes32 _ret;
  _ret = host_interface->get_storage(context, &address, (evmc::bytes32 *)key);
}

}
