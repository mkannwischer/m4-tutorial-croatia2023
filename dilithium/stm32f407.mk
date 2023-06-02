
OPENCM3_DIR =../libopencm3

DEVICE=stm32f407vg
LIBNAME= opencm3_stm32f4
ARCH_FLAGS = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16

DEFINES=-DSTM32F4 -DSTM32F407VG

CFLAGS		+= -O3\
		   -Wall -Wextra -Wimplicit-function-declaration \
		   -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes \
		   -Wundef -Wshadow \
		   -I$(OPENCM3_DIR)/include \
		   -Icommon \
		   -fno-common $(ARCH_FLAGS) -MD $(DEFINES)

LDSCRIPT = common/$(DEVICE).ld

LDLIBS += -l$(LIBNAME)
LIBDEPS += $(OPENCM3_DIR)/lib/lib$(LIBNAME).a

LDFLAGS += -L$(OPENCM3_DIR)/lib
LDFLAGS += \
	--specs=nosys.specs \
	-Wl,--wrap=_sbrk \
	-nostartfiles \
	-ffreestanding \
	-T$(LDSCRIPT) \
	$(ARCH_FLAGS)


LINKDEPS += obj/common/hal-opencm3.c.o obj/test.c.o

all: bin/stm32f407-test.bin

$(OPENCM3_DIR)/lib/lib$(LIBNAME).a:
	$(MAKE) -C $(OPENCM3_DIR)

obj/hal-opencm3.o: $(OPENCM3_DIR)/lib/lib$(LIBNAME).a

