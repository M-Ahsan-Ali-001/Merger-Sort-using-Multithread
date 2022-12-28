#include<iostream>
#include<pthread.h>
#include<time.h>
#include<cmath>
using namespace std;

//Global Variables
const int Total_Thread=4; // max threads
int Limit=20;//array Limit
int  *maxSize_Array; // array of max input size entered
int Iterator =0; // Thread Iterator

//Function for merging
void Merger(int last , int center , int top )
{
    int * left_arr = new int[center - last+ 1];
    
    int *right_arr = new int[top - center ];
    
    int size_Left = center -last + 1,
    size_Right= top - center,
    left_Incremetor , right_Incremetor;
    
    for(int left_Incremetor=0 ; left_Incremetor < size_Left;left_Incremetor++  )
    {
        
        left_arr[left_Incremetor] = maxSize_Array[left_Incremetor + last];
        
    }
    
    for(int right_Incremetor=0 ; right_Incremetor < size_Right;right_Incremetor++  )
    {
        
        right_arr[right_Incremetor] = maxSize_Array[right_Incremetor + center + 1];
        
    }
    
    int tempHold = last;
    left_Incremetor=right_Incremetor=0;
    
    while(left_Incremetor<size_Left && right_Incremetor <size_Right)
    {
        
        if(left_arr[left_Incremetor]<= right_arr[right_Incremetor])
        {
            
            maxSize_Array[tempHold++] = left_arr[left_Incremetor++];
        }
        else
        {
            maxSize_Array[tempHold++] = right_arr[right_Incremetor++];
            
        }
        
    }
    
    while(left_Incremetor < size_Left)
    {
        maxSize_Array[tempHold++] = left_arr[left_Incremetor++];
    }
    
    while(right_Incremetor < size_Right)
    {
        maxSize_Array[tempHold++] = right_arr[right_Incremetor++];
    }
    
    
}
//function to be used for dividing array into lower level
void merger_sort(int last , int top)
{
    int center = last + ( top - last) / 2;
    if(last<top){
        
        merger_sort(last,center);
        merger_sort(center+1 , top);
        Merger(last, center, top);
        
    }
    
}
// function to be used by thread for parallelism
void *merger_sort(void * arg)
{
    int thread_Sequence = Iterator++;
    
    int last = thread_Sequence * (Limit/4);
    int top =(thread_Sequence + 1)*(Limit / 4) - 1;
    
    int center =last +(top-last)/2;
    
    if(last<top)
    {
        merger_sort(last,center);
        merger_sort(center+1 , top);
        Merger(last, center, top);
        
    }
    return 0;
}

int main()
{
    int choice; // user was given 2 options
    cout<<"Enter size of Array:";
    cin>>Limit;
    
    cout<<"(1)You want add number   \n(2) Generate Numbers automatically(2) \nEnter An option: ";
    cin>>choice;
    
    maxSize_Array = new int[Limit];
    switch(choice)
    {
        case 1:
        {
            for(int i=0;i<Limit;i++)
            {
                cout<<"Enter Array Element " <<i+1<<":";
                cin>>maxSize_Array[i];
            }
            break;
        }
        case 2:
        {
            for(int i=0;i<Limit;i++)
            {
                maxSize_Array[i] = rand()%100;
         
            }
            break;
        }
    }
  
    
    
    clock_t Time_1,Time_2;
    Time_1 = clock();
    pthread_t threads[Total_Thread];
    
    for(int i=0;i<Total_Thread;i++)
    {
        
        pthread_create(&threads[i], NULL,  merger_sort,
        (void*)NULL);
    }
    
    for(int i=0;i<4;i++)
    {
        pthread_join(threads[i],NULL);
    }
    
    Merger(0, (Limit/2-1)/2, Limit/2-1);
    
    Merger(Limit/2, Limit/2+(Limit-1-Limit/2)/2, Limit-1);
    
    Merger(0, (Limit-1)/2, Limit-1);
    Time_2 =clock();
    cout<<"Merge Sorted array:";
    for(int i=0 ; i<Limit;i++)
    {
        
        cout<<maxSize_Array[i]<<" ";
    }
    
    cout<<"\n TIme Taken:"<<(Time_2-Time_1)/(double)CLOCKS_PER_SEC<<endl;
    return 0;
    
    
    
    
}
