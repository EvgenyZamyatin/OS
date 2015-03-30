SUBDIRS = lib cat revwords filter bufcat

all: $(SUBDIRS)
	
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)

