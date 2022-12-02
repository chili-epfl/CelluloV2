#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-pic32mz1024ecg064.mk)" "nbproject/Makefile-local-pic32mz1024ecg064.mk"
include nbproject/Makefile-local-pic32mz1024ecg064.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=pic32mz1024ecg064
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/cellulo-firmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/cellulo-firmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/libdots/decoder.c ../src/libdots/dots_localization.c ../src/libdots/grid.c ../src/libdots/imgproc.c ../src/libdots/imgsegment.c ../src/libdots/intgrid.c ../src/libdots/gridprobability.c ../src/libdots/intlist.c ../src/system_config/pic32mz1024ecg064/framework/driver/i2c/src/drv_i2c_static.c ../src/system_config/pic32mz1024ecg064/framework/driver/spi/src/drv_spi_static.c ../src/system_config/pic32mz1024ecg064/framework/driver/usart/drv_usart_static.c ../src/system_config/pic32mz1024ecg064/framework/system/clk/src/sys_clk_static.c ../src/system_config/pic32mz1024ecg064/framework/system/ports/src/sys_ports_static.c ../src/system_config/pic32mz1024ecg064/system_init.c ../src/system_config/pic32mz1024ecg064/system_interrupt.c ../src/system_config/pic32mz1024ecg064/system_tasks.c ../src/main.c ../src/led.c ../src/i2c.c ../src/mt9v034.c ../src/cam.c ../src/bluetooth.c ../src/at42qt1070.c ../src/touch.c ../src/battery.c ../src/timer.c ../src/power.c ../src/watchdog.c ../src/motor.c ../src/motion.c ../src/localization.c ../src/pid.c ../src/gesture.c ../src/haptics.c ../src/bezier.c /opt/microchip/harmony/v1_03/framework/system/devcon/src/sys_devcon.c /opt/microchip/harmony/v1_03/framework/system/devcon/src/sys_devcon_pic32mz.c /opt/microchip/harmony/v1_03/framework/system/devcon/src/sys_devcon_cache_pic32mz.S /opt/microchip/harmony/v1_03/framework/system/int/src/sys_int_pic32.c /opt/microchip/harmony/v1_03/framework/system/ports/src/sys_ports.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/115108523/decoder.o ${OBJECTDIR}/_ext/115108523/dots_localization.o ${OBJECTDIR}/_ext/115108523/grid.o ${OBJECTDIR}/_ext/115108523/imgproc.o ${OBJECTDIR}/_ext/115108523/imgsegment.o ${OBJECTDIR}/_ext/115108523/intgrid.o ${OBJECTDIR}/_ext/115108523/gridprobability.o ${OBJECTDIR}/_ext/115108523/intlist.o ${OBJECTDIR}/_ext/63677053/drv_i2c_static.o ${OBJECTDIR}/_ext/2001973653/drv_spi_static.o ${OBJECTDIR}/_ext/1040884377/drv_usart_static.o ${OBJECTDIR}/_ext/563981586/sys_clk_static.o ${OBJECTDIR}/_ext/672157374/sys_ports_static.o ${OBJECTDIR}/_ext/1055008133/system_init.o ${OBJECTDIR}/_ext/1055008133/system_interrupt.o ${OBJECTDIR}/_ext/1055008133/system_tasks.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/led.o ${OBJECTDIR}/_ext/1360937237/i2c.o ${OBJECTDIR}/_ext/1360937237/mt9v034.o ${OBJECTDIR}/_ext/1360937237/cam.o ${OBJECTDIR}/_ext/1360937237/bluetooth.o ${OBJECTDIR}/_ext/1360937237/at42qt1070.o ${OBJECTDIR}/_ext/1360937237/touch.o ${OBJECTDIR}/_ext/1360937237/battery.o ${OBJECTDIR}/_ext/1360937237/timer.o ${OBJECTDIR}/_ext/1360937237/power.o ${OBJECTDIR}/_ext/1360937237/watchdog.o ${OBJECTDIR}/_ext/1360937237/motor.o ${OBJECTDIR}/_ext/1360937237/motion.o ${OBJECTDIR}/_ext/1360937237/localization.o ${OBJECTDIR}/_ext/1360937237/pid.o ${OBJECTDIR}/_ext/1360937237/gesture.o ${OBJECTDIR}/_ext/1360937237/haptics.o ${OBJECTDIR}/_ext/1360937237/bezier.o ${OBJECTDIR}/_ext/431183094/sys_devcon.o ${OBJECTDIR}/_ext/431183094/sys_devcon_pic32mz.o ${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o ${OBJECTDIR}/_ext/754521456/sys_int_pic32.o ${OBJECTDIR}/_ext/1419604243/sys_ports.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/115108523/decoder.o.d ${OBJECTDIR}/_ext/115108523/dots_localization.o.d ${OBJECTDIR}/_ext/115108523/grid.o.d ${OBJECTDIR}/_ext/115108523/imgproc.o.d ${OBJECTDIR}/_ext/115108523/imgsegment.o.d ${OBJECTDIR}/_ext/115108523/intgrid.o.d ${OBJECTDIR}/_ext/115108523/gridprobability.o.d ${OBJECTDIR}/_ext/115108523/intlist.o.d ${OBJECTDIR}/_ext/63677053/drv_i2c_static.o.d ${OBJECTDIR}/_ext/2001973653/drv_spi_static.o.d ${OBJECTDIR}/_ext/1040884377/drv_usart_static.o.d ${OBJECTDIR}/_ext/563981586/sys_clk_static.o.d ${OBJECTDIR}/_ext/672157374/sys_ports_static.o.d ${OBJECTDIR}/_ext/1055008133/system_init.o.d ${OBJECTDIR}/_ext/1055008133/system_interrupt.o.d ${OBJECTDIR}/_ext/1055008133/system_tasks.o.d ${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/_ext/1360937237/led.o.d ${OBJECTDIR}/_ext/1360937237/i2c.o.d ${OBJECTDIR}/_ext/1360937237/mt9v034.o.d ${OBJECTDIR}/_ext/1360937237/cam.o.d ${OBJECTDIR}/_ext/1360937237/bluetooth.o.d ${OBJECTDIR}/_ext/1360937237/at42qt1070.o.d ${OBJECTDIR}/_ext/1360937237/touch.o.d ${OBJECTDIR}/_ext/1360937237/battery.o.d ${OBJECTDIR}/_ext/1360937237/timer.o.d ${OBJECTDIR}/_ext/1360937237/power.o.d ${OBJECTDIR}/_ext/1360937237/watchdog.o.d ${OBJECTDIR}/_ext/1360937237/motor.o.d ${OBJECTDIR}/_ext/1360937237/motion.o.d ${OBJECTDIR}/_ext/1360937237/localization.o.d ${OBJECTDIR}/_ext/1360937237/pid.o.d ${OBJECTDIR}/_ext/1360937237/gesture.o.d ${OBJECTDIR}/_ext/1360937237/haptics.o.d ${OBJECTDIR}/_ext/1360937237/bezier.o.d ${OBJECTDIR}/_ext/431183094/sys_devcon.o.d ${OBJECTDIR}/_ext/431183094/sys_devcon_pic32mz.o.d ${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o.d ${OBJECTDIR}/_ext/754521456/sys_int_pic32.o.d ${OBJECTDIR}/_ext/1419604243/sys_ports.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/115108523/decoder.o ${OBJECTDIR}/_ext/115108523/dots_localization.o ${OBJECTDIR}/_ext/115108523/grid.o ${OBJECTDIR}/_ext/115108523/imgproc.o ${OBJECTDIR}/_ext/115108523/imgsegment.o ${OBJECTDIR}/_ext/115108523/intgrid.o ${OBJECTDIR}/_ext/115108523/gridprobability.o ${OBJECTDIR}/_ext/115108523/intlist.o ${OBJECTDIR}/_ext/63677053/drv_i2c_static.o ${OBJECTDIR}/_ext/2001973653/drv_spi_static.o ${OBJECTDIR}/_ext/1040884377/drv_usart_static.o ${OBJECTDIR}/_ext/563981586/sys_clk_static.o ${OBJECTDIR}/_ext/672157374/sys_ports_static.o ${OBJECTDIR}/_ext/1055008133/system_init.o ${OBJECTDIR}/_ext/1055008133/system_interrupt.o ${OBJECTDIR}/_ext/1055008133/system_tasks.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/led.o ${OBJECTDIR}/_ext/1360937237/i2c.o ${OBJECTDIR}/_ext/1360937237/mt9v034.o ${OBJECTDIR}/_ext/1360937237/cam.o ${OBJECTDIR}/_ext/1360937237/bluetooth.o ${OBJECTDIR}/_ext/1360937237/at42qt1070.o ${OBJECTDIR}/_ext/1360937237/touch.o ${OBJECTDIR}/_ext/1360937237/battery.o ${OBJECTDIR}/_ext/1360937237/timer.o ${OBJECTDIR}/_ext/1360937237/power.o ${OBJECTDIR}/_ext/1360937237/watchdog.o ${OBJECTDIR}/_ext/1360937237/motor.o ${OBJECTDIR}/_ext/1360937237/motion.o ${OBJECTDIR}/_ext/1360937237/localization.o ${OBJECTDIR}/_ext/1360937237/pid.o ${OBJECTDIR}/_ext/1360937237/gesture.o ${OBJECTDIR}/_ext/1360937237/haptics.o ${OBJECTDIR}/_ext/1360937237/bezier.o ${OBJECTDIR}/_ext/431183094/sys_devcon.o ${OBJECTDIR}/_ext/431183094/sys_devcon_pic32mz.o ${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o ${OBJECTDIR}/_ext/754521456/sys_int_pic32.o ${OBJECTDIR}/_ext/1419604243/sys_ports.o

# Source Files
SOURCEFILES=../src/libdots/decoder.c ../src/libdots/dots_localization.c ../src/libdots/grid.c ../src/libdots/imgproc.c ../src/libdots/imgsegment.c ../src/libdots/intgrid.c ../src/libdots/gridprobability.c ../src/libdots/intlist.c ../src/system_config/pic32mz1024ecg064/framework/driver/i2c/src/drv_i2c_static.c ../src/system_config/pic32mz1024ecg064/framework/driver/spi/src/drv_spi_static.c ../src/system_config/pic32mz1024ecg064/framework/driver/usart/drv_usart_static.c ../src/system_config/pic32mz1024ecg064/framework/system/clk/src/sys_clk_static.c ../src/system_config/pic32mz1024ecg064/framework/system/ports/src/sys_ports_static.c ../src/system_config/pic32mz1024ecg064/system_init.c ../src/system_config/pic32mz1024ecg064/system_interrupt.c ../src/system_config/pic32mz1024ecg064/system_tasks.c ../src/main.c ../src/led.c ../src/i2c.c ../src/mt9v034.c ../src/cam.c ../src/bluetooth.c ../src/at42qt1070.c ../src/touch.c ../src/battery.c ../src/timer.c ../src/power.c ../src/watchdog.c ../src/motor.c ../src/motion.c ../src/localization.c ../src/pid.c ../src/gesture.c ../src/haptics.c ../src/bezier.c /opt/microchip/harmony/v1_03/framework/system/devcon/src/sys_devcon.c /opt/microchip/harmony/v1_03/framework/system/devcon/src/sys_devcon_pic32mz.c /opt/microchip/harmony/v1_03/framework/system/devcon/src/sys_devcon_cache_pic32mz.S /opt/microchip/harmony/v1_03/framework/system/int/src/sys_int_pic32.c /opt/microchip/harmony/v1_03/framework/system/ports/src/sys_ports.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-pic32mz1024ecg064.mk dist/${CND_CONF}/${IMAGE_TYPE}/cellulo-firmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MZ1024ECG064
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o: /opt/microchip/harmony/v1_03/framework/system/devcon/src/sys_devcon_cache_pic32mz.S  .generated_files/flags/pic32mz1024ecg064/b82d3939eddea565ed2f447ff5d7b52e9c5fbbbf .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/431183094" 
	@${RM} ${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o.d 
	@${RM} ${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o 
	@${RM} ${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o.ok ${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o.d"  -o ${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o /opt/microchip/harmony/v1_03/framework/system/devcon/src/sys_devcon_cache_pic32mz.S  -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1 
	@${FIXDEPS} "${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o.d" "${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o: /opt/microchip/harmony/v1_03/framework/system/devcon/src/sys_devcon_cache_pic32mz.S  .generated_files/flags/pic32mz1024ecg064/42009413d9cad69433939e51faf8cd928500996d .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/431183094" 
	@${RM} ${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o.d 
	@${RM} ${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o 
	@${RM} ${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o.ok ${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o.err 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o.d"  -o ${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o /opt/microchip/harmony/v1_03/framework/system/devcon/src/sys_devcon_cache_pic32mz.S  -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o.asm.d",--gdwarf-2 
	@${FIXDEPS} "${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o.d" "${OBJECTDIR}/_ext/431183094/sys_devcon_cache_pic32mz.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/115108523/decoder.o: ../src/libdots/decoder.c  .generated_files/flags/pic32mz1024ecg064/d3e094994d316024a886cc60bfe38c837d77666e .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/115108523" 
	@${RM} ${OBJECTDIR}/_ext/115108523/decoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/115108523/decoder.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/115108523/decoder.o.d" -o ${OBJECTDIR}/_ext/115108523/decoder.o ../src/libdots/decoder.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/115108523/dots_localization.o: ../src/libdots/dots_localization.c  .generated_files/flags/pic32mz1024ecg064/b581bca7ce5abf96ec32cd84f8496031a33d51ba .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/115108523" 
	@${RM} ${OBJECTDIR}/_ext/115108523/dots_localization.o.d 
	@${RM} ${OBJECTDIR}/_ext/115108523/dots_localization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/115108523/dots_localization.o.d" -o ${OBJECTDIR}/_ext/115108523/dots_localization.o ../src/libdots/dots_localization.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/115108523/grid.o: ../src/libdots/grid.c  .generated_files/flags/pic32mz1024ecg064/c42e8ece0d10154778ea60ce6319351a4ccafdf0 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/115108523" 
	@${RM} ${OBJECTDIR}/_ext/115108523/grid.o.d 
	@${RM} ${OBJECTDIR}/_ext/115108523/grid.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/115108523/grid.o.d" -o ${OBJECTDIR}/_ext/115108523/grid.o ../src/libdots/grid.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/115108523/imgproc.o: ../src/libdots/imgproc.c  .generated_files/flags/pic32mz1024ecg064/b6f4be1cf1c98ec65f4b8f7d1472d43958a92190 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/115108523" 
	@${RM} ${OBJECTDIR}/_ext/115108523/imgproc.o.d 
	@${RM} ${OBJECTDIR}/_ext/115108523/imgproc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/115108523/imgproc.o.d" -o ${OBJECTDIR}/_ext/115108523/imgproc.o ../src/libdots/imgproc.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/115108523/imgsegment.o: ../src/libdots/imgsegment.c  .generated_files/flags/pic32mz1024ecg064/eda89e560ad4d3fbab5bd340114e8e1887f0f622 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/115108523" 
	@${RM} ${OBJECTDIR}/_ext/115108523/imgsegment.o.d 
	@${RM} ${OBJECTDIR}/_ext/115108523/imgsegment.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/115108523/imgsegment.o.d" -o ${OBJECTDIR}/_ext/115108523/imgsegment.o ../src/libdots/imgsegment.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/115108523/intgrid.o: ../src/libdots/intgrid.c  .generated_files/flags/pic32mz1024ecg064/929767c164adea9c43f74bf7be0f614f196cc868 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/115108523" 
	@${RM} ${OBJECTDIR}/_ext/115108523/intgrid.o.d 
	@${RM} ${OBJECTDIR}/_ext/115108523/intgrid.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/115108523/intgrid.o.d" -o ${OBJECTDIR}/_ext/115108523/intgrid.o ../src/libdots/intgrid.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/115108523/gridprobability.o: ../src/libdots/gridprobability.c  .generated_files/flags/pic32mz1024ecg064/c82974f26946494882f1e498d22fb414c9cac418 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/115108523" 
	@${RM} ${OBJECTDIR}/_ext/115108523/gridprobability.o.d 
	@${RM} ${OBJECTDIR}/_ext/115108523/gridprobability.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/115108523/gridprobability.o.d" -o ${OBJECTDIR}/_ext/115108523/gridprobability.o ../src/libdots/gridprobability.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/115108523/intlist.o: ../src/libdots/intlist.c  .generated_files/flags/pic32mz1024ecg064/4761c7656f74df25a041d9c4c983d64902c2a4d6 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/115108523" 
	@${RM} ${OBJECTDIR}/_ext/115108523/intlist.o.d 
	@${RM} ${OBJECTDIR}/_ext/115108523/intlist.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/115108523/intlist.o.d" -o ${OBJECTDIR}/_ext/115108523/intlist.o ../src/libdots/intlist.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/63677053/drv_i2c_static.o: ../src/system_config/pic32mz1024ecg064/framework/driver/i2c/src/drv_i2c_static.c  .generated_files/flags/pic32mz1024ecg064/ebcf87d4a60e891c3596440d98ed0ef55f072517 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/63677053" 
	@${RM} ${OBJECTDIR}/_ext/63677053/drv_i2c_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/63677053/drv_i2c_static.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/63677053/drv_i2c_static.o.d" -o ${OBJECTDIR}/_ext/63677053/drv_i2c_static.o ../src/system_config/pic32mz1024ecg064/framework/driver/i2c/src/drv_i2c_static.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/2001973653/drv_spi_static.o: ../src/system_config/pic32mz1024ecg064/framework/driver/spi/src/drv_spi_static.c  .generated_files/flags/pic32mz1024ecg064/69d1a32b4b6ebac54d6e0dcdea8de8a3385855f5 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/2001973653" 
	@${RM} ${OBJECTDIR}/_ext/2001973653/drv_spi_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/2001973653/drv_spi_static.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/2001973653/drv_spi_static.o.d" -o ${OBJECTDIR}/_ext/2001973653/drv_spi_static.o ../src/system_config/pic32mz1024ecg064/framework/driver/spi/src/drv_spi_static.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1040884377/drv_usart_static.o: ../src/system_config/pic32mz1024ecg064/framework/driver/usart/drv_usart_static.c  .generated_files/flags/pic32mz1024ecg064/3433eee53a1fd2e0388404270dc8867b06f5c6d2 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1040884377" 
	@${RM} ${OBJECTDIR}/_ext/1040884377/drv_usart_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/1040884377/drv_usart_static.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1040884377/drv_usart_static.o.d" -o ${OBJECTDIR}/_ext/1040884377/drv_usart_static.o ../src/system_config/pic32mz1024ecg064/framework/driver/usart/drv_usart_static.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/563981586/sys_clk_static.o: ../src/system_config/pic32mz1024ecg064/framework/system/clk/src/sys_clk_static.c  .generated_files/flags/pic32mz1024ecg064/5cea7ad241baa3f7209dd86190c2f37cd9b8dedc .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/563981586" 
	@${RM} ${OBJECTDIR}/_ext/563981586/sys_clk_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/563981586/sys_clk_static.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/563981586/sys_clk_static.o.d" -o ${OBJECTDIR}/_ext/563981586/sys_clk_static.o ../src/system_config/pic32mz1024ecg064/framework/system/clk/src/sys_clk_static.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/672157374/sys_ports_static.o: ../src/system_config/pic32mz1024ecg064/framework/system/ports/src/sys_ports_static.c  .generated_files/flags/pic32mz1024ecg064/7b15adca4e4738d5fe0cdb06ac3e500beee2aaf3 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/672157374" 
	@${RM} ${OBJECTDIR}/_ext/672157374/sys_ports_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/672157374/sys_ports_static.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/672157374/sys_ports_static.o.d" -o ${OBJECTDIR}/_ext/672157374/sys_ports_static.o ../src/system_config/pic32mz1024ecg064/framework/system/ports/src/sys_ports_static.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1055008133/system_init.o: ../src/system_config/pic32mz1024ecg064/system_init.c  .generated_files/flags/pic32mz1024ecg064/72e6fdf49618bfee37c0fcdd0558307a47c27347 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1055008133" 
	@${RM} ${OBJECTDIR}/_ext/1055008133/system_init.o.d 
	@${RM} ${OBJECTDIR}/_ext/1055008133/system_init.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1055008133/system_init.o.d" -o ${OBJECTDIR}/_ext/1055008133/system_init.o ../src/system_config/pic32mz1024ecg064/system_init.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1055008133/system_interrupt.o: ../src/system_config/pic32mz1024ecg064/system_interrupt.c  .generated_files/flags/pic32mz1024ecg064/473845d906c518dc593bc416501ebea970304ce1 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1055008133" 
	@${RM} ${OBJECTDIR}/_ext/1055008133/system_interrupt.o.d 
	@${RM} ${OBJECTDIR}/_ext/1055008133/system_interrupt.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1055008133/system_interrupt.o.d" -o ${OBJECTDIR}/_ext/1055008133/system_interrupt.o ../src/system_config/pic32mz1024ecg064/system_interrupt.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1055008133/system_tasks.o: ../src/system_config/pic32mz1024ecg064/system_tasks.c  .generated_files/flags/pic32mz1024ecg064/5c5a4eaeaaef5eb282bfd2b0b3408b2d96b134eb .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1055008133" 
	@${RM} ${OBJECTDIR}/_ext/1055008133/system_tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1055008133/system_tasks.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1055008133/system_tasks.o.d" -o ${OBJECTDIR}/_ext/1055008133/system_tasks.o ../src/system_config/pic32mz1024ecg064/system_tasks.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/pic32mz1024ecg064/7c068be393a040a703c667f40d045226328eead .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/led.o: ../src/led.c  .generated_files/flags/pic32mz1024ecg064/78820ff449093866bd323c5f54a0eca3abfbfde0 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/led.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/led.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/led.o.d" -o ${OBJECTDIR}/_ext/1360937237/led.o ../src/led.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/i2c.o: ../src/i2c.c  .generated_files/flags/pic32mz1024ecg064/660e0b863b9c2349bdf8e0a0cc1f89513dc8aa93 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/i2c.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/i2c.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/i2c.o.d" -o ${OBJECTDIR}/_ext/1360937237/i2c.o ../src/i2c.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/mt9v034.o: ../src/mt9v034.c  .generated_files/flags/pic32mz1024ecg064/fb90c67861d5136469250042e86fc201ae5c743b .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/mt9v034.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/mt9v034.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/mt9v034.o.d" -o ${OBJECTDIR}/_ext/1360937237/mt9v034.o ../src/mt9v034.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/cam.o: ../src/cam.c  .generated_files/flags/pic32mz1024ecg064/428912281da8526243dd521cb7eb9c874dcd0bdb .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/cam.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/cam.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/cam.o.d" -o ${OBJECTDIR}/_ext/1360937237/cam.o ../src/cam.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/bluetooth.o: ../src/bluetooth.c  .generated_files/flags/pic32mz1024ecg064/19059e43f30bf92b50082cac3ba01080ede74c59 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bluetooth.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bluetooth.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/bluetooth.o.d" -o ${OBJECTDIR}/_ext/1360937237/bluetooth.o ../src/bluetooth.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/at42qt1070.o: ../src/at42qt1070.c  .generated_files/flags/pic32mz1024ecg064/46df9aec6027d7145bac6ea0ec83c15156b6efa3 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/at42qt1070.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/at42qt1070.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/at42qt1070.o.d" -o ${OBJECTDIR}/_ext/1360937237/at42qt1070.o ../src/at42qt1070.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/touch.o: ../src/touch.c  .generated_files/flags/pic32mz1024ecg064/df0d290762d63566155511e47ac3c99d622066a9 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/touch.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/touch.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/touch.o.d" -o ${OBJECTDIR}/_ext/1360937237/touch.o ../src/touch.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/battery.o: ../src/battery.c  .generated_files/flags/pic32mz1024ecg064/3ff9fd621d4d31b495ec4d60c74a4f9a644b629 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/battery.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/battery.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/battery.o.d" -o ${OBJECTDIR}/_ext/1360937237/battery.o ../src/battery.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/timer.o: ../src/timer.c  .generated_files/flags/pic32mz1024ecg064/12973b9f9e70eaa18c91900bd6343a1f64819ec3 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/timer.o.d" -o ${OBJECTDIR}/_ext/1360937237/timer.o ../src/timer.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/power.o: ../src/power.c  .generated_files/flags/pic32mz1024ecg064/d5f16b4a4733fd82003165373b0004bdcf6cf939 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/power.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/power.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/power.o.d" -o ${OBJECTDIR}/_ext/1360937237/power.o ../src/power.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/watchdog.o: ../src/watchdog.c  .generated_files/flags/pic32mz1024ecg064/8ec2c5c66ece9608f5df0c901f320bc6ee2ebf9a .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/watchdog.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/watchdog.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/watchdog.o.d" -o ${OBJECTDIR}/_ext/1360937237/watchdog.o ../src/watchdog.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/motor.o: ../src/motor.c  .generated_files/flags/pic32mz1024ecg064/1292c05be14e2a909ff9a5dbd2565df5c8ebb358 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/motor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/motor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/motor.o.d" -o ${OBJECTDIR}/_ext/1360937237/motor.o ../src/motor.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/motion.o: ../src/motion.c  .generated_files/flags/pic32mz1024ecg064/3048e1e3c72e61be305877505efcaba5c76ce7eb .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/motion.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/motion.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/motion.o.d" -o ${OBJECTDIR}/_ext/1360937237/motion.o ../src/motion.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/localization.o: ../src/localization.c  .generated_files/flags/pic32mz1024ecg064/9af51080a89dd1792fa824cf6a4ae61498321c08 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/localization.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/localization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/localization.o.d" -o ${OBJECTDIR}/_ext/1360937237/localization.o ../src/localization.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/pid.o: ../src/pid.c  .generated_files/flags/pic32mz1024ecg064/e4c12ae5eb3b407d3d924624285ee1ffaa8a220c .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pid.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pid.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/pid.o.d" -o ${OBJECTDIR}/_ext/1360937237/pid.o ../src/pid.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/gesture.o: ../src/gesture.c  .generated_files/flags/pic32mz1024ecg064/12af792fa994628753ea1def8016f3b8c257ba1f .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gesture.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gesture.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/gesture.o.d" -o ${OBJECTDIR}/_ext/1360937237/gesture.o ../src/gesture.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/haptics.o: ../src/haptics.c  .generated_files/flags/pic32mz1024ecg064/c9002327566eee64389c3dfb2ab4b113ec33be59 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/haptics.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/haptics.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/haptics.o.d" -o ${OBJECTDIR}/_ext/1360937237/haptics.o ../src/haptics.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/bezier.o: ../src/bezier.c  .generated_files/flags/pic32mz1024ecg064/da187a7f8ae961699bbae68e696b543223bc8225 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bezier.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bezier.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/bezier.o.d" -o ${OBJECTDIR}/_ext/1360937237/bezier.o ../src/bezier.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/431183094/sys_devcon.o: /opt/microchip/harmony/v1_03/framework/system/devcon/src/sys_devcon.c  .generated_files/flags/pic32mz1024ecg064/7247f66ef04e65d1c05ef51141e16ea206826bda .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/431183094" 
	@${RM} ${OBJECTDIR}/_ext/431183094/sys_devcon.o.d 
	@${RM} ${OBJECTDIR}/_ext/431183094/sys_devcon.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/431183094/sys_devcon.o.d" -o ${OBJECTDIR}/_ext/431183094/sys_devcon.o /opt/microchip/harmony/v1_03/framework/system/devcon/src/sys_devcon.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/431183094/sys_devcon_pic32mz.o: /opt/microchip/harmony/v1_03/framework/system/devcon/src/sys_devcon_pic32mz.c  .generated_files/flags/pic32mz1024ecg064/35e9a8b9ff8763f355568b831bcdf09bafedbea4 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/431183094" 
	@${RM} ${OBJECTDIR}/_ext/431183094/sys_devcon_pic32mz.o.d 
	@${RM} ${OBJECTDIR}/_ext/431183094/sys_devcon_pic32mz.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/431183094/sys_devcon_pic32mz.o.d" -o ${OBJECTDIR}/_ext/431183094/sys_devcon_pic32mz.o /opt/microchip/harmony/v1_03/framework/system/devcon/src/sys_devcon_pic32mz.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/754521456/sys_int_pic32.o: /opt/microchip/harmony/v1_03/framework/system/int/src/sys_int_pic32.c  .generated_files/flags/pic32mz1024ecg064/441a7cc9d5ac9159be53b22fbcf4844c29b2cdbb .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/754521456" 
	@${RM} ${OBJECTDIR}/_ext/754521456/sys_int_pic32.o.d 
	@${RM} ${OBJECTDIR}/_ext/754521456/sys_int_pic32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/754521456/sys_int_pic32.o.d" -o ${OBJECTDIR}/_ext/754521456/sys_int_pic32.o /opt/microchip/harmony/v1_03/framework/system/int/src/sys_int_pic32.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1419604243/sys_ports.o: /opt/microchip/harmony/v1_03/framework/system/ports/src/sys_ports.c  .generated_files/flags/pic32mz1024ecg064/dd6f48e4888e2520631f71284e5b54c37b4e7bc3 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1419604243" 
	@${RM} ${OBJECTDIR}/_ext/1419604243/sys_ports.o.d 
	@${RM} ${OBJECTDIR}/_ext/1419604243/sys_ports.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1419604243/sys_ports.o.d" -o ${OBJECTDIR}/_ext/1419604243/sys_ports.o /opt/microchip/harmony/v1_03/framework/system/ports/src/sys_ports.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
else
${OBJECTDIR}/_ext/115108523/decoder.o: ../src/libdots/decoder.c  .generated_files/flags/pic32mz1024ecg064/83dcd1bbaea2e0cf60683c9c3384613a05654399 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/115108523" 
	@${RM} ${OBJECTDIR}/_ext/115108523/decoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/115108523/decoder.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/115108523/decoder.o.d" -o ${OBJECTDIR}/_ext/115108523/decoder.o ../src/libdots/decoder.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/115108523/dots_localization.o: ../src/libdots/dots_localization.c  .generated_files/flags/pic32mz1024ecg064/2b47b6671c6cce42b9f517d7a644b6717ef4651 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/115108523" 
	@${RM} ${OBJECTDIR}/_ext/115108523/dots_localization.o.d 
	@${RM} ${OBJECTDIR}/_ext/115108523/dots_localization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/115108523/dots_localization.o.d" -o ${OBJECTDIR}/_ext/115108523/dots_localization.o ../src/libdots/dots_localization.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/115108523/grid.o: ../src/libdots/grid.c  .generated_files/flags/pic32mz1024ecg064/b0cff7d3401abc495b8ed47f644e2763171e54fc .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/115108523" 
	@${RM} ${OBJECTDIR}/_ext/115108523/grid.o.d 
	@${RM} ${OBJECTDIR}/_ext/115108523/grid.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/115108523/grid.o.d" -o ${OBJECTDIR}/_ext/115108523/grid.o ../src/libdots/grid.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/115108523/imgproc.o: ../src/libdots/imgproc.c  .generated_files/flags/pic32mz1024ecg064/c774eee9fb9bcbae99f47ec7a6039d5c6cc68e19 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/115108523" 
	@${RM} ${OBJECTDIR}/_ext/115108523/imgproc.o.d 
	@${RM} ${OBJECTDIR}/_ext/115108523/imgproc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/115108523/imgproc.o.d" -o ${OBJECTDIR}/_ext/115108523/imgproc.o ../src/libdots/imgproc.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/115108523/imgsegment.o: ../src/libdots/imgsegment.c  .generated_files/flags/pic32mz1024ecg064/931275dadb136ae6631e7765c1e91117f788aeae .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/115108523" 
	@${RM} ${OBJECTDIR}/_ext/115108523/imgsegment.o.d 
	@${RM} ${OBJECTDIR}/_ext/115108523/imgsegment.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/115108523/imgsegment.o.d" -o ${OBJECTDIR}/_ext/115108523/imgsegment.o ../src/libdots/imgsegment.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/115108523/intgrid.o: ../src/libdots/intgrid.c  .generated_files/flags/pic32mz1024ecg064/34aed3ce4ebe00d10e85fac95442e23f4239c9f .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/115108523" 
	@${RM} ${OBJECTDIR}/_ext/115108523/intgrid.o.d 
	@${RM} ${OBJECTDIR}/_ext/115108523/intgrid.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/115108523/intgrid.o.d" -o ${OBJECTDIR}/_ext/115108523/intgrid.o ../src/libdots/intgrid.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/115108523/gridprobability.o: ../src/libdots/gridprobability.c  .generated_files/flags/pic32mz1024ecg064/21caf5f843e66e43761006b58a6024dd79d1f6c2 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/115108523" 
	@${RM} ${OBJECTDIR}/_ext/115108523/gridprobability.o.d 
	@${RM} ${OBJECTDIR}/_ext/115108523/gridprobability.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/115108523/gridprobability.o.d" -o ${OBJECTDIR}/_ext/115108523/gridprobability.o ../src/libdots/gridprobability.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/115108523/intlist.o: ../src/libdots/intlist.c  .generated_files/flags/pic32mz1024ecg064/db50fe0411b35e54e3b12f12b29bc6b6b1c14219 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/115108523" 
	@${RM} ${OBJECTDIR}/_ext/115108523/intlist.o.d 
	@${RM} ${OBJECTDIR}/_ext/115108523/intlist.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/115108523/intlist.o.d" -o ${OBJECTDIR}/_ext/115108523/intlist.o ../src/libdots/intlist.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/63677053/drv_i2c_static.o: ../src/system_config/pic32mz1024ecg064/framework/driver/i2c/src/drv_i2c_static.c  .generated_files/flags/pic32mz1024ecg064/16c6668af792732e9a3220e49f504208920599df .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/63677053" 
	@${RM} ${OBJECTDIR}/_ext/63677053/drv_i2c_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/63677053/drv_i2c_static.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/63677053/drv_i2c_static.o.d" -o ${OBJECTDIR}/_ext/63677053/drv_i2c_static.o ../src/system_config/pic32mz1024ecg064/framework/driver/i2c/src/drv_i2c_static.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/2001973653/drv_spi_static.o: ../src/system_config/pic32mz1024ecg064/framework/driver/spi/src/drv_spi_static.c  .generated_files/flags/pic32mz1024ecg064/39c629dd6e827a6a2a098bea87f9799084912b4f .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/2001973653" 
	@${RM} ${OBJECTDIR}/_ext/2001973653/drv_spi_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/2001973653/drv_spi_static.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/2001973653/drv_spi_static.o.d" -o ${OBJECTDIR}/_ext/2001973653/drv_spi_static.o ../src/system_config/pic32mz1024ecg064/framework/driver/spi/src/drv_spi_static.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1040884377/drv_usart_static.o: ../src/system_config/pic32mz1024ecg064/framework/driver/usart/drv_usart_static.c  .generated_files/flags/pic32mz1024ecg064/a2ddb7f1e07ee8f6f2e3e09eca9081263c1eaca2 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1040884377" 
	@${RM} ${OBJECTDIR}/_ext/1040884377/drv_usart_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/1040884377/drv_usart_static.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1040884377/drv_usart_static.o.d" -o ${OBJECTDIR}/_ext/1040884377/drv_usart_static.o ../src/system_config/pic32mz1024ecg064/framework/driver/usart/drv_usart_static.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/563981586/sys_clk_static.o: ../src/system_config/pic32mz1024ecg064/framework/system/clk/src/sys_clk_static.c  .generated_files/flags/pic32mz1024ecg064/d945050f014c276e841962e0e72fb72136c73188 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/563981586" 
	@${RM} ${OBJECTDIR}/_ext/563981586/sys_clk_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/563981586/sys_clk_static.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/563981586/sys_clk_static.o.d" -o ${OBJECTDIR}/_ext/563981586/sys_clk_static.o ../src/system_config/pic32mz1024ecg064/framework/system/clk/src/sys_clk_static.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/672157374/sys_ports_static.o: ../src/system_config/pic32mz1024ecg064/framework/system/ports/src/sys_ports_static.c  .generated_files/flags/pic32mz1024ecg064/4c111788973d3cc853a2c425320ad5565d2484f0 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/672157374" 
	@${RM} ${OBJECTDIR}/_ext/672157374/sys_ports_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/672157374/sys_ports_static.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/672157374/sys_ports_static.o.d" -o ${OBJECTDIR}/_ext/672157374/sys_ports_static.o ../src/system_config/pic32mz1024ecg064/framework/system/ports/src/sys_ports_static.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1055008133/system_init.o: ../src/system_config/pic32mz1024ecg064/system_init.c  .generated_files/flags/pic32mz1024ecg064/2a6fd3c04f34b510f23cbbaa9f014df44cb5d569 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1055008133" 
	@${RM} ${OBJECTDIR}/_ext/1055008133/system_init.o.d 
	@${RM} ${OBJECTDIR}/_ext/1055008133/system_init.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1055008133/system_init.o.d" -o ${OBJECTDIR}/_ext/1055008133/system_init.o ../src/system_config/pic32mz1024ecg064/system_init.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1055008133/system_interrupt.o: ../src/system_config/pic32mz1024ecg064/system_interrupt.c  .generated_files/flags/pic32mz1024ecg064/406d07c89e50ee071888e782ce03e74366d38fd2 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1055008133" 
	@${RM} ${OBJECTDIR}/_ext/1055008133/system_interrupt.o.d 
	@${RM} ${OBJECTDIR}/_ext/1055008133/system_interrupt.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1055008133/system_interrupt.o.d" -o ${OBJECTDIR}/_ext/1055008133/system_interrupt.o ../src/system_config/pic32mz1024ecg064/system_interrupt.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1055008133/system_tasks.o: ../src/system_config/pic32mz1024ecg064/system_tasks.c  .generated_files/flags/pic32mz1024ecg064/e5fb00952cdc94c102922da04b0d940979fad1d5 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1055008133" 
	@${RM} ${OBJECTDIR}/_ext/1055008133/system_tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1055008133/system_tasks.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1055008133/system_tasks.o.d" -o ${OBJECTDIR}/_ext/1055008133/system_tasks.o ../src/system_config/pic32mz1024ecg064/system_tasks.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/pic32mz1024ecg064/a0a9301a7b6697eac6a66cc2ab74a60b11f09ba7 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/led.o: ../src/led.c  .generated_files/flags/pic32mz1024ecg064/25d966bb0724670509104f1b125a0b2bf49369a5 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/led.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/led.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/led.o.d" -o ${OBJECTDIR}/_ext/1360937237/led.o ../src/led.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/i2c.o: ../src/i2c.c  .generated_files/flags/pic32mz1024ecg064/22e3efa952d6b97a4a427e61f4cf64c8767f1edd .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/i2c.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/i2c.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/i2c.o.d" -o ${OBJECTDIR}/_ext/1360937237/i2c.o ../src/i2c.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/mt9v034.o: ../src/mt9v034.c  .generated_files/flags/pic32mz1024ecg064/bffd5c22aa77f97a8bbc15648142217feaf2d10a .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/mt9v034.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/mt9v034.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/mt9v034.o.d" -o ${OBJECTDIR}/_ext/1360937237/mt9v034.o ../src/mt9v034.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/cam.o: ../src/cam.c  .generated_files/flags/pic32mz1024ecg064/92bc9f2ec52867e1a46276630a342c01364c42ec .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/cam.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/cam.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/cam.o.d" -o ${OBJECTDIR}/_ext/1360937237/cam.o ../src/cam.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/bluetooth.o: ../src/bluetooth.c  .generated_files/flags/pic32mz1024ecg064/b729bad3a15ba87b5ab00fb74e4234e3d7acf4b9 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bluetooth.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bluetooth.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/bluetooth.o.d" -o ${OBJECTDIR}/_ext/1360937237/bluetooth.o ../src/bluetooth.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/at42qt1070.o: ../src/at42qt1070.c  .generated_files/flags/pic32mz1024ecg064/c5b8fc4d3a411b0264c65cd9f8f7e8e73c6140f5 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/at42qt1070.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/at42qt1070.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/at42qt1070.o.d" -o ${OBJECTDIR}/_ext/1360937237/at42qt1070.o ../src/at42qt1070.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/touch.o: ../src/touch.c  .generated_files/flags/pic32mz1024ecg064/52482fcafa6329adc0563ccabc58070fd4546800 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/touch.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/touch.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/touch.o.d" -o ${OBJECTDIR}/_ext/1360937237/touch.o ../src/touch.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/battery.o: ../src/battery.c  .generated_files/flags/pic32mz1024ecg064/72eefd8d12fcb25bc62fa53c7e197d7398117132 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/battery.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/battery.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/battery.o.d" -o ${OBJECTDIR}/_ext/1360937237/battery.o ../src/battery.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/timer.o: ../src/timer.c  .generated_files/flags/pic32mz1024ecg064/6ec810d4d3e948b36bfe2c74faa717bf2b767d09 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/timer.o.d" -o ${OBJECTDIR}/_ext/1360937237/timer.o ../src/timer.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/power.o: ../src/power.c  .generated_files/flags/pic32mz1024ecg064/f231f154c5f2053de868a2afb7abd7a2f555d6e9 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/power.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/power.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/power.o.d" -o ${OBJECTDIR}/_ext/1360937237/power.o ../src/power.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/watchdog.o: ../src/watchdog.c  .generated_files/flags/pic32mz1024ecg064/d7928a9e685cd53bc08c0e0ab842a81f8b8c0b2a .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/watchdog.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/watchdog.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/watchdog.o.d" -o ${OBJECTDIR}/_ext/1360937237/watchdog.o ../src/watchdog.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/motor.o: ../src/motor.c  .generated_files/flags/pic32mz1024ecg064/5d09a0981538c377b49787f421c69e022162828c .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/motor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/motor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/motor.o.d" -o ${OBJECTDIR}/_ext/1360937237/motor.o ../src/motor.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/motion.o: ../src/motion.c  .generated_files/flags/pic32mz1024ecg064/ab271c0fb5534b0655f23ac4887c0e7627a1c2c4 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/motion.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/motion.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/motion.o.d" -o ${OBJECTDIR}/_ext/1360937237/motion.o ../src/motion.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/localization.o: ../src/localization.c  .generated_files/flags/pic32mz1024ecg064/f16ca28f7d6e658b4077e7ec12ac3b3154eeb4a4 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/localization.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/localization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/localization.o.d" -o ${OBJECTDIR}/_ext/1360937237/localization.o ../src/localization.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/pid.o: ../src/pid.c  .generated_files/flags/pic32mz1024ecg064/1d75a7079204e785e5b5df0aa34c8d61e75d7f0d .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pid.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pid.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/pid.o.d" -o ${OBJECTDIR}/_ext/1360937237/pid.o ../src/pid.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/gesture.o: ../src/gesture.c  .generated_files/flags/pic32mz1024ecg064/3ddd2c68a17d76c9ef517447e647b5eb905047a .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gesture.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/gesture.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/gesture.o.d" -o ${OBJECTDIR}/_ext/1360937237/gesture.o ../src/gesture.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/haptics.o: ../src/haptics.c  .generated_files/flags/pic32mz1024ecg064/5c6b58b1e308c3817df7fdf4330b8a83046cf6f9 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/haptics.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/haptics.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/haptics.o.d" -o ${OBJECTDIR}/_ext/1360937237/haptics.o ../src/haptics.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1360937237/bezier.o: ../src/bezier.c  .generated_files/flags/pic32mz1024ecg064/576e83d88726e1f50f285cfdec8f3cb8f7dbaff4 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bezier.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/bezier.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/bezier.o.d" -o ${OBJECTDIR}/_ext/1360937237/bezier.o ../src/bezier.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/431183094/sys_devcon.o: /opt/microchip/harmony/v1_03/framework/system/devcon/src/sys_devcon.c  .generated_files/flags/pic32mz1024ecg064/e4681aa3844f37f0727e7df7ef337c83a03f2164 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/431183094" 
	@${RM} ${OBJECTDIR}/_ext/431183094/sys_devcon.o.d 
	@${RM} ${OBJECTDIR}/_ext/431183094/sys_devcon.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/431183094/sys_devcon.o.d" -o ${OBJECTDIR}/_ext/431183094/sys_devcon.o /opt/microchip/harmony/v1_03/framework/system/devcon/src/sys_devcon.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/431183094/sys_devcon_pic32mz.o: /opt/microchip/harmony/v1_03/framework/system/devcon/src/sys_devcon_pic32mz.c  .generated_files/flags/pic32mz1024ecg064/2f68e2f5bdf47fe3828cc31dd74f33afb9c4318b .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/431183094" 
	@${RM} ${OBJECTDIR}/_ext/431183094/sys_devcon_pic32mz.o.d 
	@${RM} ${OBJECTDIR}/_ext/431183094/sys_devcon_pic32mz.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/431183094/sys_devcon_pic32mz.o.d" -o ${OBJECTDIR}/_ext/431183094/sys_devcon_pic32mz.o /opt/microchip/harmony/v1_03/framework/system/devcon/src/sys_devcon_pic32mz.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/754521456/sys_int_pic32.o: /opt/microchip/harmony/v1_03/framework/system/int/src/sys_int_pic32.c  .generated_files/flags/pic32mz1024ecg064/db9c6a779a90d6cad259606dfdedaa4c2e49e66d .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/754521456" 
	@${RM} ${OBJECTDIR}/_ext/754521456/sys_int_pic32.o.d 
	@${RM} ${OBJECTDIR}/_ext/754521456/sys_int_pic32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/754521456/sys_int_pic32.o.d" -o ${OBJECTDIR}/_ext/754521456/sys_int_pic32.o /opt/microchip/harmony/v1_03/framework/system/int/src/sys_int_pic32.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1419604243/sys_ports.o: /opt/microchip/harmony/v1_03/framework/system/ports/src/sys_ports.c  .generated_files/flags/pic32mz1024ecg064/b0e54027be0f5ca37272ba941309e42b7dd31ac8 .generated_files/flags/pic32mz1024ecg064/8dd353d27da661fc42c503a8d33e2de08a447d4
	@${MKDIR} "${OBJECTDIR}/_ext/1419604243" 
	@${RM} ${OBJECTDIR}/_ext/1419604243/sys_ports.o.d 
	@${RM} ${OBJECTDIR}/_ext/1419604243/sys_ports.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -I"../src" -I"../src/system_config/pic32mz1024ecg064" -I"/opt/microchip/harmony/v1_03/framework" -MP -MMD -MF "${OBJECTDIR}/_ext/1419604243/sys_ports.o.d" -o ${OBJECTDIR}/_ext/1419604243/sys_ports.o /opt/microchip/harmony/v1_03/framework/system/ports/src/sys_ports.c    -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)    
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/cellulo-firmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  /opt/microchip/harmony/v1_03/bin/framework/peripheral/PIC32MZ1024ECG064_peripherals.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/cellulo-firmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    /opt/microchip/harmony/v1_03/bin/framework/peripheral/PIC32MZ1024ECG064_peripherals.a      -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x0:0x27F   -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_heap_size=200000,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map" 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/cellulo-firmware.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  /opt/microchip/harmony/v1_03/bin/framework/peripheral/PIC32MZ1024ECG064_peripherals.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/cellulo-firmware.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    /opt/microchip/harmony/v1_03/bin/framework/peripheral/PIC32MZ1024ECG064_peripherals.a      -DXPRJ_pic32mz1024ecg064=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=200000,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map" 
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/cellulo-firmware.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/pic32mz1024ecg064
	${RM} -r dist/pic32mz1024ecg064

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
