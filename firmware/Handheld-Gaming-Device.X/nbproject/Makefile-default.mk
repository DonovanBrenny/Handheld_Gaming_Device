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
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Handheld-Gaming-Device.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Handheld-Gaming-Device.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=18x24_font_table.c 6x8_font_table.c Ackanoid.c Ball_physics.c Buttons.c Confirm_popup_menu.c Dinosaur_game.c Joystick.c OLED.c OLED_drawings.c Pong.c Settings_menu.c Setup.c Speaker.c Tetris.c device_main.c game_menu.c main_menu.c ../super_team_final_project.X/game_ASM.s

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/18x24_font_table.o ${OBJECTDIR}/6x8_font_table.o ${OBJECTDIR}/Ackanoid.o ${OBJECTDIR}/Ball_physics.o ${OBJECTDIR}/Buttons.o ${OBJECTDIR}/Confirm_popup_menu.o ${OBJECTDIR}/Dinosaur_game.o ${OBJECTDIR}/Joystick.o ${OBJECTDIR}/OLED.o ${OBJECTDIR}/OLED_drawings.o ${OBJECTDIR}/Pong.o ${OBJECTDIR}/Settings_menu.o ${OBJECTDIR}/Setup.o ${OBJECTDIR}/Speaker.o ${OBJECTDIR}/Tetris.o ${OBJECTDIR}/device_main.o ${OBJECTDIR}/game_menu.o ${OBJECTDIR}/main_menu.o ${OBJECTDIR}/_ext/1304443211/game_ASM.o
POSSIBLE_DEPFILES=${OBJECTDIR}/18x24_font_table.o.d ${OBJECTDIR}/6x8_font_table.o.d ${OBJECTDIR}/Ackanoid.o.d ${OBJECTDIR}/Ball_physics.o.d ${OBJECTDIR}/Buttons.o.d ${OBJECTDIR}/Confirm_popup_menu.o.d ${OBJECTDIR}/Dinosaur_game.o.d ${OBJECTDIR}/Joystick.o.d ${OBJECTDIR}/OLED.o.d ${OBJECTDIR}/OLED_drawings.o.d ${OBJECTDIR}/Pong.o.d ${OBJECTDIR}/Settings_menu.o.d ${OBJECTDIR}/Setup.o.d ${OBJECTDIR}/Speaker.o.d ${OBJECTDIR}/Tetris.o.d ${OBJECTDIR}/device_main.o.d ${OBJECTDIR}/game_menu.o.d ${OBJECTDIR}/main_menu.o.d ${OBJECTDIR}/_ext/1304443211/game_ASM.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/18x24_font_table.o ${OBJECTDIR}/6x8_font_table.o ${OBJECTDIR}/Ackanoid.o ${OBJECTDIR}/Ball_physics.o ${OBJECTDIR}/Buttons.o ${OBJECTDIR}/Confirm_popup_menu.o ${OBJECTDIR}/Dinosaur_game.o ${OBJECTDIR}/Joystick.o ${OBJECTDIR}/OLED.o ${OBJECTDIR}/OLED_drawings.o ${OBJECTDIR}/Pong.o ${OBJECTDIR}/Settings_menu.o ${OBJECTDIR}/Setup.o ${OBJECTDIR}/Speaker.o ${OBJECTDIR}/Tetris.o ${OBJECTDIR}/device_main.o ${OBJECTDIR}/game_menu.o ${OBJECTDIR}/main_menu.o ${OBJECTDIR}/_ext/1304443211/game_ASM.o

