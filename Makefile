SUBDIRS = lib cat foreach revwords filter bufcat simplesh

all: $(SUBDIRS)
	
$(SUBDIRS):
	$(MAKE) -C $@


.PHONY: all $(SUBDIRS)


clean: $(SUBDIRS)


$(SUBDIRS):
	$(MAKE) clean -C $@
	find . -type f -name '*~' -exec rm -f '{}' \;
