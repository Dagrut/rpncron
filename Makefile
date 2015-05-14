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

install-conf:
	getent group rpncrontab > /dev/null 2>&1 || addgroup --system rpncrontab
	mkdir -p /var/spool/cron/rpncrontabs
	chown root:rpncrontab /var/spool/cron/rpncrontabs
	chmod 1730 /var/spool/cron/rpncrontabs
	chown root:rpncrontab /usr/local/bin/rpncrontab
	chmod 2755 /var/spool/cron/rpncrontabs
	ls -1 | xargs -r -n 1 --replace=xxx  chown 'xxx:rpncrontab' 'xxx'
	ls -1 | xargs -r -n 1 chmod 600
	#update-rc.d rpncron start 89 2 3 4 5 . >/dev/null

uninstall: uninstall-bin uninstall-conf
	

uninstall-bin:
	xargs rm < build/install_manifest.txt

uninstall-conf:
	if [ -x "/etc/init.d/rpncron" ]; then invoke-rc.d rpncrontab stop; fi
	#update-rc.d rpncron remove >/dev/null
	rm -rf /var/spool/cron/rpncrontabs/
	
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
