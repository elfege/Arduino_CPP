void workLoad (void) ;                // prototype is required


void CPU_0_task1( void * parameter )
{
  for (;;) {
    workLoad();
    delay(10);
  }
}
