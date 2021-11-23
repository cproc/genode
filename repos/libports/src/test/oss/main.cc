/*
 * \brief  OSS test
 * \author Christian Prochaska
 * \date   2021-10-07
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* libc includes */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/soundcard.h>

int main(int argc, char **argv)
{
	int res;

	int fd = open("/dev/dsp", O_RDWR);

	printf("fd: %d\n", fd);

#if 1
	int channels = 2;
	res = ioctl(fd, SNDCTL_DSP_CHANNELS, &channels);
fprintf(stderr, "channels: %d, res: %d\n", channels, res);
#endif

#if 1
	int format = AFMT_S16_LE;
	res = ioctl(fd, SNDCTL_DSP_SETFMT, &format);
fprintf(stderr, "format: %d, res: %d\n", format, res);
#endif

#if 1
	int rate = 44100;
	res = ioctl(fd, SNDCTL_DSP_SPEED, &rate);
fprintf(stderr, "rate: %d, res: %d\n", rate, res);
#endif

	audio_buf_info bi;
	oss_count_t optr;

#if 0
ioctl(fd, SNDCTL_DSP_GETOSPACE, &bi);
fprintf(stderr, "SNDCTL_DSP_GETOSPACE: fragments: %d, fragsize: %d, fragstotal: %d, bytes: %d\n",
        bi.fragments, bi.fragsize, bi.fragstotal, bi.bytes);
#endif

#if 0
ioctl(fd, SNDCTL_DSP_GETISPACE, &bi);
fprintf(stderr, "SNDCTL_DSP_GETISPACE: fragments: %d, fragsize: %d, fragstotal: %d, bytes: %d\n",
        bi.fragments, bi.fragsize, bi.fragstotal, bi.bytes);
#endif

#if 0
	int frag = (32 << 16) | 11;
	ioctl(fd, SNDCTL_DSP_SETFRAGMENT, &frag);
#endif

#if 0
ioctl(fd, SNDCTL_DSP_GETOSPACE, &bi);
fprintf(stderr, "SNDCTL_DSP_GETOSPACE: fragments: %d, fragsize: %d, fragstotal: %d, bytes: %d\n",
        bi.fragments, bi.fragsize, bi.fragstotal, bi.bytes);
#endif

#if 0
ioctl(fd, SNDCTL_DSP_GETISPACE, &bi);
fprintf(stderr, "SNDCTL_DSP_GETISPACE: fragments: %d, fragsize: %d, fragstotal: %d, bytes: %d\n",
        bi.fragments, bi.fragsize, bi.fragstotal, bi.bytes);
#endif

	static char buf[2048];

#if 0
	for (size_t i = 0; i < sizeof(buf); i++) {
		buf[i] = i;
	}
#endif

#if 0
	/* start recording */
	read(fd, buf, sizeof(buf));
#endif


	ssize_t total_bytes_read = 0;
	ssize_t total_bytes_written = 0;

	for (int i = 0; /*i < 32*/; i++) {

#if 0
ioctl(fd, SNDCTL_DSP_GETISPACE, &bi);
fprintf(stderr, "SNDCTL_DSP_GETISPACE before read(): fragments: %d, fragsize: %d, fragstotal: %d, bytes: %d\n",
        bi.fragments, bi.fragsize, bi.fragstotal, bi.bytes);
#endif

#if 1
		ssize_t bytes_read = read(fd, buf, sizeof(buf));
//fprintf(stderr, "bytes read: %zd, %zd\n", bytes_read, total_bytes_read);
#else
		ssize_t bytes_read = sizeof(buf);
#endif

#if 0
ioctl(fd, SNDCTL_DSP_GETISPACE, &bi);
fprintf(stderr, "SNDCTL_DSP_GETISPACE after read(): fragments: %d, fragsize: %d, fragstotal: %d, bytes: %d\n",
        bi.fragments, bi.fragsize, bi.fragstotal, bi.bytes);
#endif

		total_bytes_read += bytes_read;

		if (bytes_read > 0) {

#if 0
ioctl(fd, SNDCTL_DSP_GETOSPACE, &bi);
fprintf(stderr, "SNDCTL_DSP_GETOSPACE 1: fragments: %d, fragsize: %d, fragstotal: %d, bytes: %d\n",
        bi.fragments, bi.fragsize, bi.fragstotal, bi.bytes);
#endif

#if 1
			ssize_t bytes_written = write(fd, buf, sizeof(buf));

#if 0
for (;;) {
ioctl(fd, SNDCTL_DSP_GETOSPACE, &bi);
ioctl(fd, SNDCTL_DSP_CURRENT_OPTR, &optr);
fprintf(stderr, "SNDCTL_DSP_GETOSPACE 2: fragments: %d, fragsize: %d, fragstotal: %d, bytes: %d\n",
        bi.fragments, bi.fragsize, bi.fragstotal, bi.bytes);
fprintf(stderr, "SNDCTL_DSP_CURRENT_OPTR: samples: %d, fifo_samples: %d\n",
        optr.samples, optr.fifo_samples);
}
#endif

			total_bytes_written += bytes_written;

//fprintf(stderr, "bytes written: %zd, %zd\n", bytes_written, total_bytes_written);
#endif
		}

#if 0
for (;;) {
	ioctl(fd, SNDCTL_DSP_GETISPACE, &bi);
fprintf(stderr, "SNDCTL_DSP_GETISPACE 3: fragments: %d, fragsize: %d, fragstotal: %d, bytes: %d\n",
        bi.fragments, bi.fragsize, bi.fragstotal, bi.bytes);
}
#endif
	}

	return 0;
}
