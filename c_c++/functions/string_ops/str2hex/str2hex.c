#include <stdio.h>
#include <string.h>

unsigned int strHex(char *str, char **end)
{
        unsigned int hex = 0;

        // Accept leading 0x
        if (str[0] == '0' && str[1] == 'x')
                str += 2;

        while (*str) {
                if (*str >= '0' && *str <= '9') { // 数字情况
                        hex <<= 4;  // 左移4位，表示16进制左移1位
                        hex += (*str - '0');
                }
                else if (*str >= 'a' && *str <= 'f') { // a - f 情况
                        hex <<= 4;
                        hex += 0xa + (*str - 'a');
                }
                else if (*str >= 'A' && *str <= 'F') { // A －F 情况
                        hex <<= 4;
                        hex += 0xa + (*str - 'A');
                }
                else {
                        *end = str;    //转换完成后， str结束地址赋给*end
                        return hex;
                }
                str++;
        }
        *end = str;
        return hex;
}

int main()
{
	char *str = "0xf1F2f3F4";
	unsigned int  hex = 0;

	hex = strHex(str, &str);
	printf("hex = 0x%X\n", hex);

}

