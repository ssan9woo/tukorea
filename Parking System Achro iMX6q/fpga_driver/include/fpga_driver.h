#ifndef __FPGA_DRIVER_H__
#define __FPGA_DRIVER_H__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/ioport.h>
#include <linux/version.h>

#define IOM_STEP_MOTOR_MAJOR	267
#define IOM_STEP_MOTOR_ADDRESS	0x0800000C
#define IOM_STEP_MOTOR_NAME	"fpga_step_motor"

#define IOM_LED_MAJOR	260
#define IOM_LED_ADDRESS	0x08000016
#define IOM_LED_NAME	"fpga_led"

#define IOM_FND_MAJOR	261
#define IOM_FND_ADDRESS	0x08000004
#define IOM_FND_NAME	"fpga_fnd"

#define IOM_DOT_MAJOR	262
#define IOM_DOT_ADDRESS	0x08000210
#define IOM_DOT_NAME	"fpga_dot"
#define IOM_DOT_MAX_ROW 10

#define IOM_TEXT_LCD_MAJOR 	263
#define IOM_TEXT_LCD_ADDRESS	0x08000090
#define IOM_TEXT_LCD_NAME 	"fpga_text_lcd"
#define IOM_TEXT_LCD_MAX_BUF 	32

#define IOM_BUZZER_MAJOR 	264
#define IOM_BUZZER_NAME 	"fpga_buzzer"
#define IOM_BUZZER_ADDRESS	0x08000070

#define IOM_DIP_SWITCH_MAJOR 	266
#define IOM_DIP_SWITCH_NAME 	"fpga_dip_switch"
#define IOM_DIP_SWITCH_ADDRESS	0x08000000

#define IOM_PUSH_SWITCH_MAJOR 	265
#define IOM_PUSH_SWITCH_NAME 	"fpga_push_switch"
#define IOM_PUSH_SWITCH_ADDRESS	0x08000050
#define IOM_PUSH_SWITCH_MAX_BUTTON 9
#endif
