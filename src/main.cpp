#include <iostream>
#include <stdio.h>
#include <fstream>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

using namespace std;
                class imagem{
                public:
                    string tipo,nome;
                    int l,c;
                    int **mat;

                    imagem(char* nome);
                    imagem(string a,int linha,int colunas);
                    void salvarimagem(char* a);
                    void fechar();
                    void filtrar_horizontal(imagem A);
                    void filtrar_vetircal(imagem A);
                    void somando_filtros(imagem A,imagem B);
                };
imagem::imagem(char* nome){

        ifstream myfile;
        myfile.open(nome);

        if(myfile.is_open()){

            myfile >> tipo;
            myfile >> c >> l;

            mat = new int*[l];

            for(int i = 0;i < l; ++i)
                mat[i] = new int[c];

            for(int i=0;i<l;i++){
                for(int j=0;j<c;j++){
                    myfile >> mat[i][j];

                }
            }
        }
        myfile.close();
}
imagem::imagem(string a,int linha,int colunas){
    tipo=a;
    l=linha;
    c=colunas;
    mat = new int*[l];

            for(int i = 0;i < l; ++i)
                mat[i] = new int[c];

            for(int i=0;i<l;i++){
                for(int j=0;j<c;j++){
                    mat[i][j]=0;
                }
            }
}
void imagem::fechar(){
    for(int i = 0;i < this->l; ++i)
        delete []this->mat[i];

    delete []this->mat;

}
void imagem::salvarimagem(char * a){

    ofstream myfile;
    myfile.open(a);
    myfile << this->tipo << "\n";
    myfile << this->c << " " << this->l <<"\n";

    for(int i=0;i<this->l;i++){
        for(int j=0;j<this->c;j++){
            myfile << this->mat[i][j] << " ";}
        myfile << endl;
        }
    myfile.close();

}
void imagem::filtrar_vetircal(imagem A){
        int OVPrewitt[3][3]= {{1, 0, -1},{1, 0, -1},{1, 0, -1}};
        int aux;
            for(int i=1 ; i<A.c-1 ; i++){
                    for(int j=1 ; j<A.l - 1 ; j++){
                            aux=0;
                            aux =
                                OVPrewitt[0][0]*A.mat[j-1][i-1] +   OVPrewitt[0][1]*A.mat[j-1][i] + OVPrewitt[0][2]*A.mat[j-1][i+1]
                             +  OVPrewitt[1][0]*A.mat[j][i-1]   +   OVPrewitt[1][1]*A.mat[j][i]   + OVPrewitt[1][2]*A.mat[j][i+1]
                             +  OVPrewitt[2][0]*A.mat[j+1][i-1] +   OVPrewitt[2][1]*A.mat[j+1][i] + OVPrewitt[2][2]*A.mat[j+1][i+1];

                         if(aux!=0)
                            this->mat[j][i]=1;
                         else
                            this->mat[j][i]=0;
                    }
            }
}
void imagem::filtrar_horizontal(imagem A){
        int OHPrewitt[3][3]= {{1, 1, 1},{0, 0, 0},{-1, -1, -1}};
         int aux;
            for(int i=1 ; i<A.l-1 ; i++){
                    for(int j=1 ; j<A.c-1 ; j++){
                            aux=0;
                            aux =
                                OHPrewitt[0][0]*A.mat[i-1][j-1] +   OHPrewitt[0][1]*A.mat[i-1][j] + OHPrewitt[0][2]*A.mat[i-1][j+1]
                             +  OHPrewitt[1][0]*A.mat[i][j-1]   +   OHPrewitt[1][1]*A.mat[i][j]   + OHPrewitt[1][2]*A.mat[i][j+1]
                             +  OHPrewitt[2][0]*A.mat[i+1][j-1] +   OHPrewitt[2][1]*A.mat[i+1][j] + OHPrewitt[2][2]*A.mat[i+1][j+1];

                         if(aux!=0)
                            this->mat[i][j]=1;
                         else
                            this->mat[i][j]=0;
                    }
            }

}
void imagem::somando_filtros(imagem A,imagem B){
        int aux;
        for(int i=0;i<A.l;i++){
            for(int j=0;j<A.c;j++){
                       aux=A.mat[i][j]+B.mat[i][j];
                        if(aux>0)
                            this->mat[i][j]=1;
                        else
                            this->mat[i][j]=0;
            }
        }
}

int main()
{
    string img="washington.ascii";
    cout << "\tIniciando processo de filtragem de Prewitt, o qual ser� aplicado na imagem " << img << " .\n";
    cout << "\tProcesso Pai possue n�mero: " << getpid() << "\n";

    if(fork()==0){
        cout << "\n\tProcesso filho de numero : " << getpid() << " ,o qual o pai tem o numero : " << getppid() << " ,come�ando a filtragem horrizontal.\n";
        imagem A("washington.ascii.pbm");
        imagem AH("P1",A.l,A.c);
        AH.filtrar_horizontal(A);
        AH.salvarimagem("washington.ascii_filtrado_horizontalmente.pbm");
        A.fechar();
        AH.fechar();
        cout <<"\tProcesso filho de numero : " << getpid() << ", terminou a filtragem horizontal e ser� morto por um 'exit'. \n";
        exit(0);
        }
    else {
      int ret1, status1;
      ret1 = wait(&status1) ;

      if ((status1&255) == 0) {
         cout << "\tProcesso filho numero : " << ret1 << " est� morto.\n";
      }
      else{
        cout << "\tProcesso filho n�o foi finalizado corretamente.\n";
      }


      if(fork()==0){
        cout << "\n\tProcesso filho de numero : " << getpid() << " ,o qual o pai tem o numero : " << getppid() << " ,come�ando a filtragem vertical.\n";
        imagem A("washington.ascii.pbm");
        imagem AV(A.tipo,A.l,A.c);
        AV.filtrar_vetircal(A);
        AV.salvarimagem("washington.ascii_filtrado_verticalmente.pbm");
        A.fechar();
        AV.fechar();
        cout <<"\tProcesso filho de numero : " << getpid() << ", terminou a filtragem vertical e ser� morto por um 'exit'. \n";
      exit(0);
      }
      else {
        int ret2, status2;
        ret2 = wait(&status2) ;

        if ((status2&255) == 0) {
           cout << "\tProcesso filho numero: " << ret2 << ", est� morto.\n";
        }
        else {
          cout << "\tProcesso filho n�o foi finalizado corretamente.\n";
        }
       }

       cout << "\tProcesso Pai numero : " << getpid() << " , agora pode prossegir e fazer a soma da filtragem vertical e filtragem horizontal.\n";
       imagem A2("washington.ascii_filtrado_verticalmente.pbm");
       imagem B2("washington.ascii_filtrado_horizontalmente.pbm");
       imagem C2(A2.tipo,A2.l,A2.c);
       C2.somando_filtros(A2,B2);
       C2.salvarimagem("washington.ascii_filtrado.pbm");
       A2.fechar();
       B2.fechar();
       C2.fechar();

    }
return 0;
}
