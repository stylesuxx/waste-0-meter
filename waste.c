#include "filesystem/ff.h"
#include "basic/basic.h"
#include "usetable.h"

#define MAXSIZE 1024

int StrToInt(char convert[]);

void ram(void) {
  // read values from file
  char filename[]= "waste.cfg";
  uint8_t buf[MAXSIZE];
  int res;
  UINT readbytes;
  FIL file;
  int i,j=0;
  char *beginline=buf;

  // Char array for the counted values
  char *cntar[4];

  int pos = 0;
  int drink = 0;
  int button;
  
  int mateCnt = 0, beerCnt = 0 , coffeeCnt = 0, tschunkCnt = 0;

  // If file exists read values from file
  res=f_open(&file, (const char*)filename, FA_OPEN_EXISTING|FA_READ);
  if(!res){
    res=f_read(&file, (char *)buf, MAXSIZE, &readbytes);
    for(i=0;i<readbytes;i++){
      if(buf[i]=='\n'){
	j=0;
	buf[i]=0;
	cntar[pos] = beginline;
	pos++;
	beginline=&buf[i+1];
      }
    }
    f_close(&file);
  
    mateCnt = StrToInt(cntar[0]);
    beerCnt = StrToInt(cntar[1]);
    coffeeCnt = StrToInt(cntar[2]);
    tschunkCnt = StrToInt(cntar[3]);
  }

  while (1){
    // Write text to screen
    lcdClear();
    DoString(3, 4, "waste-0-meter");
    DoString(0, 12, "--------------");

    DoString(10, 20, "Mate:");
    DoString(10, 32, "Beer:");
    DoString(10, 44, "Coffee:");	
    DoString(10, 56, "Tschunk:");

    // Max value for all counters is 999
    DoString(70, 20, IntToStr(mateCnt, 3, 0));
    DoString(70, 32, IntToStr(beerCnt, 3, 0));
    DoString(70, 44, IntToStr(coffeeCnt, 3, 0));
    DoString(70, 56, IntToStr(tschunkCnt, 3, 0));

    // print Menu position Markers
    if(drink == 0) DoString(1, 20, ">");
    if(drink == 1) DoString(1, 32, ">");
    if(drink == 2) DoString(1, 44, ">");
    if(drink == 3) DoString(1, 56, ">");

    lcdDisplay();

    while((button=getInputRaw()) == BTN_NONE) delayms(23);

    // Exit
    if(button==BTN_LEFT){ return; }
      
    // Menu up
    if(button==BTN_UP){
      drink--;
      if(drink < 0) drink = 3;
	drink= drink%4;
    }

    // Menu down
    if(button==BTN_DOWN){
      drink++;
      drink= drink%4;
    }
    
    // Increase counters on push and write file
    if(button==BTN_ENTER){
      if(drink == 0) mateCnt++;
      if(drink == 1) beerCnt++;
      if(drink == 2) coffeeCnt++;
      if(drink == 3) tschunkCnt++;

      char outstr[20];
      int position = 0;

      // This is fucked up, if you initialize alle 4 values at once all have the value
      // of the last initialized, thats why I initialize before the for and then go on
      const char *mate = IntToStr(mateCnt, 3, 0);
      for(int z=0; z< strlen(mate); z++) outstr[position++] = mate[z];
      outstr[position++] = '\n';

      const char *beer = IntToStr(beerCnt, 3, 0);
      for(int z=0; z< strlen(beer); z++) outstr[position++] = beer[z];
      outstr[position++] = '\n';        

      const char *coffee = IntToStr(coffeeCnt, 3, 0);
      for(int z=0; z< strlen(coffee); z++) outstr[position++] = coffee[z];
      outstr[position++] = '\n';

      const char *tschunk = IntToStr(tschunkCnt, 3, 0); 
      for(int z=0; z< strlen(tschunk); z++) outstr[position++] = tschunk[z];
      outstr[position++]='\n';
      
      outstr[position]='\0';

      // Write to file
      writeFile("waste.cfg", outstr, strlen(outstr));
    }
    
    // Reset Counter
    if(button == BTN_RIGHT){
      while(1){
	while((button=getInputRaw()) == BTN_NONE) delayms(23);
	
	lcdClear();
	DoString(20, 4, "RESET?");
	DoString(0, 12, "--------------");
	DoString(0, 26, "push = reset");
	DoString(0, 42, "left = exit");
	lcdDisplay();
	
	if(button == BTN_ENTER){
	  // Reset Counters to 0  
	  writeFile("waste.cfg", "0\n0\n0\n0\n\0", 9);
	  mateCnt = 0;
	  beerCnt = 0;
	  coffeeCnt = 0;
	  tschunkCnt = 0;
	  
	  break;
	}
	
	if(button == BTN_LEFT){
	  break;
	}
      }
    }
    
    // We need a bit of delay to not go through the menu too fast
    delayms(150);
  }
}

// Convert String to Int
int StrToInt(char convert[]){
  int stellen = strlen(convert);
  int base = 10;
  int mul = 1;
  int value = 0;

  for(int k = (stellen-1); k>=0; k--){
    value += ((int)convert[k]-(int)'A'+17)*mul;
    mul *= base;
  }
  return value;
}