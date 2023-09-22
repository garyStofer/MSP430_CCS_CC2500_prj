################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk.c \
D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk_QMgmt.c \
D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk_api.c \
D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk_frame.c \
D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk_globals.c 

OBJS += \
./Components/nwk/nwk.obj \
./Components/nwk/nwk_QMgmt.obj \
./Components/nwk/nwk_api.obj \
./Components/nwk/nwk_frame.obj \
./Components/nwk/nwk_globals.obj 

C_DEPS += \
./Components/nwk/nwk.pp \
./Components/nwk/nwk_QMgmt.pp \
./Components/nwk/nwk_api.pp \
./Components/nwk/nwk_frame.pp \
./Components/nwk/nwk_globals.pp 

C_SRCS_QUOTED += \
"D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk.c" \
"D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk_QMgmt.c" \
"D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk_api.c" \
"D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk_frame.c" \
"D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk_globals.c" 


# Each subdirectory must supply rules for building sources it contributes
Components/nwk/nwk.obj: D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/bin/cl430" --silicon_version=msp -g --define=__MSP430F2274__ --define=MRFI_CC2500 --include_path="C:/Program Files/Texas Instruments/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/include" --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/include" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/bsp" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/bsp/boards/eZ430RF" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/bsp/drivers" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/mrfi" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/simpliciti/nwk" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/simpliciti/nwk_applications" --diag_warning=225 --sat_reassoc=off --fp_reassoc=off --plain_char=unsigned --printf_support=minimal --cmd_file="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../Configuration/smpl_nwk_config.dat" --cmd_file="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../Configuration/Range_Extender/smpl_config.dat" --preproc_with_compile --preproc_dependency="Components/nwk/nwk.pp" --obj_directory="Components/nwk" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

Components/nwk/nwk_QMgmt.obj: D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk_QMgmt.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/bin/cl430" --silicon_version=msp -g --define=__MSP430F2274__ --define=MRFI_CC2500 --include_path="C:/Program Files/Texas Instruments/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/include" --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/include" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/bsp" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/bsp/boards/eZ430RF" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/bsp/drivers" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/mrfi" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/simpliciti/nwk" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/simpliciti/nwk_applications" --diag_warning=225 --sat_reassoc=off --fp_reassoc=off --plain_char=unsigned --printf_support=minimal --cmd_file="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../Configuration/smpl_nwk_config.dat" --cmd_file="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../Configuration/Range_Extender/smpl_config.dat" --preproc_with_compile --preproc_dependency="Components/nwk/nwk_QMgmt.pp" --obj_directory="Components/nwk" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

Components/nwk/nwk_api.obj: D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk_api.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/bin/cl430" --silicon_version=msp -g --define=__MSP430F2274__ --define=MRFI_CC2500 --include_path="C:/Program Files/Texas Instruments/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/include" --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/include" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/bsp" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/bsp/boards/eZ430RF" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/bsp/drivers" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/mrfi" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/simpliciti/nwk" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/simpliciti/nwk_applications" --diag_warning=225 --sat_reassoc=off --fp_reassoc=off --plain_char=unsigned --printf_support=minimal --cmd_file="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../Configuration/smpl_nwk_config.dat" --cmd_file="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../Configuration/Range_Extender/smpl_config.dat" --preproc_with_compile --preproc_dependency="Components/nwk/nwk_api.pp" --obj_directory="Components/nwk" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

Components/nwk/nwk_frame.obj: D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk_frame.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/bin/cl430" --silicon_version=msp -g --define=__MSP430F2274__ --define=MRFI_CC2500 --include_path="C:/Program Files/Texas Instruments/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/include" --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/include" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/bsp" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/bsp/boards/eZ430RF" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/bsp/drivers" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/mrfi" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/simpliciti/nwk" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/simpliciti/nwk_applications" --diag_warning=225 --sat_reassoc=off --fp_reassoc=off --plain_char=unsigned --printf_support=minimal --cmd_file="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../Configuration/smpl_nwk_config.dat" --cmd_file="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../Configuration/Range_Extender/smpl_config.dat" --preproc_with_compile --preproc_dependency="Components/nwk/nwk_frame.pp" --obj_directory="Components/nwk" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

Components/nwk/nwk_globals.obj: D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk_globals.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/bin/cl430" --silicon_version=msp -g --define=__MSP430F2274__ --define=MRFI_CC2500 --include_path="C:/Program Files/Texas Instruments/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/include" --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/include" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/bsp" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/bsp/boards/eZ430RF" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/bsp/drivers" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/mrfi" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/simpliciti/nwk" --include_path="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../../../../Components/simpliciti/nwk_applications" --diag_warning=225 --sat_reassoc=off --fp_reassoc=off --plain_char=unsigned --printf_support=minimal --cmd_file="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../Configuration/smpl_nwk_config.dat" --cmd_file="D:/MSP430_CCS_Projects/SimpliciTI-CCS-1.1.1/Examples/eZ430RF/Polling_with_AP/Project/../Configuration/Range_Extender/smpl_config.dat" --preproc_with_compile --preproc_dependency="Components/nwk/nwk_globals.pp" --obj_directory="Components/nwk" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


