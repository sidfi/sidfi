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
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/sidfi.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/sidfi.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
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
SOURCEFILES_QUOTED_IF_SPACED=diskio.c ff.c ffunicode.c mos6502.cpp fonts.c gfx.c st7735s.c st7735s_compat.c main.c cpu.cpp

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/diskio.o ${OBJECTDIR}/ff.o ${OBJECTDIR}/ffunicode.o ${OBJECTDIR}/mos6502.o ${OBJECTDIR}/fonts.o ${OBJECTDIR}/gfx.o ${OBJECTDIR}/st7735s.o ${OBJECTDIR}/st7735s_compat.o ${OBJECTDIR}/main.o ${OBJECTDIR}/cpu.o
POSSIBLE_DEPFILES=${OBJECTDIR}/diskio.o.d ${OBJECTDIR}/ff.o.d ${OBJECTDIR}/ffunicode.o.d ${OBJECTDIR}/mos6502.o.d ${OBJECTDIR}/fonts.o.d ${OBJECTDIR}/gfx.o.d ${OBJECTDIR}/st7735s.o.d ${OBJECTDIR}/st7735s_compat.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/cpu.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/diskio.o ${OBJECTDIR}/ff.o ${OBJECTDIR}/ffunicode.o ${OBJECTDIR}/mos6502.o ${OBJECTDIR}/fonts.o ${OBJECTDIR}/gfx.o ${OBJECTDIR}/st7735s.o ${OBJECTDIR}/st7735s_compat.o ${OBJECTDIR}/main.o ${OBJECTDIR}/cpu.o

