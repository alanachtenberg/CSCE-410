/* 
    File: kernel.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University

    Date  : 12/08/23

    The "main()" function is the entry point for the kernel. 

*/



/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "console.H"

using namespace std;

/* ======================================================================= */
/* MAIN -- THIS IS WHERE THE OS KERNEL WILL BE STARTED UP */
/* ======================================================================= */

int main()
{

  /* -- INITIALIZE CONSOLE */
  Console::init(); 
  Console::puts("Initialized console.\n");
  Console::puts("\n");

  Console::puts("Replace the following <NAME> field with your name.\n");
  Console::puts("\n");
  Console::puts("WELCOME TO MY KERNEL!\n");
  Console::puts("      ");
  Console::set_TextColor(GREEN, RED);
  Console::puts("Alan Micheal Achtenberg\n");

  /* -- LOOP FOREVER! */
  for(;;);
  
}
