INCDIR := ./inc/
SRCDIR := ./src/
OUTDIR := ./out/
UNITDIR := ./unit_tests/

vpath %.c $(SRCDIR) $(UNITDIR)

CC := gcc
PATHFLAG := -I$(INCDIR)
CFLAGS := -std=c99 $(PATHFLAG) -Wall -Werror -Wpedantic
OBGSFLAGS := -lcheck -lpthread -lrt -lm

# lsubunit
SRCS := $(wildcard $(SRCDIR)*.c)
OBGS := $(patsubst $(SRCDIR)%.c,$(OUTDIR)%.o,$(SRCS))

TEST_SRCS := $(wildcard $(UNITDIR)*.c)
ALL_OBGS := $(patsubst $(UNITDIR)%.c,$(OUTDIR)%.o,$(TEST_SRCS)) $(OBGS)
DEPS := $(patsubst %.o,%.d,$(ALL_OBGS))
TEST_OBGS := $(filter-out $(OUTDIR)main.o,$(ALL_OBGS))

.PHONY : clean func collect_coverage unit gcov

release : CFLAGS += -O3 -g0 -DNDEBUG
release : lastrelease | ./app.exe

debug : CFLAGS += -g3
debug : lastdebug | ./app.exe

gcov : CFLAGS += --coverage -O0
gcov : COVERAGE_LINK_ARGS = -lgcov
gcov : lastgcov | ./app.exe

lastrelease:
	$(shell ([ -f ./.debug ] || [ -f ./.gcov ]) && rm -f *.exe $(OUTDIR)* ./.debug ./.gcov)
	touch .release

lastdebug:
	$(shell ([ -f ./.release ] || [ -f ./.gcov ]) && rm -f *.exe $(OUTDIR)* ./.release ./.gcov)
	touch .debug

lastgcov:
	$(shell ([ -f ./.debug ] || [ -f ./.release ]) && rm -f *.exe $(OUTDIR)* ./.release ./.debug)
	touch .gcov

app.exe : $(OBGS) 
	$(CC) $(CFLAGS) $^ -o $@ 

$(OUTDIR)%.o : %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTDIR)%.d : %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(PATHFLAG) -M $< | sed "s^$*.o^$(OUTDIR)$*.o^" > $@

unit_tests.exe : $(TEST_OBGS) 
	$(CC) -o $@ $^ $(OBGSFLAGS) 

include $(DEPS)

func : debug
	export USE_VALGRIND=1 && ./func_tests/scripts/func_tests.sh

unit : ./unit_tests.exe
	./unit_tests.exe

clean :
	rm -f .gcov
	rm -f func_tests/data/temp.temp
	rm -f *.exe
	rm -fr $(OUTDIR)

collect_coverage :gcov
	mv $(OUTDIR)*.gcno $(SRCDIR)
	export ARGS=1 && ./func_tests/scripts/func_tests.sh
	mv $(OUTDIR)*.gcda $(SRCDIR)
	gcov $(SRCDIR)*.c
	mv ./*.gcov $(OUTDIR)
	mv $(SRCDIR)*.gcda $(OUTDIR)
	mv $(SRCDIR)*.gcno $(OUTDIR)
