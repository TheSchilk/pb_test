# Compiler + Flags
CC=gcc
LDFLAGS=
CFLAGS=-Wall -Wextra -Wpedantic -g -I. -Ilib -Ibuild/pb
DEPFLAGS=-MMD -MP -MF $(BUILDDIR)/$*.d

SOURCES=main.c $(BUILDDIR)/pb/cmd.pb.c lib/pb_common.c lib/pb_decode.c lib/pb_encode.c
OBJECTS=$(addprefix $(BUILDDIR)/,$(SOURCES:.c=.o))
DEPENDENCIES=$(addprefix $(BUILDDIR)/,$(SOURCES:.c=.d))

BUILDDIR=build

SILENT?=

.PHONY: all

all: $(BUILDDIR)/main.out

clean:
	rm -rf $(BUILDDIR)

# Link output:
$(BUILDDIR)/main.out: $(OBJECTS)
	$(SILENT) $(CC) $(CFLAGS) $^ -o $@

# Compile sources:
$(BUILDDIR)/%.o: %.c makefile
	@mkdir -p $(dir $@)
	$(SILENT) $(CC) -c $(CFLAGS) $(DEPFLAGS) $*.c -o $@

# main.c also needs the protobuf stuff:
$(BUILDDIR)/main.o: $(BUILDDIR)/pb/cmd.pb.c

$(BUILDDIR)/pb/cmd.pb.c: ../cmd.proto
	@mkdir -p $(BUILDDIR)/pb
	$(SILENT) nanopb_generator -I.. -D$(BUILDDIR)/pb ../cmd.proto

$(BUILDDIR)/pb/cmd.pb.h: $(BUILDDIR)/pb/cmd.pb.c

# Re-generate compile_commands.json using either bear or compiledb
.PHONY: compile_commands
# GEN_COMP_COMMANDS_CMD=compiledb make
GEN_COMP_COMMANDS_CMD=bear -- make
compile_commands: clean
	$(GEN_COMP_COMMANDS_CMD)

# Keep dependencies around, make them an explicit target:
$(DEPENDENCIES):

# Keep object files and output files:
.PRECIOUS: $(BUILDDIR)/%.out
.PRECIOUS: $(BUILDDIR)/%.o

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),compile_commands)
include $(DEPENDENCIES)
endif
endif
