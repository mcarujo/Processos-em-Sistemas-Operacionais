#include <iostream>
#include <stdio.h>
#include <fstream>

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
        int OVPrewitt[3][3]= {{1, 0, -1},{2, 0, -2},{1, 0, -1}};
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
        int OHPrewitt[3][3]= {{1, 2, 1},{0, 0, 0},{-1, -2, -1}};
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
 imagem A("washington.ascii.pbm");
 imagem AH("P1",A.l,A.c);
 imagem AV("P1",A.l,A.c);
 imagem C("P1",A.l,A.c);
 AH.filtrar_horizontal(A);
 AV.filtrar_vetircal(A);
 AH.salvarimagem("sierpinski.ascii_filtrado_horizontalmente.pbm");
 AV.salvarimagem("sierpinski.ascii_filtrado_verticalmente.pbm");
 C.somando_filtros(AV,AH);
 C.salvarimagem("sierpinski.ascii_filtrado.pbm");
 A.fechar();    //DESALOCA
 AH.fechar();   //DESALOCA
 AV.fechar();   //DESALOCA
 C.fechar();    //DESALOCA



  return 0;
}