# Source Files
SOURCEFILES=diskio.c ff.c ffunicode.c mos6502.cpp fonts.c gfx.c st7735s.c st7735s_compat.c main.c cpu.cpp



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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/sidfi.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX270F256B
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/diskio.o: diskio.c  .generated_files/f2aff1722d8a5ba913a34a91064bb834db0f56b9.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/diskio.o.d 
	@${RM} ${OBJECTDIR}/diskio.o 
	${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/diskio.o.d" -o ${OBJECTDIR}/diskio.o diskio.c    -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -std=gnu99 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ff.o: ff.c  .generated_files/3195b099287d1bc2aab48ccf97896a3ce9067af6.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ff.o.d 
	@${RM} ${OBJECTDIR}/ff.o 
	${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ff.o.d" -o ${OBJECTDIR}/ff.o ff.c    -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -std=gnu99 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ffunicode.o: ffunicode.c  .generated_files/9d667aad696412ca7bda366274a3cb77755ca795.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ffunicode.o.d 
	@${RM} ${OBJECTDIR}/ffunicode.o 
	${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ffunicode.o.d" -o ${OBJECTDIR}/ffunicode.o ffunicode.c    -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -std=gnu99 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/fonts.o: fonts.c  .generated_files/c9827a97e96ae1a452196821a12409c9ecaac6af.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/fonts.o.d 
	@${RM} ${OBJECTDIR}/fonts.o 
	${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/fonts.o.d" -o ${OBJECTDIR}/fonts.o fonts.c    -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -std=gnu99 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/gfx.o: gfx.c  .generated_files/a8b44a2f2bd0f8c42ef514a9c168eb047fc86e8c.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/gfx.o.d 
	@${RM} ${OBJECTDIR}/gfx.o 
	${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/gfx.o.d" -o ${OBJECTDIR}/gfx.o gfx.c    -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -std=gnu99 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/st7735s.o: st7735s.c  .generated_files/21058474b6d9af082d4d1517865ddfd96b9f8cf7.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/st7735s.o.d 
	@${RM} ${OBJECTDIR}/st7735s.o 
	${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/st7735s.o.d" -o ${OBJECTDIR}/st7735s.o st7735s.c    -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -std=gnu99 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/st7735s_compat.o: st7735s_compat.c  .generated_files/15334f7755060a9999879b8b423ab9191a776714.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/st7735s_compat.o.d 
	@${RM} ${OBJECTDIR}/st7735s_compat.o 
	${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/st7735s_compat.o.d" -o ${OBJECTDIR}/st7735s_compat.o st7735s_compat.c    -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -std=gnu99 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/main.o: main.c  .generated_files/46a415a5bea7102b1c0f3472ec87bddfde7f0d81.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CPPC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -std=gnu99 -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/diskio.o: diskio.c  .generated_files/2cf4e851fb833de05810aaf7165396a4703050b3.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/diskio.o.d 
	@${RM} ${OBJECTDIR}/diskio.o 
	${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/diskio.o.d" -o ${OBJECTDIR}/diskio.o diskio.c    -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -std=gnu99 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ff.o: ff.c  .generated_files/ef05172eb6331efe2f2d34fea2b24dad0a280927.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ff.o.d 
	@${RM} ${OBJECTDIR}/ff.o 
	${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ff.o.d" -o ${OBJECTDIR}/ff.o ff.c    -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -std=gnu99 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/ffunicode.o: ffunicode.c  .generated_files/cb570459a3ce3d3dd6f33638870acec3cea6ccfd.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ffunicode.o.d 
	@${RM} ${OBJECTDIR}/ffunicode.o 
	${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ffunicode.o.d" -o ${OBJECTDIR}/ffunicode.o ffunicode.c    -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -std=gnu99 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/fonts.o: fonts.c  .generated_files/5422c96274e3003033be06a63eec9950d208954e.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/fonts.o.d 
	@${RM} ${OBJECTDIR}/fonts.o 
	${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/fonts.o.d" -o ${OBJECTDIR}/fonts.o fonts.c    -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -std=gnu99 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/gfx.o: gfx.c  .generated_files/c24fa7e8aafd520cdefd97bcaa9f405f9c0f07e2.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/gfx.o.d 
	@${RM} ${OBJECTDIR}/gfx.o 
	${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/gfx.o.d" -o ${OBJECTDIR}/gfx.o gfx.c    -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -std=gnu99 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/st7735s.o: st7735s.c  .generated_files/23c1e216519631f1cb1add142ec26e0c2646001d.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/st7735s.o.d 
	@${RM} ${OBJECTDIR}/st7735s.o 
	${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/st7735s.o.d" -o ${OBJECTDIR}/st7735s.o st7735s.c    -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -std=gnu99 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/st7735s_compat.o: st7735s_compat.c  .generated_files/aef7b293735795997aef262270547d62fb49d0d5.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/st7735s_compat.o.d 
	@${RM} ${OBJECTDIR}/st7735s_compat.o 
	${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/st7735s_compat.o.d" -o ${OBJECTDIR}/st7735s_compat.o st7735s_compat.c    -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -std=gnu99 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/main.o: main.c  .generated_files/5555db660a63232d9f3764b028c123f2cc8c516d.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CPPC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -std=gnu99 -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/mos6502.o: mos6502.cpp  .generated_files/d9539076330ffd620b22eb6194343e9633359874.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/mos6502.o.d 
	@${RM} ${OBJECTDIR}/mos6502.o 
	${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -MP -MMD -MF "${OBJECTDIR}/mos6502.o.d" -o ${OBJECTDIR}/mos6502.o mos6502.cpp   -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/cpu.o: cpu.cpp  .generated_files/133e9335c4b9ef9dabb30c2e5353c99b47763ee9.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cpu.o.d 
	@${RM} ${OBJECTDIR}/cpu.o 
	${MP_CPPC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -MP -MMD -MF "${OBJECTDIR}/cpu.o.d" -o ${OBJECTDIR}/cpu.o cpu.cpp   -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/mos6502.o: mos6502.cpp  .generated_files/a77e2ca44ae7e39593a31975b9b99eb938b09377.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/mos6502.o.d 
	@${RM} ${OBJECTDIR}/mos6502.o 
	${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -MP -MMD -MF "${OBJECTDIR}/mos6502.o.d" -o ${OBJECTDIR}/mos6502.o mos6502.cpp   -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/cpu.o: cpu.cpp  .generated_files/55eec4da1b3b8fa1e535b12ee2f8585b1f6d077c.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/cpu.o.d 
	@${RM} ${OBJECTDIR}/cpu.o 
	${MP_CPPC} $(MP_EXTRA_CC_PRE)  -g -x c++ -c -mprocessor=$(MP_PROCESSOR_OPTION)  -frtti -fexceptions -fno-check-new -fenforce-eh-specs -MP -MMD -MF "${OBJECTDIR}/cpu.o.d" -o ${OBJECTDIR}/cpu.o cpu.cpp   -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/sidfi.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CPPC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/sidfi.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_heap_size=256,--defsym=_min_stack_size=256,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml,--allow-multiple-definition -mdfp="${DFP_DIR}"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/sidfi.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CPPC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/sidfi.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=256,--defsym=_min_stack_size=256,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml,--allow-multiple-definition -mdfp="${DFP_DIR}"
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/sidfi.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
