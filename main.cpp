#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <cstdio>
#include <ctime>

#define ESCAPE 27
#define RETROCEDER 8

using namespace std;

string a[8][8], pos_original[8][8],copia[8][8],pieza,pos_destino;//los arreglos se muestran como un plano cartesiano
int x,y,x_inicial,y_inicial,verificar_color,jugador=0,modalidad=0, jaque_blancas=0, jaque_negras=0;           //verificar_color: 1=blanco 2= negro 0=vacio
bool mov_valido,deshacer_bool=false,enroque_negro_dama=true,enroque_negro_rey=true,enroque_blanco_dama=true,enroque_blanco_rey=true,enroque;
int origen_rey, origen_torre_dama, origen_torre_rey;

void pos_piezas();//determina la posicion inicial de las piezas en el tablero
void tablero();//pinta el tablero
bool movimiento_valido();//analiza el movimiento valido de todas las piezas
void lectura_mov();//recibe la casilla destino
void lectura_pos();//recibe la pieza que se desea mover
void copia_tablero();//guarda una copia del tablero en la jugada anterior
void turnos();//define el turno del jugador
void deshacer();//deshace una jugada
bool jaque();//analiza si hay jaque
void peon_corona();//cambia la pieza del peon si llega a la ultima fila
bool jaque_mate();//analiza si hay jaque mate
int menu();//menu introductorio
bool King_of_the_hill();//analiza las reglas de victora de su respectiva modalidad
void Horda();//genera el respectivo tablero de acuerdo a las reglas de esta modalidad
void Three_Check();//analiza las reglas de victora de su respectiva modalidad
void Chess960();//genera el respectivo tablero de acuerdo a las reglas de esta modalidad
void mov_enroque();//analiza las reglas de enroque

