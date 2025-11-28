void bubbleSort(int arr[], int n) {
    int i, j;
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}
int main() {
    int a[] = {4, 2, 8, 5};
    int n = sizeof(a) / sizeof(a[0]);
    
    bubbleSort(a, n);
    
    return 0;
}
