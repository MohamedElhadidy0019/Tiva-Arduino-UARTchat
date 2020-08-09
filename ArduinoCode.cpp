#include <Arduino.h>

#define change 6
#define send   7

#define choice1  8
#define choice2  9
#define choice3  10


#define recevived1 2
#define recevived2 3
#define recevived3 4

int last=0;
void reply();
void ask();
void Clearleds();
void ChangeLED(int);
char Qarray[150];

char Hungry[]="Hey,Are you hungry? \n";
char Thirsty[]="Hey,Are you thirsty? \n";
char Happy[]="Hey,Are you happy? \n";

char yes[]="Yes\n";
char no[]="No\n";
char maybe[]="Maybe\n";

int isEqualString(char*,char*);
void QuestionMode(int x);
void QuestionSend(int x);
int arr_size(char *ptr);

void setup() {
  Serial.begin(9600);
  pinMode(change,INPUT);
  pinMode(send,INPUT);
  pinMode(choice1,OUTPUT);
  pinMode(choice2,OUTPUT);
  pinMode(choice3,OUTPUT);

  pinMode(recevived1,OUTPUT);
  pinMode(recevived2,OUTPUT);
  pinMode(recevived3,OUTPUT);

  digitalWrite(choice1,LOW);
  digitalWrite(choice2,LOW);
  digitalWrite(choice3,LOW);

  digitalWrite(recevived1,LOW);
  digitalWrite(recevived2,LOW);
  digitalWrite(recevived3,LOW);
}

void loop()
{
  
    if(Serial.available() )
    {
      
      reply();
      
    }
    else
    {
     
      ask();
    }
    
}


void reply()
{
  unsigned char arr_counter=0;
  while(1)
  {
    
    char out=1;
    if(Serial.available())
       {
      out=Serial.read();
      Qarray[arr_counter]=out;
      arr_counter++;
    
       }
    if(out=='\n')
      {
        Qarray[arr_counter]='\0';
        break;
      } 
  }
  digitalWrite(recevived1,LOW);
  digitalWrite(recevived2,LOW);
  digitalWrite(recevived3,LOW);
  //Serial.print("SEEEEEEEEEEE    ");
  if(isEqualString(Qarray,Hungry))
  {
    
    digitalWrite(recevived1,HIGH);
    
  }
  else if(isEqualString(Qarray,Thirsty))
  {
    
    digitalWrite(recevived2,HIGH);
    
  }
  else if(isEqualString(Qarray,Happy))
  {
   
    digitalWrite(recevived3,HIGH);
    
  }
  
  static unsigned char theAnswer=1;
  while(1)
  {
  if(digitalRead(change))
  {
    if(theAnswer==3)
    {
      theAnswer=1;
    }
    else
    {
      theAnswer++;
    }
    ChangeLED(theAnswer);
    while(digitalRead(change));
  }

  if (digitalRead(send))
  {
    
    switch (theAnswer)
  {
  case 1:
    Serial.print("Yes\n");
    break;

  case 2:
    Serial.print("No\n");
    break;


  case 3:
    Serial.print("Maybe\n");
    break;
  
  default:
    break;
  }


    Clearleds();
    //Serial.println("ended");
    delay(1000);
    break;
  }
  
  
  
  }




}

void ask()
{

  static unsigned char theQuestion=0;
  unsigned char Switch_State=digitalRead(change);
  unsigned char Switch_Send=digitalRead(send);
  delay(2);
  if(Switch_State)  //the switch went high
    {
        if(theQuestion==3)   //as we have only 3 questions
        {
            theQuestion=1;
            QuestionMode(theQuestion);   //Changing the Question
        }
        else
        {
            theQuestion++;
            QuestionMode(theQuestion);
        }
        Switch_State=digitalRead(change);
        delay(2);
        //loop to wait for the switch to go low again so the qeutions wont change again if the button is long pressed
        while(Switch_State){Switch_State=digitalRead(change); delay(2);}
    }
    if(Switch_Send)
    {

     
      Switch_Send=digitalRead(send);
      while (Switch_Send)
      {
        Switch_Send=digitalRead(send);
        delay(2);
      }
       QuestionSend(theQuestion);
      unsigned char i=0;
      char replyRecevived[20];
      while (1)
      {
        if (Serial.available())
        {
          replyRecevived[i]=Serial.read();
          i++;
          if(replyRecevived[i-1]=='\n')
          {
            replyRecevived[i]='\0';
            break;
          }
        }
        
      }

  digitalWrite(recevived1,LOW);
  digitalWrite(recevived2,LOW);
  digitalWrite(recevived3,LOW);
  if(isEqualString(replyRecevived,yes))
  {
    
    digitalWrite(recevived1,HIGH);
    
  }
  else if(isEqualString(replyRecevived,no))
  {
    
    digitalWrite(recevived2,HIGH);
    
  }
  else if(isEqualString(replyRecevived,maybe))
  {
   
    digitalWrite(recevived3,HIGH);
    
  }
      


    }


}


void Clearleds()
{
  digitalWrite(choice1,LOW);
  digitalWrite(choice2,LOW);
  digitalWrite(choice3,LOW);
}


void ChangeLED(int x)
{
  Clearleds();
  switch (x)
  {
  case 1:
    digitalWrite(choice1,HIGH);
    break;

  case 2:
    digitalWrite(choice2,HIGH);
    break;


  case 3:
    digitalWrite(choice3,HIGH);
    break;
  
  default:
    break;
  }
}



int isEqualString(char* arr1,char* arr2)
{
  if(arr_size(arr1)!= arr_size(arr2))
    return 0;
int i=0;
  while(arr1[i]!='\0')
  {
    if(arr1[i] != arr2[i])
      return 0;
    i++;
  }
  return 1;
  
}



int arr_size(char *ptr)
{
    //variable used to access the subsequent array elements.
    int offset = 0;
    //variable that counts the number of elements in your array
    int count = 0;

    //While loop that tests whether the end of the array has been reached
    while (*(ptr + offset) != '\0')
    {
        //increment the count variable
        ++count;
        //advance to the next element of the array
        ++offset;
    }
    //return the size of the array
    return count;
}



void QuestionMode(int x)
{
    Clearleds();   
    switch(x)
    {
    case 1:
        digitalWrite(choice1,HIGH);

        break;
    case 2:
       digitalWrite(choice2,HIGH);
        break;
    case 3:
       digitalWrite(choice3,HIGH);
        break;


    }


}

void QuestionSend(int x)
{

  switch(x)
    {
    case 1:
        Serial.print("Hey Tiva,Are you hungry?\n");

        break;
    case 2:
       Serial.print("Hey Tiva,Are you thirsty?\n");
        break;
    case 3:
       Serial.print("Hey Tiva,Are you happy?\n");
        break;


    }
}

