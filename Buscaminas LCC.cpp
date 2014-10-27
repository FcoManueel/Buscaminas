/* ------------------------------------------------------------ */
/*                                                              */
/*   Buscaminas.                                                */
/*   Programación de Computadoras I - Proyecto Final            */
/*                                                              */
/*                          Eduardo Acuña Yeomans               */
/*                           Javier Isaac Montaño               */
/*                        Francisco Miguel Granillo             */
/*                          Francisco Manuel Valle              */
/*                                                              */
/*                              07-Jun-2012                     */
/*                                                              */
/* ------------------------------------------------------------ */

#include <iostream>
#include <cstdlib>
#include <new>
#include <iomanip>
#include <ctime>
#include <conio2.h>
using namespace std;

#define MASTER_KEY 13 //ASCII para la barra tecla de seleccion. 13=Enter
#define ARRIBA 72 //ASCII para la tecla hacia arriba
#define ABAJO 80 //ASCII para la tecla hacia abajo
#define DERECHA 77 //ASCII para la tecla hacia la derecha
#define IZQUIERDA 75 //ASCII para la tecla hacia la izquierda
#define ESC 27 //ASCII para la tecla de salida. 27=Escape

//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//--------------------------------- Estructuras ---------------------------------//
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//

struct Casilla
{
	int valor;
	bool estaDescubierta;
	bool tieneFlag;
};
struct Partida
{
	short int ren;
	short int col;
	short int totalMinas;
	short int totalFlags;
	bool tocoMina;
	int casillasDescubiertas;
	double tiempo;
	Casilla **M;
};

//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//--------------------------------- Prototipos ----------------------------------//
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//

//----------- Menus -------------------------//
inline void titleScreen();
void menuScreen();
short int dificultad(short int &);
void instrucciones();
void imprimirIndicador();
void borrarIndicador();
inline void moverCursorMenu(int ,int ,int);
//----------- Pre-Juego ---------------//
void gestorJuego(Partida &,int );
bool crearTablero(Partida &);
void preJuego(Partida &);
void minarJuego(Partida &);
void dibujarPantalla(Partida &);
//----------- InJuego -----------------//
int enJuego(Partida &);
void modificarCasilla(Partida &,int,int);
inline void quitarFlags(Partida &, int, int);
void flagCasilla(Partida &,int,int);
void colorearNumeros(Partida &,int,int);
bool abrirCasillasNulas(Partida &,int ,int );
//----------- Pos-Juego -------------------//
void mostrarTableroAbierto(Partida &);
void mensajeWIN(Partida &);			
void mensajeLOSE(Partida &);
void liberarMemoria(Partida &);

int posX=1,posY=1;
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//-----------------------------------  Main  ------------------------------------//
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
int main()
{
    Partida buscaminas;
    bool salir=false;
	short int modo=1;
    short int statusJuego;
    int seleccionMenu;

	srand(time(NULL));
    do{
    	titleScreen();
    	menuScreen();
    	seleccionMenu=posY;
        switch(seleccionMenu){
             case 1:
                    gestorJuego(buscaminas,modo);
        			preJuego(buscaminas);
        			statusJuego=enJuego(buscaminas);//Se manda a ejecutar el juego en la función enJuego();, que devuelve 1 si gana, 2 si pierde
        			
                    mostrarTableroAbierto(buscaminas);
                    dibujarPantalla(buscaminas);
        			getch();
        			switch(statusJuego){
        				case 1: //Ganar
        						mensajeWIN(buscaminas);
        						break;
        				case 2: //Perder
        						mensajeLOSE(buscaminas);
        						break;
        			}
					system("pause");
			        liberarMemoria(buscaminas);
			        break;
             case 2:
                    modo=dificultad(modo); //La función dificultad(); se le envia el modo de dificultad actual y regresa el modo nuevo
                    gotoxy(30,10); //Mueve el cursor de vuelta a la opción "Jugar"
                    posY=1;
                    break;
             case 3:
                    instrucciones();
                    gotoxy(30,10); //Mueve el cursor de vuelta a la opción "Jugar"
                    posY=1;
                    salir=false;
                    break;
             case 4:
                    salir=true;
                    break;
                  
                            
        }
    }while(!salir);
}

//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//---------------------------------- Funciones ----------------------------------//
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------- Menus -------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------//

