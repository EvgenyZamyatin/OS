SUBDIRS = lib cat foreach revwords filter bufcat simplesh

all: $(SUBDIRS)
	
$(SUBDIRS):
	$(MAKE) -C $@


.PHONY: all $(SUBDIRS)


clean: 
	for D in $(SUBDIRS); do ($(MAKE) clean -C $$D); done
	find . -type f -name '*~' -exec rm -f '{}' \;


