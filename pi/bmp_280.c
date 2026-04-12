// SPDX-License-Identifier: GPL-2.0
/*
 * Minimal I2C driver that only reads a single register (chip ID)
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/of.h>

#define REG_DEVICE_ID  0xD0   /* change if your sensor uses a different ID register */
#define REG_TEMP_CALIB 0x88 /*starting addr of temp calib*/

static int32_t dig_T1, dig_T2 , dig_T3;

/*
static int read_tmp_calib(struct i2c_client *client)
{
  int ret;
  char calib[6];
  ret = i2c_smbus_read_block_data(client,REG_TEMP_CALIB,calib);
  if(ret < 0)
   {
    pr_debug("error in block data read\n");
    return ret;
     }  
 
 dig_T1 = (calib[1] << 4) | calib[0];
 dig_T2 = (calib[3] << 4) | calib[2];
 dig_T2 = (calib[5] << 4) | calib[4];
 pr_info(" dig_T1 : %d %d %d \n",dig_T1,dig_T2,dig_T2);

 return 0;
}

*/
u32  read_temp_value(const struct i2c_client *client)
{
   u8 temp_raw[3]= {0};
   u32 ret;
   u32 cal_temp;  
   ret =  i2c_smbus_read_block_data(client,0xFA,temp_raw);
   if(ret)
     return ret;
   pr_info("raw temp values from bmp : %x %x %x\n",temp_raw[0],temp_raw[1],temp_raw[2]);
  
   return cal_temp;
}


static int simple_i2c_probe(struct i2c_client *client)
{
    int ret;
    int chip_id;
    u32 temp;
    pr_info("simple_bmp280_probe called\n");

    /* Read chip ID */
    chip_id = i2c_smbus_read_byte_data(client, REG_DEVICE_ID);
    temp = read_temp_value(client);
    if (chip_id < 0) {
        pr_err("Failed to read device ID: %d\n", chip_id);
        return chip_id;
    }


    pr_info("bmp280 I2C Device ID = 0x%02X\n", chip_id);
    return 0;
}

static void simple_i2c_remove(struct i2c_client *client)
{
    dev_info(&client->dev, "simple_bmp280_remove called\n");
    
}

/* --- Device Tree Match --- */
static const struct of_device_id simple_i2c_dt_ids[] = {
    { .compatible = "bosch,bmp280_custom" },
    { }
};
MODULE_DEVICE_TABLE(of, simple_i2c_dt_ids);

/* --- Legacy I2C Table (optional) --- */
static const struct i2c_device_id simple_i2c_id[] = {
    { "bmp280_custom", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, simple_i2c_id);

static struct i2c_driver simple_i2c_driver = {
    .driver = {
        .name = "bmp280_driver",
        .of_match_table = simple_i2c_dt_ids,
    },
    .probe    = simple_i2c_probe,
    .remove   = simple_i2c_remove,
    .id_table = simple_i2c_id,
};

module_i2c_driver(simple_i2c_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhusudhana RK");
MODULE_DESCRIPTION("Very simple I2C driver that reads device ID");
