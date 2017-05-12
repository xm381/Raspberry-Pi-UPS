# Raspberry-Pi-UPS
Arduino based UPS that emulates a Megatech -  For use with NUT "Network UPS Tools" running on Raspberry Pi

Utilizing mostly Adafruit products, this simple Arduino-based UPS works with NUT running on the Raspberry Pi.


These are VERY basic NUT install instructions, please review https://melgrubb.com/2016/12/11/rphs-v2-ups/ for comprehensive instructions.


Install NUT:

	sudo apt-get install nut

Edit ups.conf:

	sudo nano /etc/nut/ups.conf

Add this:

		[RPUPS]
        		driver = blazer_ser
        		port = /dev/ttyUSB0
        		desc = “Raspberry Pi UPS"
			default.battery.voltage.high = 4.10
			default.battery.voltage.low = 3.45
			default.battery.voltage.nominal = 3.70
			runtimecal = 1500,100,3000,50
			#offdelay values in seconds
			offdelay = 60
			#ondelay values in minutes
			ondelay = 1

Edit nut.conf:

	sudo nano /etc/nut/nut.conf

Add this:

		MODE=standalone


Start NUT:

	sudo upsdrvctl start


Edit upsd.users:

	sudo nano /etc/nut/upsd.users

Add this:

		[upsmon]
        		password  = raspberry
        		upsmon master
		[admin]
        		password = raspberry
        		actions = SET
       			instcmds = ALL
		
Edit upsmon.conf:

	sudo nano /etc/nut/upsmon.conf

Add to MONITOR section:

		MONITOR RPUPS@localhost 1 upsmon raspberry master

Customize NOTIFYFLAG section (un-comment the messages you want to see):

		NOTIFYFLAG 

reboot

Querry UPS Status:

	upsc RPUPS


NUT webpage:
  http://networkupstools.org
  
Very nice writeup that I used to install NUT:
  https://melgrubb.com/2016/12/11/rphs-v2-ups/
