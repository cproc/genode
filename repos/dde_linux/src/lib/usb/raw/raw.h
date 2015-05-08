#ifndef _RAW_H_
#define _RAW_H_

struct usb_device;
struct usb_driver;

extern struct usb_device_driver raw_driver;
extern struct usb_driver        raw_intf_driver;

int raw_notify(struct notifier_block *self, unsigned long action, void *dev); 

#endif /* _RAW_H_ */