int main()
{
    if(modalidad==0)//al iniciar redirige al menu
        menu();
    else{//desarrollo normal del juego
        tablero();
        copia_tablero();
        if(modalidad==2){//introducw las condiciones de victoria si se juega horda
            bool peon=false;
            for(int j=0; j<8; j++){//se analiza si quedan peones blancos en el tablero
                for(int k=0; k<8; k++){
                    if(a[j][k]=="B_P")
                        peon=true;
                }
            }//si no hay peones el negro gana la partida
            if(!peon){
                cout<<"Ganan las negras"<<endl;
                return 0;
            }
        }
        if(modalidad==3){//se se juega king of the hill redirigue a analizar las reglas de victoria respectivas
            if(King_of_the_hill())
                return 0;
        }
        turnos();
    }
}
void pos_piezas()//determina la posicion inicial de las piezas en el tablero
{
    for(int b=0; b<8; b++)//posciciona peones
    {
        a[b][6]="N_P";
        a[b][1]="B_P";
    }
    a[0][0]="B_T";       //posiciona las demas piezas en el arreglo
    a[7][0]="B_T";
    a[0][7]="N_T";
    a[7][7]="N_T";
    a[1][0]="B_C";
    a[6][0]="B_C";
    a[1][7]="N_C";
    a[6][7]="N_C";
    a[2][0]="B_A";
    a[5][0]="B_A";
    a[2][7]="N_A";
    a[5][7]="N_A";
    a[3][0]="B_D";
    a[3][7]="N_D";
    a[4][0]="B_R";
    a[4][7]="N_R";
}
void tablero()//pinta el tablero
{
    system("cls");
    if(a[0][0]==""){     //llena el arreglo del tablero y el arreglo del patron (solo se ejecuta una vez mientras el arreglo este vacio)
    for(int b=0; b<8; b++)
    {
        for(int c=0; c<8; c++)
        {
            if((b%2==0 && c%2==0) || (b%2==1 && c%2==1)){
                a[b][c]="-";
                pos_original[b][c]=a[b][c];}
            else{
                a[b][c]="*";
                pos_original[b][c]=a[b][c];}
        }
    }
    pos_piezas();// se llama la funcion para colocar las piezas sobre el arreglo
    if(modalidad==2)//si la modalidad es horda pinta las piezas segun sus reglas
        Horda();
    if(modalidad==4)//si la modalidad es chess960 pinta las piezas segun sus reglas
        Chess960();
    }
    for(int b=7; b>-1; b--)//imprime el tablero en la consola
    {
        cout<<'\n'<<'\n';
        for(int c=0; c<8; c++)
        {
            cout<<a[c][b]<<'\t';
        }
        cout<<" "<<b+1;
    }
    cout<<'\n'<<'\n';
    for(int b=1; b<9; b++)//imprime los numeros y letras del tablero
    {
        char h[8];
        for(int j=0; j<9; j++)
        {
            h[j]=('A'+j)-1;
        }
        h[8]='H';
        cout<<h[b]<<'\t';
    }
    cout<<'\n'<<'\n';
}
bool movimiento_valido()//analiza el movimiento valido de todas las piezas
{

    //para no comer las piezas del mismo equipo

    if((pos_destino=="B_P" || pos_destino=="B_T" || pos_destino=="B_A" || pos_destino=="B_C" || pos_destino=="B_D" || pos_destino=="B_R") && verificar_color==1)
        return false;
    else if((pos_destino=="N_P" || pos_destino=="N_T" || pos_destino=="N_A" || pos_destino=="N_C" || pos_destino=="N_D" || pos_destino=="N_R") && verificar_color==2)
        return false;
    else
        goto mov;

    mov:

    //movimiento valido peones
    if((pieza=="B_P" && (y==y_inicial+1 && x==x_inicial)) || (pieza=="N_P" && (y==y_inicial-1 && x==x_inicial))){//movimiento vertical
        if(pos_destino=="*" || pos_destino=="-")
            return true;
        else
            return false;
    }
    else if((pieza=="B_P" && (y==y_inicial+1 && (x==x_inicial+1 || x==x_inicial-1))) || (pieza=="N_P" && (y==y_inicial-1 && (x==x_inicial+1 || x==x_inicial-1)))){//captura de pieza
        if(pos_destino=="*" || pos_destino=="-")
            return false;
        else
            return true;
    }
    else if(((pieza=="B_P" && y_inicial==1) && (y==y_inicial+2 && x==x_inicial)) || ((pieza=="N_P" && y_inicial==6) && (y==y_inicial-2 && x==x_inicial))){//movimiento de doble casilla
        if(pos_destino=="*" || pos_destino=="-"){
            if((pieza=="B_P" && (a[x_inicial][y-1]=="*" || a[x_inicial][y-1]=="-")) || (pieza=="N_P" && (a[x_inicial][y+1]=="*" || a[x_inicial][y+1]=="-")))
                return true;
            else
                return false;
        }
        else
            return false;
    }


    //movimiento valido rey
    if (pieza=="B_R" || pieza=="N_R"){
        if((y==y_inicial||y==y_inicial-1||y==y_inicial+1)&&(x==x_inicial||x==x_inicial-1||x==x_inicial+1))
            return true;
    //enroques
        else if(pieza=="B_R" && (x_inicial==origen_rey && y_inicial==0)){//enroque de blancas
            if((a[0][0]=="B_T" && (x==2 && y==0))&& enroque_blanco_dama){//enroque largo
                if((a[1][0]=="*" || a[1][0]=="-") && (a[2][0]=="*" || a[2][0]=="-") && (a[3][0]=="*" || a[3][0]=="-")){
                    a[3][0]="B_T";
                    a[0][0]=pos_original[0][0];
                    enroque=true;
                    return true;
                }
                else
                    return false;
            }
            else if((a[7][0]=="B_T" && (x==6 && y==0))&& enroque_blanco_rey){//enroque corto
                if((a[6][0]=="*" || a[6][0]=="-") && (a[5][0]=="*" || a[5][0]=="-")){
                    a[5][0]="B_T";
                    a[7][0]=pos_original[7][0];
                    enroque=true;
                    return true;
                }
                else
                    return false;
            }





            else
                return false;
        }
        else if(pieza=="N_R" && (x_inicial==4 && y_inicial==7)){//enroque de negras
            if((a[0][7]=="N_T" && (x==2 && y==7))&& enroque_negro_dama){//enroque largo
                if((a[1][7]=="*" || a[1][7]=="-") && (a[2][7]=="*" || a[2][7]=="-") && (a[3][7]=="*" || a[3][7]=="-")){
                    a[3][7]="N_T";
                    a[0][7]=pos_original[0][7];
                    enroque=true;
                    return true;
                }
                else
                    return false;
            }
            else if((a[7][7]=="N_T" && (x==6 && y==7))&& enroque_negro_rey){//enroque corto
                if((a[6][7]=="*" || a[6][7]=="-") && (a[5][7]=="*" || a[5][7]=="-")){
                    a[5][7]="N_T";
                    a[7][7]=pos_original[7][7];
                    enroque=true;
                    return true;
                }
                else
                    return false;
            }
            else
                return false;
        }
        else
            return false;
    }


    //movimiento valido alfiles y reina

    else if((pieza=="B_A" || pieza=="N_A") || (pieza=="B_D" || pieza=="N_D")){
        int g;
        if(((x!=x_inicial && y==y_inicial) || (x==x_inicial && y!=y_inicial)) && (pieza=="B_A" || pieza=="N_A"))
            return false;
        else{
            if(x>x_inicial && y>y_inicial){//analiza la diagonal ascendente derecha
                g=y_inicial+1;
                for(int j=x_inicial+1; j<=x; j++){
                    if(j==x && g==y){
                        mov_valido=true;
                        break;
                    }
                    if((j==x && g!=y) || (g==y && j!=x))
                        return false;
                    if(a[j][g]=="*" || a[j][g]=="-")
                        mov_valido=true;
                    else
                        return false;
                    g++;
                }
                return mov_valido;
            }
            if(x>x_inicial && y<y_inicial){//analiza la diagonal ascendente izquierda
                g=y_inicial-1;
                for(int j=x_inicial+1; j<=x; j++){
                    if(j==x && g==y){
                        mov_valido=true;
                        break;
                    }
                    if((j==x && g!=y) || (g==y && j!=x))
                        return false;
                    if(a[j][g]=="*" || a[j][g]=="-")
                        mov_valido=true;
                    else
                        return false;
                    g--;
                }
                return mov_valido;
            }
            if(x<x_inicial && y>y_inicial){//analiza la diagonal descendente derecha
                g=y_inicial+1;
                for(int j=x_inicial-1; j>=x; j--){
                    if(j==x && g==y){
                        mov_valido=true;
                        break;
                    }
                    if((j==x && g!=y) || (g==y && j!=x))
                        return false;
                    if(a[j][g]=="*" || a[j][g]=="-")
                        mov_valido=true;
                    else
                        return false;
                    g++;
                }
                return mov_valido;
            }
            if(x<x_inicial && y<y_inicial){//analiza la diagonal descendente izquierda
                g=y_inicial-1;
                for(int j=x_inicial-1; j>=x; j--){
                    if(j==x && g==y){
                        mov_valido=true;
                        break;
                    }
                    if((j==x && g!=y) || (g==y && j!=x))
                        return false;
                    if(a[j][g]=="*" || a[j][g]=="-")
                        mov_valido=true;
                    else
                        return false;
                    g--;
                }
                return mov_valido;
            }
            if(pieza=="B_A" || pieza=="N_A")
                return false;
            else//permite el resto del analisis del movimiento de la reina
                goto reina;
        }
    }

    //movimiento valido caballo
    else if(pieza=="B_C" || pieza=="N_C"){
        if(x==x_inicial+2 || x==x_inicial-2){
            if(y==y_inicial+1 || y==y_inicial-1)
                return true;
            else
                return false;
        }
        else if(x==x_inicial+1 || x==x_inicial-1){
            if(y==y_inicial+2 || y==y_inicial-2)
                return true;
            else
                return false;
        }
        else
            return false;
    }

    reina:

    //movimiento valido torre y reina
    if((pieza=="B_T" || pieza=="N_T") || (pieza=="B_D" || pieza=="N_D")){
        if((x!=x_inicial && y!=y_inicial) && (pieza=="B_T" || pieza=="N_T"))
            return false;
        else{
            if(y==y_inicial && x>x_inicial){//analiza movimiento horizontal hacia la derecha
                for(int j=x_inicial+1; j<=x; j++){
                    if(j==x){
                        mov_valido=true;
                        break;
                    }
                    if(a[j][y_inicial]=="*" || a[j][y_inicial]=="-")
                        mov_valido=true;
                    else
                        return false;
                }
                return mov_valido;
            }
            if(y==y_inicial && x<x_inicial){//analiza movimiento horizontal hacia la izquierda
                for(int j=x_inicial-1; j>=x; j--){
                    if(j==x){
                        mov_valido=true;
                        break;
                    }
                    if(a[j][y_inicial]=="*" || a[j][y_inicial]=="-")
                        mov_valido=true;
                    else
                        return false;
                }
                return mov_valido;
            }
            if(x==x_inicial && y>y_inicial){//analiza movimiento vertical hacia arriba
                for(int j=y_inicial+1; j<=y; j++){
                    if(j==y){
                        mov_valido=true;
                        break;
                    }
                    if((a[x_inicial][j]=="*" || a[x_inicial][j]=="-"))
                        mov_valido=true;
                    else
                        return false;
                }
                return mov_valido;
            }
            if(x==x_inicial && y<y_inicial){//analiza movimiento vertical hacia abajo
                for(int j=y_inicial-1; j>=y; j--){
                    if(j==y){
                        mov_valido=true;
                        break;
                    }
                    if((a[x_inicial][j]=="*" || a[x_inicial][j]=="-"))
                        mov_valido=true;
                    else
                        return false;
                }
                return mov_valido;
            }
            return false;
        }
    }



    else
        return false;
}
void lectura_mov()//recibe la casilla destino
{
    y=0;//borramos los datos de las variables x y y
    x=0;
    char z;
    cout<<"Pieza seleccionada: "<<pieza<<endl;
    cout<<"Si desea otra pieza oprima la tecla borrar "<<endl;//pregunta si la seleccion de casilla es la deseada
    if(getch()==RETROCEDER)
        lectura_pos();
    cout<<endl<<"Ingrese casilla"<<endl;
    cin>>z>>y;//recibe las coordenadas
    for(char d='a'; d<'i'; d++)//cambia el caracter recibido a un entero para poder utilizarlo en las coordenadas del arreglo
    {
        if(z!=d)
            x++;
        else
            break;
    }
    y--;//resta 1 a la coordenada x para poder trabajarla en el arreglo
    pos_destino=a[x][y];//guarda la posicion de destino
    enroque=false;
    mov_valido=movimiento_valido();//redirige a analizar si el movimiento es valido
    if(mov_valido){//si el movimiento es valido se dirige a la funcion que realiza el movimiento respectivo
        mov_enroque();//analiza si el enroque es valido en caso de que se ejecute
        string aux=a[x][y], aux2=a[x_inicial][y_inicial];//guarda datos anteriores a la jugada
        a[x][y]=pieza;//realiza el movimiento de la pieza
        a[x_inicial][y_inicial]=pos_original[x_inicial][y_inicial];
        if(jaque()){//analiza si despues de hacer el movimiento ocurre un jaque
            cout<<"Movimiento invalido"<<endl;
            a[x][y]=aux;//en caso de que ocurra se prohibe el movimiento y se retornan las piezas a su posicion anterior
            a[x_inicial][y_inicial]=aux2;
            lectura_pos();
        }
        peon_corona();//analiza si un peon puede coronar
    }
    else{// si el movimiento es invalido pide de nuevo las coordenadas de destino
        cout<<"El movimiento es invalido"<<endl;
        lectura_mov();
    }
    tablero();//pienta el tablero conla nueva jugada
    deshacer();//pregunta si se quiere deshacer la jugada
    mov_enroque();//si la jugada se deshace se restaura la posiblidad de enrocar en caso de que se pueda
    main();
}
void lectura_pos()//recibe la pieza que se desea mover
{
    if(jaque()){//analiza si el rey esta en jaque
        if(jaque_mate()){//analiza si hay jaque mate
            cout<<"Jaque mate"<<endl;
            exit(0);
        }
        cout<<"Jaque"<<endl;
        if(jugador%2==1)//cuenta el numero de jaques realizados
            jaque_blancas++;
        else
            jaque_negras++;
    }
    if(modalidad==5){//si la modalidad es three-check muestra los jaque que se han realizado
        cout<<"jaque blancas= "<<jaque_blancas<<endl;
        cout<<"jaque negras= "<<jaque_negras<<endl;
        Three_Check();//analiza las reglas de esta modalidad
    }
    x=0;
    y=0;
    char z;
    cout<<endl<<"Ingrese casilla"<<endl;
    cin>>z>>y;
    for(char d='a'; d<'i'; d++)//cambia el caracter recibido a un entero para poder utilizarlo en las coordenadas del arreglo
    {
        if(z!=d)
            x++;
        else
            break;
    }
    y--;//resta 1 a la coordenada x para poder trabajarla en el arreglo
    x_inicial=x;//guarda las coordenadas de origen de la pieza
    y_inicial=y;
    if(a[x][y]=="*" || a[x][y]=="-"){//analiza si hay una casilla en la posicion ingresada
        cout<<"Opcion invalida! no hay ninguna ficha en esa poscicion"<<endl;
        lectura_pos();
    }
    pieza=a[x][y];
    if(pieza=="B_P" || pieza=="B_T" || pieza=="B_A" || pieza=="B_C" || pieza=="B_D" || pieza=="B_R")//asigna un numero a la variable verificar_color dependiendo de la pieza seleccionada
        verificar_color=1;
    else if(pieza=="N_P" || pieza=="N_T" || pieza=="N_A" || pieza=="N_C" || pieza=="N_D" || pieza=="N_R")
        verificar_color=2;
    if(jugador%2==1 && verificar_color==2){//segun el turno analiza si la seleccion de pieza es valida
        cout<<"Opcion invalida, es el turno de las blancas"<<endl;
        lectura_pos();
    }
    else if(jugador%2==0 && verificar_color==1){
        cout<<"Opcion invalida, es el turno de las negras"<<endl;
        lectura_pos();
    }
    lectura_mov();//redirige a la lectura de del movimiento
}
void copia_tablero()//llena el arreglo copia para guardar la jugada anterior
{
    for(int j=0; j<8; j++){//llena el arreglo copia
        for(int k=0; k<8; k++){
            copia[j][k]=a[j][k];
        }
    }
}
void turnos()//identifica el turno
{
    jugador++;//suma de uno en uno y asigna los turnos pares al jugador negro y los impares al las blancas
    if(jugador%2==1){
        cout<<"Juegan las blancas"<<endl;
    }
    else{
        cout<<"Juegan las negras"<<endl;
    }
    lectura_pos();
}
void deshacer()//deshace la jugada realizada si es deseado
{
    deshacer_bool=false;
    cout<<"Deshacer jugada?"<<endl;//pregunta si desea deshacer la jugada
    if(getch()==RETROCEDER){
        for(int j=0; j<8; j++){
            for(int k=0; k<8; k++){
                a[j][k]=copia[j][k];
            }
        }
         jugador--;
         deshacer_bool=true;
    }
    else{//pregunta si desea salir del juego
        cout<<"Para salir presione ESC"<<endl;
        if(getch()==ESCAPE)
            exit(0);
    }
}
bool jaque()//analiza si hay jaque
{
    int pos_rey_x,pos_rey_y;//guardan la posicion del rey

    //variables auxiliares para guardar los datos anteriores
    string pos_destino_aux=pos_destino, pieza_aux=pieza;
    int x_inicial_aux=x_inicial, y_inicial_aux=y_inicial, x_aux=x, y_aux=y,verificar_color_aux=verificar_color;

    for(int j=0; j<8; j++){//guardo coordenadas de rey
        for(int k=0; k<8; k++){
            if((jugador%2==1 && a[j][k]=="B_R") || (jugador%2==0 && a[j][k]=="N_R")){
                pos_rey_x=j;
                pos_rey_y=k;
                pos_destino=a[j][k];
            }
        }
    }
    if(jugador%2==1){//turno de las blancas
        for(int j=0; j<8; j++){
            for(int k=0; k<8; k++){
                if(a[j][k]=="N_P" || a[j][k]=="N_T" || a[j][k]=="N_A" || a[j][k]=="N_C" || a[j][k]=="N_D"){//mira si alguna ficha negra ataca el rey blanco
                        x_inicial=j;//guarda las coordenadas de la pieza en su posicion inicial
                        y_inicial=k;
                        x=pos_rey_x;//guarda como coordenadas de destino las coordenadas donde se encuentra el rey
                        y=pos_rey_y;
                        pieza=a[j][k];//guarda la pieza que se esta analizando
                        verificar_color=2;//guarda el color de la pieza
                        if(movimiento_valido()){
                            pos_destino=pos_destino_aux;
                            pieza=pieza_aux;
                            x_inicial=x_inicial_aux;
                            y_inicial=y_inicial_aux;
                            x=x_aux;
                            y=y_aux;
                            verificar_color=verificar_color_aux;
                            return true;
                        }
                }
            }
        }
    }
    if(jugador%2==0){//turno de las negras
        for(int j=0; j<8; j++){
            for(int k=0; k<8; k++){
                if(a[j][k]=="B_P" || a[j][k]=="B_T" || a[j][k]=="B_A" || a[j][k]=="B_C" || a[j][k]=="B_D"){//mira si alguna ficha blanca ataca el rey negro
                        x_inicial=j;//guarda las coordenadas de la pieza en su posicion inicial
                        y_inicial=k;
                        x=pos_rey_x;//guarda como coordenadas de destino las coordenadas donde se encuentra el rey
                        y=pos_rey_y;
                        pieza=a[j][k];//guarda la pieza que se esta analizando
                        verificar_color=1;//guarda el color de la pieza
                        if(movimiento_valido()){
                            pos_destino=pos_destino_aux;//restaura las variables
                            pieza=pieza_aux;
                            x_inicial=x_inicial_aux;
                            y_inicial=y_inicial_aux;
                            x=x_aux;
                            y=y_aux;
                            verificar_color=verificar_color_aux;
                            return true;
                        }
                }
            }
        }
    }
    return false;

}
void  peon_corona()//cambia la pieza del peon si llega a la ultima fila
{
    int opcion;
    for(int j=0; j<8; j++){
        if(a[j][0]=="N_P" || a[j][7]=="B_P"){//analiza toda la fila 1 y 8 para encontrar un peon
            seleccion:
            cout<<"Seleccione la pieza que desea cambiar por su peon"<<endl;//pregunta que pieza quiere cambiar
            cout<<"1. Dama"<<endl;
            cout<<"2. Alfil"<<endl;
            cout<<"3. Caballo"<<endl;
            cout<<"4. Torre"<<endl;
            cin>>opcion;
            //se realiza el cambio dependiendo del peon que llegue a su ultima fila
            if(a[j][0]=="N_P"){//coronar negras
                switch(opcion)
                {
                    case 1:
                        a[j][0]="N_D";
                        break;
                    case 2:
                        a[j][0]="N_A";
                        break;
                    case 3:
                        a[j][0]="N_C";
                        break;
                    case 4:
                        a[j][0]="N_T";
                        break;
                    default:
                        cout<<"Opcion incorrecta"<<endl;
                        goto seleccion;
                }
            }
            else//coronar blancas
                switch(opcion)
                {
                    case 1:
                        a[j][7]="B_D";
                        break;
                    case 2:
                        a[j][7]="B_A";
                        break;
                    case 3:
                        a[j][7]="B_C";
                        break;
                    case 4:
                        a[j][7]="B_T";
                        break;
                    default:
                        cout<<"Opcion incorrecta"<<endl;
                        goto seleccion;
                }
        }
    }
}
bool jaque_mate()//analiza si hay jaque mate
{
    int pos_rey_x, pos_rey_y, mate=0;

    for(int j=0; j<8; j++){//guardo coordenadas de rey
        for(int k=0; k<8; k++){
            if((jugador%2==1 && a[j][k]=="B_R") || (jugador%2==0 && a[j][k]=="N_R")){
                pos_rey_x=j;
                pos_rey_y=k;
            }
        }
    }
    if(jugador%2==0){
        for(int j=-1; j<=1; j++){//mueve el rey a todas sus casillas disponibles y analiza casillas seguras
            for(int k=-1; k<=1; k++){
                bool espacio_vacio;
                string aux=a[pos_rey_x+j][pos_rey_y+k];
                if(a[pos_rey_x+j][pos_rey_y+k]=="*" || a[pos_rey_x+j][pos_rey_y+k]=="-")
                    espacio_vacio=true;
                if(espacio_vacio){
                    a[pos_rey_x+j][pos_rey_y+k]="N_R";
                    a[pos_rey_x][pos_rey_y]=pos_original[pos_rey_x][pos_rey_y];
                }
                if(jaque())
                    mate ++;
                if(espacio_vacio){
                    a[pos_rey_x][pos_rey_y]="N_R";
                    a[pos_rey_x+j][pos_rey_y+k]=aux;
                }
                if(mate==8)
                    return true;
            }
        }
        return false;
    }
    if(jugador%2==1){
        for(int j=-1; j<=1; j++){//mueve el rey a todas sus casillas disponibles y analiza casillas seguras
            for(int k=-1; k<=1; k++){
                bool espacio_vacio;
                string aux=a[pos_rey_x+j][pos_rey_y+k];
                if(a[pos_rey_x+j][pos_rey_y+k]=="*" || a[pos_rey_x+j][pos_rey_y+k]=="-")
                    espacio_vacio=true;
                if(espacio_vacio){
                    a[pos_rey_x+j][pos_rey_y+k]="B_R";
                    a[pos_rey_x][pos_rey_y]=pos_original[pos_rey_x][pos_rey_y];
                }
                if(jaque())
                    mate ++;
                if(espacio_vacio){
                    a[pos_rey_x][pos_rey_y]="B_R";
                    a[pos_rey_x+j][pos_rey_y+k]=aux;
                }
                if(mate==8)
                    return true;
            }
        }
        return false;
    }
    return false;
}
int menu()//menu introductorio
{
    cout<<"----------------------------------------"<<endl<<endl;
    cout<<"          Bienvenido al ajedrez "<<endl<<endl;
    cout<<"El siguiente programa emula un ajedrez"<<endl;
    cout<<"con todas las reglas con las  que este "<<endl;
    cout<<"clasico juego cuenta. "<<endl<<endl;
    cout<<"Para seleccionar una pieza usted  debe "<<endl;
    cout<<"ingresar la  posicion  en en el eje  X "<<endl;
    cout<<"del trablero denotado de la a hasta la "<<endl;
    cout<<"h y luego ingresar la posicion  en  el  "<<endl;
    cout<<"eje Y del trablero denotado del 1 hasta"<<endl;
    cout<<"el 8. "<<endl<<endl;
    cout<<"Ejemplos: d2, a8, h5, etc."<<endl<<endl;
    cout<<"Y por ultimo para seleccionar la posi-"<<endl;
    cout<<"cion de destino de la pieza se utiliza"<<endl;
    cout<<"la misma notacion."<<endl<<endl;
    cout<<"----------------------------------------"<<endl;
    cout<<"---------------DISFRUTALO---------------"<<endl<<endl;
    system("pause");
    system("cls");
    cout<<"Selecione la modalidad de juego que prefiere:"<<endl<<endl;
    cout<<"          1. Ajedrez Estandar "<<endl<<endl;
    cout<<"          2. Ajedrez Horda "<<endl<<endl;
    cout<<"          3. King of the hill "<<endl<<endl;
    cout<<"          4. Chess960 "<<endl<<endl;
    cout<<"          5. Three-check"<<endl<<endl;
    cout<<"Quiero jugar:   ";
    cin>> modalidad;
    return main();
}
bool King_of_the_hill()//analiza las reglas de victora de su respectiva modalidad
{
    //analiza si un rey llega a los cuadros centrales
     if((a[3][3]=="B_R")||(a[3][4]=="B_R")||(a[4][3]=="B_R")||(a[4][4]=="B_R")){
             cout<<"El equipo Blanco gana la partida"<<endl;
             return true;
         }
    if ((a[3][3]=="N_R")||(a[3][4]=="N_R")||(a[4][3]=="N_R")||(a[4][4]=="N_R")){
             cout<<"El equipo Negro gana la partida"<<endl;
             return true;
        }
        else
            return false;
}
void Horda()//genera el respectivo tablero de acuerdo a las reglas de esta modalidad
{
    for(int b=0; b<8; b++){
        for (int c=0;c<4;c++){
        a[b][c]="B_P";
        }
    }
    a[1][4]="B_P";
    a[2][4]="B_P";
    a[5][4]="B_P";
    a[6][4]="B_P";
}
void Three_Check()//analiza las reglas de victora de su respectiva modalidad
{
    if(jugador%2==1 && jaque_blancas==3){
        cout<<"Ganan las negras"<<endl;
        exit(0);
    }
    if(jugador%2==0 && jaque_negras==3){
        cout<<"Ganan las blancas"<<endl;
        exit(0);
    }
}
void Chess960()//genera el respectivo tablero de acuerdo a las reglas de esta modalidad
{
    int piezas_ale[8];
    srand(time(NULL));
    for(int j=0; j<8; j++){//guarda numeros de 1-8 aleatoriamente con algunas restricciones
        piezas_ale[j]=rand()%8;
        if(j==0 && (piezas_ale[j]==0 || piezas_ale[j]==7)){//j=0 pos rey
            j--;
        }
        else if(j==2 && ((piezas_ale[0]<piezas_ale[1] && piezas_ale[0]<piezas_ale[2]) || (piezas_ale[0]>piezas_ale[1] && piezas_ale[0]>piezas_ale[2])))
            j--;
        else if(j==4 && ((piezas_ale[3]%2==0 && piezas_ale[4]%2==0) || (piezas_ale[3]%2==1 && piezas_ale[4]%2==1)))
            j--;
        else if(j>0){
            for(int k=0; k<j; k++){
                if(piezas_ale[k]==piezas_ale[j]){
                    j--;
                    break;
                }
            }
        }
    }
    string piezas_blancas[8]={"B_R","B_T","B_T","B_A","B_A","B_D","B_C","B_C"};
    string piezas_negras[8]={"N_R","N_T","N_T","N_A","N_A","N_D","N_C","N_C"};

    origen_rey=piezas_ale[0];


    for(int j=0; j<8; j++){//llena el arreglo con los num aleatrorios
        a[piezas_ale[j]][0]=piezas_blancas[j];
        a[piezas_ale[j]][7]=piezas_negras[j];
    }

}
void mov_enroque()//analiza las reglas de enroque
{
    if (deshacer_bool==true && enroque){//si se deshace la jugada y previamente se jugo un enroque permite otra vez enrocar
        if (((pieza=="B_T")&&(x_inicial==origen_torre_dama))||(pieza=="B_R"))
            enroque_blanco_dama=true;
        if (((pieza=="B_T")&&(x_inicial==origen_torre_rey))||(pieza=="B_R"))
            enroque_blanco_rey=true;
        if (((pieza=="N_T")&&(x_inicial==origen_torre_dama))||(pieza=="N_R"))
            enroque_negro_dama=true;
        if (((pieza=="N_T")&&(x_inicial==origen_torre_rey))||(pieza=="N_R"))
            enroque_negro_rey=true;
    }//restinge el enroque si se mueve el rey o la torre
    else if (((pieza=="B_T")&&(x_inicial==origen_torre_dama))||(pieza=="B_R"))
        enroque_blanco_dama=false;
    else if (((pieza=="B_T")&&(x_inicial==origen_torre_rey))||(pieza=="B_R"))
        enroque_blanco_rey=false;
    else if (((pieza=="N_T")&&(x_inicial==origen_torre_dama))||(pieza=="N_R"))
        enroque_negro_dama=false;
    else if (((pieza=="N_T")&&(x_inicial==origen_torre_rey))||(pieza=="N_R"))
        enroque_negro_rey=false;
}

