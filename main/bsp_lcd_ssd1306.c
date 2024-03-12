/********************************************************************

   Created     :   2024.01.30.

   Author      : louiey.dev@gmail.com

   Description :

   History     :

*********************************************************************/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c.h"

#include "oled_ssd1306/ssd1306.h"
#include "bsp_lcd_ssd1306.h"
#include "bsp.h"

static const char *TAG = "lcd";

SSD1306_t g_lcd = {0};

/*!
 @brief        LCD initialize, init -> clear screen -> set contrast
 @param[in]    void
 @param[out]   
 @return       ok
*/
int bsp_lcd_ssd1306_init(void)
{    
    i2c_master_init(&g_lcd, CONFIG_ESP_I2C_SDA_PIN, CONFIG_ESP_I2C_SCL_PIN, -1);

    ESP_LOGI(TAG, "LCD Panel is %dx%d", LCD_WIDTH, LCD_HEIGHT);
	ssd1306_init(&g_lcd, LCD_WIDTH, LCD_HEIGHT);

    ssd1306_clear_screen(&g_lcd, false);
	ssd1306_contrast(&g_lcd, 0xff);
	// ssd1306_display_text_x3(&g_lcd, 0, "Hello", 5, false);

    return 0;
}

/*!
 @brief        write string at page
 @note         write string at lcd with given page number
 @param[in]    page number (0 ~ 7), string, length of string, invert or not (0 : non invert, 1 : invert)
 @param[out]   
 @return       ok
*/
int bsp_lcd_ssd1306_write_text_page(int page, char * text, int text_len, bool invert)
{
    ssd1306_display_text(&g_lcd, page, text, text_len, invert);

    return 0;
}

/*!
 @brief        clear lcd
 @note         clear full lcd screen
 @param[in]    void
 @param[out]   
 @return       ok
*/
int bsp_lcd_ssd1306_clear(void)
{
    ssd1306_clear_screen(&g_lcd, 0);
    return 0;
}

/*!
 @brief        clear page
 @note         clears lcd page which is given page number, 0 ~ 7
 @param[in]    
 @param[out]   
 @return       
*/
int bsp_lcd_ssd1306_page_clear(int page)
{
    ssd1306_clear_line(&g_lcd, page, 0);
    return 0;
}

/*!
 @brief        write string at page
 @note         Don't set page number over 6, maximum is 5. 5 means 5 ~ 7. In case of over 5, it'll causes problem
 @param[in]    page number (0 ~ 7), string, length of string, invert or not (0 : non invert, 1 : invert)
 @param[out]   
 @return       ok
*/
int bsp_lcd_ssd1306_write_text_page_x3(int page, char * text, int text_len, bool invert)
{
    ssd1306_display_text_x3(&g_lcd, page, text, text_len, invert);

    return 0;
}