inline void titleScreen()
{
  //Texto matón que dice "Buscaminas"
  textbackground(YELLOW);
	clrscr();
	textcolor(RED);
	cout<<"\t                                       (\n";
	cout<<"\t         (                                         )\\ )\n";
	cout<<"\t       ( )\\   (            )    )   (           )(()/(\n";
    cout<<"\t       )((_) ))\\ (   (  ( /(   (    )\\  (    ( /( /(_))\n";
	textcolor(LIGHTRED);
	cout<<"\t      ((_)_ /((_))\\  )\\ )(_))  )\\ '((_) )\\ ) )(_)|_))\n";
    textcolor(BLACK);
    cout<<"\t       | _ |";
    textcolor(LIGHTRED);
    cout<<"_))(((_)((_|(_)_ _((_))'(_)_(_/(((_)_";
    textcolor(BLACK);
    cout<<"/ __|\n";
	cout<<"\t       | _ \\ || (_-< _|/ _` | '  \\";
    textcolor(LIGHTRED);
    cout<<"()";
    textcolor(BLACK);
    cout<<"| | ' \\)) _` \\__ \\\n";
	cout<<"\t       |___/\\_,_/__|__|\\__,_|_|_|_| |_|_||_|\\__,_|___/\n";
}

void menuScreen()
{
    short int keystroke;
    textcolor(LIGHTRED);
    cout<<"\n\t\t             ->";
    textcolor(BROWN);
    cout<<"  Jugar               "<<endl;
    textcolor(BLACK);
    cout<<"\n\n";
    cout<<"\t\t               Dificultad      "<<endl;
    cout<<"\n\n";
    cout<<"\t\t              Instrucciones    "<<endl;
    cout<<"\n\n";
    cout<<"\t\t             Salir del Juego    "<<endl;
    cout<<"\n\n\n";
    gotoxy(3,25);
    textcolor(DARKGRAY);
    cout<<"Navegacion <Flechas>\t\t\t\t\tSeleccion <Enter>";
    textcolor(LIGHTRED); //Color con el que se imprimira el indicador de posición
    gotoxy(30,10);
    do{
       keystroke=getch();
       moverCursorMenu(keystroke,1,4);
    }while(keystroke!=MASTER_KEY);
}

short int dificultad(short int &modoAnterior)
{
    short int keystroke, modoNuevo;
    clrscr();
    gotoxy(1,6);
    textcolor(LIGHTRED);
    cout<<"\t\t\t\t"<<"Dificultad :S!"<<"\n\n\n"<<endl;
    cout<<"\t\t\t"<<"     -> ";
    textcolor(MAGENTA);
    cout<<"Principiante"<<endl;
    textcolor(BLUE);
    cout<<"\t\tMinas:10"<<"\tAlto: 9 "<<"\tAncho:9\n"<<endl;

    textcolor(MAGENTA);
    cout<<"\t\t\t\t"<<"Intermedio"<<endl;
    textcolor(BLUE);
    cout<<"\t\tMinas:40"<<"\tAlto: 16"<<"\tAncho:16\n"<<endl;

    textcolor(MAGENTA);
    cout<<"\t\t\t\t"<<"Dificil"<<endl;
    textcolor(BLUE);
    cout<<"\t\tMinas:99"<<"\tAlto: 16"<<"\tAncho:30\n"<<endl;
    
    gotoxy(1,25);
    textcolor(DARKGRAY);
    cout<<"...Presione la tecla [ESC] para regresar";
    textcolor(LIGHTRED);
    posY=1;
    gotoxy(30,10);
    do{
       keystroke=getch();
       moverCursorMenu(keystroke,1,3);
    }while(keystroke != MASTER_KEY && keystroke != ESC);
    modoNuevo=posY;
    posY=1;
    if (keystroke==MASTER_KEY) return modoNuevo;
    else return modoAnterior;
}

