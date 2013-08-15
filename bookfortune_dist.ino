// book fortune teller v. 0.1b
// by jesse henning - http://thisisasentence.tumblr.com

// code designed to make an arduino tell a thermal printer
// to spit out a cute slip with some words of wisdom, a book
// you want people to read, and where to find the book in your library.

#include <Entropy.h>              //much thanks to Walter Anderson
                                  //https://code.google.com/p/avr-hardware-random-number-generation/
#include "SoftwareSerial.h"
#include "Adafruit_Thermal.h"     //also thanks to Adafruit/Limor Fried
                                  //https://github.com/adafruit/Adafruit-Thermal-Printer-Library
#include <avr/pgmspace.h>

int printer_RX_Pin = 5;  // this is the green wire
int printer_TX_Pin = 6;  // this is the yellow wire
int buttonPin = 7;
int ledPin = 4;          // if you're not using a lighted button, comment this

//copying fortune strings into flash memory
prog_char fortune0[] PROGMEM="Accept that some days you're\nthe pigeon and some days\nyou're the statue.";
prog_char fortune1[] PROGMEM="When everything's coming your\nway, you're in the wrong lane.";
prog_char fortune2[] PROGMEM="Always keep your words soft and\nsweet, just in case you have to eat them.";
prog_char fortune3[] PROGMEM="Don't fry bacon in the nude.";
prog_char fortune4[] PROGMEM="Some people dream of fortunes,\nothers dream of cookies.";
prog_char fortune5[] PROGMEM="Ease up on the red button,\nman.";
prog_char fortune6[] PROGMEM="You are not quite young enough \nto know everything.";
prog_char fortune7[] PROGMEM="You have Van Gogh's ear for\nmusic.";
prog_char fortune8[] PROGMEM="A good listener is usually\nthinking about something else.";
prog_char fortune9[] PROGMEM="If you believe in telekinetics,\nraise my hand.";
prog_char fortune10[] PROGMEM="Time is a drug. Too much of it\nkills you.";
prog_char fortune11[] PROGMEM="Collect all 70!";
prog_char fortune12[] PROGMEM="When ideas fail, words\ncome in very handy.";
prog_char fortune13[] PROGMEM="Be careful about reading\nhealth books. You may die of a\nmisprint.";
prog_char fortune14[] PROGMEM="The early bird may get the\nworm, but it's the second mouse\nthat gets the cheese.";
prog_char fortune15[] PROGMEM="Weather forecast for tonight:\ndark.";
prog_char fortune16[] PROGMEM="If you want a guarantee,\nbuy a toaster.";
prog_char fortune17[] PROGMEM="Never judge a book by\nits movie.";
prog_char fortune18[] PROGMEM="Let a smile be your umbrella\nand you'll end up with a face \nfull of rain.";
prog_char fortune19[] PROGMEM="If you can't get rid of the\nfamily skeleton, you might as\nwell make it dance.";

PROGMEM const char *fortune_table[]=
{
  fortune0, fortune1, fortune2, fortune3, fortune4, fortune5,
  fortune6, fortune7, fortune8, fortune9, fortune10, fortune11,
  fortune12, fortune13, fortune14, fortune15, fortune16, fortune17,
  fortune18, fortune19
};

char fortune[90]; //array size set to longest string size (or thereabout)

