/*
 * SO2 Lab - Linux device drivers (#4)
 * User-space test file
 */

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define DEVICE_PATH	"/dev/vda"

struct fuzz_data {
	int len;
	char buf[256];
};

#define FUZZ_IOCTL _IOC(_IOC_WRITE, 'k', 1, sizeof(struct fuzz_data))

int main(int argc, char **argv)
{
	int fd;
	struct fuzz_data fuzz = {
		.len = 56,
		.buf = {
			0xb0, 0x00, 0x00, 0x00, 0x00, 0xdd, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
			0x40, 0x14, 0x00, 0x00, 0xf3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,


			// 30, 10, 10, 00, 00, 00, 00, 00, 70, 00, 00, 00, 01, 00, 01, 00,
			// 40, 10, 10, 00, 00, 00, 00, 00, 00, 02, 00, 00, 03, 00, 02, 00,
			// 40, 12, 10, 00, 00, 00, 00, 00, 01, 00, 00, 00, 02, 00, 03, 00,
			// 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
		},
		// .buf = {30, 10, 10, 00, 00, 00, 00, 00, 10, 00, 00, 00, 01, 00, 01, 00,
		// 		 40, 10, 10, 00, 00, 00, 00, 00, 00, 02, 00, 00, 03, 00, 02, 00,
		// 		 40, 12, 10, 00, 00, 00, 00, 00, 01, 00, 00, 00, 02, 00, 03, 00,
		// 		},
	};

	fd = open(DEVICE_PATH, O_RDONLY);
	if (fd < 0) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	memset(fuzz.buf, 0, fuzz.len);
	if (ioctl(fd, FUZZ_IOCTL, &fuzz) < 0) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	close(fd);

	return 0;
}
