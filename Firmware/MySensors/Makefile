#-------------------------------------------------------------------------------
# Denis Zheleznyakov aka ZiB @ http://ziblog.ru
#-------------------------------------------------------------------------------

SRC_C += main.c
SRC_C += macros.c
SRC_C += interrupt.c
SRC_C += button.c
SRC_C += si24r01.c
SRC_C += mysensors.c

ifeq ($(MCU),STM32L011F4P6)

SRC_C += stm32l0xx_ll_adc.c
SRC_C += stm32l0xx_ll_comp.c
SRC_C += stm32l0xx_ll_crc.c
SRC_C += stm32l0xx_ll_crs.c
SRC_C += stm32l0xx_ll_dac.c
SRC_C += stm32l0xx_ll_dma.c
SRC_C += stm32l0xx_ll_exti.c
SRC_C += stm32l0xx_ll_gpio.c
SRC_C += stm32l0xx_ll_i2c.c
SRC_C += stm32l0xx_ll_lptim.c
SRC_C += stm32l0xx_ll_lpuart.c
SRC_C += stm32l0xx_ll_pwr.c
SRC_C += stm32l0xx_ll_rcc.c
SRC_C += stm32l0xx_ll_rng.c
SRC_C += stm32l0xx_ll_rtc.c
SRC_C += stm32l0xx_ll_spi.c
SRC_C += stm32l0xx_ll_tim.c
SRC_C += stm32l0xx_ll_usart.c

SRC_ASM += stm32l011f4p6_startup.s

INCLUDES += -Imcu/std_lib/stm32l0xx/inc

VPATH += mcu/std_lib/stm32l0xx/src

FLAGS_C += -march=armv6-m
FLAGS_C += -mtune=cortex-m0plus.small-multiply
FLAGS_C += -mcpu=cortex-m0plus.small-multiply
FLAGS_C += -mfix-cortex-m3-ldrd
FLAGS_C += -DARM_MATH_CM0
FLAGS_C += -D__thumb2__=1
FLAGS_C += -mno-unaligned-access

FLAGS_LD += -mcpu=cortex-m0plus.small-multiply

endif

INCLUDES += -I$(GCC_PATH)/arm-none-eabi/include
INCLUDES += -I$(GCC_PATH)/arm-none-eabi/include/lib
INCLUDES += -Imcu
INCLUDES += -Imcu/core
INCLUDES += -Imcu/startup
INCLUDES += -Imcu/peripherals
INCLUDES += -Iutility
INCLUDES += -Izhome
INCLUDES += -Idevice

VPATH += $(GCC_PATH)/arm-none-eabi/include
VPATH += $(GCC_PATH)/arm-none-eabi/include/lib
VPATH += mcu
VPATH += mcu/core
VPATH += mcu/startup
VPATH += mcu/peripherals
VPATH += utility
VPATH += zhome
VPATH += device

FLAGS_C += $(INCLUDES) -I.
FLAGS_C += -D$(MCU)
FLAGS_C += -D$(PCB)
FLAGS_C += -Os -g
FLAGS_C += -std=c99
FLAGS_C += -Wall
FLAGS_C += -Wstack-usage=128
FLAGS_C += -funroll-loops
FLAGS_C += -fmessage-length=0
FLAGS_C += -fno-builtin
FLAGS_C += -ffunction-sections
FLAGS_C += -fdata-sections
FLAGS_C += -fno-hosted
FLAGS_C += -fno-strict-aliasing
FLAGS_C += -ffast-math
FLAGS_C += -msoft-float
FLAGS_C += -mfloat-abi=soft
FLAGS_C += -mapcs-frame
FLAGS_C += -mno-sched-prolog
FLAGS_C += -mthumb
FLAGS_C += -nostdlib -nostartfiles

FLAGS_LD += -mthumb
FLAGS_LD += -static
FLAGS_LD += -Wl,-Map=target/target.map
FLAGS_LD += -Wl,--gc-sections
FLAGS_LD += -Wl,--start-group -lc -lm -Wl,--end-group
FLAGS_LD += -Wl,-init,hello_init,-fini,hello_fini
FLAGS_LD += --specs=nano.specs

FLAGS_ASM  = -D__ASSEMBLY__
FLAGS_ASM += -g $(FLAGS_C)
FLAGS_ASM += -I. -x assembler-with-cpp

all: clean
	@make -f MyMakefile MCU=STM32L011F4P6 PCB=PCB_R1 -s pre_build
	@make -f MyMakefile MCU=STM32L011F4P6 PCB=PCB_R1 -s -j main
	@make -f MyMakefile MCU=STM32L011F4P6 PCB=PCB_R1 -s post_build	
	
main: $(SRC_ASM:%.S=target/%.o) $(SRC_C:%.c=target/%.o)
	@$(GCC_PREFIX)gcc.exe $(FLAGS_LD) -T'mcu/startup/$(MCU)_flash.ld' -o 'target/main.elf' $^ $(LIB_LD)
	@$(GCC_PREFIX)size 'target/main.elf'
	@$(GCC_PREFIX)objcopy -O ihex 'target/main.elf' 'target/main.hex'
	@rm -f target/*.o target/*.map

$(SRC_C:%.c=target/%.o): target/%.o: %.c
	@$(GCC_PREFIX)gcc.exe $(FLAGS_C) -c $< -o $@

$(SRC_ASM:%.s=target/%.o): target/%.o: %.s
	@$(GCC_PREFIX)gcc.exe $(FLAGS_ASM) -c $< -o $@

clean:
	@mkdir -p target
	@rm -f target/*.*
	