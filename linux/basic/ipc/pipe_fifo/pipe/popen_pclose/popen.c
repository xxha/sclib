#include <stdio.h>
#include <string.h>

#define _LINE_LENGTH 300

int get_path_total(const char *path, long long* total) {
	int err=-1;
	FILE *file;
	char line[_LINE_LENGTH];
	char *p;
	char tmp[100];
	char *token;

	sprintf(tmp, "df %s", path);
	file = popen(tmp, "r");
	if (file != NULL) {
		if (fgets(line, _LINE_LENGTH, file) != NULL) {
			if (fgets(line, _LINE_LENGTH, file) != NULL) {
				token = strtok(line, " ");
				if (token != NULL) {
					printf("token 1 = %s\n", token);
				}
				token = strtok(NULL, " ");
				if (token != NULL) {
					printf("token 2 = %s\n", token);
					*total=atoll(token)/1024;  /* k/1024 */
					err=0;
				}
			}
		}
		pclose(file);
	}
	return err;
}

int main()
{
	char *path = "/dev";
	long long total;

	get_path_total(path, &total);

	printf("total = %lld KB\n", total);
	return 0;
}
