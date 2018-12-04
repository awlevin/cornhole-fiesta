// Copyright (c) 2016, Joe Krachey
// All rights reserved.
//
// Redistribution and use in binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in 
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include "../include/ece453.h"

//*****************************************************************************
//*****************************************************************************
int ece453_reg_read(char *reg_name)
{
	int fd = open(reg_name, O_RDONLY); 
	char string_val[10];

	memset(string_val, 0, 10);

	if(fd < 0) {
		perror("open");
		return -1;
	}
	if (read(fd, string_val,8) < 0) {
		perror("fread"); 
		return -1;
	}
  close(fd);

  return strtoul(string_val, NULL, 16);

}

//*****************************************************************************
//*****************************************************************************
int ece453_reg_write(char *reg_name, int value)
{
	char string_val[9];

	int fd = open(reg_name, O_WRONLY); 
	
	sprintf(string_val, "%08x",value);

	if(fd < 0) {
		perror("open");
		return -1;
	}
	if (write(fd, string_val,sizeof(string_val)) < 0) {
		perror("fwrite"); 
		return -1;
	}
  close(fd);
  return 0;
}