//copy book titles into flash memory
//titles, authors, and locations should all have the same array address
prog_char title0[] PROGMEM="The Perfect Ghost";
prog_char title1[] PROGMEM="The Effects of Light";
prog_char title2[] PROGMEM="Once Upon a River";
prog_char title3[] PROGMEM="The Other Typist";
prog_char title4[] PROGMEM="The Talk Funny Girl";
prog_char title5[] PROGMEM="The Jungle";
prog_char title6[] PROGMEM="The Glass of Time";
prog_char title7[] PROGMEM="I Am Legend";
prog_char title8[] PROGMEM="Tell the Wolves I'm Home";
prog_char title9[] PROGMEM="Northanger Abbey";
prog_char title10[] PROGMEM="The Killer Angels";
prog_char title11[] PROGMEM="The Fallback Plan";
prog_char title12[] PROGMEM="Sister of My Heart";
prog_char title13[] PROGMEM="Saturday";
prog_char title14[] PROGMEM="Flimsy Little Plastic Miracles";
prog_char title15[] PROGMEM="Geek Love";
prog_char title16[] PROGMEM="Alias Grace";
prog_char title17[] PROGMEM="Truth in Advertising";
prog_char title18[] PROGMEM="Miles from Nowhere";
prog_char title19[] PROGMEM="Domestic Violets";
prog_char title20[] PROGMEM="Revolutionary Road";
prog_char title21[] PROGMEM="Interpreter of Maladies";
prog_char title22[] PROGMEM="A Clockwork Orange";
prog_char title23[] PROGMEM="Salmon Fishing in the Yemen";
prog_char title24[] PROGMEM="The Maytrees";
prog_char title25[] PROGMEM="City of Veils";
prog_char title26[] PROGMEM="Orphan Train";
prog_char title27[] PROGMEM="The Things They Carried";
prog_char title28[] PROGMEM="The Abstinence Teacher";
prog_char title29[] PROGMEM="Pattern Recognition";
prog_char title30[] PROGMEM="A Good American";
prog_char title31[] PROGMEM="Charity Girl";
prog_char title32[] PROGMEM="A Good American";
prog_char title33[] PROGMEM="The Expats";
prog_char title34[] PROGMEM="Parlor Games";
prog_char title35[] PROGMEM="The Enchanted Life of Adam Hope";
prog_char title36[] PROGMEM="Darling Jim";
prog_char title37[] PROGMEM="The Age of Innocence";
prog_char title38[] PROGMEM="All You Could Ask For";
prog_char title39[] PROGMEM="The Thirteenth Tale";
prog_char title40[] PROGMEM="Where'd You Go, Bernadette?";
prog_char title41[] PROGMEM="One Day";
prog_char title42[] PROGMEM="Mother Night";
prog_char title43[] PROGMEM="The End of Your Life Book Club";
prog_char title44[] PROGMEM="Blindness";
prog_char title45[] PROGMEM="Rabbit, Run";
prog_char title46[] PROGMEM="Heart of the Matter";
prog_char title47[] PROGMEM="The Gargoyle";
prog_char title48[] PROGMEM="A Dual Inheritance";
prog_char title49[] PROGMEM="The Feast of Love";
prog_char title50[] PROGMEM="Double Feature";
prog_char title51[] PROGMEM="Motherless Brooklyn";
prog_char title52[] PROGMEM="This is Where I Leave You";
prog_char title53[] PROGMEM="The Comfort of Lies";
prog_char title54[] PROGMEM="A Reliable Wife";
prog_char title55[] PROGMEM="Pomegranate Soup";
prog_char title56[] PROGMEM="Spin";
prog_char title57[] PROGMEM="Things Fall Apart";
prog_char title58[] PROGMEM="Gold";
prog_char title59[] PROGMEM="City of Thieves";
prog_char title60[] PROGMEM="Run";
prog_char title61[] PROGMEM="Tinkers";
prog_char title62[] PROGMEM="The Book of Joe";
prog_char title63[] PROGMEM="These Is My Words";
prog_char title64[] PROGMEM="The Newlyweds";
prog_char title65[] PROGMEM="On Beauty";
prog_char title66[] PROGMEM="The Next Thing On My List";
prog_char title67[] PROGMEM="The Postmistress";
prog_char title68[] PROGMEM="Everything Is Illuminated";
prog_char title69[] PROGMEM="Duty Free";


