EXE=inch
GCC=g++ --std=c++11
INCLUDES=-I./include
FLAGS=-W -Wall -Wextra -Werror -pedantic -ggdb -O2 -DLOCAL_PATH=\"${PWD}\"

ECHO=@/bin/echo -e

CreateDir=@mkdir -p ${@D}

ObjectDir=obj/
SourceDir=src/
BinDir=bin/
IncludeDir=include/

Includes=$(wildcard ${IncludeDir}*.h)
Sources=$(sort $(wildcard ${SourceDir}*.cpp))
Objects=$(Sources:${SourceDir}%.cpp=${ObjectDir}%.o)

GitCommit=$(shell git describe)
Version=$(firstword $(subst -, ,${GitCommit}))

.PHONY: clean veryclean dist

#The project has moved to cmake so this file will no longer be updated.
#Attempts to build this way will fail with a warning/info message.
#If required, you can 're-active' this build method, but the file may
#be deleted in the future.
ifeq (1, 0)
all: ${BinDir}${EXE}
else
all: depricated
endif

depricated:
	${ECHO} "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	${ECHO} "cmake is now the prefered build method."
	${ECHO} "This makefile may disappear in the future"
	${ECHO} "See the file INSTALL.md for cmake build instructions"
	${ECHO} "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"

${BinDir}${EXE}: ${Objects}
	${CreateDir}
	${ECHO} "\033[34mVersion = \033[31m${Version}\033[0m"
	@${GCC} ${FLAGS} ${INCLUDES} $^ -o $@
	${ECHO} "\033[34mLinking and building \033[35m./$@\033[0m"

${ObjectDir}%.o: ${SourceDir}%.cpp ${Includes}
	${CreateDir}
	${ECHO} "\033[34mBuilding \033[32m$@ (source: $<)\033[0m"
	@${GCC} ${FLAGS} ${INCLUDES} -c $< -o $@

clean:
	rm -vf ${ObjectDir}*.o ${BinDir}${EXE}

veryclean: clean
	-rm -vf callgrind.out.*
	-find . -name '*~' -exec rm -v {} +
	-rmdir -v ${BinDir} ${ObjectDir}

#Create a tarball, in the directory above, to distribute
dist: veryclean
	tar -cvzf ../${EXE}_${GitCommit}.tgz -X excludefiles.txt ../${EXE}

#valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes --collect-jumps=yes
#valgrind --tool=memcheck --leak-check=full --show-reachable=yes --num-callers=20 --track-fds=yes --track-origins=yes