void instrucciones()
{
    short int out;
        clrscr();
        textcolor(MAGENTA);
        cout<<"Gameplay:\n"<<endl;
        textcolor(BLUE);
        cout<<"\tEl objetivo del juego es despejar el campo de minas sin detonar ninguna.\n";
        
        cout<<"\tEl juego termina cuando no quedan mas casillas por destapar o cuando se\n";
        cout<<"\tdestapa una casilla minada.\n";
        
        cout<<"\tLa manera de deducir si una casilla tiene o no mina es observando las \n";
        cout<<"\tcasillas de su alrrededor. Las casillas sin minas pueden contener un \n";
        cout<<"\tnumero, el cual indica cuantas minas hay a su alrrededor (incluyendo \n";
        cout<<"\tsus adyacentes diagonales).\n";
        cout<<"\n";
        
        textcolor(MAGENTA);
        cout<<"Controles:\n"<<endl;
        textcolor(BLUE);
        cout<<"\t-Utilice las flechas del teclado para desplazarse sobre el campo minado.\n"<<endl;
        cout<<"\t-Pulse la tecla enter para destapar una casilla y mostrar su contenido.\n"<<endl;
        cout<<"\t-Con la tecla F puedes marcar una casilla, lo puedes hacer cuando creas\n";
        cout<<"\t que dicha casilla es una mina o cuando quieras marcarla para despues \n";
        cout<<"\t decidir que hacer con ella.\n"<<endl;
        textcolor(DARKGRAY);
        cout<<"\n\n...Presione la tecla [ESC] para regresar";
    do{
       out=getch();
    }while(out!=ESC);
}

inline void moverCursorMenu(int keystroke,int lim_Sup,int lim_Inf)
{
   borrarIndicador();
   if(keystroke==ARRIBA){
       (posY==lim_Sup)? posY=lim_Inf : posY-=1;
       gotoxy(posX,posY);
   }
   if(keystroke==ABAJO){
   	   posY=(posY%lim_Inf)+1;
       gotoxy(posX,posY);
   }
   imprimirIndicador();
}

void imprimirIndicador() //Imprime el indicador de posición [-->] en la pantalla (de los menús)
{
  switch(posY){
     case 1:{
          gotoxy(30,10); //Coordenadas con la posicion de la opción "Jugar"
          cout<<"->";
     }break;
     case 2:{
          gotoxy(29,13); //Coordenadas con la posicion de la opción "Dificultad"
          cout<<"->";
     }break;
     case 3:{
          gotoxy(28,16); //Coordenadas con la posicion de la opción "Instrucciones"
          cout<<"->";
     }break;
     case 4:{
          gotoxy(27,19); //Coordenadas con la posicion de la opción "Salir"
          cout<<"->";
          }
  }
  cout<<"\b\b";
}

void borrarIndicador() //Borra el indicador de posición [-->] en la pantalla (de los menús)
{
  switch(posY){
     case 1:{
          gotoxy(30,10); //Coordenadas con la posicion de la opción "Jugar"
          cout<<"  ";
     }break;
     case 2:{
          gotoxy(29,13); // Coordenadas con la posicion de la opción "Dificultad"
          cout<<"  ";
     }break;
     case 3:{
          gotoxy(28,16); //Coordenadas con la posicion de la opción "Instrucciones"
          cout<<"  ";
     }break;
     case 4:{
          gotoxy(27,19); //Coordenadas con la posicion de la opción "Salir"
          cout<<"  ";
          }
  }
}

//---------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------- Pre-Juego -----------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------//

bool crearTablero(Partida &buscaminas)
{
	buscaminas.M = new (nothrow) Casilla *[buscaminas.ren];
	if(buscaminas.M==NULL){
		textbackground(RED);
        clrscr();
        textcolor(WHITE);
	    cout<<"\nBuuuu no se pudo mij@, nos falto memoria disponible. Liberando memoria ocupada...\n";
		return 1;
	}
	for(int i=0;i<buscaminas.ren;i++){
		buscaminas.M[i] = new (nothrow) Casilla[buscaminas.col];
		if(buscaminas.M[i]==NULL){
            textbackground(RED);
            clrscr();
            textcolor(WHITE);
			cout<<"\nNo se pudo mij@, nos falto memoria disponible. Liberando memoria ocupada...\n";
			for(int j=0;j<i;j++) delete []buscaminas.M[j];
			delete []buscaminas.M;
		}
	}
}

