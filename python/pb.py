import struct
import subprocess
from subprocess import PIPE, STDOUT, Popen

import cmd_pb2

# Create first package:
cmd = cmd_pb2.Cmd()
cmd.type = cmd_pb2.Cmd.CmdType.PING
cmd.ack_py = False
cmd.ack_rs = False
cmd.ack_c = False
cmd.payload = bytes(10)

# Serialise:
s = bytes(cmd.SerializeToString())

# Add length at beginning:
len_data = struct.pack('<H', len(s))
data = bytes(bytearray(len_data) + bytearray(s))

# Pass to C:
p = Popen(['../c/build/main.out'], stdout=PIPE, stdin=PIPE, stderr=PIPE)
proc_data = p.communicate(input=data)
stdout_data = proc_data[0]
stderr_data = proc_data[1]

if p.returncode != 0:
    print("C error...")
    print(p.returncode)
    print(stderr_data)
    exit()


# Decode:
stdout_data = stdout_data[2:]
cmd.ParseFromString(stdout_data)
cmd.ack_py = True
print(cmd)
print(cmd.payload)

# person = addressbook_pb2.Person()
# person.id = 1234
# person.name = "John Doe"
# person.email = "jdoe@example.com"
# phone = person.phones.add()
# phone.number = "555-4321"
# phone.type = addressbook_pb2.Person.HOME
