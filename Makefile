all: make
	

%:
	+make -C build/ $@

make:
	+make -C build/

cmake:
	mkdir -p build/
	cd build && cmake ..

install: install-bin install-conf
	

install-bin:
	+make -C build/ install

install-conf: install-bin
	@echo "Installing configuration...\n"
	getent group rpncrontab > /dev/null 2>&1 || addgroup --system rpncrontab
	mkdir -p /var/spool/rpncron
	chown root:rpncrontab /var/spool/rpncron
	chmod 1730 /var/spool/rpncron
	chown root:rpncrontab /usr/local/bin/rpncrontab
	chmod 2755 /usr/local/bin/rpncrontab
	chmod +x /etc/init.d/rpncron
	#update-rc.d rpncron start 89 2 3 4 5 . >/dev/null

uninstall: uninstall-bin uninstall-conf
	

uninstall-bin:
	xargs rm < build/install_manifest.txt

uninstall-conf:
	if [ -x "/etc/init.d/rpncron" ]; then invoke-rc.d rpncrontab stop; fi
	#update-rc.d rpncron remove >/dev/null
	rm -rf /var/spool/rpncron/
	
	@echo
	@echo ">>>>>                                                    <<<<<"
	@echo ">>>>> You may also want to remove the 'rpncrontab' group <<<<<"
	@echo ">>>>>                                                    <<<<<"

clean:
	+make -C build/ clean

mrproper:
	rm -rf build/

rebuild:
	rm -rf build/
	mkdir -p build/
	cd build && cmake ..
	+make -C build/
