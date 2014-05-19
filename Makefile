EXE=inch
GCC=g++
INCLUDES=-I./
FLAGS=-Wall -Wpedantic -ggdb -O2 -DLOCAL_PATH=\"${PWD}\"

DATE=`date +%Y%m%d`

CreateDir=@mkdir -p ${@D}

ObjectDir=obj/
SourceDir=src/
BinDir=bin/
IncludeDir=include/

Includes=$(wildcard ${IncludeDir}*.h)
Sources=$(sort $(wildcard ${SourceDir}*.cpp))
Objects=$(patsubst ${SourceDir}%.cpp,${ObjectDir}%.o,${Sources})

Version=$(shell grep version ${SourceDir}inputs.cpp | sed 's/.*version("\(.*\)").*/\1/')
GitCommit=$(shell git rev-parse --short HEAD)

.PHONY: clean veryclean dist

all: ${BinDir}${EXE}

${BinDir}${EXE}: ${Objects}
	${CreateDir}
	@echo -e "\033[34mVersion = \033[31m${Version}\033[0m"
	@${GCC} ${FLAGS} ${INCLUDES} $^ -o $@
	@echo -e "\033[34mLinking and building \033[35m./$@\033[0m"

${ObjectDir}%.o: ${SourceDir}%.cpp ${Includes}
	${CreateDir}
	@echo -e "\033[34mBuilding \033[32m$@ (source: $<)\033[0m"
	@${GCC} ${FLAGS} ${INCLUDES} -c $< -o $@

clean:
	rm -vf ${ObjectDir}*.o ${BinDir}${EXE}

veryclean: clean
	rm -vf callgrind.out.*
	find . -name '*~' -exec rm -v {} +
	rmdir -v ${BinDir} ${ObjectDir}

#Create a tarball, in the directory above, to distribute
dist: veryclean
	mv -v ../${EXE}_*.tgz ../Old_versions_and_tests
	tar -cvzf ../${EXE}_${Version}-${GitCommit}_${DATE}.tgz -X excludefiles.txt ../${EXE}

#valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes --collect-jumps=yes
#valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes
