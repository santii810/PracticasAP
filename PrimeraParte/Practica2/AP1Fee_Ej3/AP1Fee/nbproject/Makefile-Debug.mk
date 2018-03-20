#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=Cygwin-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/86563f2e/mainAzul.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/cygdrive/C/opencv3.0.0/x64 /cygdrive/C/opencv3.0.0/x64/opencv_world300.dll

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ap1fee.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ap1fee.exe: /cygdrive/C/opencv3.0.0/x64/opencv_world300.dll

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ap1fee.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ap1fee ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/86563f2e/mainAzul.o: /cygdrive/C/Users/ap1/Documents/NetBeansProjects/Practica1/mainAzul.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/86563f2e
	${RM} "$@.d"
	$(COMPILE.c) -g -I/cygdrive/C/opencv3.0.0/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/86563f2e/mainAzul.o /cygdrive/C/Users/ap1/Documents/NetBeansProjects/Practica1/mainAzul.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ap1fee.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
