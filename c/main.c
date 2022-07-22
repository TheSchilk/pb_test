#include "cmd.pb.h"
#include <pb_decode.h>
#include <pb_encode.h>
#include <stdio.h>

uint8_t msg_buf[psMCU_Cmd_size];
psMCU_Cmd cmd = {0};

int main() {
  uint32_t err = 0;
  freopen(0, "rb", stdin);

  // Read msg len:
  size_t r = 0;
  uint8_t len_buf[2];
  r = fread(len_buf, 1, 2, stdin);
  err |= (r != 2);
  if (err)
    return 1;

  uint32_t size_in = len_buf[1] << 8 | len_buf[0];

  fprintf(stderr, "Got len: %u\r\n", size_in);

  // Read msg:
  r = fread(msg_buf, 1, size_in, stdin);
  err |= (r != size_in);
  if (err)
    return 2;

  // Decode:
  pb_istream_t istream = pb_istream_from_buffer(msg_buf, size_in);
  bool status = pb_decode(&istream, psMCU_Cmd_fields, &cmd);
  if (!status)
    return 3;
  if (istream.bytes_left != 0)
    return 4;

  if (cmd.type == psMCU_Cmd_CmdType_ACK) {
    return 100;
  }

  cmd.type = psMCU_Cmd_CmdType_ACK;
  cmd.ack_c = true;
  cmd.payload.bytes[2] = 0xff;

  // Re-Encode:
  pb_ostream_t ostream = pb_ostream_from_buffer(msg_buf, psMCU_Cmd_size);
  status = pb_encode(&ostream, psMCU_Cmd_fields, &cmd);
  if (!status)
    return 5;
  if (ostream.bytes_written != size_in)
    return 6;

  // Echo back:
  freopen(0, "wb", stdout);
  r = fwrite(len_buf, 1, 2, stdout);
  if (r != 2) {
    return 10;
  }
  r = fwrite(msg_buf, 1, size_in, stdout);
  if (r != size_in) {
    return 12;
  }

  fflush(stdout);

  return 0;
}
