# ---------------------------------------------------------
#TODO test llvm
ifeq ($(TOOLCHAIN),llvm)
  CC      := clang --target=arm-none-eabi
  LD      := clang --target=arm-none-eabi -fuse-ld=lld
  OBJCOPY := llvm-objcopy
  OBJDUMP := llvm-objdump
  SIZE    := llvm-size
else
  PREFIX  ?= arm-none-eabi
  CC      := $(PREFIX)-gcc
  LD      := $(PREFIX)-gcc
  OBJCOPY := $(PREFIX)-objcopy
  OBJDUMP := $(PREFIX)-objdump
  SIZE    := $(PREFIX)-size
endif

GDB = gdb-multiarch

# ---------------------------------------------------------