void dibujarPantalla(Partida &buscaminas)
{
	clrscr();
	for(int i=1;i<buscaminas.ren-1;i++){
		for(int j=1;j<buscaminas.col-1;j++){
			gotoxy(j,i);
            if(buscaminas.M[i][j].tieneFlag && buscaminas.M[i][j].estaDescubierta==false){
   				cout<<"!";//Contenido de casilla con flag
			}else{
				if(buscaminas.M[i][j].estaDescubierta==true){
                    buscaminas.M[i][j].tieneFlag=false;
					if(buscaminas.M[i][j].valor>0){
                       colorearNumeros(buscaminas,i,j);
					   cout<<buscaminas.M[i][j].valor; //Contenido de casilla con valor
					   textcolor(BLACK);
					}else if(buscaminas.M[i][j].valor==0){
						cout<<" ";//Contenido de casilla nula
					}else{
						cout<<"*"; // Contenido de casilla de mina
					}
				}else{
                    textcolor(LIGHTGRAY);
					cout<<char(254); //Contenido de casilla tapada. 254:Rectángulo
					textcolor(BLACK);
				}
			}
		}
		textcolor(WHITE);
        cout<<char(179)<<"\n";
        textcolor(BLACK);
	}
	textcolor(WHITE);
	for(int m=1;m<buscaminas.col-1;m++) cout<<char(196);
	cout<<char(217)<<"\n";
	textcolor(BLACK);
	
	gotoxy(buscaminas.col+5,4);
	cout<<"Total de Minas: "<<buscaminas.totalMinas<<endl;
//    cout<<"Casillas restantes: "<<(buscaminas.ren-2)*(buscaminas.col-2)-buscaminas.totalMinas - buscaminas.casillasDescubiertas<<endl;
	gotoxy(buscaminas.col+5,5);
	cout<<"Total de Flags: "<<buscaminas.totalFlags<<endl;
   	gotoxy(1,25);
   	textcolor(DARKGRAY);
	cout<<"<Flechas> Moverse\t<Enter> Destapar casilla\t<F> Colocar flag";
	textcolor(BLACK);
}

void preJuego(Partida &buscaminas)
{
  short int xCoord=1, yCoord=1;
  char keystroke;
  buscaminas.totalFlags=0;
  for(int i=0;i<buscaminas.ren;i++){
    for(int j=0;j<buscaminas.col;j++){
      buscaminas.M[i][j].valor=0;
      buscaminas.M[i][j].estaDescubierta=false;
      buscaminas.M[i][j].tieneFlag=false; 
      if(j==0||i==buscaminas.ren-1||i==0||j==buscaminas.col-1){
        buscaminas.M[i][j].estaDescubierta=true;
      }
    }
  }
  dibujarPantalla(buscaminas);
  gotoxy(xCoord,yCoord);
    do{
       keystroke=getch();
       if(keystroke==ARRIBA && yCoord>1){
          yCoord--;
          gotoxy(xCoord,yCoord);
       }
       
       if(keystroke==ABAJO && yCoord<buscaminas.ren-2){
          yCoord++;
          gotoxy(xCoord,yCoord);
       }
       
       if(keystroke==DERECHA && xCoord<buscaminas.col-2){
          xCoord++;
          gotoxy(xCoord,yCoord);
       }
       
       if(keystroke==IZQUIERDA && xCoord>1){
          xCoord--;
          gotoxy(xCoord,yCoord);
       }
       
     if(keystroke == 'f' || keystroke == 'F'){
          flagCasilla(buscaminas,yCoord,xCoord);
          modificarCasilla(buscaminas, yCoord,xCoord);
        }
  }while(keystroke != MASTER_KEY);
  buscaminas.M[yCoord][xCoord].estaDescubierta=true;
  quitarFlags(buscaminas,yCoord,xCoord);
  minarJuego(buscaminas);
  if(buscaminas.M[yCoord][xCoord].valor==0)
    abrirCasillasNulas(buscaminas,yCoord,xCoord);
  dibujarPantalla(buscaminas);
  gotoxy(xCoord,yCoord);
}


//---------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------ InJuego ------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------//

