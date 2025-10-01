#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/of.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define DRIVER_NAME "bbb_i2c"
#define MAX_DEVICES 4

struct bbb_i2c_dev 
{
    struct i2c_client *client;
    struct cdev cdev;
    dev_t devno;
    struct class *class;
    struct device *device;
};

static int bbb_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    struct bbb_i2c_dev *dev;
    int ret;
    dev = devm_kzalloc(&client->dev, sizeof(*dev), GFP_KERNEL);
    if (!dev)
        return -ENOMEM;
    dev->client = client;
    i2c_set_clientdata(client, dev);
    ret = i2c_smbus_read_byte(client);
    if (ret < 0) 
    {
        dev_err(&client->dev, "Device Not Responding At Address 0x%02x\n", client->addr);
        return ret;
    }
    dev_info(&client->dev, "I2C Device Detected At 0x%02x\n", client->addr);
    return 0;
}

static int bbb_i2c_remove(struct i2c_client *client)
{
    struct bbb_i2c_dev *dev = i2c_get_clientdata(client);
    dev_info(&client->dev, "I2C Device Removed\n");
    return 0;
}

static const struct i2c_device_id bbb_i2c_id[] = 
{
    { "bbb_i2c_device", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, bbb_i2c_id);

static const struct of_device_id bbb_i2c_of_match[] = 
{
    { .compatible = "bbb,i2c-device" },
    { }
};
MODULE_DEVICE_TABLE(of, bbb_i2c_of_match);
static struct i2c_driver bbb_i2c_driver = 
{
    .driver = 
    {
        .name = DRIVER_NAME,
        .of_match_table = bbb_i2c_of_match,
    },
    .probe = bbb_i2c_probe,
    .remove = bbb_i2c_remove,
    .id_table = bbb_i2c_id,
};

static s32 bbb_i2c_read_byte(struct i2c_client *client, u8 command)
{
    return i2c_smbus_read_byte_data(client, command);
}

static s32 bbb_i2c_write_byte(struct i2c_client *client, u8 command, u8 value)
{
    return i2c_smbus_write_byte_data(client, command, value);
}

static int __init bbb_i2c_init(void)
{
    int ret;
    ret = i2c_add_driver(&bbb_i2c_driver);
    if (ret)
        pr_err("BBB I2C: Failed To Register Driver : %d\n", ret);
    else
        pr_info("BBB I2C: Driver Registered Successfully\n");
    return ret;
}

static void __exit bbb_i2c_exit(void)
{
    i2c_del_driver(&bbb_i2c_driver);
    pr_info("BBB I2C: Driver Unregistered\n");
}

module_init(bbb_i2c_init);
module_exit(bbb_i2c_exit);

MODULE_LICENSE("MIT");
MODULE_AUTHOR("Ala Hassine");
MODULE_DESCRIPTION("BeagleBone Black I2C Driver");