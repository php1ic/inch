EXE=inch
GCC=g++
INCLUDES=-I./
FLAGS=-W -Wall -Wextra -Werror -pedantic -ggdb -O2 -DLOCAL_PATH=\"${PWD}\"

ECHO=@/bin/echo -e

CreateDir=@mkdir -p ${@D}

ObjectDir=obj/
SourceDir=src/
BinDir=bin/
IncludeDir=include/

Includes=$(wildcard ${IncludeDir}*.h)
Sources=$(sort $(wildcard ${SourceDir}*.cpp))
Objects=$(patsubst ${SourceDir}%.cpp,${ObjectDir}%.o,${Sources})

Version=$(shell awk -F'"' '/version\(.*\)/ {print $$2}' ${SourceDir}inputs.cpp)

GitCommit=$(shell git rev-parse --short HEAD)

.PHONY: clean veryclean dist

all: ${BinDir}${EXE}

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
	tar -cvzf ../${EXE}_${Version}-${GitCommit}.tgz -X excludefiles.txt ../${EXE}

#valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes --collect-jumps=yes
#valgrind --tool=memcheck --leak-check=full --show-reachable=yes --num-callers=20 --track-fds=yes --track-origins=yes
