 #define WATING       0
 #define IGNITION_ON  1
 #define STARTING     2
 #define WORKING      3
 #define PROGRAMMING  4
 #define FAST_START   5
 #define PUMP_PAUSE   6

 // определяем индекс массива по значению АЦП
int ButtonIndex(int value, int buttons[], int measerr) 
{
  int i;
  for(i = 0; i < 5; i++) 
  {
    if(value >= buttons[i] - measerr && value <= buttons[i] + measerr) 
    {
      return i;
    }
  }
  return 100;
}
