
.PHONY: clean global_clean plugins install test

plugins: 
	$(MAKE) -C monitor

global_clean: clean
	$(MAKE) -C monitor global_clean
	$(MAKE) -C test global_clean

clean:
	rm -f *.so

test:
	$(MAKE) -C test

install: plugins
	if ! [[ -d $(MIR_INSTALL_DIR)plugins/ ]]; then  mkdir -p $(MIR_INSTALL_DIR)plugins/ ; fi
	cp *.so $(MIR_INSTALL_DIR)plugins/

