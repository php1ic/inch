gcc=g++
exe=inch
flags=-Wall -O2 --pedantic -ggdb

date=`date +%Y%m%d`

src=${wildcard *.cpp}
obj=${src:.cpp=.o}
includes=${wildcard *.h}

all: ${exe}

${exe}: ${obj}
	${gcc} ${flags} ${obj} -o $@

%.o: %.cpp ${includes}
	${gcc} ${flags} -DLOCAL_PATH=\"${PWD}\" -c $<

.PHONY: clean veryclean dist

clean:
	rm -vf ${exe} ${obj}

veryclean: clean
	rm -vf *.*~ callgrind.out.*

#Create a tarball, in the directory above, to distribute
dist: clean
	mv -v ../${exe}_*.tgz ../Old_versions_and_tests
	tar -cvzf ../${exe}_${date}.tgz --exclude=".git*" --exclude='*~' --exclude='*.eps' --exclude='*.svg' ../${exe}

#valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes --collect-jumps=yes
#valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes
