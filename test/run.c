#include <stdio.h>

int main(){
	int a;
	int t;
	int arr[10000000];
	for(int i=10000000 -1; i>=0; i--)
		arr[i] = 1;

	scanf("%d", &t);
	while(t--){
		scanf("%d", &a);
		printf("%d\n", a);
	}
}