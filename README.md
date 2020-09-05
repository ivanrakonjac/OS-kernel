# OS-kernel

A preemptive multithreaded kernel for Intel 8086 CPU with time sharing on a single CPU which supports context switching, event handling and semaphores using C++ and 8086 assembly language.

Memory model is huge. 

This was a final project in Operating Systems 1 course at School of Electrical Engineering, University of Belgrade.


How to use OS kernel:
----------------------

* Implement function userMain(argc, argv) in your user code, this is the main entry point for your application, arguments are passed from command line.

* Create other user threads by extending OS class thread. You can specify thread name, thread stack size and time slice (minimum run time before thread can be interrupted). Implement custom thread code inside run method. Start thread by calling start method, thread will be started and run will be called on thread selection that depends on scheduler.

Saveti za izradu projekta:
----------------------------

* Izradu zapoceti od zadatka 4 sa vezbi. Krajnji izgled zadatka (tj. pocetnu fazu projekta), ali i sve commitove, mozet videti ovde: https://github.com/ivanrakonjac/OS1-zadaci/commits/master/Zadatak_4. 

* Projekat graditi po fazama. Prvo napraviti PCB, pa SEMAFORE, pa EVENTOVE.

* Detaljno testirati projekat izmedju svake faze.

* Truditi se da pisete svoje testove, a i ako koristite one sa boxa, trudite se da razumete sta zapravo rade (ne zadovoljavati se da se program samo zavrsi).

* Veliki broj gresaka je vezan za liste, tako da je kvalitetna lista (i za PCB i za SEMAFORE) zlata vredna.

* Pre upotrebe lista u projektu dobro ih istestirati, posebno obratiti paznju na granicne slucajeve (tipa dohvatanje iz prazne liste i slicno). Mozete ih testirati sa debuggerom u normalnom Visual Studiu, pa ih kopirati u projekat na virtuelnoj masini.

* Cuvati verzije koda, kako bi nakon pojave bagova mogli da se vratite na poslednju verziju koja radi.

* Pisati destruktore za klase i dealocirati memoriju.

* Memory leak mozete proveriti gledajuci koliko proces zauzima memorije pri pokretanjima u Task menadzeru.

* Ako imate problem, i imate koga da pitate, PITAJTE. Bolje nego da gubite 3 dana vremena da sami debagujete.

* Ne odustajte!


Javni test:
---------------

* Skup klasa koje dodate u vas projekat.
	
* Ako vam radi javni test (pokrece se i zavrsava) ne znaci da vam radi i projekat.

Saveti za testiranje:
------------------------

* Pokretati vise puta projekat bez gasenja konzole (svaki put mora da se zavrsi).

* Pri gasenju izvrsavanja drzati strelicu i pritiskati ESC vise puta (ne sme da pukne).

* Za proveru memory leaka pustiti 100 100 2 (pre toga skinite ogranicenje), i pustite da se izvrsava 10ak min.

* Obavezno probati kombinaciju 1 0 0 za IDLE nit.

* Takodje vise puta zaredom probati 50 50 2, 19 19 2, 10 10 2, najbolje u kombinaciji.


Nadam se da ce ovi saveti nekome pomoci!

Mnogo srece pri izradi!
