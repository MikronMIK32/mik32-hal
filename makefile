
CHILD_MAKEFILES := $(wildcard */makefile)
TEST_DIRS := $(dir $(CHILD_MAKEFILES))
MAKE_TEST_DIRS := $(addprefix _make_,$(TEST_DIRS))
CLEAN_TEST_DIRS := $(addprefix _clean_,$(TEST_DIRS))


all: $(MAKE_TEST_DIRS)

clean: $(CLEAN_TEST_DIRS)

info:
	@echo -e "Test directories:$(addprefix \n  , $(TEST_DIRS))"


_make_%: %
	@echo 
	$(MAKE) -C $<


_clean_%: %
	@echo 
	$(MAKE) -C $< clean