void modificarCasilla(Partida &buscaminas,int yCoord,int xCoord)
{
    if(buscaminas.M[yCoord][xCoord].tieneFlag && buscaminas.M[yCoord][xCoord].estaDescubierta==false){
		gotoxy(xCoord,yCoord);
		cout<<"!\b"; //Contenido de casilla con flag
	}else{
        quitarFlags(buscaminas,yCoord,xCoord);
    	if(buscaminas.M[yCoord][xCoord].estaDescubierta==true){
            buscaminas.M[yCoord][xCoord].tieneFlag=false;
    		if(buscaminas.M[yCoord][xCoord].valor>0){
    			gotoxy(xCoord,yCoord);
    			colorearNumeros(buscaminas,yCoord,xCoord);
    			cout<<buscaminas.M[yCoord][xCoord].valor<<"\b"; //Contenido de casilla con valor
    			textcolor(BLACK);
    		}else if(buscaminas.M[yCoord][xCoord].valor==0){
    			gotoxy(xCoord,yCoord);
    			cout<<" \b"; //Contenido de casilla nula
    	        abrirCasillasNulas(buscaminas,yCoord,xCoord);    
    		}else if(buscaminas.M[yCoord][xCoord].valor<0){           
    			gotoxy(xCoord,yCoord);
    			cout<<"*\b"; // Contenido de casilla de mina
    		}
        	}else{
                textcolor(LIGHTGRAY);
                cout<<char(254)<<"\b"; //Contenido de casilla tapada. 254:Rectángulo
                textcolor(BLACK);
    	}
     }
}

void minarJuego(Partida &buscaminas)
{
	int jMina,iMina;
	for(int i=1;i<=buscaminas.totalMinas;i++){
        jMina=rand()%(buscaminas.col-2)+1;
        iMina=rand()%(buscaminas.ren-2)+1;
        if(buscaminas.M[iMina][jMina].valor >= 0 && buscaminas.M[iMina][jMina].estaDescubierta==false){
            buscaminas.M[iMina][jMina].valor=-9;
            buscaminas.M[iMina-1][jMina-1].valor+=1;
            buscaminas.M[iMina-1][jMina].valor+=1;
            buscaminas.M[iMina-1][jMina+1].valor+=1;
            buscaminas.M[iMina][jMina-1].valor+=1;
            buscaminas.M[iMina][jMina+1].valor+=1;
            buscaminas.M[iMina+1][jMina-1].valor+=1;
            buscaminas.M[iMina+1][jMina].valor+=1;
            buscaminas.M[iMina+1][jMina+1].valor+=1;
        }else i--;
    }
}

bool abrirCasillasNulas(Partida &buscaminas,int posy,int posx)
{
    quitarFlags(buscaminas,posy,posx);
	if(buscaminas.M[posy-1][posx-1].valor >=0 && buscaminas.M[posy-1][posx-1].estaDescubierta==false){
		buscaminas.M[posy-1][posx-1].estaDescubierta=true;
		modificarCasilla(buscaminas,posy-1,posx-1);
		buscaminas.casillasDescubiertas++;
        quitarFlags(buscaminas,posy-1,posx-1);
		if(buscaminas.M[posy-1][posx-1].valor==0)
			abrirCasillasNulas(buscaminas,posy-1,posx-1);
	}
	if(buscaminas.M[posy-1][posx].valor>=0 && buscaminas.M[posy-1][posx].estaDescubierta==false){
		buscaminas.M[posy-1][posx].estaDescubierta=true;
		modificarCasilla(buscaminas,posy-1,posx);
		buscaminas.casillasDescubiertas++;
		quitarFlags(buscaminas,posy-1,posx);
        if(buscaminas.M[posy-1][posx].valor==0)
			abrirCasillasNulas(buscaminas,posy-1,posx);
	}
	if(buscaminas.M[posy-1][posx+1].valor>=0 && buscaminas.M[posy-1][posx+1].estaDescubierta==false){
		buscaminas.M[posy-1][posx+1].estaDescubierta=true;
		modificarCasilla(buscaminas,posy-1,posx+1);
		buscaminas.casillasDescubiertas++;
		quitarFlags(buscaminas,posy-1,posx+1);
		if(buscaminas.M[posy-1][posx+1].valor==0)
			abrirCasillasNulas(buscaminas,posy-1,posx+1);
	}
	if(buscaminas.M[posy][posx-1].valor>=0 && buscaminas.M[posy][posx-1].estaDescubierta==false){
		buscaminas.M[posy][posx-1].estaDescubierta=true;
		modificarCasilla(buscaminas,posy,posx-1);
		buscaminas.casillasDescubiertas++;
		quitarFlags(buscaminas,posy,posx-1);
		if(buscaminas.M[posy][posx-1].valor==0)
			abrirCasillasNulas(buscaminas,posy,posx-1);
	}
	if(buscaminas.M[posy][posx+1].valor>=0 && buscaminas.M[posy][posx+1].estaDescubierta==false){
		buscaminas.M[posy][posx+1].estaDescubierta=true;
		modificarCasilla(buscaminas,posy,posx+1);
		buscaminas.casillasDescubiertas++;
		quitarFlags(buscaminas,posy,posx+1);
		if(buscaminas.M[posy][posx+1].valor==0)
			abrirCasillasNulas(buscaminas,posy,posx+1);
	}
	if(buscaminas.M[posy+1][posx-1].valor>=0 && buscaminas.M[posy+1][posx-1].estaDescubierta==false){
		buscaminas.M[posy+1][posx-1].estaDescubierta=true;
		modificarCasilla(buscaminas,posy+1,posx-1);
		buscaminas.casillasDescubiertas++;
		quitarFlags(buscaminas,posy+1,posx-1);
		if(buscaminas.M[posy+1][posx-1].valor==0)
			abrirCasillasNulas(buscaminas,posy+1,posx-1);
	}
	if(buscaminas.M[posy+1][posx].valor>=0 && buscaminas.M[posy+1][posx].estaDescubierta==false){
		buscaminas.M[posy+1][posx].estaDescubierta=true;
		modificarCasilla(buscaminas,posy+1,posx);
		buscaminas.casillasDescubiertas++;
		quitarFlags(buscaminas,posy+1,posx);
		if(buscaminas.M[posy+1][posx].valor==0)
			abrirCasillasNulas(buscaminas,posy+1,posx);
	}
	if(buscaminas.M[posy+1][posx+1].valor>=0 && buscaminas.M[posy+1][posx+1].estaDescubierta==false){
		buscaminas.M[posy+1][posx+1].estaDescubierta=true;
		modificarCasilla(buscaminas,posy+1,posx+1);
		buscaminas.casillasDescubiertas++;
		quitarFlags(buscaminas,posy+1,posx+1);
		if(buscaminas.M[posy+1][posx+1].valor==0)
			abrirCasillasNulas(buscaminas,posy+1,posx+1);
	}
}