# Source Files
SOURCEFILES=18x24_font_table.c 6x8_font_table.c Ackanoid.c Ball_physics.c Buttons.c Confirm_popup_menu.c Dinosaur_game.c Joystick.c OLED.c OLED_drawings.c Pong.c Settings_menu.c Setup.c Speaker.c Tetris.c device_main.c game_menu.c main_menu.c ../super_team_final_project.X/game_ASM.s



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Handheld-Gaming-Device.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ64GA002
MP_LINKER_FILE_OPTION=,--script=p24FJ64GA002.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/18x24_font_table.o: 18x24_font_table.c  .generated_files/flags/default/1cf31ff05ff7236a970799012e0a660862273e41 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/18x24_font_table.o.d 
	@${RM} ${OBJECTDIR}/18x24_font_table.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  18x24_font_table.c  -o ${OBJECTDIR}/18x24_font_table.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/18x24_font_table.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/6x8_font_table.o: 6x8_font_table.c  .generated_files/flags/default/a29bd3fcb5db24d69b33261e1766c1d490932667 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/6x8_font_table.o.d 
	@${RM} ${OBJECTDIR}/6x8_font_table.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  6x8_font_table.c  -o ${OBJECTDIR}/6x8_font_table.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/6x8_font_table.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Ackanoid.o: Ackanoid.c  .generated_files/flags/default/b617b0e638d624427fc06eca7535cd5d05d4232f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Ackanoid.o.d 
	@${RM} ${OBJECTDIR}/Ackanoid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Ackanoid.c  -o ${OBJECTDIR}/Ackanoid.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Ackanoid.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Ball_physics.o: Ball_physics.c  .generated_files/flags/default/50900ed49605baa60f7c959ce10644aff05c0ad1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Ball_physics.o.d 
	@${RM} ${OBJECTDIR}/Ball_physics.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Ball_physics.c  -o ${OBJECTDIR}/Ball_physics.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Ball_physics.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Buttons.o: Buttons.c  .generated_files/flags/default/b24d294caae0c21adb95be0cce220173157fb75f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Buttons.o.d 
	@${RM} ${OBJECTDIR}/Buttons.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Buttons.c  -o ${OBJECTDIR}/Buttons.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Buttons.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Confirm_popup_menu.o: Confirm_popup_menu.c  .generated_files/flags/default/fe74f6fff8e60d33d4fb7cf147dc979c2a0a911f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Confirm_popup_menu.o.d 
	@${RM} ${OBJECTDIR}/Confirm_popup_menu.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Confirm_popup_menu.c  -o ${OBJECTDIR}/Confirm_popup_menu.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Confirm_popup_menu.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Dinosaur_game.o: Dinosaur_game.c  .generated_files/flags/default/5496225a78f68992324d9e9d0d85f0ee875c3fd6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Dinosaur_game.o.d 
	@${RM} ${OBJECTDIR}/Dinosaur_game.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Dinosaur_game.c  -o ${OBJECTDIR}/Dinosaur_game.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Dinosaur_game.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Joystick.o: Joystick.c  .generated_files/flags/default/4b9e9b42a8a6f3f3f06a490fee1e51fb49345320 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Joystick.o.d 
	@${RM} ${OBJECTDIR}/Joystick.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Joystick.c  -o ${OBJECTDIR}/Joystick.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Joystick.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/OLED.o: OLED.c  .generated_files/flags/default/efe1750809d1c94d56f665c288b303df8ed3f5f6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/OLED.o.d 
	@${RM} ${OBJECTDIR}/OLED.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  OLED.c  -o ${OBJECTDIR}/OLED.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/OLED.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/OLED_drawings.o: OLED_drawings.c  .generated_files/flags/default/69dd6813c2b45f077d110cad70d02cb57c0f940b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/OLED_drawings.o.d 
	@${RM} ${OBJECTDIR}/OLED_drawings.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  OLED_drawings.c  -o ${OBJECTDIR}/OLED_drawings.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/OLED_drawings.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Pong.o: Pong.c  .generated_files/flags/default/972710662a4369fc3bf710a2a3f430398e9310ce .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Pong.o.d 
	@${RM} ${OBJECTDIR}/Pong.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Pong.c  -o ${OBJECTDIR}/Pong.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Pong.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Settings_menu.o: Settings_menu.c  .generated_files/flags/default/b8339bfee35844eff6d2050c66fedabbbb67d720 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Settings_menu.o.d 
	@${RM} ${OBJECTDIR}/Settings_menu.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Settings_menu.c  -o ${OBJECTDIR}/Settings_menu.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Settings_menu.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Setup.o: Setup.c  .generated_files/flags/default/290594eea6121b502013ea829bb670f5ea2d7a88 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Setup.o.d 
	@${RM} ${OBJECTDIR}/Setup.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Setup.c  -o ${OBJECTDIR}/Setup.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Setup.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Speaker.o: Speaker.c  .generated_files/flags/default/80f3b0d35c5a67ffe7263cac299d5a8ac94a3b42 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Speaker.o.d 
	@${RM} ${OBJECTDIR}/Speaker.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Speaker.c  -o ${OBJECTDIR}/Speaker.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Speaker.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Tetris.o: Tetris.c  .generated_files/flags/default/51bdca49619dbefe30944fcafe5744888e1f3448 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Tetris.o.d 
	@${RM} ${OBJECTDIR}/Tetris.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Tetris.c  -o ${OBJECTDIR}/Tetris.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Tetris.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/device_main.o: device_main.c  .generated_files/flags/default/45fd45cc3edb04f7ee92019f670eca53542ea348 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/device_main.o.d 
	@${RM} ${OBJECTDIR}/device_main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  device_main.c  -o ${OBJECTDIR}/device_main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/device_main.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/game_menu.o: game_menu.c  .generated_files/flags/default/f6c3c15ff436e01366e37d890c396c4b55a61e43 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/game_menu.o.d 
	@${RM} ${OBJECTDIR}/game_menu.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  game_menu.c  -o ${OBJECTDIR}/game_menu.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/game_menu.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main_menu.o: main_menu.c  .generated_files/flags/default/1c234a4ac9fca7ca27687c134b395ad0370048bf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main_menu.o.d 
	@${RM} ${OBJECTDIR}/main_menu.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main_menu.c  -o ${OBJECTDIR}/main_menu.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main_menu.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/18x24_font_table.o: 18x24_font_table.c  .generated_files/flags/default/539274e6b20bc0b202a5d147e6fcc7402d20741b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/18x24_font_table.o.d 
	@${RM} ${OBJECTDIR}/18x24_font_table.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  18x24_font_table.c  -o ${OBJECTDIR}/18x24_font_table.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/18x24_font_table.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/6x8_font_table.o: 6x8_font_table.c  .generated_files/flags/default/44c78be1cb818e12400372eaa4b4db7387af5178 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/6x8_font_table.o.d 
	@${RM} ${OBJECTDIR}/6x8_font_table.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  6x8_font_table.c  -o ${OBJECTDIR}/6x8_font_table.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/6x8_font_table.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Ackanoid.o: Ackanoid.c  .generated_files/flags/default/7309c4de0488d76d1647f67a3a4bfdd4807f86b5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Ackanoid.o.d 
	@${RM} ${OBJECTDIR}/Ackanoid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Ackanoid.c  -o ${OBJECTDIR}/Ackanoid.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Ackanoid.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Ball_physics.o: Ball_physics.c  .generated_files/flags/default/a3ccbcc237eaf29b3376bdb9ac521851bbd623e6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Ball_physics.o.d 
	@${RM} ${OBJECTDIR}/Ball_physics.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Ball_physics.c  -o ${OBJECTDIR}/Ball_physics.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Ball_physics.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Buttons.o: Buttons.c  .generated_files/flags/default/d16a4734f1f8edf6b1b6979c054dedbba691db6d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Buttons.o.d 
	@${RM} ${OBJECTDIR}/Buttons.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Buttons.c  -o ${OBJECTDIR}/Buttons.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Buttons.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Confirm_popup_menu.o: Confirm_popup_menu.c  .generated_files/flags/default/48c9ee9237d34b4006cdeff48774376d5fd0f7c2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Confirm_popup_menu.o.d 
	@${RM} ${OBJECTDIR}/Confirm_popup_menu.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Confirm_popup_menu.c  -o ${OBJECTDIR}/Confirm_popup_menu.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Confirm_popup_menu.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Dinosaur_game.o: Dinosaur_game.c  .generated_files/flags/default/7b0cbf60d6fd9d8d0673dad92e98a7addb1d2b7d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Dinosaur_game.o.d 
	@${RM} ${OBJECTDIR}/Dinosaur_game.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Dinosaur_game.c  -o ${OBJECTDIR}/Dinosaur_game.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Dinosaur_game.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Joystick.o: Joystick.c  .generated_files/flags/default/e64938b7eb0fdbf2a8d54b4dbd6b39fbe23a3765 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Joystick.o.d 
	@${RM} ${OBJECTDIR}/Joystick.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Joystick.c  -o ${OBJECTDIR}/Joystick.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Joystick.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/OLED.o: OLED.c  .generated_files/flags/default/490ddc09cde99f44fe4201bab06ce94155a8fdcf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/OLED.o.d 
	@${RM} ${OBJECTDIR}/OLED.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  OLED.c  -o ${OBJECTDIR}/OLED.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/OLED.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/OLED_drawings.o: OLED_drawings.c  .generated_files/flags/default/62184a10391504ac2a1bbdbf29c9b8424a6baedd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/OLED_drawings.o.d 
	@${RM} ${OBJECTDIR}/OLED_drawings.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  OLED_drawings.c  -o ${OBJECTDIR}/OLED_drawings.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/OLED_drawings.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Pong.o: Pong.c  .generated_files/flags/default/b8951c2700aaee642381457549ffa4b68f53fc52 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Pong.o.d 
	@${RM} ${OBJECTDIR}/Pong.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Pong.c  -o ${OBJECTDIR}/Pong.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Pong.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Settings_menu.o: Settings_menu.c  .generated_files/flags/default/ead7fb79beb8bfaa627b0f8af0708b196a5a16ef .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Settings_menu.o.d 
	@${RM} ${OBJECTDIR}/Settings_menu.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Settings_menu.c  -o ${OBJECTDIR}/Settings_menu.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Settings_menu.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Setup.o: Setup.c  .generated_files/flags/default/c04025558276e9f1a53ca1a0451a6802ae4de291 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Setup.o.d 
	@${RM} ${OBJECTDIR}/Setup.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Setup.c  -o ${OBJECTDIR}/Setup.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Setup.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Speaker.o: Speaker.c  .generated_files/flags/default/50d99ef673231ed3e891e14c7d088e3b2abadf65 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Speaker.o.d 
	@${RM} ${OBJECTDIR}/Speaker.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Speaker.c  -o ${OBJECTDIR}/Speaker.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Speaker.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Tetris.o: Tetris.c  .generated_files/flags/default/eb71106fd4f08a1d575fb408248d20bf556fe790 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Tetris.o.d 
	@${RM} ${OBJECTDIR}/Tetris.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Tetris.c  -o ${OBJECTDIR}/Tetris.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Tetris.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/device_main.o: device_main.c  .generated_files/flags/default/e6627b7267509bd9d7251d64604808b2f6016dd4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/device_main.o.d 
	@${RM} ${OBJECTDIR}/device_main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  device_main.c  -o ${OBJECTDIR}/device_main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/device_main.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/game_menu.o: game_menu.c  .generated_files/flags/default/2d190dde5460886edf0a6425a48fefdf022a65b6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/game_menu.o.d 
	@${RM} ${OBJECTDIR}/game_menu.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  game_menu.c  -o ${OBJECTDIR}/game_menu.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/game_menu.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main_menu.o: main_menu.c  .generated_files/flags/default/2f204f4e6054d2b53bb47a5b3395ea1ca48c1a98 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main_menu.o.d 
	@${RM} ${OBJECTDIR}/main_menu.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main_menu.c  -o ${OBJECTDIR}/main_menu.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main_menu.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1304443211/game_ASM.o: ../super_team_final_project.X/game_ASM.s  .generated_files/flags/default/cdd7383b0cd82bb0c3e864699004bbe57dce1200 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1304443211" 
	@${RM} ${OBJECTDIR}/_ext/1304443211/game_ASM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1304443211/game_ASM.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../super_team_final_project.X/game_ASM.s  -o ${OBJECTDIR}/_ext/1304443211/game_ASM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    -Wa,-MD,"${OBJECTDIR}/_ext/1304443211/game_ASM.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/_ext/1304443211/game_ASM.o: ../super_team_final_project.X/game_ASM.s  .generated_files/flags/default/4edc5720131117cf82d85f85d91b13294356a8e8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1304443211" 
	@${RM} ${OBJECTDIR}/_ext/1304443211/game_ASM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1304443211/game_ASM.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../super_team_final_project.X/game_ASM.s  -o ${OBJECTDIR}/_ext/1304443211/game_ASM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DXPRJ_default=$(CND_CONF)    -Wa,-MD,"${OBJECTDIR}/_ext/1304443211/game_ASM.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Handheld-Gaming-Device.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Handheld-Gaming-Device.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/Handheld-Gaming-Device.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Handheld-Gaming-Device.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/Handheld-Gaming-Device.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
