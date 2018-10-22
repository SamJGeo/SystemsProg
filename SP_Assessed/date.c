#include "date.h"
#include "stdio.h"



struct date{
	int day;
	int month;
	int year;
};

Date *date_create(const char *datestr){
	int day; int month; int year;
	int ret = sscanf(datestr, "%d/%d/%d",&day,&month,&year);
	if(ret<3){
		return NULL;
	}
	
	Date * date = malloc(sizeof(Date));
	date->day = day;
	date->month = month;
	date->year = year;
	
	return(date);
}


Date *date_duplicate(Date *d) {
	if (d == NULL) return NULL;
	Date * new_date = malloc(sizeof(Date));
	if (new_date) {
		new_date->day = d->day;
		new_date->month = d->month;
		new_date->year = d->year;
	}
	return new_date;
}

int date_compare(Date *date1,Date *date2){
	if(date1->year==date2->year){
		if(date1->month==date2->month){
			if(date1->day==date2->day){
				return(0);
			} else{
				return(date2->day-date1->day);
			}
		}else{
			return(date2->month-date1->month);
		}
	}else{
		return(date2->year-date1->year);
	}

}




void date_destroy(Date *d){
	free(d);
}




int main(){
	struct date * d;
	d = date_create("12/10/2005");
	printf("%d,%d,%d\n", d->day,d->month,d->year);
	date_destroy(d);



	d = date_create("111/111/111");
	printf("%d,%d,%d\n", d->day,d->month,d->year);
}
