#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/serial.h>
#include <linux/serial_core.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/of.h>
#include <linux/interrupt.h>

#define DRIVER_NAME "bbb_uart"
#define BUFFER_SIZE 256

struct bbb_uart_dev 
{
    struct uart_port port;
    struct device *dev;
    char buffer[BUFFER_SIZE];
    int buffer_len;
};

static struct bbb_uart_dev *bbb_uart_device;

static unsigned int bbb_uart_tx_empty(struct uart_port *port)
{
    return TIOCSER_TEMT;
}

static void bbb_uart_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
    // Set modem control lines
}

static unsigned int bbb_uart_get_mctrl(struct uart_port *port)
{
    return TIOCM_CTS | TIOCM_DSR | TIOCM_CAR;
}

static void bbb_uart_stop_tx(struct uart_port *port)
{
    // Stop transmission
}

static void bbb_uart_start_tx(struct uart_port *port)
{
    struct bbb_uart_dev *dev = container_of(port, struct bbb_uart_dev, port);
    printk(KERN_INFO "BBB UART: Start Transmission\n");
}

static void bbb_uart_stop_rx(struct uart_port *port)
{
    // Stop reception
}

static void bbb_uart_break_ctl(struct uart_port *port, int break_state)
{
    // Handle break control
}

static int bbb_uart_startup(struct uart_port *port)
{
    printk(KERN_INFO "BBB UART: Startup\n");
    return 0;
}

static void bbb_uart_shutdown(struct uart_port *port)
{
    printk(KERN_INFO "BBB UART: Shutdown\n");
}

static void bbb_uart_set_termios(struct uart_port *port, struct ktermios *new,
                                struct ktermios *old)
{
    unsigned int baud = tty_termios_baud_rate(new);
    printk(KERN_INFO "BBB UART: Set BaudRate To %u\n", baud);
}

static const char *bbb_uart_type(struct uart_port *port)
{
    return DRIVER_NAME;
}

static struct uart_ops bbb_uart_ops = 
{
    .tx_empty = bbb_uart_tx_empty,
    .set_mctrl = bbb_uart_set_mctrl,
    .get_mctrl = bbb_uart_get_mctrl,
    .stop_tx = bbb_uart_stop_tx,
    .start_tx = bbb_uart_start_tx,
    .stop_rx = bbb_uart_stop_rx,
    .break_ctl = bbb_uart_break_ctl,
    .startup = bbb_uart_startup,
    .shutdown = bbb_uart_shutdown,
    .set_termios = bbb_uart_set_termios,
    .type = bbb_uart_type,
};

static int bbb_uart_probe(struct platform_device *pdev)
{
    struct bbb_uart_dev *dev;
    struct resource *res;
    int ret;
    dev = devm_kzalloc(&pdev->dev, sizeof(*dev), GFP_KERNEL);
    if (!dev)
        return -ENOMEM;
    // Initialize UART port
    dev->port.dev = &pdev->dev;
    dev->port.ops = &bbb_uart_ops;
    dev->port.type = PORT_16550A;
    dev->port.fifosize = 16;
    dev->port.line = 0;
    dev->port.iotype = UPIO_MEM;
    dev->port.uartclk = 48000000;
    // Get memory resources
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res) {
        dev_err(&pdev->dev, "No Memory Resource\n");
        return -ENODEV;
    }
    dev->port.mapbase = res->start;
    dev->port.membase = devm_ioremap_resource(&pdev->dev, res);
    if (IS_ERR(dev->port.membase))
        return PTR_ERR(dev->port.membase);
    // Register UART driver
    ret = uart_add_one_port(&serial8250_reg, &dev->port);
    if (ret) {
        dev_err(&pdev->dev, "Failed To Add UART Port\n");
        return ret;
    }
    platform_set_drvdata(pdev, dev);
    bbb_uart_device = dev;
    printk(KERN_INFO "BBB UART: Driver Probed Successfully\n");
    return 0;
}

static int bbb_uart_remove(struct platform_device *pdev)
{
    struct bbb_uart_dev *dev = platform_get_drvdata(pdev);
    uart_remove_one_port(&serial8250_reg, &dev->port);
    printk(KERN_INFO "BBB UART: Driver Removed\n");
    return 0;
}

static const struct of_device_id bbb_uart_of_match[] = 
{
    { .compatible = "ti,am3352-uart" },
    {},
};
MODULE_DEVICE_TABLE(of, bbb_uart_of_match);

static struct platform_driver bbb_uart_driver = 
{
    .probe = bbb_uart_probe,
    .remove = bbb_uart_remove,
    .driver = 
    {
        .name = DRIVER_NAME,
        .of_match_table = bbb_uart_of_match,
    },
};

module_platform_driver(bbb_uart_driver);

MODULE_LICENSE("MIT");
MODULE_AUTHOR("Ala Hassine");
MODULE_DESCRIPTION("BeagleBone Black UART Driver");