PROGMEM const char *title_table[]=
{
  title0, title1, title2, title3, title4, title5, title6, title7,
  title8, title9, title10, title11, title12, title13, title14, title15,
  title16, title17, title18, title19, title20, title21, title22,
  title23, title24, title25, title26, title27, title28, title29, 
  title30, title31, title32, title33, title34, title35, title36, 
  title37, title38, title39, title40, title41, title42, title43, 
  title44, title45, title46, title47, title48, title49, title50,
  title51, title52, title53, title54, title55, title56, title57, 
  title58, title59, title60, title61, title62, title63, title64, 
  title65, title66, title67, title68, title69
};

char title[35]; //longest title string

//copy authors into flash memory
//format text to match what your spine label looks like
prog_char author0[] PROGMEM="BARNES\nLINDA";
prog_char author1[] PROGMEM="BEVERLY-WHITTEMORE\nMIRANDA";
prog_char author2[] PROGMEM="CAMPBELL\nBONNIE JO";
prog_char author3[] PROGMEM="RINDELL\nSUZANNE";
prog_char author4[] PROGMEM="MERULLO\nRONALD";
prog_char author5[] PROGMEM="SINCLAIR\nUPTON";
prog_char author6[] PROGMEM="COX\nMICHAEL";
prog_char author7[] PROGMEM="MATHESON\nRICHARD";
prog_char author8[] PROGMEM="BRUNT\nCAROL";
prog_char author9[] PROGMEM="AUSTEN\nJANE";
prog_char author10[] PROGMEM="SHAARA\nMICHAEL";
prog_char author11[] PROGMEM="STEIN\nLEIGH";
prog_char author12[] PROGMEM="DIVAKARUNI\nCHITRA";
prog_char author13[] PROGMEM="MCEWAN\nIAN";
prog_char author14[] PROGMEM="CURRIE\nRON";
prog_char author15[] PROGMEM="DUNN\nKATHERINE";
prog_char author16[] PROGMEM="ATWOOD\nMARGARET";
prog_char author17[] PROGMEM="KENNEY\nJOHN";
prog_char author18[] PROGMEM="MUN\nNAMI";
prog_char author19[] PROGMEM="NORMAN\nMATTHEW";
prog_char author20[] PROGMEM="YATES\nRICHARD";
prog_char author21[] PROGMEM="LAHIRI\nJHUMPA";
prog_char author22[] PROGMEM="BURGESS\nANTHONY";
prog_char author23[] PROGMEM="TORDAY\nPAUL";
prog_char author24[] PROGMEM="DILALRD\nANNIE";
prog_char author25[] PROGMEM="FERRARIS\nZOE";
prog_char author26[] PROGMEM="KLINE\nCHRISTINA";
prog_char author27[] PROGMEM="OBRIEN\nTIM";
prog_char author28[] PROGMEM="PERROTTA\nTOM";
prog_char author29[] PROGMEM="GIBSON\nWILLIAM";
prog_char author30[] PROGMEM="GEORGE\nALEX";
prog_char author31[] PROGMEM="LOWENTHAL\nMICHAEL";
prog_char author32[] PROGMEM="MORIARTY\nLIANE";
prog_char author33[] PROGMEM="PAVONE\nCHRIS";
prog_char author34[] PROGMEM="BIAGGIO\nMARYKA";
prog_char author35[] PROGMEM="RILEY\nRHONDA";
prog_char author36[] PROGMEM="MOERK\nCHRISTIAN";
prog_char author37[] PROGMEM="WHARTON\nEDITH";
prog_char author38[] PROGMEM="GREENBERG\nMIKE";
prog_char author39[] PROGMEM="SETTERFIELD\nDIANE";
prog_char author40[] PROGMEM="SEMPLE\nMARIA";
prog_char author41[] PROGMEM="NICHOLLS\nDAVID";
prog_char author42[] PROGMEM="VONNEGUT\nKURT";
prog_char author43[] PROGMEM="SCHWALBE\nWILL";
prog_char author44[] PROGMEM="SARAMAGO\nJOSE";
prog_char author45[] PROGMEM="UPDIKE\nJOHN";
prog_char author46[] PROGMEM="GIFFIN\nEMILY";
prog_char author47[] PROGMEM="DAVIDSON\nANDREW";
prog_char author48[] PROGMEM="HERSHON\nJOANNA";
prog_char author49[] PROGMEM="BAXTER\nCHARLES";
prog_char author50[] PROGMEM="KING\nOWEN";
prog_char author51[] PROGMEM="LETHEM\nJONATHAN";
prog_char author52[] PROGMEM="TROPPER\nJONATHAN";
prog_char author53[] PROGMEM="MEYERS\nRANDY";
prog_char author54[] PROGMEM="GOOLRICK\nROBERT";
prog_char author55[] PROGMEM="MEHRAN\nMARSHA";
prog_char author56[] PROGMEM="MCKENZIE\nCATHERINE";
prog_char author57[] PROGMEM="ACHEBE\nCHINUA";
prog_char author58[] PROGMEM="CLEAVE\nCHRIS";
prog_char author59[] PROGMEM="BENIOFF\nDAVID";
prog_char author60[] PROGMEM="PATCHETT\nANN";
prog_char author61[] PROGMEM="HARDING\nPAUL";
prog_char author62[] PROGMEM="TROPPER\nJONATHAN";
prog_char author63[] PROGMEM="TURNER\nNANCY";
prog_char author64[] PROGMEM="FREUDENBERGER\nNELL";
prog_char author65[] PROGMEM="SMITH\nZADIE";
prog_char author66[] PROGMEM="SMOLINSKI\nJILL";
prog_char author67[] PROGMEM="BLAKE\nSARA";
prog_char author68[] PROGMEM="FOER\nJONATHAN";
prog_char author69[] PROGMEM="MOHSIN\nMONI";

