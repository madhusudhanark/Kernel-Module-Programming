sudo rmmod bmp_280
make clean
make
sudo cp bmp_280.ko /lib/modules/6.12.47+rpt-rpi-v8/
sudo cp bmp_280.ko /lib/modules/6.12.47+rpt-rpi-v8/kernel/drivers/i2c/
sudo insmod bmp_280.ko
