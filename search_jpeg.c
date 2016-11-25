# include "common.h"

lnode* search_jpeg_to_list(char *dir_name)
{
	DIR *dp;
	FILE *fp;

	if(NULL == (dp = opendir(dir_name))){
		fprintf(stderr, "Error open dir %s:<%s>\n", 
				dir_name, strerror(errno));
		exit(1);
	}

	struct dirent *sd;
	lnode *tmp = NULL;
	lnode *rear = NULL;

	chdir(dir_name);

	while(NULL != (sd = readdir(dp))){
		if(sd->d_type == DT_REG){
			if(NULL == (fp = fopen(sd->d_name, "r"))){
				fprintf(stderr, "Error fopen %s:<%s>\n",
						sd->d_name, strerror(errno));
				continue;
			}
			if(NULL != strstr(sd->d_name, "ioc.jpg")){
				continue;
			}
			if(NULL != strstr(sd->d_name, ".jpg") || NULL != strstr(sd->d_name, ".JPG")){	
				tmp = (lnode *)malloc(sizeof(lnode));
				strcpy(tmp->filename, sd->d_name);
				tmp -> next = NULL;
				if(NULL == head){
					head = tmp;
					rear = tmp;
				}else{
					rear -> next = tmp;
					rear = tmp;
					rear -> next = NULL;
				}
			}
		}
	}
	rear -> next = head;
	closedir(dp);

	return head;
}
