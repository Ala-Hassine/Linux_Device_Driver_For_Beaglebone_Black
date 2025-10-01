#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define DRIVER_NAME "bbb_spi"
#define SPI_BUFFER_SIZE 4096

struct bbb_spi_dev 
{
    struct spi_device *spi;
    struct cdev cdev;
    dev_t devno;
    struct class *class;
    struct device *device;
    u8 *tx_buffer;
    u8 *rx_buffer;
};

static int bbb_spi_transfer(struct bbb_spi_dev *dev, const u8 *tx, u8 *rx, size_t len)
{
    struct spi_transfer t = 
    {
        .tx_buf = tx,
        .rx_buf = rx,
        .len = len,
    };
    struct spi_message m;
    spi_message_init(&m);
    spi_message_add_tail(&t, &m);
    return spi_sync(dev->spi, &m);
}

static int bbb_spi_probe(struct spi_device *spi)
{
    struct bbb_spi_dev *dev;
    int ret;
    dev = devm_kzalloc(&spi->dev, sizeof(*dev), GFP_KERNEL);
    if (!dev)
        return -ENOMEM;
    // Allocate buffers
    dev->tx_buffer = devm_kzalloc(&spi->dev, SPI_BUFFER_SIZE, GFP_KERNEL);
    dev->rx_buffer = devm_kzalloc(&spi->dev, SPI_BUFFER_SIZE, GFP_KERNEL);
    if (!dev->tx_buffer || !dev->rx_buffer)
        return -ENOMEM;
    dev->spi = spi;
    spi_set_drvdata(spi, dev);
    // Configure SPI
    spi->mode = SPI_MODE_0;
    spi->bits_per_word = 8;
    spi->max_speed_hz = 10000000; // 10 MHz
    ret = spi_setup(spi);
    if (ret < 0) {
        dev_err(&spi->dev, "Failed To Setup SPI : %d\n", ret);
        return ret;
    }
    // Test communication
    dev->tx_buffer[0] = 0x9F; // Read ID command
    ret = bbb_spi_transfer(dev, dev->tx_buffer, dev->rx_buffer, 4);
    if (ret) {
        dev_err(&spi->dev, "SPI Transfer Failed : %d\n", ret);
        return ret;
    }

    dev_info(&spi->dev, "SPI Device Probed Successfully\n");
    dev_info(&spi->dev, "SPI Mode : %d, Max Speed : %d Hz\n", 
             spi->mode, spi->max_speed_hz);
    return 0;
}

static int bbb_spi_remove(struct spi_device *spi)
{
    struct bbb_spi_dev *dev = spi_get_drvdata(spi);
    dev_info(&spi->dev, "SPI Device Removed\n");
    return 0;
}

static const struct of_device_id bbb_spi_of_match[] = 
{
    { .compatible = "bbb,spi-device" },
    { }
};
MODULE_DEVICE_TABLE(of, bbb_spi_of_match);

static struct spi_driver bbb_spi_driver = 
{
    .driver = 
    {
        .name = DRIVER_NAME,
        .of_match_table = bbb_spi_of_match,
    },
    .probe = bbb_spi_probe,
    .remove = bbb_spi_remove,
};

static int __init bbb_spi_init(void)
{
    int ret;
    ret = spi_register_driver(&bbb_spi_driver);
    if (ret)
        pr_err("BBB SPI : Failed To Register Driver : %d\n", ret);
    else
        pr_info("BBB SPI : Driver Registered Successfully\n");  
    return ret;
}

static void __exit bbb_spi_exit(void)
{
    spi_unregister_driver(&bbb_spi_driver);
    pr_info("BBB SPI: Driver Unregistered\n");
}

module_init(bbb_spi_init);
module_exit(bbb_spi_exit);

MODULE_LICENSE("MIT");
MODULE_AUTHOR("Ala Hassine");
MODULE_DESCRIPTION("BeagleBone Black SPI Driver");
