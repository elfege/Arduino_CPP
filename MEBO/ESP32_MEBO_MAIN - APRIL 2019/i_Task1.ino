void workLoad (void) ;                // prototype is required


void CPU_0_task1( void * parameter )
{

  for (;;) {

    while (mvt && noturn || makeEncHappen) // only if no mvt, and this is not a turn request, unless otherwise requested (makeEncHappen)
    {

      if(EncL.p)
      {
        cl++;
        EncL.p = false;
      }
      if(EncR.p)
      {
        cr++;
        EncR.p = false;
      }
      workLoad();
      //delay(1);
    }
    
  }
  delay(5);
}
