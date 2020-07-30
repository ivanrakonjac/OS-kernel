Modifikacija vezana za signale, JUL 2020.

Zadatak:
Modifikovati metodu registerHandler na sledeci potpis:

registerHandler(SignalId id,SignalHandler sh[],int maska);

sh[] predstavlja niz pokazivaca na fukcije - signalHandlere.
maska je ceo broj koji kada se pretvori u bitski oblik odredjuje da li ce se neki signal izvrsiti ili ne.
maximalan broj signalHandlera po signalu je 5.

Primer:
SignalHandler sh[5]={sh1,sh2,sh3,sh4,sh5};

maska=5 <=> 00101
izvrsice se samo sh3 i sh5
------------------------------
maska=3 <=> 00011
izvrsice se samo sh4 i sh5
------------------------------