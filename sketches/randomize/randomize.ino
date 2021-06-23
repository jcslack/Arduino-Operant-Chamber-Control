void swap (int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void randomize ( int arr[], int n ){
    // Use a different seed value so that we don't get same
    // result each time we run this program
    randomSeed(analogRead(A0));
 
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = n-1; i > 0; i--){
        long j = random(0, n);
        swap(&arr[i], &arr[j]);
    }   
}
