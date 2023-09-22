################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/main_AP_RE.c 

OBJS += \
./App/main_AP_RE.obj 

C_DEPS += \
./App/main_AP_RE.pp 

C_SRCS_QUOTED += \
"../App/main_AP_RE.c" 


# Each subdirectory must supply rules for building sources it contributes
App/main_AP_RE.obj: ../App/main_AP_RE.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/bin/cl430" --silicon_version=msp -g --define=__MSP430F2274__ --define=MRFI_CC2500 --include_path="C:/Program Files/Texas Instruments/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/include" --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/include" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/bsp" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/bsp/boards/eZ430RF" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/bsp/drivers" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/mrfi" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/simpliciti/nwk" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/simpliciti/nwk_applications" --diag_warning=225 --sat_reassoc=off --fp_reassoc=off --plain_char=unsigned --printf_support=minimal --cmd_file="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../Configuration/smpl_nwk_config.dat" --cmd_file="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../Configuration/Range_Extender/smpl_config.dat" --preproc_with_compile --preproc_dependency="App/main_AP_RE.pp" --obj_directory="App" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