PROGMEM const char *author_table[]=
{
  author0, author1, author2, author3, author4, author5, author6, author7,
  author8, author9, author10, author11, author12, author13, author14, author15,
  author16, author17, author18, author19, author20, author21, author22,
  author23, author24, author25, author26, author27, author28,
  author29, author30, author31, author32, author33, author34,
  author35, author36, author37, author38, author39, author40, author41,
  author42, author43, author44, author45, author46, author47, author48,
  author49, author50, author51, author52, author53, author54, author55,
  author56, author57, author58, author59, author60, author61, author62,
  author63, author64, author65, author66, author67, author68, author69
};

char author[26]; //set to match longest author string

Adafruit_Thermal printer(printer_RX_Pin, printer_TX_Pin); //initialize printer

void setup(){
  printer.begin();
  Entropy.Initialize();        //provides random number generation
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  digitalWrite(buttonPin, HIGH);
}
  
void loop(){
  
    int val = digitalRead(buttonPin);
    if(val == LOW)
   {
     digitalWrite(ledPin, LOW);
     printer.wake();
     delay(50);
     getFortune();
     printFortune();
     printer.feed(4);
     digitalWrite(ledPin, HIGH);
   }
   else{}
}

void getFortune()  //retrieves full fortune from flash memory, 
                   //fills fortune/author/title arrays
{
  int f = Entropy.random(0,20);
  strcpy_P(fortune, (char*)pgm_read_word(&(fortune_table[f])));
  int b = Entropy.random(0,70);
  strcpy_P(title, (char*)pgm_read_word(&(title_table[b])));
  strcpy_P(author, (char*)pgm_read_word(&(author_table[b])));
}

void printFortune() //prints output to printer
{
  printer.println("Today's words of wisdom:");
  printer.print("'"); printer.print(fortune); printer.print("'");
  printer.println();
  printer.println();
  printer.println("I predict you'll be reading...");
  printer.underlineOn();
  printer.println(title);
  printer.underlineOff();
  printer.println();
  printer.println("..and you can find it here:");
  printer.println("FICTION");
  printer.println(author);
  printer.justify('C');
//  printer.println(F("westervillelibrary.org"));   uncomment and use
//  printer.println(F("@westervlibrary"));          your library's info
  printer.justify('L');
}
