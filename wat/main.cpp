#include <iostream>
using namespace std;

const int MAX=10;

void Sacaminymax_Ref(const int a[], int res[])
{
    int min=100, max=0;
    for(int i=0; i<MAX; i++)
    {
      if (a[i]<min)
      {
        min=a[i];
      }

      if (a[i]>max)
      {
        max=a[i];
      }
    }
    
    res[0]=min;
    res[1]=max;

    for(int i=2; i<MAX; i++)
    {
      res[i]=0;
    }
}

int main()
{
    int v[MAX]= {8,5,3,4,7,12,1,9,10,4}, sol[MAX];

    Sacaminymax_Ref(v, sol);

    for(int i=0; i<MAX; i++)
    {
      cout << sol[i] << " ";
    }
}