inline void quitarFlags(Partida &buscaminas, int yCoord, int xCoord){
	if(buscaminas.M[yCoord][xCoord].tieneFlag){
        buscaminas.M[yCoord][xCoord].tieneFlag=false;
        buscaminas.totalFlags--;
        gotoxy(buscaminas.col+21,5); //Ir al mensaje que muestra el total de flags
        inittextinfo();//Borrar antiguo numero de flags
        cout<<buscaminas.totalFlags; //Escribir nuevo numero de flags       
     }
     gotoxy(xCoord,yCoord);
}
/*
    quitarFlags solo trabaja con las casillas que YA tienen flag, lo
    cuál la diferencía de flagCasilla, que funciona para cualquier casilla.
    La utilidad de está sub-rutina se da a la hora de llamar a la función
    recursiva abrirCasillasNulas, para que 
*/   
void flagCasilla(Partida &buscaminas,int yCoord,int xCoord)
{
	if(buscaminas.M[yCoord][xCoord].tieneFlag){
        buscaminas.M[yCoord][xCoord].tieneFlag=false;
        buscaminas.totalFlags--;
        gotoxy(buscaminas.col+21,5); //Ir al mensaje que muestra el total de flags
        inittextinfo();//Borrar antiguo numero de flags
        cout<<buscaminas.totalFlags; //Escribir nuevo numero de flags
	}else{
        buscaminas.M[yCoord][xCoord].tieneFlag=true;
        buscaminas.totalFlags++;
        gotoxy(buscaminas.col+21,5); //Ir al mensaje que muestra el total de flags
        inittextinfo();//Borrar antiguo numero de flags
        cout<<buscaminas.totalFlags;
     }
     gotoxy(xCoord,yCoord);
}

void colorearNumeros(Partida &buscaminas,int posy,int posx){
    switch(buscaminas.M[posy][posx].valor){
        case 1:
            textcolor(LIGHTBLUE);
            break;
        case 2:
            textcolor(GREEN);
            break;
        case 3:
            textcolor(LIGHTRED);
            break;
        case 4:
            textcolor(BLUE);
            break;
        case 5:
            textcolor(RED);
            break;
        case 6:
            textcolor(CYAN);
            break;
        case 7:
            textcolor(BROWN);
            break;
        case 8:
            textcolor(DARKGRAY);
            break;
    }
}

