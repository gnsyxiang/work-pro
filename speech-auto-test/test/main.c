/**
 * 
 * Release under GPLv2.
 * 
 * @file    main.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/02 2018 19:03
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/02 2018      create the file
 * 
 *     last modified: 28/02 2018 19:03
 */
#include <stdio.h>

#include "dir_helper.h"

void handle_file(const char *base_path, const char *name)
{
	printf("base_path: %s, name: %s \n", base_path, name);
}

int main(int argc, char **argv)
{
	read_file_list("./", handle_file, NULL);

	return 0;
}

