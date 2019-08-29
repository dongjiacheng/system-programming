#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void what_size(){
	int a = 0;
	size_t a_size = sizeof(a++);
	printf("size: %zu, a: %d\n", a_size, a);
}

#define swap(a, b) temp = a; \
    a = b; \
    b = temp;
    
void selection_sort(int* a, size_t len){
    size_t temp = len - 1;
    for(size_t i = 0; i < temp; ++i){
        size_t min_index = i;
        for(size_t j = i+1; j < len; ++i){
			if(a[j] < a[i]) 
				min_index = j;
        }
        if(i != min_index) 
			swap(a[i], a[min_index]);     
    }
}

short mystery_bits(short input){
	short max_set = ~0;
	short masked = input & (0xFF00 ^ max_set);
	short shifted = masked << 16;
	short ret = (shifted | 0xCC);
	return ret;
}

void positive_under_ten(int input){
	if(0 < input < 10){
		printf("Input is in the range\n");
	}else{
		printf("Input is not in the range\n");
	}
}

int print_error(int err_num){
	switch(err_num){
	case ENOENT:
		printf("No such file or entry\n");
	case EINTR:
		printf("Interrupted\n");
	default:
		break;
	}
}

int main(int argc, char* argv[]){

}