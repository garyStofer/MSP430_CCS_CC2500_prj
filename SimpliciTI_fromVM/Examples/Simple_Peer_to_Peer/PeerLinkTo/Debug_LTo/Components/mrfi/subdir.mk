################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/MSP430_CCS_CC2500_prj/SimpliciTI_fromVM/Components/mrfi/mrfi.c 

OBJS += \
./Components/mrfi/mrfi.obj 

C_DEPS += \
./Components/mrfi/mrfi.pp 

C_SRCS_QUOTED += \
"D:/MSP430_CCS_CC2500_prj/SimpliciTI_fromVM/Components/mrfi/mrfi.c" 


# Each subdirectory must supply rules for building sources it contributes
Components/mrfi/mrfi.obj: D:/MSP430_CCS_CC2500_prj/SimpliciTI_fromVM/Components/mrfi/mrfi.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/bin/cl430" --silicon_version=msp -g --define=__MSP430F2274__ --define=MRFI_CC2500 --define=THIS_DEVICE_ADDRESS={0x78,0x56,0x34,0x12} --include_path="C:/Program Files/Texas Instruments/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/include" --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/include" --include_path="D:/MSP430_CCS_CC2500_prj/SimpliciTI_fromVM/Examples/Simple_Peer_to_Peer/PeerLinkTo/../../../Components/bsp" --include_path="D:/MSP430_CCS_CC2500_prj/SimpliciTI_fromVM/Examples/Simple_Peer_to_Peer/PeerLinkTo/../../../Components/mrfi" --include_path="D:/MSP430_CCS_CC2500_prj/SimpliciTI_fromVM/Examples/Simple_Peer_to_Peer/PeerLinkTo/../../../Components/bsp/drivers" --include_path="D:/MSP430_CCS_CC2500_prj/SimpliciTI_fromVM/Examples/Simple_Peer_to_Peer/PeerLinkTo/../../../Components/bsp/boards/eZ430RF" --include_path="D:/MSP430_CCS_CC2500_prj/SimpliciTI_fromVM/Examples/Simple_Peer_to_Peer/PeerLinkTo/../../../Components/simpliciti/nwk" --include_path="D:/MSP430_CCS_CC2500_prj/SimpliciTI_fromVM/Examples/Simple_Peer_to_Peer/PeerLinkTo/../../../Components/simpliciti/nwk_applications" --diag_warning=225 --sat_reassoc=off --fp_reassoc=off --plain_char=unsigned --printf_support=minimal --cmd_file="D:/MSP430_CCS_CC2500_prj/SimpliciTI_fromVM/Examples/Simple_Peer_to_Peer/PeerLinkTo/../Common/smpl_nwk_config.dat" --cmd_file="D:/MSP430_CCS_CC2500_prj/SimpliciTI_fromVM/Examples/Simple_Peer_to_Peer/PeerLinkTo/../Common/smpl_config.dat" --preproc_with_compile --preproc_dependency="Components/mrfi/mrfi.pp" --obj_directory="Components/mrfi" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


