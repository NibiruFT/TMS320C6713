/*flash_sine_hex.cmd Hex Converter for flash. Use:

 	>hex6x flash_sine_hex.cmd to create .hex file*/

debug\flash_sine.out

-a
-memwidth  8
-map flash_sine.map
-image

ROMS
{
FLASH: org=000h,len=0x04000,romwidth=8,files={flash_sine.hex}
}
