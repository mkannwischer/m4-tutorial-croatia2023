
all: bin/qemu-test.bin

OBJS=obj/test.c.o
MPS2_DATA_IN_FLASH = 1
LDSCRIPT = obj/ldscript.ld
ARCH_FLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

CFLAGS += \
	$(ARCH_FLAGS) \
	--specs=nosys.specs \
	-Icommon -Icommon/mps2

CPPFLAGS += \
	-DMPS2_AN386

LDFLAGS += \
	--specs=nosys.specs \
	-Wl,--wrap=_sbrk \
	-ffreestanding \
	-Lobj \
	-T$(LDSCRIPT) \
	$(ARCH_FLAGS)

LIBHAL_SRC := \
	common/mps2/startup_MPS2.S \
	common/hal-mps2.c \
	common/randombytes.c

objs = $(addprefix obj/,$(addsuffix .o,$(1)))
obj/libpqm4hal.a: $(call objs,$(LIBHAL_SRC))
obj/libpqm4hal.a: CPPFLAGS += -Icommon/mps2
$(LDSCRIPT): CPPFLAGS += $(if $(MPS2_DATA_IN_FLASH),-DDATA_IN_FLASH)
obj/common/mps2/startup_MPS2.S.o: CFLAGS += -DDATA_IN_FLASH

LDLIBS += -lpqm4hal$(if $(NO_RANDOMBYTES),-nornd)
LIBDEPS += obj/libpqm4hal.a

$(LDSCRIPT): common/mps2/MPS2.ld
	@printf "  GENLNK  $@\n"; \
	[ -d $(@D) ] || $(Q)mkdir -p $(@D); \
	arm-none-eabi-gcc -x assembler-with-cpp -E -Wp,-P $(CPPFLAGS) $< -o $@

$(LDSCRIPT): CPPFLAGS += -Icommon/mps2

LINKDEPS += $(LDSCRIPT) $(LIBDEPS) obj/test.c.o obj/chacha20.c.o obj/chacha.S.o

ENABLE_QEMU_TESTS = 1
QEMU = qemu-system-arm
QEMUFLAGS = -M mps2-an386 -nographic -semihosting


run-qemu: bin/qemu-test.bin
	$(QEMU) $(QEMUFLAGS) -kernel bin/qemu-test.bin