void gestorJuego(Partida &buscaminas,int modo)
{
	int ancho=0;
	int alto=0;

	switch(modo){
		case 1:								//Facil
			{
				alto=ancho=9;
				buscaminas.ren=alto+2;
				buscaminas.col=ancho+2;
				buscaminas.totalMinas=10;
			}break;
		case 2:								//Intermedio
			{
				alto=ancho=16;
				buscaminas.ren=alto+2;
				buscaminas.col=ancho+2;
				buscaminas.totalMinas=40;
			}break;
		case 3:								//Dificil
			{
				alto=16;
				ancho=30;
				buscaminas.ren=alto+2;
				buscaminas.col=ancho+2;
				buscaminas.totalMinas=99;
			}break;
	}
	buscaminas.tocoMina=false;
	buscaminas.casillasDescubiertas=1;

	crearTablero(buscaminas);

}

int enJuego(Partida &buscaminas)
{
	short int xCoord, yCoord;
	xCoord=wherex();
	yCoord=wherey();
	char keystroke;
	int totalCasillasSeguras=(buscaminas.ren-2)*(buscaminas.col-2)-buscaminas.totalMinas;
	time_t inicio,fin;
	time(&inicio);
    while(!buscaminas.tocoMina && buscaminas.casillasDescubiertas!=totalCasillasSeguras){
        gotoxy(xCoord,yCoord);
        do{
           keystroke=getch();
           if(keystroke==ARRIBA && yCoord>1){
              yCoord--;
              gotoxy(xCoord,yCoord);
           }
           if(keystroke==ABAJO && yCoord<buscaminas.ren-2){
              yCoord++;
              gotoxy(xCoord,yCoord);
           }
           if(keystroke==DERECHA && xCoord<buscaminas.col-2){
              xCoord++;
              gotoxy(xCoord,yCoord);
           }
           if(keystroke==IZQUIERDA && xCoord>1){
              xCoord--;
              gotoxy(xCoord,yCoord);
           }
    	   if(keystroke == 'f' || keystroke == 'F'){
              flagCasilla(buscaminas,yCoord,xCoord);
              modificarCasilla(buscaminas, yCoord,xCoord);
            }
    	}while(keystroke != MASTER_KEY);
    		if(buscaminas.M[yCoord][xCoord].valor<0)
    			buscaminas.tocoMina=true;
    		if(buscaminas.M[yCoord][xCoord].estaDescubierta==false){
                buscaminas.M[yCoord][xCoord].estaDescubierta=true;
                buscaminas.casillasDescubiertas++;
                modificarCasilla(buscaminas,yCoord,xCoord);
            }
	}

	time(&fin);
	buscaminas.tiempo = difftime(fin,inicio);

	if(buscaminas.tocoMina) return 2;
	else if(buscaminas.casillasDescubiertas==totalCasillasSeguras) return 1;
}

//---------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------- Pos-Juego -----------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------------//
void mostrarTableroAbierto(Partida &buscaminas)
{
    for(int i=1;i<buscaminas.ren;i++){
            for(int j=1;j<buscaminas.col;j++){
                    buscaminas.M[i][j].tieneFlag=false;
                    buscaminas.M[i][j].estaDescubierta=true;
            }
    }
}
void mensajeWIN(Partida &buscaminas)
{
	gotoxy(1,buscaminas.ren+1);
	textcolor(GREEN);
	cout<<"\t\t\tHaz GANADO el juego, pilluel@!! :D\n\n";
	cout<<"\t\t\t      Solo te tomo "<<buscaminas.tiempo<<" seg"<<endl;
    gotoxy(1,23);
}
void mensajeLOSE(Partida &buscaminas)
{
	gotoxy(1,buscaminas.ren+1);
	textcolor(LIGHTRED);
	cout<<"\t\t\tHaz PERDIDO el juego peque"<<char(164)<<"in!! :(\n\n";
	cout<<"\t\t\t  No aguantaste mas de "<<buscaminas.tiempo<<" seg"<<endl;
	gotoxy(1,23);
}

void liberarMemoria(Partida &buscaminas)
{
  for(int j=0;j<buscaminas.ren;j++) delete []buscaminas.M[j];
  delete buscaminas.M;
}
