#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>

int main()
{
	long int copy_file = syscall(326, "paper.pdf", "temp/paper.pdf");
	printf("the syscall function returned %ld\n", copy_file);

	FILE *fp_src, *fp_dst;
	fp_src = fopen("paper.pdf", "wb");
	fp_dst = fopen("temp/paper.pdf", "rb");
	
	int result = 1;
	char ch1, ch2;
	
	while ( (ch1 = (char)fgetc(fp_src) != EOF) && (ch2 = (char)fgetc(fp_dst)!=EOF )) {
		if (ch1 != ch2) {
			result = 0;
			break;
		}
	}
	if (result == 1)
		printf("src file and dst file are the same.\n");
	else
		printf("not same!\n");
	fclose(fp_src);
	fclose(fp_dst);
	return 0;
}
