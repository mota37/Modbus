################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
enc28j60.obj: ../enc28j60.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --include_path="C:/StellarisWare" --include_path="C:/StellarisWare/boards/ek-lm4f120xl" --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="C:/Users/Ben/Documents/workspace/enc28j60-booster-example" --include_path="C:/Users/Ben/Documents/workspace/enc28j60-booster-example/uip-1.0/uip" --include_path="C:/Users/Ben/Documents/workspace/enc28j60-booster-example/uip-1.0" --relaxed_ansi --gcc --define=TARGET_IS_BLIZZARD_RA2 --define=PART_LM4F120H5QR --define=UART_BUFFERED --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="enc28j60.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --include_path="C:/StellarisWare" --include_path="C:/StellarisWare/boards/ek-lm4f120xl" --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="C:/Users/Ben/Documents/workspace/enc28j60-booster-example" --include_path="C:/Users/Ben/Documents/workspace/enc28j60-booster-example/uip-1.0/uip" --include_path="C:/Users/Ben/Documents/workspace/enc28j60-booster-example/uip-1.0" --relaxed_ansi --gcc --define=TARGET_IS_BLIZZARD_RA2 --define=PART_LM4F120H5QR --define=UART_BUFFERED --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

modbusd.obj: ../modbusd.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --include_path="C:/StellarisWare" --include_path="C:/StellarisWare/boards/ek-lm4f120xl" --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="C:/Users/Ben/Documents/workspace/enc28j60-booster-example" --include_path="C:/Users/Ben/Documents/workspace/enc28j60-booster-example/uip-1.0/uip" --include_path="C:/Users/Ben/Documents/workspace/enc28j60-booster-example/uip-1.0" --relaxed_ansi --gcc --define=TARGET_IS_BLIZZARD_RA2 --define=PART_LM4F120H5QR --define=UART_BUFFERED --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="modbusd.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_ccs.obj: ../startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --include_path="C:/StellarisWare" --include_path="C:/StellarisWare/boards/ek-lm4f120xl" --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="C:/Users/Ben/Documents/workspace/enc28j60-booster-example" --include_path="C:/Users/Ben/Documents/workspace/enc28j60-booster-example/uip-1.0/uip" --include_path="C:/Users/Ben/Documents/workspace/enc28j60-booster-example/uip-1.0" --relaxed_ansi --gcc --define=TARGET_IS_BLIZZARD_RA2 --define=PART_LM4F120H5QR --define=UART_BUFFERED --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


