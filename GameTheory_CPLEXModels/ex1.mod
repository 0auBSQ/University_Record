/* Exercice 1 */

dvar float x1;
dvar float x2;
dvar float x3;

dvar float g;     // Gain
/*-----------------------*/


// Maximisation du gain
/*-----------------------*/
maximise
    g;
/*-----------------------*/

// Equations de résolution
/*----------------------*/
subject to
{
    0*x1 + -50*x2 + 30*x3 <= g;
    50*x1 + 0*x2 + -40*x3 <= g;
    -30*x1 + 40*x2 + 0*x3 <= g;
    x1 + x2 + x3 == 1;
    x1 >= 0;
    x2 >= 0;
    x3 >= 0;
} 