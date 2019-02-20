asmlinkage long sys_mycopy(const char *__src_file, const char *__dest_file) 
{
	int src_fd, dst_fd;
	int count;
	char buf[1024];

	mm_segment_t fs;
	fs = get_fs();
	set_fs(get_ds());
	if ((src_fd = sys_open(__src_file, O_RDONLY, 0)) == -1) 
	{
		return 1;
	}
	if ((dst_fd = sys_open(__dest_file, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) == -1) 
	{
		return 2;
	}
	while ((count = sys_read(src_fd, buf, 1024)) > 0) 
	{
		if (sys_write(dst_fd, buf, count) != count)
			return 3;
	}
	if (count < 0)
		return 4;
	sys_close(src_fd);
	sys_close(dst_fd);
	set_fs(fs);
	return 0;
}