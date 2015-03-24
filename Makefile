SUBDIRS = lib cat revwords filter

all: $(SUBDIRS)
	
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)

